#pragma once

#pragma warning ( push, 3 )
#include <map>
#include <string>
#include <vector>
#pragma warning ( pop )

#include "SockDefine.h"

#include "CommMsg.h"
#include "CommUtil.h"
#include "DataSock.h"
#include "ServerSock.h"
#include "SockCmnd.h"
#include "SockMan.h"
#include "StreamBuf.h"


#ifndef NMSOCK_EXPORTS

#ifdef _DEBUG
	#pragma comment(lib, "nmsock_d.lib")
#else
	#pragma comment(lib, "nmsock.lib")
#endif

#endif // MODEL_EXPORT