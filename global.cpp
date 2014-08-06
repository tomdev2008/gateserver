/*
 * global.cpp
 *
 *  Created on: 2011-12-12
 *      Author: luocj
 */

#include "global.h"


FRAME_GATESERVER_NAMESPACE_BEGIN

uint8_t g_nRobotTimeoutTime = 0;
bool g_bTest = false;
bool g_bHasCommnad = false;
bool g_bHallServerIsRuning = true;
uint8_t g_messageHead[4096];
uint8_t g_messageBody[65535*10];

FRAME_GATESERVER_NAMESPACE_END

