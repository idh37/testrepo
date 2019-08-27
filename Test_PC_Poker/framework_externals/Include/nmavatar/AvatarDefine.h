/**    
@file    AvatarDefine.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Avatar Import / Export Define
*/
#pragma once


#ifndef NMAVATAR_API

	#ifdef NMAVATAR_EXPORTS
		#define NMAVATAR_API		__declspec(dllexport )
	#else
		#define NMAVATAR_API		__declspec(dllimport )
	#endif 

#endif	// NMAVATAR_API

#ifndef NMAVATAR_CLASS 
	#define NMAVATAR_CLASS NMAVATAR_API
#endif	// NMAVATAR_CLASS

#define MAX_LAYER_4HERE		26

#define APF_RGB555			555		// 타겟의 픽셀 포멧이 16bit RGB 5:5:5인 경우
#define APF_RGB565			565		// 타겟의 픽셀 포멧이 16bit RGB 5:6:5인 경우
#define APF_RGB24			24		// 타겟의 픽셀 포멧이 24bit인 경우
#define APF_RGB32			32		// 타겟의 픽셀 포멧이 32bit인 경우

//////////////////////////////////////////////////////////////////////
typedef UINT HAVATAR;
