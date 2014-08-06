/*
 * bll_timer_checkbeat.h
 *
 *  Created on: 2013-1-18
 *      Author: luocj
 */

#ifndef BLL_TIMER_CHECKBEAT_H_
#define BLL_TIMER_CHECKBEAT_H_

#include "frame_namespace.h"
#include "frame_def.h"
#include "lightframe_impl.h"
#include <stdlib.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "main_frame.h"
#include "bll_base.h"
#include "common/common_datetime.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

class CCheckBeatEvent : public ITimerEvent
{
public:
	CCheckBeatEvent(){}
	virtual ~CCheckBeatEvent(){}
	virtual int32_t OnTimerEvent(CFrameTimer *pTimer);
private:
};

#define	CREATE_CHECKBEATEVENT_INSTANCE		CSingleton<CCheckBeatEvent>::CreateInstance
#define	g_CheckBeatEvent					CSingleton<CCheckBeatEvent>::GetInstance()
#define	DESTROY_CHECKBEATEVENT_INSTANCE		CSingleton<CCheckBeatEvent>::DestroyInstance

FRAME_GATESERVER_NAMESPACE_END

#endif /* BLL_TIMER_CHECKBEAT_H_ */
