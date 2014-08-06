/*
 * dal_room.h
 *
 *  Created on: 2013-1-11
 *      Author: luocj
 */

#ifndef DAL_ROOM_H_
#define DAL_ROOM_H_

#include "common/common_complextype.h"
#include "common/common_hashlist.h"
#include "common/common_string.h"
#include "frame_typedef.h"
#include "dal/dal_player.h"
#include "def/server_namespace.h"

#include "public_typedef.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

typedef CHashList<RoleID, MaxUserCountPerRoom, RoomPlayerBucketSize> RoomPlayerList;

class CRoom : public CObject
{
public:

	CRoom()
	{
		Reset();
	}

	virtual ~CRoom()
	{

	}

	int32_t Initialize()
	{
		Reset();
		return m_sRoomPlayerList.Initialize();
	}

	int32_t Resume()
	{
		Reset();
		return m_sRoomPlayerList.Resume();
	}

	int32_t Uninitialize()
	{
		Reset();
		return m_sRoomPlayerList.Uninitialize();
	}

	int32_t AddPlayer(const RoleID nRoleID);

	int32_t DeletePlayer(const RoleID nRoleID);

	bool IsPlayerInRoom(const RoleID nRoleID);

	int32_t GetAllPlayers(RoleID arrRoleID[], const int32_t arrSize, int32_t &nPlayerCount);

	int32_t DeleteAllPlayers();

	void SetRoomID(const RoomID nRoomID)
	{
		m_nRoomID = nRoomID;
	}

	RoomID GetRoomID() const
	{
		return m_nRoomID;
	}


	void SetRoomServerID(const RoomID nRoomServerID)
	{
		m_nRoomServerID = nRoomServerID;
	}

	ServerID GetRoomServerID() const
	{
		return m_nRoomServerID;
	}

	uint32_t GetUserCount()
	{
		return m_sRoomPlayerList.ObjectCount();
	}
protected:
	void Reset()
	{
		m_nRoomServerID = enmInvalidServerID;
		m_nRoomID = enmInvalidRoomID;
		m_sRoomPlayerList.Initialize();
	}

private:
	RoomPlayerList::Key MakePlayerKey(const RoleID nRoleID) const;

private:
	ServerID							m_nRoomServerID;					//所在的roomserver
	RoomID								m_nRoomID;							//房间ID
	RoomPlayerList						m_sRoomPlayerList;					//玩家对象池
};

FRAME_GATESERVER_NAMESPACE_END

CCOMPLEXTYPE_SPECIFICALLY(FRAME_GATESERVER_NAMESPACE::CRoom)

#endif /* DAL_ROOM_H_ */
