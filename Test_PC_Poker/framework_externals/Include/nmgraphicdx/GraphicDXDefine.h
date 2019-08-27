// Sound.h: interface for the Sound class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#ifndef NMGRAPHICDX_API

	#ifdef NMGRAPHICDX_EXPORTS
		#define NMGRAPHICDX_API	__declspec(dllexport )
	#else
		#define NMGRAPHICDX_API	__declspec(dllimport )
	#endif 

#endif

#ifndef NMGRAPHICDX_CLASS 
	#define NMGRAPHICDX_CLASS NMGRAPHICDX_API
#endif





#define GRAPHIC_OPTION_FIRST			0		// 아직 설정 안함.
#define GRAPHIC_OPTION_NONE				1		// 아직 설정 안함.
#define GRAPHIC_OPTION_LOW				2		// low
#define GRAPHIC_OPTION_NORMAL			3		// normal
#define GRAPHIC_OPTION_HIGH				4		// high