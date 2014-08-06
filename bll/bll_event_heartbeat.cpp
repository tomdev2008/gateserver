/*
 * bll_event_heartbeat.cpp
 *
 *  Created on: 2013-1-17
 *      Author: luocj
 */

#include "bll_event_heartbeat.h"
#include "ctl/data_center.h"
#include "def/from_client_message.h"
#include <algorithm>

using namespace std;

FRAME_GATESERVER_NAMESPACE_BEGIN

int32_t CHeartBeatMessageEvent::OnMessageEvent(MessageHeadCS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen, const void *pOptionData)
{
	int32_t nRet = S_OK;
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n",pMsgHead,pMsgBody);
		return E_NULLPOINTER;
	}

	CPlayerHeartBeat *pPlayerHeartBeat = dynamic_cast<CPlayerHeartBeat *>(pMsgBody);
	if(NULL == pPlayerHeartBeat)
	{
		WRITE_ERROR_LOG("null pointer:{pPlayerHeartBeat=0x%08x}\n",pPlayerHeartBeat);
		return E_NULLPOINTER;
	}

	list<RoleID>::iterator iter;
	iter = find(g_NeedRecvHeartBeatList.begin(),g_NeedRecvHeartBeatList.end(),pPlayerHeartBeat->nRoleID);
	if(iter != g_NeedRecvHeartBeatList.end())
	{
		WRITE_DEBUG_LOG("find player {nRoleID=%d}",pPlayerHeartBeat->nRoleID);
	}

	g_NeedRecvHeartBeatList.remove(pPlayerHeartBeat->nRoleID);

	g_RecvHeartBeatList.push_back(pPlayerHeartBeat->nRoleID);

	return S_OK;
}

FRAME_GATESERVER_NAMESPACE_END
