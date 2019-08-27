#pragma once

#pragma warning ( push, 3 )
#include <map>
#include <string>
#include <vector>
#pragma warning ( pop )

#include "ModelDefine.h"

#include "ChannelModel.h"
#include "FrameModel.h"
#include "LobbyModel.h"
#include "NetWorkModel.h"


#ifndef NMMODEL_EXPORTS

#ifdef _DEBUG
	#pragma comment(lib, "nmmodel_d.lib")
#else
	#pragma comment(lib, "nmmodel.lib")
#endif

#endif // MODEL_EXPORT