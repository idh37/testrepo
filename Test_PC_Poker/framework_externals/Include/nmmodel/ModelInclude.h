/**    
@file    ModelInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Model Lib�� ����ϴ� ������ Include�ϸ� �˴ϴ�.
*/
#pragma once

#include "ModelDefine.h"

#include "LogInModel.h"
#include "ChannelModel.h"
#include "LobbyModel.h"
#include "FrameModel.h"
#include "NetWorkModel.h"
#include "NetWorkCamelModel.h"


#ifndef NMMODEL_EXPORTS
	#ifdef _DEBUG
		#pragma comment(lib, "nmmodel_d.lib")
	#else
		#pragma comment(lib, "nmmodel.lib")
	#endif
#endif
