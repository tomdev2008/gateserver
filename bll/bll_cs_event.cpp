/*
 * bll_cs_event.cpp
 *   从客户端发往服务器的消息处理
 *  Created on: 2013-1-11
 *      Author: luocj
 */

#include "bll_cs_event.h"
#include "config/msg_cs_config.h"
#include "def/from_client_message.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

int32_t CCSEvent::OnMessageEvent(MessageHeadCS * pMsgHead, const uint8_t* pBuf,const uint32_t nBufLen,
		const uint16_t nOptionLen, const void *pOptionData)
{
	int32_t nRet = S_OK;
	//转发客户端请求到服务器
	if(pMsgHead == NULL || pBuf == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x,pBuf=0x%08x}\n",pMsgHead,pBuf);
		return E_NULLPOINTER;
	}

	//获取消息的转发目的
	EntityType nDestEntityType = g_CCSMsgConfig.GetEntityTypeByMsgId(pMsgHead->nMessageID);
	TransType nTransType = g_CCSMsgConfig.GetTransTypeByMsgId(pMsgHead->nMessageID);
	int32_t nDestServerID = enmInvalidServerID;

	WRITE_DEBUG_LOG("recv clent message {nRoleID=%d, nMsgID=0x%08x, nDestEntityType=%d, nTransType=%d}",pMsgHead->nRoleID,pMsgHead->nMessageID,nDestEntityType,nTransType);

	//编码conn
	ConnInfo * pConnInfo = (ConnInfo *)pOptionData;
	if(pConnInfo == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pConnInfo=0x%08x}\n",pConnInfo);
		return E_NULLPOINTER;
	}
	uint32_t nOffset = 0;
	uint8_t arrOptionData[MaxOptionDataSize];
	nRet = pConnInfo->MessageEncode(arrOptionData, MaxOptionDataSize, nOffset);
	if(nRet < 0)
	{
		WRITE_ERROR_LOG("encode connuin failed!{ret=0x%08x}\n", nRet);
		return nRet;
	}

	//获取P2P类型的转发
	if(nTransType == enmTransType_P2P)
	{
		if(nDestEntityType == enmEntityType_Room)
		{
			nDestServerID = GetRoomServerID(pMsgHead->nRoomID);

			if(nDestServerID == enmInvalidServerID)
			{
				CGetRoomServerIDSession stSession;
				stSession.Init(pMsgHead,pBuf,nBufLen,nOffset,arrOptionData,this);

				CGetRoomServerIDReq_Public stMsgBody;
				stMsgBody.nRoleID = pMsgHead->nRoleID;
				stMsgBody.nRoomID = pMsgHead->nRoomID;
				stMsgBody.nConnIndex = pConnInfo->nTunnelIndex;

				nRet = g_Frame.SendMessageRequest(MSGID_TSHS_GETROOMSERVERID_REQ, &stMsgBody, &stSession);
				if(0 > nRet)
				{
					WRITE_ERROR_LOG("send get roomserverid req failed!{nRet=0x%08X}",nRet);
					return nRet;
				}
			}
		}
		if(nDestServerID == enmInvalidServerID)
		{
			return S_OK;
		}
	}

	//如果是登出加入服务器的body，方便hallserver统计退出原因
	if(pMsgHead->nMessageID == MSGID_CLHS_LOGOUT_REQ)
	{
		if(g_bHallServerIsRuning)
		{
			PlayerLogout(pMsgHead->nRoleID,enmLogoutReason_ClientRequest,nOffset,arrOptionData);
		}
		else
		{
			CPlayer *pPlayer = NULL;
			nRet = g_PlayerMgt.GetPlayer(pMsgHead->nRoleID,pPlayer);
			if(nRet < 0)
			{
				WRITE_WARNING_LOG("creat player object error!{nRoleID=%d, nRet=0x%08x}",pMsgHead->nRoleID,nRet);
				return nRet;
			}
			pPlayer->SetLogoutReason(enmLogoutReason_ClientRequest);
			g_PlayerMgt.m_arrNeedLogout.push_back(pMsgHead->nRoleID);

			WRITE_DEBUG_LOG("player logout but hallserver is not running !{nRoleID=%d}\n",pMsgHead->nRoleID);
		}
		return S_OK;
	}

	//处理玩家的登录消息
	if(pMsgHead->nMessageID == MSGID_CLHS_LOGIN_REQ)
	{
		//创建玩家
		CPlayer *pPlayer = NULL;
		nRet = g_PlayerMgt.GetPlayer(pMsgHead->nRoleID,pPlayer);
		if(nRet < 0 || pPlayer == NULL)
		{
			//创建玩家
			nRet = g_PlayerMgt.CreatePlayer(pMsgHead->nRoleID,pPlayer);
			if(nRet < 0 || pPlayer == NULL)
			{
				WRITE_ERROR_LOG("creat player object error!{nRoleID=%d, nRet=0x%08x}",pMsgHead->nRoleID,nRet);
				return nRet;
			}
		}
		pPlayer->SetConnInfoTemp(pConnInfo);
	}
	SendMessageToServer(pMsgHead->nMessageID,pMsgHead->nSequence,enmMessageType_Request,pBuf,nBufLen,pMsgHead->nRoomID,nTransType,pMsgHead->nRoleID,nDestEntityType,nDestServerID,nOffset,arrOptionData);

	return S_OK;
}
int32_t CCSEvent::OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		CFrameSession* pSession, const uint16_t nOptionLen, const void *pOptionData)
{

	if(pMsgBody == NULL || pMsgHead == NULL || pSession == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x, pSession=0x%08x}\n",
				pMsgHead, pMsgBody, pSession);
		return E_NULLPOINTER;
	}

	switch(pSession->GetSessionType())
	{
		case enmSessionType_GetRoomServerID:
		GetRoomServerIDResp(pMsgHead, pMsgBody, pSession);
		break;
		default:
		WRITE_ERROR_LOG("login:get wrong session type!{nSessionType=0x%04X}\n", pSession->GetSessionType());
		break;
	}

	return S_OK;
}

