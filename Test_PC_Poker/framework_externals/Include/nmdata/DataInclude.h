/**    
@file    DataInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Data Lib를 사용하는 곳에서 Include하면 됩니다.
*/
#pragma once

#include "DataDefine.h"

#include "GlobalInfo.h"
#include "PlayerDataBase.h"
#include "RunparamInfo.h"


#ifndef NMDATA_EXPORTS
	#ifdef _DEBUG
		#pragma comment(lib, "nmdata_d.lib")
	#else
		#pragma comment(lib, "nmdata.lib")
	#endif
#endif
 