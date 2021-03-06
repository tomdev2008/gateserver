/*
 * player_mgt.h
 *
 *  Created on: 2013-1-11
 *      Author: luocj
 */

#ifndef PLAYER_MGT_H_
#define PLAYER_MGT_H_

#include "common/common_pool.h"
#include "common/common_hashlist.h"
#include "dal/dal_player.h"
#include "def/server_namespace.h"

#include <vector>

using namespace std;

FRAME_GATESERVER_NAMESPACE_BEGIN

//定义玩家对象池和索引类型
typedef CPool<CPlayer, MaxOnlinePlayerCount>	PlayerPool;
typedef CHashList<PlayerIndex, MaxOnlinePlayerCount, enmPlayerBucketSize>	PlayerList;

class CPlayerMgt : public CObject
{
protected:
	enum
	{
		enmAdditionalIndex_HashListIndex			= 0,
	};
public:
	CPlayerMgt();
	virtual ~CPlayerMgt();

	//初始化玩家管理器
	virtual int32_t Initialize();
	//恢复玩家管理器
	virtual int32_t Resume();
	//注销玩家管理器
	virtual int32_t Uninitialize();

	int32_t CreatePlayer(const RoleID nRoleID, CPlayer *&pPlayer);

	int32_t GetPlayer(const RoleID nRoleID, CPlayer *&pPlayer);

	int32_t GetAllPlayer(CPlayer *arrPlayerObject[], const int32_t arrSize, int32_t &nPlayerCount);

	int32_t GetAllPlayer(RoleID arrRole[], const int32_t arrSize, int32_t &nPlayerCount);

	int32_t DestroyPlayer(const RoleID nRoleID);

	int32_t Clear();

	//获取对象池容量
	uint32_t GetCapacity() const;
	//获取对象池中对象数量
	uint32_t GetPlayerCount() const;
	//对象池是否为空
	bool IsEmpty() const;
	//对象池是否已满
	bool IsFull() const;

protected:
	int32_t CreatePlayer(const RoleID nRoleID, PlayerPool::CIndex *&pIndex);

	int32_t GetPlayer(const RoleID nRoleID, PlayerPool::CIndex*& pIndex);

protected:
	PlayerList::Key MakeUserKey(const RoleID nRoleID) const;
private:
	PlayerPool					m_sPlayerPool;
	PlayerList					m_sPlayerList;
public:
	vector<RoleID>				m_arrNeedLogout;   						//保存大厅处于关闭的时候登出的玩家
};

FRAME_GATESERVER_NAMESPACE_END

#endif /* PLAYER_MGT_H_ */
