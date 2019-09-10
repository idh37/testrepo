/**    
@file    ControllerInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Controller Lib�� ����ϴ� ������ Include�ϸ� �˴ϴ�.
*/
#pragma once

#include "ControllerDefine.h"

#include "ChannelController.h"
#include "FrameController.h"
#include "LobbyController.h"
#include "NetWorkController.h"
#include "NetWorkCamelController.h"
#include "LogInController.h"


#ifndef NMCONTROLLER_EXPORTS
	#ifdef _DEBUG
		#pragma comment(lib, "nmcontroller_d.lib")
	#else
		#pragma comment(lib, "nmcontroller.lib")
	#endif
#endif
