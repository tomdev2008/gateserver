/*
 * bll_timer_checkbeat.cpp
 *
 *  Created on: 2013-1-18
 *      Author: luocj
 */

#include "bll_timer_checkbeat.h"
#include "ctl/data_center.h"
#include "config/msg_cs_config.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

int32_t CCheckBeatEvent::OnTimerEvent(CFrameTimer *pTimer)
{
	WRITE_DEBUG_LOG("-----------on check beat timer----------\n");
	int32_t nRet = S_OK;

	//检查没有收到心跳的玩家
	list<RoleID>::iterator  iter = g_NeedRecvHeartBeatList.begin();
	for(iter = g_NeedRecvHeartBeatList.begin();iter != g_NeedRecvHeartBeatList.end();iter++)
	{
		nRet = g_BallBase.ClosePlayerSocket(*iter);
		if(nRet < 0)
		{
			WRITE_ERROR_LOG("player timeout but kick it error{nRoleID=%d, nRet=0x%08x}\n",*iter,nRet);
		}
	}

	g_NeedRecvHeartBeatList.clear();

	list<RoleID> arrRecvlistTemp(g_RecvHeartBeatList);
	g_RecvHeartBeatList = g_NeedRecvHeartBeatList;
	g_NeedRecvHeartBeatList = arrRecvlistTemp;

	return S_OK;
}

FRAME_GATESERVER_NAMESPACE_END
