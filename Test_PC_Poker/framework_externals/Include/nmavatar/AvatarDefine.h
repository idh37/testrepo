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

#define APF_RGB555			555		// Ÿ���� �ȼ� ������ 16bit RGB 5:5:5�� ���
#define APF_RGB565			565		// Ÿ���� �ȼ� ������ 16bit RGB 5:6:5�� ���
#define APF_RGB24			24		// Ÿ���� �ȼ� ������ 24bit�� ���
#define APF_RGB32			32		// Ÿ���� �ȼ� ������ 32bit�� ���

//////////////////////////////////////////////////////////////////////
typedef UINT HAVATAR;
