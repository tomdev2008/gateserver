/*
 * player_mgt.cpp
 *
 *  Created on: 2013-1-11
 *      Author: luocj
 */

#include "frame_errordef.h"
#include "player_mgt.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

CPlayerMgt::CPlayerMgt()
{
}
CPlayerMgt::~CPlayerMgt()
{

}

//��ʼ����ҹ�����
int32_t CPlayerMgt::Initialize()
{
	m_sPlayerPool.Initialize();
	m_sPlayerList.Initialize();

	m_arrNeedLogout.clear();

	return S_OK;
}

//�ָ���ҹ�����
int32_t CPlayerMgt::Resume()
{
	m_sPlayerPool.Resume();
	m_sPlayerList.Resume();

	m_arrNeedLogout.clear();

	return S_OK;
}

//ע����ҹ�����
int32_t CPlayerMgt::Uninitialize()
{
	m_sPlayerPool.Uninitialize();
	m_sPlayerList.Uninitialize();

	m_arrNeedLogout.clear();

	return S_OK;
}

int32_t CPlayerMgt::CreatePlayer(const RoleID nRoleID, PlayerPool::CIndex *&pIndex)
{
	pIndex = m_sPlayerPool.CreateObject();
	if(NULL == pIndex)
	{
		return E_LIBF_CREATEPLAYER;
	}

	PlayerList::Key key = MakeUserKey(nRoleID);
	PlayerList::CIndex *pListIndex = m_sPlayerList.Insert(key, pIndex->Index());
	if (NULL == pListIndex)
	{
		m_sPlayerPool.DestroyObject(pIndex);
		pIndex = NULL;
		return E_LIBF_CREATEPLAYERINDEX;
	}

	//���������浽�������ݱ���
	int32_t ret = pIndex->SetAdditionalData(enmAdditionalIndex_HashListIndex, (uint32_t)pListIndex->Index());
	if (0 > ret)
	{
		m_sPlayerList.Erase(pListIndex);
		m_sPlayerPool.DestroyObject(pIndex);
		pIndex = NULL;
		return ret;
	}

	return S_OK;
}

int32_t CPlayerMgt::CreatePlayer(const RoleID nRoleID, CPlayer *&pPlayer)
{
	PlayerPool::CIndex* pIndex = NULL;

	int32_t ret = CreatePlayer(nRoleID, pIndex);
	if (0 > ret)
	{
		return ret;
	}

	pPlayer = pIndex->ObjectPtr();
	pPlayer->SetRoleID(nRoleID);

	return S_OK;
}

int32_t CPlayerMgt::GetPlayer(const RoleID nRoleID, PlayerPool::CIndex*& pIndex)
{
	//������������
	PlayerList::Key key = MakeUserKey(nRoleID);
	PlayerList::CIndex* pListIndex = m_sPlayerList.Find(key);
	if (NULL == pListIndex)
	{
		return E_LIBF_PLAYERNOTFOUND;
	}

	pIndex = m_sPlayerPool.GetIndex(pListIndex->GetObject());
	if (NULL == pIndex)
	{
		return E_LIBF_PLAYERNOTFOUND;
	}

	return S_OK;
}

int32_t CPlayerMgt::GetPlayer(const RoleID nRoleID, CPlayer *&pPlayer)
{
	PlayerPool::CIndex* pIndex = NULL;

	int32_t ret = GetPlayer(nRoleID, pIndex);
	if (0 > ret)
	{
		return ret;
	}

	pPlayer = pIndex->ObjectPtr();

	return S_OK;
}

int32_t CPlayerMgt::DestroyPlayer(const RoleID nRoleID)
{
	//������������
	PlayerList::Key key = MakeUserKey(nRoleID);
	PlayerList::CIndex* pListIndex = m_sPlayerList.Find(key);
	if (NULL == pListIndex)
	{
		return E_LIBF_PLAYERNOTEXIST;
	}

	PlayerIndex playerIndex = pListIndex->GetObject();

	//����ҴӶ���ؼ���������ɾ��
	m_sPlayerList.Erase(pListIndex);
	m_sPlayerPool.DestroyObject(playerIndex);

	return S_OK;
}

int32_t CPlayerMgt::GetAllPlayer(CPlayer *arrPlayerObject[], const int32_t arrSize, int32_t &nPlayerCount)
{
	nPlayerCount = 0;
	PlayerPool::CIndex *pIndex = m_sPlayerPool.First();

	while(NULL != pIndex && nPlayerCount < arrSize)
	{
		arrPlayerObject[nPlayerCount++] = pIndex->ObjectPtr();
		pIndex = pIndex->Next();
	}

	return S_OK;
}

int32_t CPlayerMgt::GetAllPlayer(RoleID arrRole[], const int32_t arrSize, int32_t &nPlayerCount)
{
	nPlayerCount = 0;
	PlayerPool::CIndex *pIndex = m_sPlayerPool.First();

	while(NULL != pIndex && nPlayerCount < arrSize)
	{
		if(NULL == pIndex->ObjectPtr())
		{
			pIndex = pIndex->Next();
			continue;
		}
		arrRole[nPlayerCount++] = pIndex->ObjectPtr()->GetRoleID();
		pIndex = pIndex->Next();
	}

	return S_OK;
}


int32_t CPlayerMgt::Clear()
{
	m_sPlayerPool.Clear();
	m_sPlayerList.Clear();
	return S_OK;
}

//��ȡ���������
uint32_t CPlayerMgt::GetCapacity() const
{
	return m_sPlayerPool.Capacity();
}

//��ȡ������ж�������
uint32_t CPlayerMgt::GetPlayerCount() const
{
	return m_sPlayerPool.ObjectCount();
}

//������Ƿ�Ϊ��
bool CPlayerMgt::IsEmpty() const
{
	return m_sPlayerPool.IsEmpty();
}

//������Ƿ�����
bool CPlayerMgt::IsFull() const
{
	return m_sPlayerPool.IsFull();
}

PlayerList::Key CPlayerMgt::MakeUserKey(const RoleID nRoleID) const
{
	PlayerList::Key key = { 0 };
	key.nKeyLength = (uint32_t)sizeof(RoleID);
	*(RoleID*)key.arrKey = nRoleID;

	return key;
}

FRAME_GATESERVER_NAMESPACE_END

