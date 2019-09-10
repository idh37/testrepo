/**    
@file    DataInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Data Lib�� ����ϴ� ������ Include�ϸ� �˴ϴ�.
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
 