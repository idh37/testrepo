#pragma once

#pragma warning ( push, 3 )
#include <map>
#include <string>
#include <vector>
#pragma warning ( pop )

#include "DialogDefine.h"

#include "AdminMessageDlg.h"
#include "AdminRemoveRoomDlg.h"
#include "AdminTitleDlg.h"
#include "BanishmentDlg.h"
#include "BanishVoteDlg.h"
#include "BannerDlg.h"
#include "ConfirmVote.h"
#include "DialogCtrlReposition.h"
#include "GameGradeDlg.h"
#include "LoginDlg.h"
#include "MsgBoxDlg.h"
#include "MyVoteDlg.h"
#include "VipCloseDlg.h"


#ifndef NMDIALOG_EXPORTS

#ifdef _DEBUG
	#pragma comment(lib, "nmdialog_d.lib")
#else
	#pragma comment(lib, "nmdialog.lib")
#endif

#endif // MODEL_EXPORT