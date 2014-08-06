/*
 * main_frame.cpp
 *
 *  Created on: 2011-12-12
 *      Author: luocj
 */

#include "main_frame.h"
#include "test_main.h"
#include "frame_eventid.h"

#include "config/msg_cs_config.h"
#include "config/msg_sc_config.h"

#include "hallserver_message_define.h"
#include "roomserver_message_define.h"
#include "daemon_message_define.h"
#include "tunnelserver_message_define.h"
#include "def/from_client_message.h"
#include "bll/bll_ss_event.h"
#include "bll/bll_cs_event.h"
#include "bll/bll_sys_connection.h"

#include "bll/bll_event_exitroom.h"
#include "bll/bll_event_enterroom.h"
#include "bll/bll_server_serverstop.h"
#include "bll/bll_event_hallrun.h"
#include "bll/bll_event_heartbeat.h"

#include "bll/bll_timer_checkbeat.h"

#include "ctl/data_center.h"


FRAME_GATESERVER_NAMESPACE_BEGIN

CMainFrame::CMainFrame()
:CLightFrame(SERVER_NAME_STRING)
{

}

CMainFrame::~CMainFrame()
{
}

//框架初始话
int32_t CMainFrame::Initialize()
{
	int32_t ret=S_OK;
	//加入socket
    //加入配置
	AddConfigCenter(0,DEFAULT_CS_MSG_CONFIGFILENAME,&g_CCSMsgConfig);

	AddConfigCenter(0,DEFAULT_SC_MSG_CONFIGFILENAME,&g_SCMsgConfig);



	//增加server的数据中心
	ret = AddDataCenter(&g_ServerDataCenter);
	if(ret < 0)
	{
		g_FrameLogEngine.WriteBaseLog(enmLogLevel_Error, "Frame(add data center) failed!\n");
		return ret;
	}

	ret = CLightFrame::Initialize();
	if(0 > ret)
	{
		return ret;
	}

	//添加定时器
	int32_t nCheckBeatTimerIndex = enmInvalidTimerIndex;
	ret = g_Frame.CreateTimer(static_cast<TimerProc>(&CCheckBeatEvent::OnTimerEvent), &g_CheckBeatEvent, NULL,enmCheckBeatTimePeriod , true, nCheckBeatTimerIndex);
	if (ret < 0)
	{
		return ret;
	}
	//注册所有的消息
	RegistMsg();

	return S_OK;
}

//恢复框架
int32_t CMainFrame::Resume()
{
	return CLightFrame::Resume();

}
//销毁框架
int32_t CMainFrame::Terminate()
{
	g_eServerCommand = enmServerCommand_Terminate;
	return S_OK;
}

void CMainFrame::Run()
{
	CLightFrame::Run();
}

void CMainFrame::Reload()
{
	g_eServerCommand = enmServerCommand_Reload;
}

void CMainFrame::OnTestRequest()
{
	CTestMain::OnTestRequest();
}
//次处添加的为bll需要处理的
void CMainFrame::RegistMsg()
{
	//注册链接断开事件通知
	RegistSysEvent(SYS_EVENT_CONN_CLIENT_CLOSED, &g_SYSEvent);
	RegistSysEvent(SYS_EVENT_CONN_SERVER_CLOSED, &g_SYSEvent);
	RegistSysEvent(SYS_EVENT_CONN_ERROR, &g_SYSEvent);

	CsMsgInfoMap arrCsMsgInfo = g_CCSMsgConfig.GetMsgInfo();
	CsMsgInfoMap::iterator iter;
	for(iter = arrCsMsgInfo.begin(); iter != arrCsMsgInfo.end(); iter++)
	{
		RegistDefEvent(iter->first,&g_CCSEvent);
	}

	ScMsgInfoMap arrScMsgInfo = g_SCMsgConfig.GetMsgInfo();
	ScMsgInfoMap::iterator sc_iter;
	for(sc_iter = arrScMsgInfo.begin(); sc_iter != arrScMsgInfo.end(); sc_iter++)
	{
		RegistDefEvent(sc_iter->first,&g_CSSEvent);
	}

	GET_MESSAGEMAPDECL_INSTANCE().AddMsgBody(MSGID_HSTT_HALLSERVERISRUNING_NOTI,(new CHallServerIsRuningNoti()));
	GET_MESSAGEMAPDECL_INSTANCE().AddMsgBody(MSGID_RSMS_ENTER_ROOM_NOTICE,(new CEnterRoomNotice_Public()));
	GET_MESSAGEMAPDECL_INSTANCE().AddMsgBody(MSGID_RSMS_EXIT_ROOM_NOTICE,(new CExitRoomNotice_Public()));
	GET_MESSAGEMAPDECL_INSTANCE().AddMsgBody(MSGID_CLHS_HEART_BEAT,(new CPlayerHeartBeat()));
	GET_MESSAGEMAPDECL_INSTANCE().AddMsgBody(MSGID_DAEMON_STOP_NOTIFY,(new CDaemonStopNotify_Public()));
	GET_MESSAGEMAPDECL_INSTANCE().AddMsgBody(MSGID_HSTS_GETROOMSERVERID_RESP,(new CGetRoomServerIDResp_Public()));

	RegistMsgHandler(MSGID_RSMS_ENTER_ROOM_NOTICE, &g_EnterRoomNoticeMessageEvent);
	RegistMsgHandler(MSGID_RSMS_EXIT_ROOM_NOTICE, &g_ExitRoomNoticeMessageEvent);
	RegistMsgHandler(MSGID_DAEMON_STOP_NOTIFY, &g_ServerStopMessageEvent);
	RegistMsgHandler(MSGID_HSTT_HALLSERVERISRUNING_NOTI, &g_HallRunMessageEvent);
	RegistMsgHandler(MSGID_CLHS_HEART_BEAT, &g_HeartBeatMessageEvent);
}

FRAME_GATESERVER_NAMESPACE_END

