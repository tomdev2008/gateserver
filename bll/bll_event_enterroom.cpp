/*
 *  bll_event_enterroom.cpp
 *
 *  To do£º
 *  Created on: 	2012-12-21
 *  Author: 		luocj
 */

#include "bll_event_enterroom.h"
#include "ctl/data_center.h"
#include "roomserver_message_define.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

int32_t CEnterRoomNoticeMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	int32_t nRet = S_OK;
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n", pMsgHead, pMsgBody);
		return E_NULLPOINTER;
	}

	CEnterRoomNotice_Public *pEnterRoomNotice = dynamic_cast<CEnterRoomNotice_Public *>(pMsgBody);
	if(NULL == pEnterRoomNotice)
	{
		WRITE_ERROR_LOG("null pointer:pMsgBody transform to CEnterRoomNotice class failed![nMessageID=0x%08x, nRoleID=%d]\n",
				pMsgHead->nMessageID, pMsgHead->nRoleID);
		return E_NULLPOINTER;
	}

	CRoom *pRoom = NULL;
	nRet = g_RoomMgt.GetRoom(pEnterRoomNotice->nRoomID, pRoom);
	if (pRoom == NULL || nRet < 0)
	{
		WRITE_ERROR_LOG("get pRoom error {nRoomID=%d, nRet=0x%08x}\n",pEnterRoomNotice->nRoomID,nRet);
		return nRet;
	}

	pRoom->AddPlayer(pEnterRoomNotice->nRoleID);
	return S_OK;
}

FRAME_GATESERVER_NAMESPACE_END
