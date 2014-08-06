/*
 * bll_ss_event.cpp
 *
 *  Created on: 2013-1-11
 *      Author: luocj
 */

#include "bll_ss_event.h"
#include "ctl/data_center.h"
#include "def/to_client_message.h"


FRAME_GATESERVER_NAMESPACE_BEGIN

int32_t CSSEvent::OnMessageEvent(MessageHeadSS * pMsgHead, const uint8_t* pBuf,const uint32_t nBufLen,
		const uint16_t nOptionLen, const void *pOptionData)
{
	int32_t nRet = S_OK;
	if(pMsgHead == NULL || pBuf == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x,pBuf=0x%08x}\n",pMsgHead,pBuf);
		return E_NULLPOINTER;
	}
	//发送回应给客户端
	WRITE_DEBUG_LOG("msg need to send to clent nMsgID=0x%08x",pMsgHead->nMessageID);

	//处理玩家的登录消息
	if(pMsgHead->nMessageID == MSGID_HSCL_LOGIN_RESP)
	{
		return PlayerLoginResp(pMsgHead,pBuf,nBufLen,nOptionLen,pOptionData);
	}

	//转发处理
	if(pMsgHead->nTransType == enmTransType_P2P)
	{
		SendMeesageToClient(pMsgHead->nMessageID,pMsgHead->nRoleID,pMsgHead->nRoomID,pMsgHead->nSequence,pBuf,nBufLen);
	}
	else if(pMsgHead->nTransType == enmTransType_Broadcast)
	{
		if(pMsgHead->nRoomID != enmInvalidRoomID)
		{
			//房间内的群发
			CRoom *pRoom = NULL;
			nRet = g_RoomMgt.GetRoom(pMsgHead->nRoomID, pRoom);
			if (pRoom == NULL || nRet < 0)
			{
				WRITE_ERROR_LOG("get pRoom error {nRoomID=%d, nRet=0x%08x}\n",pMsgHead->nRoomID,nRet);
				return nRet;
			}
			else
			{
				int32_t nPlayerCount = 0;
				RoleID arrRoleID[MaxUserCountPerRoom];
				pRoom->GetAllPlayers(arrRoleID, MaxUserCountPerRoom, nPlayerCount);

				RoleID nSrcRoleID = pMsgHead->nRoleID;
				for(int32_t i = 0; i < nPlayerCount; ++i)
				{
					if((nSrcRoleID != enmInvalidRoleID) && (arrRoleID[i] == nSrcRoleID))
					{
						continue;
					}
					SendMeesageToClient(pMsgHead->nMessageID,arrRoleID[i],pMsgHead->nRoomID,pMsgHead->nSequence,pBuf,nBufLen);
				}
			}
		}
		else
		{
			//send message to all client
			int32_t nPlayerCount = 0;
			RoleID arrRoleID[MaxOnlinePlayerCount];
			g_PlayerMgt.GetAllPlayer(arrRoleID,MaxOnlinePlayerCount,nPlayerCount);

			for(int32_t i = 0;i < nPlayerCount;i++)
			{
				SendMeesageToClient(pMsgHead->nMessageID,arrRoleID[i],pMsgHead->nRoomID,pMsgHead->nSequence,pBuf,nBufLen);
			}
		}
	}
	else
	{
		SendMeesageToClient(pMsgHead->nMessageID,pMsgHead->nRoleID,pMsgHead->nRoomID,pMsgHead->nSequence,pBuf,nBufLen);
	}

	if(pMsgHead->nMessageID == MSGID_HSCL_KICKUSER_NOTI)
	{
		WRITE_INFO_LOG("recv kick user noti!{nRoleID=%d}\n", pMsgHead->nRoleID);
		ClosePlayerSocket(pMsgHead->nRoleID);
	}

	return S_OK;
}

int32_t CSSEvent::SendMeesageToClient(uint32_t nMsgID,RoleID nRoleID,RoomID nRoomID,uint32_t nSequence,const uint8_t* pBuf,const uint32_t nBufLen)
{
	if(pBuf == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pBuf=0x%08x}\n",pBuf);
		return E_NULLPOINTER;
	}
	//创建玩家
	CPlayer *pPlayer = NULL;
	int32_t nRet = g_PlayerMgt.GetPlayer(nRoleID,pPlayer);
	if(nRet < 0 || pPlayer == NULL)
	{
		WRITE_ERROR_LOG("get player object error!{nRoleID=%d, nRet=0x%08x}",nRoleID,nRet);
		return nRet;
	}
	//构造头部
	MessageHeadCS stSendMessage;
	stSendMessage.nMessageID = nMsgID;
	stSendMessage.nRoleID = nRoleID;
	stSendMessage.nRoomID = nRoomID;
	stSendMessage.nSequence = nSequence;
	//推送消息
	g_Frame.PostMessage(&stSendMessage,pBuf,nBufLen,pPlayer->GetConnInfo()->nTunnelIndex);

	return S_OK;
}

int32_t CSSEvent::PlayerLoginResp(MessageHeadSS * pMsgHead, const uint8_t* pBuf,const uint32_t nBufLen,const uint16_t nOptionLen, const void *pOptionData )
{
	int32_t nRet = S_OK;
	if(pBuf == NULL || pMsgHead == NULL || pOptionData == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pBuf=0x%08x,pOptionData=0x%08x}\n",pMsgHead,pBuf,pOptionData);
		return E_NULLPOINTER;
	}

	uint32_t offset = 0;
	CLoginResp stLoginResp ;
	nRet = stLoginResp.MessageDecode(pBuf,nBufLen,offset);
	if( 0 > nRet)
	{
		WRITE_ERROR_LOG("decode msg login body failed!{ret=0x%08x}\n", nRet);
		return S_OK;
	}
	WRITE_DEBUG_LOG("player login !{nRoleID=%d}",pMsgHead->nRoleID);
	//创建玩家
	CPlayer *pPlayer = NULL;
	nRet = g_PlayerMgt.GetPlayer(pMsgHead->nRoleID,pPlayer);
	if(nRet < 0)
	{
		WRITE_ERROR_LOG("get player object error!{nRoleID=%d, nRet=0x%08x}",pMsgHead->nRoleID,nRet);
		return nRet;
	}
	ConnUin stInfo;
	offset = 0;
	nRet = stInfo.MessageDecode((uint8_t *)pOptionData, nOptionLen, offset);
	if(nRet < 0)
	{
		WRITE_ERROR_LOG("decode connuin failed!{nRoleID=%d, nRet=0x%08x}",pMsgHead->nRoleID,nRet);
		return nRet;
	}
	pPlayer->SetConnInfo(&stInfo);

//	pPlayer->PlayerLoginSuccess();

	SendMeesageToClient(pMsgHead->nMessageID,pMsgHead->nRoleID,pMsgHead->nRoomID,pMsgHead->nSequence,pBuf,nBufLen);

	if(stLoginResp.nResult > 0)
	{
		g_PlayerMgt.DestroyPlayer(pMsgHead->nRoleID);
		WRITE_DEBUG_LOG("player login error!{nRoleID=%d,nRet=0x%08x}",pMsgHead->nRoleID,stLoginResp.nResult);
		return S_OK;
	}

	g_RecvHeartBeatList.push_back(pMsgHead->nRoleID);
	return S_OK;
}
FRAME_GATESERVER_NAMESPACE_END
