/*
 * msg_cs_config.h
 *
 *  Created on: 2013-1-10
 *      Author: luocj
 */

#ifndef MSG_CS_CONFIG_H_
#define MSG_CS_CONFIG_H_

#include <memory.h>
#include <map>
#include "common/common_singleton.h"
#include "common/common_def.h"
#include "def/server_namespace.h"
#include "public_typedef.h"
#include "main_frame.h"


using namespace std;
FRAME_GATESERVER_NAMESPACE_BEGIN

#define DEFAULT_CS_MSG_CONFIGFILENAME	"./config/gate_msg_cs.xml"
struct CsMsgNodeInfo
{
	uint32_t   nMsgID;
	EntityType nToEntityType;
	TransType  nTransType;
};
typedef map<uint32_t, CsMsgNodeInfo>		CsMsgInfoMap;

class CCSMsgConfig :public IConfigCenter
{
public:
	CCSMsgConfig() {};
	virtual ~CCSMsgConfig() {};

	virtual int32_t Initialize(const char* szFileName = NULL, const int32_t type=0);
	virtual int32_t Reload(const char* szFileName = NULL, const int32_t type=0) ;
	virtual int32_t Uninitialize() ;

	EntityType GetEntityTypeByMsgId(uint32_t nMsgID);

	TransType GetTransTypeByMsgId(uint32_t nMsgID);

	CsMsgInfoMap GetMsgInfo()
	{
		return arrMsgInfo;
	}

private:
	CsMsgInfoMap arrMsgInfo ;
};

#define	CREATE_CSMSG_INSTANCE				CSingleton<CCSMsgConfig>::CreateInstance
#define	g_CCSMsgConfig						CSingleton<CCSMsgConfig>::GetInstance()
#define	DESTROY_CSMSG_INSTANCE				CSingleton<CCSMsgConfig>::DestroyInstance


FRAME_GATESERVER_NAMESPACE_END

#endif /* MSG_CS_CONFIG_H_ */
