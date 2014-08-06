/*
 * from_client_message.h
 *
 *  Created on: 2013-1-11
 *      Author: luocj
 */

#ifndef FROM_CLIENT_MESSAGE_H_
#define FROM_CLIENT_MESSAGE_H_

#include "common/common_singleton.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_msg_impl.h"
#include "common/common_string.h"
#include "public_typedef.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

//登录请求
#define MSGID_CLHS_LOGIN_REQ			0x00018001

//登出请求
#define MSGID_CLHS_LOGOUT_REQ			0x00018002

class CLogoutReq : public IMsgBody
{
	SIZE_INTERFACE()
public:
	CLogoutReq()
	{
		nLogoutReason = enmLogoutReason_Unknown;
	}
	int32_t MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
	int32_t MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
	void Dump(char* buf, const uint32_t size, uint32_t& offset);

public:
	LogoutReason nLogoutReason;
};


//心跳
#define MSGID_CLHS_HEART_BEAT			0x00018005

class CPlayerHeartBeat : public IMsgBody
{
	SIZE_INTERFACE()
public:
	int32_t MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
	int32_t MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
	void Dump(char* buf, const uint32_t size, uint32_t& offset);
public:
	RoleID						nRoleID;
	uint8_t						nRoomCount;
	RoomID						arrRoomID[MaxEnterRoomCount];
};

FRAME_GATESERVER_NAMESPACE_END

#endif /* FROM_CLIENT_MESSAGE_H_ */
