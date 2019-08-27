#pragma once

#pragma warning ( push, 3 )
#include <map>
#include <string>
#include <vector>
#pragma warning ( pop )

#include "ControllerDefine.h"

#include "ChannelController.h"
#include "FrameController.h"
#include "LobbyController.h"
#include "NetWorkController.h"


#ifndef NMCONTROLLER_EXPORTS

#ifdef _DEBUG
	#pragma comment(lib, "nmcontroller_d.lib")
#else
	#pragma comment(lib, "nmcontroller.lib")
#endif

#endif // MODEL_EXPORT