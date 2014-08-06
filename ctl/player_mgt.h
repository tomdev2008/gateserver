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

//������Ҷ���غ���������
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

	//��ʼ����ҹ�����
	virtual int32_t Initialize();
	//�ָ���ҹ�����
	virtual int32_t Resume();
	//ע����ҹ�����
	virtual int32_t Uninitialize();

	int32_t CreatePlayer(const RoleID nRoleID, CPlayer *&pPlayer);

	int32_t GetPlayer(const RoleID nRoleID, CPlayer *&pPlayer);

	int32_t GetAllPlayer(CPlayer *arrPlayerObject[], const int32_t arrSize, int32_t &nPlayerCount);

	int32_t GetAllPlayer(RoleID arrRole[], const int32_t arrSize, int32_t &nPlayerCount);

	int32_t DestroyPlayer(const RoleID nRoleID);

	int32_t Clear();

	//��ȡ���������
	uint32_t GetCapacity() const;
	//��ȡ������ж�������
	uint32_t GetPlayerCount() const;
	//������Ƿ�Ϊ��
	bool IsEmpty() const;
	//������Ƿ�����
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
	vector<RoleID>				m_arrNeedLogout;   						//����������ڹرյ�ʱ��ǳ������
};

FRAME_GATESERVER_NAMESPACE_END

#endif /* PLAYER_MGT_H_ */
