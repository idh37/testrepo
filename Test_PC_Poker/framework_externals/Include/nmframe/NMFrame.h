#pragma once

#pragma warning ( push, 3 )
#include <map>
#include <string>
#include <vector>
#pragma warning ( pop )

#include "FrameDefine.h"

#include "NMApp.h"
#include "NMMainFrm.h"


#ifndef NMFRAME_EXPORTS

#ifdef _DEBUG
	#pragma comment(lib, "nmframe_d.lib")
#else
	#pragma comment(lib, "nmframe.lib")
#endif

#endif // MODEL_EXPORT