/*
 *  data_center.h
 *
 *  To do：
 *  Created on: 	2012-12-19
 *  Author: 		luocj
 */

#ifndef DATA_CENTER_H_
#define DATA_CENTER_H_

#include "common/common_singleton.h"
#include "def/server_namespace.h"
#include "def/server_errordef.h"
#include "player_mgt.h"
#include "room_mgt.h"
#include "lightframe_impl.h"
#include "global.h"

#include "public_typedef.h"

#include <list>

using namespace std;

FRAME_GATESERVER_NAMESPACE_BEGIN

class CServerDataCenter : public IDataCenter
{
public:
	CServerDataCenter()
	{
		m_pPlayerMgt = NULL;
		m_pRoomMgt = NULL;
		m_NeedRecvHeartBeatList.clear();
		m_RecvHeartBeatList.clear();
	}
	virtual ~CServerDataCenter()
	{

	}

public:
	virtual const char* GetName()
	{
		return SERVER_NAME_STRING"DataCenter";
	}

	//向Frame提供自己所需内存大小的查询接口
	virtual size_t GetSize()
	{
		return sizeof(CPlayerMgt) + sizeof(CRoomMgt);
	}

	//初始化接口调用(内存自己分配)
	virtual int32_t Initialize()
	{
		int32_t nRet = S_OK;
		m_pPlayerMgt = new CPlayerMgt();
		if(NULL == m_pPlayerMgt)
		{
			return E_RS_CREATEDATAMGTERROR;
		}

		nRet = m_pPlayerMgt->Initialize();
		if(0 > nRet)
		{
			return nRet;
		}

		m_pRoomMgt = new CRoomMgt();
		if(NULL == m_pRoomMgt)
		{
			return E_RS_CREATEDATAMGTERROR;
		}

		nRet = m_pRoomMgt->Initialize();
		if(0 > nRet)
		{
			return nRet;
		}
		m_NeedRecvHeartBeatList.clear();
		m_RecvHeartBeatList.clear();

		return nRet;
	}
	virtual int32_t Resume()
	{
		int32_t nRet = S_OK;
		m_pPlayerMgt = new CPlayerMgt();
		if(NULL == m_pPlayerMgt)
		{
			return E_RS_CREATEDATAMGTERROR;
		}

		nRet = m_pPlayerMgt->Resume();
		if(0 > nRet)
		{
			return nRet;
		}

		m_pRoomMgt = new CRoomMgt();
		if(NULL == m_pRoomMgt)
		{
			return E_RS_CREATEDATAMGTERROR;
		}

		nRet = m_pRoomMgt->Resume();
		if(0 > nRet)
		{
			return nRet;
		}
		m_NeedRecvHeartBeatList.clear();
		m_RecvHeartBeatList.clear();

		return nRet;
	}
	virtual int32_t Uninitialize()
	{
		if(m_pPlayerMgt != NULL)
		{
			delete m_pPlayerMgt;
			m_pPlayerMgt = NULL;
		}

		if(m_pRoomMgt != NULL)
		{
			delete m_pRoomMgt;
			m_pRoomMgt = NULL;
		}
		m_NeedRecvHeartBeatList.clear();
		m_RecvHeartBeatList.clear();

		return S_OK;
	}

public:
	//获取玩家管理器对象
	CPlayerMgt& GetPlayerMgt()
	{
		return *m_pPlayerMgt;
	}

	CRoomMgt& GetRoomMgt()
	{
		return *m_pRoomMgt;
	}
private:
	CPlayerMgt            *m_pPlayerMgt;
	CRoomMgt			  *m_pRoomMgt;

public:
	list<RoleID>   		   m_NeedRecvHeartBeatList;
	list<RoleID>   		   m_RecvHeartBeatList;
};

#define	CREATE_DALDATACENTER_INSTANCE		CSingleton<CServerDataCenter>::CreateInstance
#define	g_ServerDataCenter					CSingleton<CServerDataCenter>::GetInstance()
#define	DESTROY_DALDATACENTER_INSTANCE		CSingleton<CServerDataCenter>::DestroyInstance

#define g_PlayerMgt g_ServerDataCenter.GetPlayerMgt()
#define g_RoomMgt g_ServerDataCenter.GetRoomMgt()

#define g_NeedRecvHeartBeatList g_ServerDataCenter.m_NeedRecvHeartBeatList
#define g_RecvHeartBeatList g_ServerDataCenter.m_RecvHeartBeatList


FRAME_GATESERVER_NAMESPACE_END

#endif /* DATA_CENTER_H_ */
