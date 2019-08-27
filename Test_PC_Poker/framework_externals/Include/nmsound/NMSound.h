#pragma once

#pragma warning ( push, 3 )
#include <map>
#include <string>
#include <vector>
#pragma warning ( pop )

#include "SoundDefine.h"

#include "MciMedia.h"
#include "Sound.h"
#include "SoundManager.h"
#include "xOgg.h"
#include "xSound.h"

#ifndef NMSOUND_EXPORTS

#ifdef _DEBUG
	#pragma comment(lib, "nmsound_d.lib")
#else
	#pragma comment(lib, "nmsound.lib")
#endif

#endif // MODEL_EXPORT