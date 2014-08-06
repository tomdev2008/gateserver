/*
 *  bll_event_exitroom.h
 *
 *  To do：
 *  Created on: 	2012-12-21
 *  Author: 		luocj
 */

#ifndef BLL_EVENT_EXITROOM_H_
#define BLL_EVENT_EXITROOM_H_

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

class CExitRoomNoticeMessageEvent : public CBllBase
{
public:
	CExitRoomNoticeMessageEvent()
	{
	}

	virtual ~CExitRoomNoticeMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

private:

};

#define	CREATE_EXITROOMNOTICEEVENT_INSTANCE		CSingleton<CExitRoomNoticeMessageEvent>::CreateInstance
#define	g_ExitRoomNoticeMessageEvent			CSingleton<CExitRoomNoticeMessageEvent>::GetInstance()
#define	DESTROY_EXITROOMNOTICEEVENT_INSTANCE	CSingleton<CExitRoomNoticeMessageEvent>::DestroyInstance

FRAME_GATESERVER_NAMESPACE_END

#endif /* BLL_EVENT_EXITROOM_H_ */
