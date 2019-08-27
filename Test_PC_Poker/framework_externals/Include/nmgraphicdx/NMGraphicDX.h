#pragma once

#pragma warning ( push, 3 )
#include <map>
#include <string>
#include <vector>
#pragma warning ( pop )

#include "GraphicDXDefine.h"

#include "Display.h"
#include "Graphic.h"
#include "xHan.h"
#include "xMultiScene.h"
#include "xScene.h"
#include "xSprite.h"


#ifndef NMGRAPHICDX_EXPORTS

#ifdef _DEBUG
	#pragma comment(lib, "nmgraphicdx_d.lib")
#else
	#pragma comment(lib, "nmgraphicdx.lib")
#endif

#endif // NMGRAPHICDX_EXPORTS