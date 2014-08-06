/*
 * from_client_message_req.cpp
 *
 *  Created on: 2013-1-11
 *      Author: luocj
 */

#include <memory.h>

#include "from_client_message.h"
#include "frame_errordef.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

int32_t CLogoutReq::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
{
	int32_t nRet = S_OK;
	nRet = CCodeEngine::Encode(buf, size, offset, nLogoutReason);
	if (0 > nRet)
	{
		return nRet;
	}
	return nRet;
}

int32_t CLogoutReq::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
{
	int32_t nRet = S_OK;
	nRet = CCodeEngine::Decode(buf, size, offset, nLogoutReason);
	if (0 > nRet)
	{
		return nRet;
	}
	return nRet;

}
void CLogoutReq::Dump(char* buf, const uint32_t size, uint32_t& offset)
{
	sprintf(buf + offset, "{nLogoutReason=%d}", nLogoutReason);
	offset = (uint32_t)strlen(buf);
}

int32_t CPlayerHeartBeat::MessageEncode(uint8_t* buf, const uint32_t size, uint32_t& offset)
{
	int32_t nRet = S_OK;

	nRet = CCodeEngine::Encode(buf, size, offset, nRoleID);
	if (nRet < 0)
	{
		return nRet;
	}

	nRet = CCodeEngine::Encode(buf, size, offset, nRoomCount);
	if (nRet < 0)
	{
		return nRet;
	}

	if(nRoomCount>MaxEnterRoomCount)
	{
		return ENCODE_DECODE_FILED;
	}
	for(int32_t i = 0;i<nRoomCount;i++)
	{
		nRet = CCodeEngine::Encode(buf, size, offset, arrRoomID[i] );
		if (nRet < 0)
		{
			return nRet;
		}
	}
	return S_OK;
}

int32_t CPlayerHeartBeat::MessageDecode(const uint8_t *buf, const uint32_t size, uint32_t& offset)
{
	int32_t nRet = S_OK;
	nRet = CCodeEngine::Decode(buf, size, offset, nRoleID);
	if (nRet < 0)
	{
		return nRet;
	}

	nRet = CCodeEngine::Decode(buf, size, offset, nRoomCount);
	if (nRet < 0)
	{
		return nRet;
	}
	if(nRoomCount>MaxEnterRoomCount)
	{
		return ENCODE_DECODE_FILED;
	}
	for(int32_t i = 0;i<nRoomCount;i++)
	{
		nRet = CCodeEngine::Decode(buf, size, offset, arrRoomID[i] );
		if (nRet < 0)
		{
			return nRet;
		}
	}
	return S_OK;

}

void CPlayerHeartBeat::Dump(char* buf, const uint32_t size, uint32_t& offset)
{
	sprintf(buf + offset, "{nRoleID=%d,nRoomCount=%d arrRoomID={",nRoleID,nRoomCount);
	offset = (uint32_t)strlen(buf);
	for(int32_t i =0 ;i<nRoomCount&&i<MaxEnterRoomCount;i++)
	{
		sprintf(buf + offset, "%d",arrRoomID[i]);
		offset = (uint32_t)strlen(buf);
		if(i+1!=nRoomCount)
		{
			sprintf(buf + offset, ", ");
			offset = (uint32_t)strlen(buf);
		}
	}
	sprintf(buf + offset, "}}");
	offset = (uint32_t)strlen(buf);
}
FRAME_GATESERVER_NAMESPACE_END

