#pragma once

#pragma warning ( push, 3 )
#include <map>
#include <string>
#include <vector>
#pragma warning ( pop )

#include "GraphicGDIDefine.h"

#include "Display.h"
#include "Graphic.h"
#include "xHan.h"
#include "xMultiScene.h"
#include "xScene.h"
#include "xSprite.h"


#ifndef NMGRAPHICGDI_EXPORTS

#ifdef _DEBUG
	#pragma comment(lib, "nmgraphicgdi_d.lib")
#else
	#pragma comment(lib, "nmgraphicgdi.lib")
#endif

#endif // NMGRAPHICGDI_EXPORTS