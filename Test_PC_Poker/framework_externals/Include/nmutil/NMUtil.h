#pragma once

#pragma warning ( push, 3 )
#include <map>
#include <string>
#include <vector>
#pragma warning ( pop )

#include "UtilDefine.h"


#include "BadFilter.h"
#include "DesktopIconMan.h"
#include "DesktopIconManEx.h"
#include "DialogCtrlReposition.h"
#include "FontBag.h"
#include "GFUtil.h"
#include "NMRunParam.h"
#include "NMSTR.h"
#include "NMSTRMAN.h"
#include "NoChat.h"
#include "ShellMan.h"
#include "ShortCut.h"
#include "SSOWeb.h"
#include "StringUtil.h"
#include "SyncFPS.h"
#include "SysUtil.h"
#include "ThreadLock.h
#include "./Delegate/Delegate.h"
#include "./Delegate/Functor.h"
#include "./Delegate/MessageManager.h"
#include "./WebControl/webbro.h"
#include "./WebControl/WebCtrl.h"
#include "./WebControl/WebCtrl4New.h
#include "./WebControl/WebEventDisplayer.h"

#ifndef NMUTIL_EXPORTS

#ifdef _DEBUG
	#pragma comment(lib, "nmutil_d.lib")
#else
	#pragma comment(lib, "nmutil.lib")
#endif

#endif // MODEL_EXPORT