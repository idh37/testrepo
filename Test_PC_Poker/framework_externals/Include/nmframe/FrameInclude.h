/**    
@file    FrameInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Data Lib를 사용하는 곳에서 Include하면 됩니다.
*/
#pragma once

#pragma warning ( push, 3 )
#include <map>
#include <string>
#include <vector>
#pragma warning ( pop )

#include "FrameDefine.h"

#include "ViewManager.h"
#include "NMApp.h"
#include "NMMainFrmView.h"
#include "NMGameView.h"
#include "NMChannelProcess.h"
#include "NMChannelView.h"
#include "NMGameProcess.h"
#include "NMGameView.h"
#include "NMLobbyProcess.h"
#include "NMLobbyView.h"
#include "NMMainFrmProcess.h"
#include "NMMainFrmView.h"

#ifndef NMFRAME_EXPORTS

	#ifdef _DEBUG
		#pragma comment(lib, "nmframe_d.lib")
	#else
		#pragma comment(lib, "nmframe.lib")
	#endif

#endif // FRAME_EXPORT