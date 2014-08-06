/*
 * bll_event_heartbeat.h
 *
 *  Created on: 2013-1-17
 *      Author: luocj
 */

#ifndef BLL_EVENT_HEARTBEAT_H_
#define BLL_EVENT_HEARTBEAT_H_

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

class CHeartBeatMessageEvent : public IMessageEventCS
{
public:
	CHeartBeatMessageEvent()
	{
	}

	virtual ~CHeartBeatMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadCS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

private:

};

#define	CREATE_HEARTBEATEVENT_INSTANCE		CSingleton<CHeartBeatMessageEvent>::CreateInstance
#define	g_HeartBeatMessageEvent				CSingleton<CHeartBeatMessageEvent>::GetInstance()
#define	DESTROY_EHEARTBEATEVENT_INSTANCE	CSingleton<CHeartBeatMessageEvent>::DestroyInstance

FRAME_GATESERVER_NAMESPACE_END

#endif /* BLL_EVENT_HEARTBEAT_H_ */
