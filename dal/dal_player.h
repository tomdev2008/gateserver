/*
 * dal_player.h
 *
FRAME_GATESERVER_NAMESPACE_BEGIN
 */

#ifndef DAL_PLAYER_H_
#define DAL_PLAYER_H_

#include "common/common_complextype.h"
#include "common/common_object.h"
#include "common/common_string.h"
#include "frame_typedef.h"
#include "frame_protocolhead.h"
#include "def/server_namespace.h"
#include "public_typedef.h"
#include "def/server_errordef.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

class CPlayer : public CObject
{
public:
	CPlayer()
	{
		Reset();
	}

	virtual ~CPlayer()
	{

	}

	int32_t Initialize()
	{
		Reset();
		return S_OK;
	}

	int32_t Resume()
	{
		Reset();
		return S_OK;
	}

	int32_t Uninitialize()
	{
		Reset();
		return S_OK;
	}

	void SetRoleID(const RoleID nRoleID)
	{
		m_nRoleID = nRoleID;
	}

	RoleID GetRoleID() const
	{
		return m_nRoleID;
	}

	void SetLogoutReason(const LogoutReason nLogoutReason)
	{
		m_nLogoutReason = nLogoutReason;
	}

	RoleID GetLogoutReason() const
	{
		return m_nLogoutReason;
	}
	void SetConnInfo(const ConnInfo *stConnInfo)
	{
		m_stConnInfo = *stConnInfo;
	}

	ConnInfo* GetConnInfo()
	{
		return &m_stConnInfo;
	}

	void PlayerLoginSuccess()
	{
		m_stConnInfo = m_stConnInfoTemp;
	}

	void SetConnInfoTemp(const ConnInfo *stConnInfo)
	{
		m_stConnInfoTemp = *stConnInfo;
	}

	ConnInfo* GetConnInfoTemp()
	{
		return &m_stConnInfoTemp ;
	}

private:
	void Reset()
	{
		m_nRoleID = enmInvalidRoleID;
		m_nLogoutReason = enmLogoutReason_Unknown;
		m_stConnInfo.Reset();
		m_stConnInfoTemp.Reset();
	}
private:
	RoleID				m_nRoleID;
	LogoutReason		m_nLogoutReason;
	ConnInfo 			m_stConnInfo;
	ConnInfo			m_stConnInfoTemp;
};

FRAME_GATESERVER_NAMESPACE_END

CCOMPLEXTYPE_SPECIFICALLY(FRAME_GATESERVER_NAMESPACE::CPlayer)

#endif /* DAL_PLAYER_H_ */


