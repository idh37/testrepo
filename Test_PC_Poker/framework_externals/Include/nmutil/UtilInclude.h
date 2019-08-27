/**    
@file    UtilInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Util Lib를 사용하는 곳에서 Include하면 됩니다.
*/
#pragma once

#include "UtilDefine.h"

// Util헤더 모음
#include "BadFilter.h"
#include "DataManager.h"
#include "DesktopIconMan.h"
#include "DesktopIconManEx.h"
#include "DialogCtrlReposition.h"
#include "FontBag.h"
#include "GameDirectGo.h"
#include "GFUtil.h"
#include "IPAdapterInfo.h"
#include "MacRetriever.h"
#include "NMRunParam.h"
#include "NoChat.h"
#include "PlayTimeWarning.h"
#include "ShellMan.h"
#include "ShortCut.h"
#include "SSOWeb.h"
#include "StringBase.h"
#include "StringManager.h"
#include "StringUtil.h"
#include "SyncFPS.h"
#include "SysUtil.h"
#include "ThreadLock.h"
#include "ThreadQueue.h"
#include "StatisticsManager.h"
#include "./BugSplat/BugSplatWrapper.h"

#include "Delegate/MessageManager.h"
#include "Delegate/Functor.h"
#include "Delegate/IComponent.h"

#include "WebControl/webbro.h"
#include "WebControl/WebCtrl.h"
#include "WebControl/WebCtrl4New.h"

#ifndef NMUTIL_EXPORTS
	#ifdef _DEBUG
		#pragma comment(lib, "nmutil_d.lib")
	#else
		#pragma comment(lib, "nmutil.lib")
	#endif
#endif