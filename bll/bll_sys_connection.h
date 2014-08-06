/*
 * bll_sys_connection.h
 *
 *  Created on: 2013-1-11
 *      Author: luocj
 */

#ifndef BLL_SYS_CONNECTION_H_
#define BLL_SYS_CONNECTION_H_

#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "main_frame.h"
#include "bll_base.h"
#include "config/msg_cs_config.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

class CSYSEvent : public CBllBase, public ISystemEvent
{
public:
	CSYSEvent()
	{

	}

	virtual ~CSYSEvent()
	{

	}

	virtual int32_t OnSystemEvent(uint16_t nEventID, void *pParam);

private:

};

#define	CREATE_SYSEVENT_INSTANCE				CSingleton<CSYSEvent>::CreateInstance
#define	g_SYSEvent								CSingleton<CSYSEvent>::GetInstance()
#define	DESTROY_SYSEVENT_INSTANCE				CSingleton<CSYSEvent>::DestroyInstance

FRAME_GATESERVER_NAMESPACE_END

#endif /* BLL_SYS_CONNECTION_H_ */
