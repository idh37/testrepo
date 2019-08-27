/**    
@file    SkinGDIInclude.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   SkinGDI Lib를 사용하는 곳에서 Include하면 됩니다.
*/
#pragma once

#include "SkinGDIDefine.h"


#include "BmpCheckButton.h"
#include "BmpRadioBtn.h"
#include "BmpToRegion.h"
#include "ChatViewEdit.h"
#include "CustomHtml.h"
#include "GraphButton.h"
#include "GraphButton_Text.h"
#include "GraphButtonEx.h"
#include "GraphButtonList.h"
#include "GraphScrollThumbButton.h"
#include "GrpChatEdit.h"
#include "GrpChatEdit2.h"
#include "HTransRegion.h"
#include "ListCtrlEx.h"
#include "MEMDC.h"
#include "MyBitmap.h"
#include "MyButton.h"
#include "MyEdit.h"
#include "MyTextButton.h"
#include "NChatView.h"
#include "NEditBox.h"
#include "NMultiScene.h"
#include "NScene.h"
#include "NText.h"
#include "NTextScroll.h"
#include "ProgressCtrlEx.h"
#include "SendEdit.h"
#include "SkinVerticleScrollbar.h"
#include "SkinVerticleScrollbar2.h"
#include "SliderCtrlEx.h"
#include "TitleTip.h"

#include "./NMWidget/NMButton.h"
#include "./NMWidget/NMChatWidget.h"
#include "./NMWidget/NMComboBox.h"
#include "./NMWidget/NMDialog.h"
#include "./NMWidget/NMEdit.h"
#include "./NMWidget/NMListWidget.h"
#include "./NMWidget/NMResourceManager.h"
#include "./NMWidget/NMScrollChatWidget.h"
#include "./NMWidget/NMTab.h"
#include "./NMWidget/NMTextButton.h"
#include "./NMWidget/NMTimeGauge.h"
#include "./NMWidget/NMTimer.h"
#include "./NMWidget/NMToggleButton.h"
#include "./NMWidget/NMUnitWdget.h"
#include "./NMWidget/NMWidgetManager.h"
#include "./NMWidget/SSOWebWidget.h"



#ifndef NMSKINGDI_EXPORTS
	#ifdef _DEBUG
		#pragma comment(lib, "nmskingdi_d.lib")
	#else
		#pragma comment(lib, "nmskingdi.lib")
	#endif
#endif