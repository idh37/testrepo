/**    
@file    SockInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Sock Lib�� ����ϴ� ������ Include�ϸ� �˴ϴ�.
*/
#pragma once

#include "SockCDefine.h"


#ifndef NMSOCKC_EXPORTS
	#ifdef _DEBUG
		#pragma comment(lib, "nmsockc_d.lib")
	#else
		#pragma comment(lib, "nmsockc.lib")
	#endif
#endif