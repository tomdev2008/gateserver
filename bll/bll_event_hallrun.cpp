/*
 * bll_event_hallrun.cpp
 *
 *  Created on: 2013-1-16
 *      Author: luocj
 */

#include "bll_event_hallrun.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

int32_t CHallRunMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
		const uint16_t nOptionLen , const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n",pMsgHead,pMsgBody);
		return E_NULLPOINTER;
	}

	CHallServerIsRuningNoti *pHallServerIsRuningNoti = dynamic_cast<CHallServerIsRuningNoti *>(pMsgBody);
	if(NULL == pHallServerIsRuningNoti)
	{
		WRITE_ERROR_LOG("null pointer:{pHallServerIsRuningNoti=0x%08x}\n",pHallServerIsRuningNoti);
		return E_NULLPOINTER;
	}

	//发送登出通知到hallserver
	for(uint32_t i = 0;i<g_PlayerMgt.m_arrNeedLogout.size();i++)
	{
		SendLogoutMessage(g_PlayerMgt.m_arrNeedLogout.at(i));
	}

	g_bHallServerIsRuning = true;
	return S_OK;
}

int32_t CHallRunMessageEvent::SendLogoutMessage(RoleID nRoleID)
{
	int32_t nRet = S_OK;
	CPlayer *pPlayer = NULL;
	nRet = g_PlayerMgt.GetPlayer(nRoleID,pPlayer);
	if(nRet < 0)
	{
		WRITE_ERROR_LOG("creat player object error!{nRoleID=%d, nRet=0x%08x}",nRoleID,nRet);
		return nRet;
	}
	ConnInfo *pConnInfo = pPlayer->GetConnInfo();
	if(pConnInfo == NULL)
	{
		WRITE_ERROR_LOG("null pointer:{stConnInfo=0x%08x}\n",pConnInfo);
		return E_NULLPOINTER;
	}

	uint32_t nOffset = 0;
	uint8_t arrOptionData[MaxOptionDataSize];
	nRet = pConnInfo->MessageEncode(arrOptionData, MaxOptionDataSize, nOffset);
	if(nRet < 0)
	{
		WRITE_ERROR_LOG("encode connuin failed!{nRet=0x%08x}\n", nRet);
		return nRet;
	}

	PlayerLogout(nRoleID,pPlayer->GetLogoutReason(),nOffset,arrOptionData);
	return nRet;
}

FRAME_GATESERVER_NAMESPACE_END
