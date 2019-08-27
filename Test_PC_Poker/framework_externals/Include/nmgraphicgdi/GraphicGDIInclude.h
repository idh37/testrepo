/**    
@file    GraphicInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   GraphicGDI Lib를 사용하는 곳에서 Include하면 됩니다.
*/
#pragma once

#pragma warning ( push, 3 )
#include <map>
#include <string>
#include <vector>
#pragma warning ( pop )

#include "GraphicGDIDefine.h"

#include "xSprite.h"
#include "xScene.h"
#include "xMultiScene.h"
#include "xLayer.h"
#include "xAniMan.h"
#include "xAniFile.h"
#include "GraphMod8.h"
#include "GraphMod.h"
#include "Graphic.h"
#include "Dither.h"
#include "Display.h"
#include "ScreenCapture.h"

#ifndef NMGRAPHICGDI_EXPORTS
	#ifdef _DEBUG
		#pragma comment(lib, "nmgraphicgdi_d.lib")
	#else
		#pragma comment(lib, "nmgraphicgdi.lib")
	#endif
#endif