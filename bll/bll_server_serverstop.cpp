/*
 * bll_server_serverstop.cpp
 *
 *  Created on: 2013-1-16
 *      Author: luocj
 */
#include "bll_server_serverstop.h"
#include "daemon_message_define.h"
#include "ctl/data_center.h"
#include <vector>

using namespace std;

FRAME_GATESERVER_NAMESPACE_BEGIN

int32_t CServerStopMessageEvent::OnMessageEvent(MessageHeadSS * pMsgHead, IMsgBody* pMsgBody,
			const uint16_t nOptionLen , const void *pOptionData)
{
	if(pMsgBody==NULL || pMsgHead==NULL)
	{
		WRITE_ERROR_LOG("null pointer:{pMsgHead=0x%08x, pMsgBody=0x%08x}\n",pMsgHead,pMsgBody);
		return E_NULLPOINTER;
	}

	CDaemonStopNotify_Public *pDaemonStopNotify_Public = dynamic_cast<CDaemonStopNotify_Public *>(pMsgBody);
	if(NULL == pDaemonStopNotify_Public)
	{
		WRITE_ERROR_LOG("null pointer:{pDaemonStopNotify_Public=0x%08x}\n",pDaemonStopNotify_Public);
		return E_NULLPOINTER;
	}

	switch(pDaemonStopNotify_Public->serverType)
	{
		case enmEntityType_Room:
		{
			RoomServerStop(pDaemonStopNotify_Public->serverID);
		}
		break;
		case enmEntityType_Hall:
		{
			//暂时不做处理，等hallserver的代码写完
			g_bHallServerIsRuning = false;
		}
		default:
			break;
	}
	return S_OK;
}

void CServerStopMessageEvent::RoomServerStop(ServerID nServerID)
{
	//清空房间的serverID
	vector<CRoom *> arrRoomInServer;
	arrRoomInServer.clear();

	g_RoomMgt.GetAllRoomByServerID(arrRoomInServer,nServerID);

	for(uint32_t i = 0;i < arrRoomInServer.size();i++)
	{
		CRoom * pRoom = arrRoomInServer.at(i);
		if(pRoom == NULL)
		{
			continue;
		}
		pRoom->SetRoomServerID(enmInvalidServerID);

		WRITE_NOTICE_LOG("room in sevrer stop{nRoomID=%d, nServerID=%d}",pRoom->GetRoomID(),nServerID);
	}
}

FRAME_GATESERVER_NAMESPACE_END
