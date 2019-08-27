/**    
@file    AvatarInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Avatar Lib를 사용하는 곳에서 Include하면 됩니다.
*/
#pragma once

#pragma warning ( push, 3 )
#include <map>
#include <string>
#include <vector>
#pragma warning ( pop )

#include "AvatarDefine.h"

#include "Avatar.h"

#ifndef NMAVATAR_EXPORTS
	#ifdef _DEBUG
		#pragma comment(lib, "nmavatar_d.lib")
	#else
		#pragma comment(lib, "nmavatar.lib")
	#endif
#endif