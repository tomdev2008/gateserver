/*
 *  bll_base.h
 *
 *  To do：
 *  Created on: 	2012-12-19
 *  Author: 		luocj
 */

#ifndef BLL_BASE_H_
#define BLL_BASE_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "frame_configmgt.h"
#include "main_frame.h"
#include "frame_socket_mgt.h"
#include "frame_eventid.h"
#include "def/from_client_message.h"
#include "def/to_client_message.h"
#include "ctl/data_center.h"
#include "config/msg_cs_config.h"


FRAME_GATESERVER_NAMESPACE_BEGIN

class CBllBase : public IMessageEventSS, public ISessionEvent
{
public:
	CBllBase();

	virtual ~CBllBase();

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL)
	{
		return S_OK;
	}

	//会话事件激活
	virtual int32_t OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			CFrameSession* pSession, const uint16_t nOptionLen = 0, const void *pOptionData = NULL)
	{
		return S_OK;
	}

	int32_t OnSessionTimeOut(CFrameSession *pSession)
	{
			return S_OK;
	}

public:

	//一些基本的接口
	void SendMessageToServer(uint32_t nMsgID,const uint32_t nSequence,MessageType nMessageType,const uint8_t* pBuf,const uint32_t nBufLen,const RoomID nRoomID,const TransType nTransType,
			const RoleID nRoleID,const EntityType nDestType,const int32_t nDestID = 0,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL,const char *szDumpContent = "send message to server")
	{
		if(pBuf == NULL)
		{
			WRITE_ERROR_LOG("null pointer:{pBuf=0x%08x}\n",pBuf);
			return;
		}
		MessageHeadSS stSendMessage;
		stSendMessage.nMessageID = nMsgID;
		stSendMessage.nMessageType = nMessageType;
		stSendMessage.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
		stSendMessage.nDestType = nDestType;
		stSendMessage.nSourceID = g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
		stSendMessage.nDestID = nDestID;
		stSendMessage.nRoleID = nRoleID;
		stSendMessage.nSequence = nSequence;
		stSendMessage.nTransType = nTransType;
		stSendMessage.nSessionIndex = enmInvalidSessionIndex;
		stSendMessage.nRoomID = nRoomID;
		stSendMessage.nZoneID = g_Frame.GetZoneID(enmRouterType_Sync);
		stSendMessage.nRouterIndex = 0;

		g_Frame.PostMessage(&stSendMessage,pBuf,nBufLen,nOptionLen,(char*)pOptionData);
	}

	void SendMessageToServer(uint32_t nMsgID,MessageType nMessageType,IMsgBody* pMsgBody,const RoomID nRoomID,const TransType nTransType,
			const RoleID nRoleID,const EntityType nDestType,const int32_t nDestID = 0,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL,const char *szDumpContent = "send message to server")
	{
		if(pMsgBody == NULL)
		{
			WRITE_ERROR_LOG("null pointer:{pMsgBody=0x%08x}\n",pMsgBody);
			return;
		}
		MessageHeadSS stSendMessage;
		stSendMessage.nMessageID = nMsgID;
		stSendMessage.nMessageType = nMessageType;
		stSendMessage.nSourceType = g_FrameConfigMgt.GetFrameBaseConfig().GetServerType();
		stSendMessage.nDestType = nDestType;
		stSendMessage.nSourceID = g_FrameConfigMgt.GetFrameBaseConfig().GetServerID();
		stSendMessage.nDestID = nDestID;
		stSendMessage.nRoleID = nRoleID;
		stSendMessage.nSequence = 0;
		stSendMessage.nTransType = nTransType;
		stSendMessage.nSessionIndex = enmInvalidSessionIndex;
		stSendMessage.nRoomID = nRoomID;
		stSendMessage.nZoneID = g_Frame.GetZoneID(enmRouterType_Sync);
		stSendMessage.nRouterIndex = 0;

		DumpMessage(szDumpContent,&stSendMessage,pMsgBody, nOptionLen, (char*)pOptionData);
		SendMessage(&stSendMessage, pMsgBody, nOptionLen, (char*)pOptionData);
	}

	int32_t SendMessage(MessageHeadSS* pMsgHead, IMsgBody *pMsgBody, const int32_t nOptionLen = 0,
			const char *pOptionData = NULL)
	{
		if(pMsgHead == NULL || pMsgBody == NULL)
		{
			return E_NULLPOINTER;
		}

		g_Frame.PostMessage(pMsgHead, pMsgBody, nOptionLen, pOptionData);
		return S_OK;
	}

	void DumpMessage(const char* szContent, MessageHeadSS *pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const char *pOptionData = NULL)
	{
		if(pMsgBody == NULL || pMsgHead == NULL)
		{
			WRITE_ERROR_LOG("null pointer:{pMsgBody=0x%08x, pMsgHead=0x%08x}\n",pMsgBody,pMsgHead);
			return;
		}
		//判断等级是否足够
		uint32_t offset = 0;
		char szLog[enmMaxLogInfoLength];
		szLog[0] = 0;
		sprintf(szLog + offset, szContent);
		offset = (uint32_t)strlen(szLog);

		sprintf(szLog + offset, " MessageHead=");
		offset = (uint32_t)strlen(szLog);

		pMsgHead->Dump(szLog, enmMaxLogInfoLength, offset);

		sprintf(szLog + offset, " MessageBody=");
		offset = (uint32_t)strlen(szLog);
		pMsgBody->Dump(szLog, enmMaxLogInfoLength, offset);

		WRITE_NOTICE_LOG("%s\n", szLog);
	}

	int32_t ClosePlayerSocket(RoleID nRoleID)
	{
		CPlayer *pPlayer = NULL;
		int32_t nRet = g_PlayerMgt.GetPlayer(nRoleID,pPlayer);
		if(nRet < 0 || pPlayer == NULL)
		{
			WRITE_ERROR_LOG("get player error:{nRoleID=%d, nRet=0x%08x}\n", nRoleID, nRet);
			return S_OK;
		}
		uint8_t *pMem = MALLOC(sizeof(ConnInfo));//g_FrameMemMgt.AllocBlock(nPacketLen);
		if(pMem == NULL)
		{
			return E_NULLPOINTER;
		}
		NetPacket *pNetPacket = new(pMem)NetPacket();
		pNetPacket->m_nNetPacketLen = sizeof(ConnInfo);

		ConnInfo *pConnInfo = new(&pNetPacket->m_pNetPacket[0]) ConnInfo;
//		pConnInfo = pPlayer->GetConnInfo();
		if(pPlayer->GetConnInfo() == NULL)
		{
			WRITE_ERROR_LOG("null pointer:{pConnInfo=0x%08x}\n",pConnInfo);
			FREE((uint8_t *)pNetPacket);
			return E_NULLPOINTER;
		}
		pConnInfo->nErrorCode = SYS_EVENT_CONN_SERVER_CLOSED;

		g_FrameNetQueue.PushSendCSQueue(pNetPacket);

		return S_OK;
	}

	int32_t PlayerLogout(RoleID nRoleID,LogoutReason nLogoutReason,const uint16_t nOptionLen, const void *pOptionData)
	{
		int32_t nRet = S_OK;
		CPlayer *pPlayer = NULL;
		nRet = g_PlayerMgt.GetPlayer(nRoleID,pPlayer);
		if(nRet < 0 || pPlayer == NULL)
		{
			return S_OK;
		}
		ConnInfo * pConnInfo = pPlayer->GetConnInfo();
		//ConnInfo * pConnInfoTemp = pPlayer->GetConnInfoTemp();

		if(pConnInfo == NULL/* || pConnInfoTemp == NULL*/)
		{
			WRITE_ERROR_LOG("null pointer:{pConnInfo=0x%08x}\n",pConnInfo);
			return E_NULLPOINTER;
		}

		CLogoutReq stLogoutReq;
		stLogoutReq.nLogoutReason = nLogoutReason;

		EntityType nEntityType = g_CCSMsgConfig.GetEntityTypeByMsgId(MSGID_CLHS_LOGOUT_REQ);
		TransType nTransType = enmTransType_Broadcast;
		int32_t nDestID = 0;

		SendMessageToServer(MSGID_CLHS_LOGOUT_REQ,enmMessageType_Request,&stLogoutReq,enmInvalidRoomID,nTransType,nRoleID,nEntityType,nDestID,nOptionLen,pOptionData);

		ConnInfo stConnInfoTemp;
		uint32_t nOffset = 0;
		nRet = stConnInfoTemp.MessageDecode((uint8_t *)pOptionData, nOptionLen, nOffset);
		if(nRet < 0)
		{
			WRITE_ERROR_LOG("conn info decode failed!{nRoleID=%d, nLogoutReason=%d}\n", nRoleID, nLogoutReason);
			return nRet;
		}
		//销毁玩家
		if((pConnInfo->nRoleID != enmInvalidRoleID) || stConnInfoTemp.Compare(pConnInfo))
		{
			g_PlayerMgt.DestroyPlayer(nRoleID);
			WRITE_DEBUG_LOG("delete player object{nRoleID=%d}\n",nRoleID);
		}
		//从心跳检查中删除
		g_NeedRecvHeartBeatList.remove(nRoleID);
		g_RecvHeartBeatList.remove(nRoleID);

		return S_OK;
	}
private:

};

#define	CREATE_BLLBASE_INSTANCE					CSingleton<CBllBase>::CreateInstance
#define	g_BallBase								CSingleton<CBllBase>::GetInstance()
#define	DESTROY_BLLBASE_INSTANCE				CSingleton<CBllBase>::DestroyInstance

FRAME_GATESERVER_NAMESPACE_END

#endif /* BLL_BASE_H_ */
