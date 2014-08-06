/*
 * to_client_message.cpp
 *
 *  Created on: 2013-1-18
 *      Author: luocj
 */

#include <memory.h>

//#include "def_message_body.h"
#include "to_client_message.h"
#include "frame_errordef.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

int32_t CLoginResp::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
{
	int32_t nRet = S_OK;
	nRet = CCodeEngine::Encode(buf, size, offset, nResult);
	if (0 > nRet)
	{
		return nRet;
	}

	if(nResult > 0)
	{

		nRet = strFailReason.MessageEncode(buf, size, offset);
		if (0 > nRet)
		{
			return nRet;
		}
		if(nResult==enmLoginResult_AccountLock || nResult==enmLoginResult_IpLock || nResult==enmLoginResult_MacLock)
		{
			nRet = CCodeEngine::Encode(buf, size, offset, nKickDays);
			if (0 > nRet)
			{
				return nRet;
			}
			nRet = strLockEndDate.MessageEncode(buf, size, offset);
			if (0 > nRet)
			{
				return nRet;
			}
		}
	}
	else
	{
		nRet = CCodeEngine::Encode(buf, size, offset, nLastVersion);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nLastLoginTimeHigh32);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nLastLoginTimeLow32);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nSystemTimeHigh32);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nSystemTimeLow32);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nGender);
		if (0 > nRet)
		{
			return nRet;
		}

		nRet = strRoleName.MessageEncode(buf, size, offset);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, n179ID);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nVipLevel);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nUserLevel);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nOnlineTime);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nLeftTime);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nLeftMoney);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nNetType);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nSelfSetting);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nRmbUserType);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nAccountSafeStatus);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nMagnateLevel);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nNextLevelNeedMoney);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(buf, size, offset, nSelf_business);
		if (0 > nRet)
		{
			return nRet;
		}
	}

	return S_OK;
}

int32_t CLoginResp::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
{
	int32_t nRet = S_OK;
	nRet = CCodeEngine::Decode(buf, size, offset, nResult);
	if (0 > nRet)
	{
		return nRet;
	}

	if(nResult > 0)
	{

		nRet = strFailReason.MessageDecode(buf, size, offset);
		if (0 > nRet)
		{
			return nRet;
		}
		if(nResult==enmLoginResult_AccountLock || nResult==enmLoginResult_IpLock || nResult==enmLoginResult_MacLock)
		{
			nRet = CCodeEngine::Decode(buf, size, offset, nKickDays);
			if (0 > nRet)
			{
				return nRet;
			}
			nRet = strLockEndDate.MessageDecode(buf, size, offset);
			if (0 > nRet)
			{
				return nRet;
			}
		}
	}
	else
	{
		nRet = CCodeEngine::Decode(buf, size, offset, nLastVersion);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nLastLoginTimeHigh32);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nLastLoginTimeLow32);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nSystemTimeHigh32);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nSystemTimeLow32);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nGender);
		if (0 > nRet)
		{
			return nRet;
		}

		nRet = strRoleName.MessageDecode(buf, size, offset);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, n179ID);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nVipLevel);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nUserLevel);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nOnlineTime);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nLeftTime);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nLeftMoney);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nNetType);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nSelfSetting);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nRmbUserType);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nAccountSafeStatus);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nMagnateLevel);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nNextLevelNeedMoney);
		if (0 > nRet)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(buf, size, offset, nSelf_business);
		if (0 > nRet)
		{
			return nRet;
		}
	}
	return S_OK;
}

void CLoginResp::Dump(char* buf, const uint32_t size, uint32_t& offset)
{
	sprintf(buf + offset, "{nResult=%d,", nResult);
	offset = (uint32_t)strlen(buf);
	if(nResult > 0)
	{
		sprintf(buf + offset, "strFailReason=%s}", strFailReason.GetString());
		if(nResult==enmLoginResult_AccountLock || nResult==enmLoginResult_IpLock || nResult==enmLoginResult_MacLock)
		{
			offset = (uint32_t)strlen(buf);
			sprintf(buf + offset, "nKickDays=%d,strLockEndDate=%s}", nKickDays,strLockEndDate.GetString());
		}
	}
	else
	{
		sprintf(buf + offset, "nLastVersion=%d,nLastLoginTimeHigh32=%d,nLastLoginTimeLow32=%d,"
				"nSystemTimeHigh32=%d,nSystemTimeLow32=%d,Gender=%d,strRoleName=%s,n179ID=%d,nVipLevel=%d"
				"nUserLevel=%d,nOnlineTime=%d,nLeftTime=%d,nLeftMoney=%d,nNetType=%d,nSelfSetting=%u,"
				"nRmbUserType=%d,nAccountSafeStatus=%d, nMagnateLevel=%d, nNextLevelNeedMoney=%d, nSelf_business=%u}", nLastVersion, nLastLoginTimeHigh32,
				nLastLoginTimeLow32, nSystemTimeHigh32, nSystemTimeLow32, nGender, strRoleName.GetString(), n179ID,
				nVipLevel, nUserLevel, nOnlineTime, nLeftTime, nLeftMoney, nNetType, nSelfSetting, nRmbUserType, nAccountSafeStatus,
				nMagnateLevel, nNextLevelNeedMoney, nSelf_business);
	}
	offset = (uint32_t)strlen(buf);
}

FRAME_GATESERVER_NAMESPACE_END
