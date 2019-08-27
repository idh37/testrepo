/**    
@file    SkinGDIDefine.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   SkinGDI Import / Export Define
*/
#pragma once


#ifndef NMSKINGDI_API

	#ifdef NMSKINGDI_EXPORTS
		#define NMSKINGDI_API		__declspec(dllexport )
	#else
		#define NMSKINGDI_API		__declspec(dllimport )
	#endif 

#endif

#ifndef NMSKINGDI_CLASS 
	#define NMSKINGDI_CLASS NMSKINGDI_API
#endif


#define COLOR_565(dw)		((WORD)((dw&0xff)>>3) | (WORD)((dw&0xff00)>>10)<<5 | (WORD)((dw&0xff0000)>>19)<<11)
#define COLOR_555(dw)		(WORD) ( ((WORD)((dw&0xff)>>3) | (WORD)((dw&0xff00)>>11)<<5 | (WORD)((dw&0xff0000)>>19)<<10) )
#define COLOR555(r,g,b)		(WORD) ( (((WORD)(((r)/8)<<10))) | (((WORD)(((g)/8)<<5))) | ((WORD)(b)/8)  )
#define COLOR565(r,g,b)		(WORD) ( (((WORD)(((r)/8)<<11))) | (((WORD)(((g)/4)<<5))) | ((WORD)(b)/8)  )

#define MAKELONGLONG(a,b)	((LONGLONG)((DWORD)(a) | ((DWORDLONG)(DWORD)(b)) << 32)) 
#define LOLONG(dwl)			((DWORD)(dwl)) 
#define HILONG(dwl)			((DWORD)(((DWORDLONG)(dwl) >> 32) & 0xFFFFFFFF)) 