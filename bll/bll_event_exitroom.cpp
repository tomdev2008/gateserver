/*
 *  bll_event_exitroom.cpp
 *
 *  To do£º
 *  Created on: 	2012-12-21
 *  Author: 		luocj
 */

#include "bll_event_exitroom.h"
#include "def/server_errordef.h"
#include "ctl/data_center.h"
#include "roomserver_message_define.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

int32_t CExitRoomNoticeMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	int32_t nRet = S_OK;
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	CExitRoomNotice_Public *pExitRoomNotice = dynamic_cast<CExitRoomNotice_Public *>(pMsgBody);
	if(NULL == pExitRoomNotice)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CExitRoomNotice class failed![nMessageID=0x%08x, nRoleID=%d]\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}

	CRoom *pRoom = NULL;
	nRet = g_RoomMgt.GetRoom(pExitRoomNotice->nRoomID,pRoom);
	if(pRoom == NULL || nRet < 0)
	{
		WRITE_ERROR_LOG("get pRoom error {nRoomID=%d, nRet=0x%08x}\n",pExitRoomNotice->nRoomID,nRet);
		return nRet;
	}

	pRoom->DeletePlayer(pExitRoomNotice->nRoleID);

	if(pRoom->GetUserCount() == 0)
	{
		g_RoomMgt.DestroyRoom(pExitRoomNotice->nRoomID);
	}
	return S_OK;
}

FRAME_GATESERVER_NAMESPACE_END
