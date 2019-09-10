/**    
@file    SkinGDIInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   SkinGDI Lib�� ����ϴ� ������ Include�ϸ� �˴ϴ�.
*/
#pragma once

#include "SkinGDIDefine.h"

#include "MEMDC.H"
#include "SliderCtrlEx.h"
#include "TitleTip.h"



#ifndef NMSKINGDI_EXPORTS
	#ifdef _DEBUG
		#pragma comment(lib, "nmskingdi_d.lib")
	#else
		#pragma comment(lib, "nmskingdi.lib")
	#endif
#endif