int32_t CCSEvent::GetRoomServerID(RoomID nRoomID)
{
	int32_t nDestServerID = enmInvalidServerID;
	CRoom *pRoom = NULL;
	g_RoomMgt.GetRoom(nRoomID, pRoom);
	if (pRoom != NULL)
	{
		nDestServerID = pRoom->GetRoomServerID();
	}
	else
	{
		nDestServerID = enmInvalidServerID;
		WRITE_NOTICE_LOG("need get room server id {nRoomID=%d}\n",nRoomID);
	}

	return nDestServerID;
}

int32_t CCSEvent::GetRoomServerIDResp(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,CFrameSession* pSession)
{
	int32_t nRet = S_OK;
	if(pMsgBody == NULL || pMsgHead == NULL || pSession == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x, pSession=0x%08x}\n",
				pMsgHead, pMsgBody, pSession);
		return E_NULLPOINTER;
	}
	CGetRoomServerIDSession * pData =(CGetRoomServerIDSession *)(pSession->GetSessionData());
	if(NULL == pData)
	{
		WRITE_ERROR_LOG("null pointer:get server id session, get data failed!{nSessionIndex=%d}\n",
				pSession->GetSessionIndex());
		return E_NULLPOINTER;
	}

	CGetRoomServerIDResp_Public *pGetRoomServerIDResp = dynamic_cast<CGetRoomServerIDResp_Public *>(pMsgBody);
	if(NULL == pGetRoomServerIDResp)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to pGetRoomServerIDResp class failed!{nMessageID=0x%08x, nRoleID=%d, nRoomID=%d"
				"nSessionType=0x%04X, nSessionId=%d}\n", pData->m_sMsgHead.nMessageID, pData->m_sMsgHead.nRoleID,pData->m_sMsgHead.nRoomID,
				pSession->GetSessionType(), pSession->GetSessionIndex());

		return E_NULLPOINTER;
	}
    if(pGetRoomServerIDResp->nResult < 0)
    {
    	WRITE_ERROR_LOG("get server id error{nRoomID=%d, nRet=0x%08x}",pData->m_sMsgHead.nRoomID,pGetRoomServerIDResp->nResult);
    	return nRet;
    }
	CRoom *pRoom = NULL;
	nRet = g_RoomMgt.GetRoom(pData->m_sMsgHead.nRoomID, pRoom);
	if (pRoom == NULL)
	{
		nRet = g_RoomMgt.CreateRoom(pData->m_sMsgHead.nRoomID, pRoom);
		if(nRet < 0 || pRoom == NULL)
		{
			WRITE_ERROR_LOG("creat room error{nRoomID=%d, nRet=0x%08x}",pData->m_sMsgHead.nRoomID,nRet);
			return nRet;
		}
	}
	pRoom->SetRoomServerID(pGetRoomServerIDResp->nServerID);

	//转发消息
	SendMessageToServer(pData->m_sMsgHead.nMessageID,pData->m_sMsgHead.nSequence,enmMessageType_Request,
			pData->m_pMsgBodyBuf,pData->m_nBodyLen,pData->m_sMsgHead.nRoomID,enmTransType_P2P,
			pData->m_sMsgHead.nRoleID,enmEntityType_Room,pGetRoomServerIDResp->nServerID,
			pData->m_nOptionLen,pData->m_arrOptionData);

	return nRet;
}
FRAME_GATESERVER_NAMESPACE_END
