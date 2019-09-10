/**    
@file    SockInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Sock Lib�� ����ϴ� ������ Include�ϸ� �˴ϴ�.
*/
#pragma once

#include "SockCamelDefine.h"
#include "Connector.h"
#include "SockCamelMan.h"

#ifndef NMSOCKCAMEL_EXPORTS
	#ifdef _DEBUG
		#pragma comment(lib, "nmsockcamel_d.lib")
	#else
		#pragma comment(lib, "nmsockcamel.lib")
	#endif
#endif