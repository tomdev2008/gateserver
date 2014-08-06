/*
 * server_def.h
 *
 *  Created on: 2013-1-14
 *      Author: luocj
 */

#ifndef SERVER_DEF_H_
#define SERVER_DEF_H_

#include "def/server_namespace.h"
#include "frame_typedef.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

enum
{
	enmMaxRoomMessageSize					= (15 * 1024),	//最大到roomserver的消息, 15k
};
//会话类型定义
enum
{
	enmSessionType_Invalid = 0x00,
	enmSessionType_GetRoomServerID = 0x01, //获取角色信息
};

enum
{
	enmCheckBeatTimePeriod  = 2* US_PER_MINUTE,
};

typedef uint32_t LoginResult;
enum
{
	enmLoginResult_OK = 0x00,
	enmLoginResult_AccountLock = 0x01,
	enmLoginResult_IpLock = 0x02,
	enmLoginResult_MacLock = 0x03,
	enmLoginResult_Timeout = 0x04,
	enmLoginResult_VerError = 0x05,
	enmLoginResult_Unknown = 0xff,
};
FRAME_GATESERVER_NAMESPACE_END
#endif /* SERVER_DEF_H_ */
