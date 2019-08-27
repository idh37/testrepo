#pragma once


#include "./GameDef.h"
#include "../src/GlobalBase.h"
#include "../src/ImageDef.h"

////////////////////////////////////////////////////
//headers for Dialogs
#include "../src/EndPopup.h"
//메세지팝업
#include "../src/MsgBoxDlg.h"
#include "../src/MsgBoxChkDlg.h"
#include "../src/MsgImgBox.h"
#include "../src/VIPChanceAlert.h"
//추방
#include "../src/BanishmentDlg.h"
#include "../src/BanishVoteDlg.h"
#include "../src/MyVoteDlg.h"
//신고
#include "../src/BadUserComplete.h"
#include "../src/BadUserDlg.h"
#include "../src/InGameBadUserComplainMsgBox.h"
//초대창
#include "../src/InviteDlg.h"
#include "../src/RecvInviteDlg.h"

//선택시작
#include "../src/SelectQuickstarterDlg.h"
#include "../src/ResetQuickStarterDlg.h"

#include "../src/WhisperDlg.h"
#include "../src/LobbyEventDlg.h"
#include "../src/GameManualDlg.h"
#include "../src/GameGradeDlg.h"
#include "../src/KisaDlg.h"
#include "../src/BuyLuckAvataDlg.h"
#include "../src/BuyTimeLimitAvataDlg.h"
#include "../src/JackpotRankPopupDlg.h"
#include "../src/UniversalItemDlg.h"
#include "../src/ProfileDlg.h"

#include "../src/MyFamilyDlg.h"
#include "../src/ComboBoxImageDlg.h"
#include "../src/VipServiceDlg.h"
#include "../src/VipCloseDlg.h"
#include "../src/CharView.h"

//관리자
#include "../src/AdminMessageDlg.h"
#include "../src/AdminRemoveRoomDlg.h"
#include "../src/AdminTitleDlg.h"

#include "../src/ObserverDlg.h"
#include "../src/NewAskChargeDlg.h"
#include "../src/LoadingDlg.h"
#include "../src/NewGameItemDlg.h"
#include "../src/OptionDlg.h"

#include "../src/RefuseQJDlg.h"

#include "../src/VipService.h"


//Base Classses
#include "../src/Player.h"
#include "../src/GameProcess.h"
#include "../src/GameViewBase.h"
#include "../src/Game.h"
#include "../src/GameManager.h"
#include "../src/GameDataManager.h"

#include "../src/GameURL.h"
#include "../src/HideChatList.h"
#include "../src/LogFile.h"
//gamecommon classes
#include "../src/BaseObject.h"
#include "../src/ObjectList.h"
//#include "../src/GameDirectGo.h"

//managers
#include "../src/RewardItemMan.h"
#include "../src/MessageManager.h"

#include "../src/GameRes.h"
#include "../src/ThreadLock.h"
#include "../src/UPGDlgBase.h"

//커뮤니티 관련
#include "../src/community/CommunityManager.h"
#include "../src/community/MessageNotificationPopup.h"
#include "../src/community/ReceiveMessageDlg.h"
#include "../src/community/ReceiveAdminMessageDlg.h"
#include "../src/community/SendMessageDlg.h"
#include "../src/community/MessageBox.h"
#include "../src/community/MemoNotifyDlg.h"

#include "../src/community/ChattingRoomDlg.h"
#include "../src/community/ChattingRoomInviteDlg.h"
#include "../src/community/ChattingRoomAddUserDlg.h"
#include "../src/ClientStats.h" //클라이언트 통계코드

#include "../src/Community/Buddy.h"
#include "../src/Community/BuddyGroupAddDlg.h"
#include "../src/Community/BlackBuddyAddDlg.h"
#include "../src/Community/BlackBuddyRemoveDlg.h"
#include "../src/Community/BuddyAddDlg.h"
#include "../src/Community/BuddyAddResultDlg.h"
#include "../src/Community/BuddyRemoveDlg.h"
#include "../src/Community/BuddyMoveGroupDlg.h"
#include "../src/Community/BuddyGroupRemoveDlg.h"
#include "../src/Community/AddUserSelectDlg.h"

#include "../src/Community/CongMessageDlg.h"

#include "../src/TimerNotifierDlg.h"
#include "../src/UPPInfoPopup.h"
#include "../src/UPPPopup.h"
#include "../src/EventInfopopup.h"
#include "../src/JackpotTimepopup.h"
#include "../src/RestrictionInfoPopup.h"
#include "../src/LoginInfoPopup.h"
#include "../src/PostBoxDlg.h"
#include "../src/MD5.h"
#include "../src/GoldChangeDlg.h"
#include "../src/ClubDlg.h"
#include "../src/GameBase/GameCommon/Popup/GoldLuckyBag/GoldLuckyBagPopup.h"
#include "../src/GameBase/GameCommon/Popup/NewYearAttendance/NewYearAttendancePopup.h"
#include "../src/GameBase/GameCommon/Popup/GoldMileage/GoldMileagePopup.h"
#include "../src/GameBase/GameCommon/Popup/GoldSupportFund/GoldSupportFundPopup.h"
#include "../src/GameBase/GameCommon/Popup/GoldAttendanceBook/GoldAttendanceBookPopup.h"
#include "../src/GameBase/GameCommon/Popup/ChipAttend/ChipAttendPopup.h"
#include "../src/GameBase/GameCommon/Popup/GoldBigWheel/GoldBigWheelDlg.h"
#include "../src/GameBase/GameCommon/Popup/GoldWithdraw/GoldWithdrawPopup.h"
#include "../src/GameBase/GameCommon/Popup/ChuSeok/ChuSeokAttendanceDlg.h"
#include "../src/GameBase/GameCommon/Popup/ChuSeok/ChuSeokPopup.h"
#include "../src/GameBase/GameCommon/Popup/LobbyNotify/LobbyNotifyDlg.h"

//buy in, 입장머니제한으로 추가한 다이얼로그
#include "../src/AskContinueGameDlg.h"