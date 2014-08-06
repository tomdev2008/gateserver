/*
 * bll_ss_event.h
 *
 *  Created on: 2013-1-10
 *      Author: luocj
 */

#ifndef BLL_SS_EVENT_H_
#define BLL_SS_EVENT_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "frame_configmgt.h"
#include "main_frame.h"
#include "bll_base.h"


FRAME_GATESERVER_NAMESPACE_BEGIN

class CSSEvent : public CBllBase, public IDefaultMessageSS
{
public:
	CSSEvent(){}

	virtual ~CSSEvent(){}

	virtual int32_t OnMessageEvent(MessageHeadSS * pMsgHead, const uint8_t* pBuf,const uint32_t nBufLen,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

private:
	int32_t SendMeesageToClient(uint32_t nMsgID,RoleID nRoleID,RoomID nRoomID,uint32_t nSequence,const uint8_t* pBuf,const uint32_t nBufLen);

	int32_t PlayerLoginResp(MessageHeadSS * pMsgHead, const uint8_t* pBuf,const uint32_t nBufLen,const uint16_t nOptionLen = 0, const void *pOptionData = NULL);

};

#define	CREATE_SSEVENT_INSTANCE					CSingleton<CSSEvent>::CreateInstance
#define	g_CSSEvent								CSingleton<CSSEvent>::GetInstance()
#define	DESTROY_SSEVENT_INSTANCE				CSingleton<CSSEvent>::DestroyInstance

FRAME_GATESERVER_NAMESPACE_END
#endif /* BLL_SS_EVENT_H_ */
