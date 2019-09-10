/**    
@file    SockInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Sock Lib�� ����ϴ� ������ Include�ϸ� �˴ϴ�.
*/
#pragma once

#include "SockDefine.h"

#include "SockCmnd.h"
#include "SockMan.h"
#include "StreamBuf.h"

#ifndef NMSOCK_EXPORTS
	#ifdef _DEBUG
		#pragma comment(lib, "nmsock_d.lib")
	#else
		#pragma comment(lib, "nmsock.lib")
	#endif
#endif