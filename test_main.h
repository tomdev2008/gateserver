/*
 * test_main.h
 *
 *  Created on: 2011-12-12
 *      Author: luocj
 */

#ifndef TEST_MAIN_H_
#define TEST_MAIN_H_

#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_protocolhead.h"
#include "public_typedef.h"
#include "hallserver_message_define.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

static uint8_t g_messageHead[4096] = {0};
static uint8_t g_messageBody[65535*10] = {0};

class CTestMain
{
public:
	CTestMain();
	virtual ~CTestMain();

public:
	//²âÊÔÇëÇó´¦Àí
	static void OnTestRequest()
	{
		printf("Enter msgid(hex):\n");
		uint32_t command = 0;
		scanf("%x", &command);

		switch(command)
		{
			case MSGID_HSTA_ONLINE_NOTI:
			OnlineMessageRequest(command);
			break;
			case MSGID_HSTA_OFFLINE_NOTI:
			OfflineMessageRequest(command);
			break;

			default:
				break;
		}
	}

	static void OfflineMessageRequest(uint32_t msgid)
	{
		printf("Enter roleid:");
		RoleID nRoleID = 0;
		scanf("%d", &nRoleID);

		MessageHeadSS *pMessageHead = new(g_messageHead)MessageHeadSS;
		GenerateMessageHead(msgid, nRoleID, enmInvalidRoomID, *pMessageHead);

		COfflineNoti *pMessageBody = new(g_messageBody)COfflineNoti;
		pMessageBody->nRoleID = nRoleID;

		g_Frame.PushCommand(pMessageHead,pMessageBody);
	}

	static void OnlineMessageRequest(uint32_t msgid)
	{
		printf("Enter roleid:");
		RoleID nRoleID = 0;
		scanf("%d", &nRoleID);

		MessageHeadSS *pMessageHead = new(g_messageHead)MessageHeadSS;
		GenerateMessageHead(msgid, nRoleID, enmInvalidRoomID, *pMessageHead);

		COnlineNoti *pMessageBody = new(g_messageBody)COnlineNoti;
		pMessageBody->nRoleID = nRoleID;

		g_Frame.PushCommand(pMessageHead,pMessageBody);
	}

	static void GenerateMessageHead(uint32_t nMessageID, RoleID nRoleID,
			RoomID nRoomID, MessageHeadSS& stMessageHead)
	{
		stMessageHead.nMessageID = nMessageID;
		stMessageHead.nMessageType = enmMessageType_Request;
		stMessageHead.nSourceType = 0;
		stMessageHead.nDestType = 0;
		stMessageHead.nSourceID = 1;
		stMessageHead.nDestID = 1;
		stMessageHead.nRoleID = nRoleID;
		stMessageHead.nSequence = 0;
		stMessageHead.nSessionIndex = enmInvalidSessionIndex;
		stMessageHead.nRoomID = nRoomID;

		stMessageHead.nTransType = enmTransType_ByKey;
		stMessageHead.nHeadSize = 0;
		stMessageHead.nTotalSize = 0;
	}
};

FRAME_GATESERVER_NAMESPACE_END

#endif /* TEST_MAIN_H_ */
