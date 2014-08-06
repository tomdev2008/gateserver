/*
 * bll_event_hallrun.h
 *
 *  Created on: 2013-1-16
 *      Author: luocj
 */

#ifndef BLL_EVENT_HALLRUN_H_
#define BLL_EVENT_HALLRUN_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "global.h"
#include "main_frame.h"
#include "bll_base.h"
#include "hallserver_message_define.h"
#include "ctl/data_center.h"
#include "def/from_client_message.h"
#include "config/msg_cs_config.h"


FRAME_GATESERVER_NAMESPACE_BEGIN

class CHallRunMessageEvent : public CBllBase
{
public:
	CHallRunMessageEvent()
	{
	}

	virtual ~CHallRunMessageEvent()
	{
	}

	//消息事件
	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);
private:
	int32_t SendLogoutMessage(RoleID nRoleID);
};

#define	CREATE_HALLRUNEVENT_INSTANCE		     		CSingleton<CHallRunMessageEvent>::CreateInstance
#define	g_HallRunMessageEvent			 			CSingleton<CHallRunMessageEvent>::GetInstance()
#define	DESTROY_HALLRUNEVENT_INSTANCE	     		CSingleton<CHallRunMessageEvent>::DestroyInstance

FRAME_GATESERVER_NAMESPACE_END
#endif /* BLL_EVENT_HALLRUN_H_ */
