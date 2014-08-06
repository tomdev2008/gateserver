/*
 * bll_cs_event.h
 *
 *  Created on: 2013-1-10
 *      Author: luocj
 */

#ifndef BLL_CS_EVENT_H_
#define BLL_CS_EVENT_H_

#include <stdlib.h>
#include <time.h>
#include "common/common_api.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_errordef.h"
#include "frame_configmgt.h"
#include "main_frame.h"
#include "bll_base.h"
#include "def/server_def.h"
#include "ctl/data_center.h"
#include "tunnelserver_message_define.h"
#include "global.h"


FRAME_GATESERVER_NAMESPACE_BEGIN

class CCSEvent : public CBllBase, public IDefaultMessageCS
{
public:
	CCSEvent(){}

	virtual ~CCSEvent(){}

	virtual int32_t OnMessageEvent(MessageHeadCS * pMsgHead, const uint8_t* pBuf,const uint32_t nBufLen,
			const uint16_t nOptionLen = 0, const void *pOptionData = NULL);
	//会话事件激活
	virtual int32_t OnSessionEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			CFrameSession* pSession, const uint16_t nOptionLen = 0, const void *pOptionData = NULL);


private:

	int32_t GetRoomServerID(RoomID nRoomID);

	int32_t GetRoomServerIDResp(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,CFrameSession* pSession);

private:
	class CGetRoomServerIDSession : public ISessionData
	{
		SESSION_INTERFACE() ;
	public:
		MessageHeadCS 	m_sMsgHead;
		uint32_t	  	m_nBodyLen;
		uint8_t 	  	m_pMsgBodyBuf[enmMaxRoomMessageSize];
		uint16_t 	 	m_nOptionLen;
		char          	m_arrOptionData[MaxOptionDataSize];

		CGetRoomServerIDSession()
		{
			memset(&m_sMsgHead, 0, sizeof(m_sMsgHead));
			m_nBodyLen = 0;
			memset(&m_pMsgBodyBuf, 0, sizeof(m_pMsgBodyBuf));
			m_nOptionLen = 0;
			memset(&m_arrOptionData, 0, sizeof(m_arrOptionData));
		}

		void Init(MessageHeadCS *pMsgHead,const uint8_t* pBuf,const uint32_t nBufLen,
				const uint16_t nOptionLen, const void *pOptionData,ISessionEvent *pHandler)
		{
			m_sMsgHead = *pMsgHead;
			if(nBufLen > 0)
			{
				m_nBodyLen = nBufLen;
				memcpy(m_pMsgBodyBuf,pBuf,nBufLen);
			}

			if(nOptionLen > 0)
			{
				m_nOptionLen = nOptionLen;
				memcpy(m_arrOptionData, pOptionData, nOptionLen);
			}
			SetSessionData(pMsgHead->nRoomID, pMsgHead->nRoleID, enmSessionType_GetRoomServerID,pHandler);
		}
	};
};

#define	CREATE_CSEVENT_INSTANCE					CSingleton<CCSEvent>::CreateInstance
#define	g_CCSEvent								CSingleton<CCSEvent>::GetInstance()
#define	DESTROY_CSEVENT_INSTANCE				CSingleton<CCSEvent>::DestroyInstance

FRAME_GATESERVER_NAMESPACE_END
#endif /* BLL_CS_EVENT_H_ */
