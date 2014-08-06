/*
 * global.h
 *
 *  Created on: 2011-12-12
 *      Author: luocj
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "def/server_namespace.h"
#include "common/common_typedef.h"


FRAME_GATESERVER_NAMESPACE_BEGIN


#define LOGPREFIX			"log/"SERVER_NAME_STRING

extern uint8_t g_nRobotTimeoutTime;
extern bool g_bTest;
extern bool g_bHasCommnad;
extern bool g_bHallServerIsRuning;

FRAME_GATESERVER_NAMESPACE_END

#endif /* GLOBAL_H_ */
