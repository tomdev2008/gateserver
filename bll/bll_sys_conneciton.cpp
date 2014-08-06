/*
 * bll_sys_conneciton.cpp
 *
 *  Created on: 2013-1-11
 *      Author: luocj
 */

#include "bll_sys_connection.h"
#include "def/from_client_message.h"
#include "ctl/data_center.h"
#include "global.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

int32_t CSYSEvent::OnSystemEvent(uint16_t nEventID, void *pParam)
{
	int32_t nRet = S_OK;
	ConnInfo *pConnInfo = (ConnInfo *)pParam;
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

	WRITE_NOTICE_LOG("player conn is colse !{nRoleID=%d, nEventID=%d ip=%s,port=%d}\n",pConnInfo->nRoleID,nEventID,inet_ntoa_f(pConnInfo->nAddress),pConnInfo->nPort);

	//´óÌüserver´¦ÓÚÍ£Ö¹½×¶Î
	if(!g_bHallServerIsRuning)
	{
		CPlayer *pPlayer = NULL;
		nRet = g_PlayerMgt.GetPlayer(pConnInfo->nRoleID,pPlayer);
		if(nRet < 0)
		{
			WRITE_WARNING_LOG("get player object error!{nRoleID=%d, nRet=0x%08x}",pConnInfo->nRoleID,nRet);
			return nRet;
		}
		pPlayer->SetLogoutReason(pConnInfo->nErrorCode);
		g_PlayerMgt.m_arrNeedLogout.push_back(pConnInfo->nRoleID);

		WRITE_DEBUG_LOG("player conn close but hallserver is not running !{nRoleID=%d}\n",pConnInfo->nRoleID);

		return S_OK;
	}

	PlayerLogout(pConnInfo->nRoleID,pConnInfo->nErrorCode,nOffset,arrOptionData);

	return S_OK;
}

FRAME_GATESERVER_NAMESPACE_END

