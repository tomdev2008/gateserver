/*
 * bll_server_serverstop.h
 *
 *  Created on: 2013-1-16
 *      Author: luocj
 */

#ifndef BLL_SERVER_SERVERSTOP_H_
#define BLL_SERVER_SERVERSTOP_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "global.h"
#include "main_frame.h"
#include "bll_base.h"


FRAME_GATESERVER_NAMESPACE_BEGIN

class CServerStopMessageEvent : public CBllBase
{
public:
	CServerStopMessageEvent()
	{
	}

	virtual ~CServerStopMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

private:
	void RoomServerStop(ServerID nServerID);
};

#define	CREATE_SERVERSTOPEVENT_INSTANCE		     		CSingleton<CServerStopMessageEvent>::CreateInstance
#define	g_ServerStopMessageEvent			 			CSingleton<CServerStopMessageEvent>::GetInstance()
#define	DESTROY_SERVERSTOPEVENT_INSTANCE	     		CSingleton<CServerStopMessageEvent>::DestroyInstance

FRAME_GATESERVER_NAMESPACE_END

#endif /* BLL_SERVER_SERVERSTOP_H_ */
