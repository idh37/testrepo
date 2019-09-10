/**    
    @file    SockInclude.h
    @date    2011/2/11
    @author  PJH(gamking@cj.com)
    @brief   Trace�� ����ϱ� ���� lib
	@brief   ��ũ�� �߰� ���̺귯�� ������ �߰� �ϼž� �մϴ�.
*/

#pragma once

#ifdef _DEBUG
	#define TRACE_OUTPUT			///< Trace�� ����Ϸ��� ���� �ؾ� �Ѵ�.
#else
	//#define TRACE_OUTPUT			///< Trace�� ����Ϸ��� ���� �ؾ� �Ѵ�.
#endif




#ifdef TRACE_OUTPUT

	#include "tracetool.h"

	#ifdef _DEBUG
		#pragma comment(lib, "tracetool_d.lib")
	#else
		#pragma comment(lib, "tracetool.lib")
	#endif

	// WinSock Lib
	#pragma comment (lib, "WSock32.Lib")

#endif