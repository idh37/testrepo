// Sound.h: interface for the Sound class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


#ifndef NMDIALOG_API

	#ifdef NMDIALOG_EXPORTS
		#define NMDIALOG_API	__declspec(dllexport )
	#else
		#define NMDIALOG_API	__declspec(dllimport )
	#endif 

#endif

#ifndef NMDIALOG_CLASS 
	#define NMDIALOG_CLASS NMDIALOG_API
#endif