/**    
@file    AvatarInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Avatar Lib�� ����ϴ� ������ Include�ϸ� �˴ϴ�.
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