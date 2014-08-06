/*
 * msg_sc_config.h
 *
 *  Created on: 2013-1-11
 *      Author: luocj
 */

#ifndef MSG_SC_CONFIG_H_
#define MSG_SC_CONFIG_H_

#include <memory.h>
#include <map>
#include "common/common_singleton.h"
#include "common/common_def.h"
#include "def/server_namespace.h"
#include "public_typedef.h"
#include "main_frame.h"


using namespace std;
FRAME_GATESERVER_NAMESPACE_BEGIN

#define DEFAULT_SC_MSG_CONFIGFILENAME	"./config/gate_msg_sc.xml"
struct ScMsgNodeInfo
{
	uint32_t   nMsgID;
};
typedef map<uint32_t, ScMsgNodeInfo>		ScMsgInfoMap;

class CSCMsgConfig :public IConfigCenter
{
public:
	CSCMsgConfig() {};
	virtual ~CSCMsgConfig() {};

	virtual int32_t Initialize(const char* szFileName = NULL, const int32_t type=0);
	virtual int32_t Reload(const char* szFileName = NULL, const int32_t type=0) ;
	virtual int32_t Uninitialize() ;

	ScMsgInfoMap GetMsgInfo()
	{
		return arrMsgInfo;
	}

private:
	ScMsgInfoMap arrMsgInfo ;
};

#define	CREATE_SCMSG_INSTANCE				CSingleton<CSCMsgConfig>::CreateInstance
#define	g_SCMsgConfig						CSingleton<CSCMsgConfig>::GetInstance()
#define	DESTROY_SCMSG_INSTANCE				CSingleton<CSCMsgConfig>::DestroyInstance


FRAME_GATESERVER_NAMESPACE_END

#endif /* MSG_SC_CONFIG_H_ */
