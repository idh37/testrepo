/**    
    @file    SockInclude.h
    @date    2011/2/11
    @author  PJH(gamking@cj.com)
    @brief   Trace를 사용하기 위한 lib
	@brief   링크에 추가 라이브러리 폴더를 추가 하셔야 합니다.
*/

#pragma once

#ifdef _DEBUG
	#define TRACE_OUTPUT			///< Trace를 출력하려면 정의 해야 한다.
#else
	//#define TRACE_OUTPUT			///< Trace를 출력하려면 정의 해야 한다.
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