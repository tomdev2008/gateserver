/*
 * to_client_message.h
 *
 *  Created on: 2013-1-18
 *      Author: luocj
 */

#ifndef TO_CLIENT_MESSAGE_H_
#define TO_CLIENT_MESSAGE_H_

#include "common/common_singleton.h"
#include "def/server_namespace.h"
#include "frame_typedef.h"
#include "frame_msg_impl.h"
#include "common/common_string.h"
#include "common/common_codeengine.h"
#include "public_typedef.h"
#include "server_def.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

#define MSGID_HSCL_KICKUSER_NOTI  0x00030405

#define MSGID_HSCL_LOGIN_RESP  0x00020401
class CLoginResp : public IMsgBody
{
	SIZE_INTERFACE()
public:
	int32_t		MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset);
	int32_t		MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset);
	void		Dump(char* buf, const uint32_t size, uint32_t& offset);
public:
	uint32_t	nResult;
	CString<MaxLoginFailReasonLength>	strFailReason;
	uint16_t	nKickDays;//表示被踢多少天
	CString<enmMaxDateTimeStringLength> strLockEndDate;//封停截止日期
	uint32_t	nLastVersion;
	uint32_t	nLastLoginTimeHigh32;
	uint32_t	nLastLoginTimeLow32;
	uint32_t	nSystemTimeHigh32;
	uint32_t	nSystemTimeLow32;
	Gender		nGender;
	CString<MaxRoleNameLength>	strRoleName;
	uint32_t	n179ID;
	VipLevel	nVipLevel;
	UserLevel	nUserLevel;
	uint16_t	nOnlineTime;
	uint16_t	nLeftTime;
	uint32_t	nLeftMoney;
	NetType		nNetType;
	uint32_t	nSelfSetting;
	uint8_t     nRmbUserType;//0为非付费用户，1为付费用户
	uint8_t     nAccountSafeStatus;//0为未设置密保，1为设置了密保
	MagnateLevel nMagnateLevel;            //富豪等级
	uint32_t    nNextLevelNeedMoney;       //到下一级还需要的消费额
	IdentityType nSelf_business;
};

MESSAGEMAP_DECL_BEGIN(to_client)

MESSAGEMAP_DECL_MSG(MSGID_HSCL_LOGIN_RESP, (new CLoginResp()))

MESSAGEMAP_DECL_END()
FRAME_GATESERVER_NAMESPACE_END
#endif /* TO_CLIENT_MESSAGE_H_ */
