#pragma once

#pragma warning ( push, 3 )
#include <map>
#include <string>
#include <vector>
#pragma warning ( pop )

#include "skingdidefine.h"


#include "BmpCheckButton.h"
#include "BmpRadioBtn.h"
#include "BmpToRegion.h"
#include "ChatViewEdit.h"
#include "CustomHtml.h"
#include "GrpChatEdit2.h"
#include "GrpChatEdit.h"
#include "HTransRegion.h"
#include "ListCtrlEx.h"
#include "MEMDC.H"
#include "MyBitmap.h"
#include "MyButton.h"
#include "MyEdit.h"
#include "MyTextButton.h"
#include "ProgressCtrlEx.h"
#include "SendEdit.h"
#include "SkinVerticleScrollbar2.h"
#include "SkinVerticleScrollbar.h"
#include "SliderCtrlEx.h"
#include "TitleTip.h"

#ifndef NMSKINGDI_EXPORTS

#ifdef _DEBUG
	#pragma comment(lib, "nmskingdi_d.lib")
#else
	#pragma comment(lib, "nmskingdi.lib")
#endif

#endif // MODEL_EXPORT