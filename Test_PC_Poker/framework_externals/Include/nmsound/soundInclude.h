/**    
@file    soundInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Sound Lib를 사용하는 곳에서 Include하면 됩니다.
*/
#pragma once

#include "SoundDefine.h"

#include "SoundManager.h"
#include "Sound.h"


#ifndef NMSOUND_EXPORTS
	#ifdef _DEBUG
		#pragma comment(lib, "nmsound_d.lib")
	#else
		#pragma comment(lib, "nmsound.lib")
	#endif
#endif