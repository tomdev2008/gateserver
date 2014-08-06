/*
 * dal_room.cpp
 *
 *  Created on: 2013-1-11
 *      Author: luocj
 */

#include "dal_room.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

int32_t CRoom::AddPlayer(const RoleID nRoleID)
{
	RoomPlayerList::Key key = MakePlayerKey(nRoleID);
	RoomPlayerList::CIndex* pIndex = m_sRoomPlayerList.Insert(key, nRoleID);

	return S_OK;
}

int32_t CRoom::DeletePlayer(const RoleID nRoleID)
{
	RoomPlayerList::Key key = MakePlayerKey(nRoleID);
	int32_t ret = m_sRoomPlayerList.Erase(key);

	return ret;
}

bool CRoom::IsPlayerInRoom(const RoleID nRoleID)
{
	RoomPlayerList::Key key = MakePlayerKey(nRoleID);
	RoomPlayerList::CIndex *pIndex = m_sRoomPlayerList.Find(key);

	return NULL != pIndex;
}


int32_t CRoom::GetAllPlayers(RoleID arrRoleID[], const int32_t arrSize, int32_t &nPlayerCount)
{
	nPlayerCount = 0;
	RoomPlayerList::CIndex *pIndex = m_sRoomPlayerList.First();

	while(NULL != pIndex && nPlayerCount < arrSize)
	{
		arrRoleID[nPlayerCount++] = pIndex->Object();
		pIndex = pIndex->Next();
	}

	return S_OK;
}

int32_t CRoom::DeleteAllPlayers()
{
	RoleID arrRoleID[MaxUserCountPerRoom];
	int32_t nPlayerCount = 0;

	GetAllPlayers(arrRoleID, MaxUserCountPerRoom, nPlayerCount);

	for(int32_t i = 0; i < nPlayerCount; ++i)
	{
		DeletePlayer(arrRoleID[i]);
	}

	return S_OK;
}

RoomPlayerList::Key CRoom::MakePlayerKey(const RoleID nRoleID) const
{
	RoomPlayerList::Key key = { 0 };
	key.nKeyLength = (uint32_t)sizeof(RoleID);
	*(RoleID*)key.arrKey = nRoleID;

	return key;
}


FRAME_GATESERVER_NAMESPACE_END
