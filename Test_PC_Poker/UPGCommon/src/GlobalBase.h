#pragma once

#define UPG_LSYLIB_EXPORT	__declspec(dllimport)
#include "../../LSYLib/include/LSYLib.h"

#include <Pigeon.GS.ConnectorAPI.h>
#include <Pigeon.SS.ConnectorAPI.h>

#include "../include/GameDef.h"
#include "AdminMessageDlg.h"
#include "WhisperDlg.h"
#include "Player.h"
#include "GameViewBase.h"
#include "OptionDlg.h"
#include "ObserverDlg.h"
#include "InviteDlg.h"
#include "DailyPopup.h"
#include "EndEventPopup.h"
#include "RestrictionPopup.h"
#include "RestrictionOverPopup.h"
#include "NewAskChargeDlg.h"
#include "ProfileDlg.h"
#include "CWebCallThread.h"
#include "UPPDlg.h"
#include "./Community/ReceiveMessageDlg.h"
#include "./Community/ReceiveAdminMessageDlg.h"
#include "./Community/SendMessageDlg.h"
#include "./Community/MessageBox.h"
#include <EventCommon.h>

//UPG Common Message Key
#define UPG_CMK_LOAD_IMAGE_LIST_CLEAER		"UPG_CMK_LOAD_IMAGE_LIST_CLEAER"
#define UPG_CMK_LOAD_IMAGE_LIST_ADD			"UPG_CMK_LOAD_IMAGE_LIST_ADD"
#define UPG_CMK_LOADING_IMAGE_LIST			"UPG_CMK_LOADING_IMAGE_LIST"
#define UPG_CMK_CHANGE_VIEW					"UPG_CMK_CHANGE_VIEW"
#define UPG_CMK_SET_MAINWINDOW_TEXT			"UPG_CMK_SET_MAINWINDOW_TEXT"
#define UPG_CMK_CLICK_LOBBY_EXIT_BUTTON		"UPG_CMK_CLICK_LOBBY_EXIT_BUTTON"
#define UPG_CMK_SHOW_ENDING_POPUP			"UPG_CMK_SHOW_ENDING_POPUP"

#define UPG_CMK_MAIN_MESSAGE_SHOW			"UPG_CMK_MAIN_MESSAGE_SHOW"
#define UPG_CMK_MAIN_MESSAGE_CLOSE			"UPG_CMK_MAIN_MESSAGE_CLOSE"
#define UPG_CMK_MAIN_MESSAGE_CLEAR			"UPG_CMK_MAIN_MESSAGE_CLEAR"
#define UPG_CMK_MAIN_MESSAGE_ISVISIBLE		"UPG_CMK_MAIN_MESSAGE_ISVISIBLE"

#define UPG_CMK_MSG_CHK_SHOW				"UPG_CMK_MSG_CHK_SHOW"

#define UPG_CMK_SAVE_CONFIG					"UPG_CMK_SAVE_CONFIG"
#define UPG_CMK_LOAD_CONFIG					"UPG_CMK_LOAD_CONFIG"
#define UPG_CMK_GET_OBSERVER_DLG			"UPG_CMK_GET_OBSERVER_DLG"
#define UPG_CMK_GET_INVITE_DLG				"UPG_CMK_GET_INVITE_DLG"
#define UPG_CMK_GET_ENDEVENT_DLG			"UPG_CMK_GET_ENDEVENT_DLG"
#define UPG_CMK_GET_RESTRICTION_DLG			"UPG_CMK_GET_RESTRICTION_DLG"
#define UPG_CMK_GET_PROFILE_DLG				"UPG_CMK_GET_PROFILE_DLG"
#define UPG_CMK_GET_WHISPER_DLG				"UPG_CMK_GET_WHISPER_DLG"
#define UPG_CMK_GET_RECEIVEMESSSAGE_DLG		"UPG_CMK_GET_RECEIVEMESSSAGE_DLG"
#define UPG_CMK_GET_RECEIVEADMINMESSSAGE_DLG "UPG_CMK_GET_RECEIVEADMINMESSSAGE_DLG"
#define UPG_CMK_GET_SENDMESSSAGE_DLG		"UPG_CMK_GET_SENDMESSSAGE_DLG"
#define UPG_CMK_GET_MESSSAGEBOX_DLG			"UPG_CMK_GET_MESSSAGEBOX_DLG"
#define UPG_CMK_GET_GOLDCHANGE_DLG			"UPG_CMK_GET_GOLDCHANGE_DLG"
#define UPG_CMK_GET_GOLDBIGWHEEL_DLG		"UPG_CMK_GET_GOLDBIGWHEEL_DLG"
#define UPG_CMK_GET_CLUB_DLG				"UPG_CMK_GET_CLUB_DLG"
#define UPG_CMK_ADD_LOBBY_CHATVIEW_MESSAGE	"UPG_CMK_ADD_LOBBY_CHATVIEW_MESSAGE"
#define UPG_CMK_SHOW_UPP_DLG				"UPG_CMK_SHOW_UPP_DLG"
#define UPG_CMK_SHOW_RESTRICTION_PLAYOVER	"UPG_CMK_SHOW_RESTRICTION_PLAYOVER"
#define UPG_CMK_SHOW_REWARD_DAILYLOSS_OVER	"UPG_CMK_SHOW_REWARD_DAILYLOSS_OVER"

#define UPG_CMK_RESET_BADUSERTIMER			"UPG_CMK_RESET_BADUSERTIMER"
#define UPG_CMK_CHANGE_DISPLAY_MODE			"UPG_CMK_CHANGE_DISPLAY_MODE"
#define UPG_CMK_SCREEN_CAPTURE				"UPG_CMK_SCREEN_CAPTURE"

#define UPG_CMK_SHOW_TIPMAN_DLG				"UPG_CMK_SHOW_TIPMAN_DLG"
#define UPG_CMK_SHOW_DIRECTGO_DLG			"UPG_CMK_SHOW_DIRECTGO_DLG"
#define UPG_CMK_CLOSE_LOADING_DLG			"UPG_CMK_CLOSE_LOADING_DLG"

#define UPG_CMK_SHOW_SELECTSTART_DLG		"UPG_CMK_SHOW_SELECTSTART_DLG" //���ý��� â.
#define UPG_CMK_SHOW_RESETSELECTSTART_DLG	"UPG_CMK_SHOW_RESETSELECTSTART_DLG" //���ý��� �缳��â.

#define UPG_CMK_GAMEITEM_DLG_REFRESH_MYITEM	"UPG_CMK_GAMEITEM_DLG_REFRESH_MYITEM"
#define UPG_CMK_GAMEITEM_DLG_CLOSE			"UPG_CMK_GAMEITEM_DLG_CLOSE"
#define UPG_CMK_GAMEITEM_DLG_SHOW			"UPG_CMK_GAMEITEM_DLG_SHOW"
#define UPG_CMK_GET_USER_SMALLINFO			"UPG_CMK_GET_USER_SMALLINFO"

#define UPG_CMK_TIMELIMITITEM_DLG_CLOSE		"UPG_CMK_TIMELIMITITEM_DLG_CLOSE"
#define UPG_CMK_TIMELIMITITEM_DLG_SHOW		"UPG_CMK_TIMELIMITITEM_DLG_SHOW"

#define UPG_CMK_CHECK_BADUSER_REPORT		"UPG_CMK_CHECK_BADUSER_REPORT"	//�ֱ� 1�е��� BadUser �Ű� �߾������� �˻��Ѵ�.
#define UPG_CMK_SHOW_NEWASKCHARGEDLG		"UPG_CMK_SHOW_NEWASKCHARGEDLG"

#define UPG_CMK_AVATAR_SHOP					"UPG_CMK_AVATAR_SHOP"
#define UPG_CMK_CHANCEITEMSHOP				"UPG_CMK_CHANCEITEMSHOP"
#define UPG_CMK_ITEMSHOP					"UPG_CMK_ITEMSHOP"

#define UPG_CMK_BADUSERCOMPLETE				"UPG_CMK_BADUSERCOMPLETE"
#define UPG_CMK_ONDESTROY_LOADINGDIALOG		"UPG_CMK_ONDESTROY_LOADINGDIALOG"
#define UPG_CMK_SHOW_MESSAGE_IMAGE_BOX		"UPG_CMK_SHOW_MESSAGE_IMAGE_BOX"

#define UPG_CMK_SHOW_EXPLORER				"UPG_CMK_SHOW_EXPLORER"
#define UPG_CMK_SPRITE_LOADING_END			"UPG_CMK_SPRITE_LOADING_END"
#define UPG_CMK_SET_LIMIT_MONEY				"UPG_CMK_SET_LIMIT_MONEY"
#define UPG_CMK_SET_LIMIT_GOLD				"UPG_CMK_SET_LIMIT_GOLD"
#define UPG_CMK_GET_MONEY_GRADE				"UPG_CMK_GET_MONEY_GRADE"
#define UPG_CMK_GET_JOKBO_NAME				"UPG_CMK_GET_JOKBO_NAME"
#define UPG_CMK_GET_MYJOKBO_NAME			"UPG_CMK_GET_MYJOKBO_NAME"
#define UPG_CMK_GET_RESULT_MONEY			"UPG_CMK_GET_RESULT_MONEY"

#define UPG_CMK_SHOW_CHANCE_BUTTON			"UPG_CMK_SHOW_CHANCE_BUTTON"

#define UPG_CMK_OPEN_HIDDEN_URL				"UPG_CMK_OPEN_HIDDEN_URL"
#define UPG_CMK_GENERATE_RANDOM_ROOM_NAME	"UPG_CMK_GENERATE_RANDOM_ROOM_NAME"
#define UPG_CMK_KICK_OUT					"UPG_CMK_KICK_OUT"
#define UPG_CMK_SETPOKERICON				"UPG_CMK_SETPOKERICON"
#define UPG_CMK_CREATEDESKTOPICON			"UPG_CMK_CREATEDESKTOPICON"
#define UPG_CMK_UPDATE_ITEM_DATA			"UPG_CMK_UPDATE_ITEM_DATA"
#define UPG_CMK_CHANGE_SHOWCHAGE_USERINFO	"UPG_CMK_CHANGE_SHOWCHAGE_USERINFO"

#define UPG_CMK_GAME_RESULT_OPEN			"UPG_CMK_GAME_RESULT_OPEN"
#define UPG_CMK_OPTION_SETVOLUME			"UPG_CMK_OPTION_SETVOLUME"
#define UPG_CMK_SHOW_DAILYPOPUP_DIALOG		"UPG_CMK_SHOW_DAILYPOPUP_DIALOG"
#define UPG_CMK_SHOW_ENDEVENT_DIALOG		"UPG_CMK_SHOW_ENDEVENT_DIALOG"
#define UPG_CMK_SHOW_RESTRICTION_DIALOG		"UPG_CMK_SHOW_RESTRICTION_DIALOG"
#define UPG_CMK_SHOW_CHICKEN_PROMOTION_BTN	"UPG_CMK_SHOW_CHICKEN_PROMOTION_BTN"
#define UPG_CMK_SHOW_GOLDBACK_PROMOTION_BTN	"UPG_CMK_SHOW_GOLDBACK_PROMOTION_BTN"

#define UPG_CMK_HIDE_RESTRICTION_DIALOG		"UPG_CMK_HIDE_RESTRICTION_DIALOG"
#define UPG_CMK_SHOW_RESTRICTION_BUTTON		"UPG_CMK_SHOW_RESTRICTION_BUTTON"
#define UPG_CMK_HIDE_RESTRICTION_BUTTON		"UPG_CMK_HIDE_RESTRICTION_BUTTON"
#define UPG_CMK_CHANGE_RESTRICTION_BUTTON   "UPG_CMK_CHANGE_RESTRICTION_BUTTON"
#define UPG_CMK_SHOW_RESTRICTION		    "UPG_CMK_SHOW_RESTRICTION"
#define UPG_CMK_SHOW_RESTRICTIONOVER		"UPG_CMK_SHOW_RESTRICTIONOVER"
#define UPG_CMK_SHOW_PROMOTION_COMEBACK		"UPG_CMK_SHOW_PROMOTION_COMEBACK"
#define UPG_CMK_SHOW_PROMOTION_COMEBACK_DLG "UPG_CMK_SHOW_PROMOTION_COMEBACK_DLG"
#define UPG_CMK_SHOW_PROMOTION_COMEBACK_DLGUPDATE "UPG_CMK_SHOW_PROMOTION_COMEBACK_DLGUPDATE"

#define UPG_CMK_SHOW_PROFILE_DIALOG			"UPG_CMK_SHOW_PROFILE_DIALOG"
#define UPG_CMK_CLOSE_PROFILE_DIALOG		"UPG_CMK_CLOSE_PROFILE_DIALOG"
#define UPG_CMK_SHOW_WHISPER_DIALOG			"UPG_CMK_SHOW_WHISPER_DIALOG"
#define UPG_CMK_CLOSE_WHISPER_DIALOG		"UPG_CMK_CLOSE_WHISPER_DIALOG"
#define UPG_CMK_SHOW_RECEIVEMESSAGE_DIALOG	"UPG_CMK_SHOW_RECEIVEMESSAGE_DIALOG"
#define UPG_CMK_CLOSE_RECEIVEMESSAGE_DIALOG	"UPG_CMK_CLOSE_RECEIVEMESSAGE_DIALOG"
#define UPG_CMK_SHOW_SENDMESSAGE_DIALOG		"UPG_CMK_SHOW_SENDMESSAGE_DIALOG"
#define UPG_CMK_SET_MONEY_NUM				"UPG_CMK_SET_MONEY_NUM"
#define UPG_CMK_SET_LOBBY_BUTTON_ENABLE		"UPG_CMK_SET_LOBBY_BUTTON_ENABLE"
#define UPG_CMK_CLOSE_SENDMESSAGE_DIALOG	"UPG_CMK_CLOSE_SENDMESSAGE_DIALOG"
#define UPG_CMK_SHOW_MESSAGEBOX_DIALOG		"UPG_CMK_SHOW_MESSAGEBOX_DIALOG"
#define UPG_CMK_CLOSE_MESSAGEBOX_DIALOG		"UPG_CMK_CLOSE_MESSAGEBOX_DIALOG"
#define UPG_CMK_SHOW_GOLDCHANGE_DIALOG		"UPG_CMK_SHOW_GOLDCHANGE_DIALOG"
#define UPG_CMK_SHOW_GOLDBIGWHEEL_DIALOG	"UPG_CMK_SHOW_GOLDBIGWHEEL_DIALOG"
#define UPG_CMK_SHOW_CLUB_DIALOG			"UPG_CMK_SHOW_CLUB_DIALOG"
#define UPG_CMK_GET_GOLDLUCKYBAG_POPUP "UPG_CMK_GET_GOLDLUCKYBAG_POPUP" // �� �̺�Ʈ Ȳ�� ���ָӴ� �˾�
#define UPG_CMK_SHOW_GOLDLUCKYBAG_POPUP "UPG_CMK_SHOW_GOLDLUCKYBAG_POPUP" // �� �̺�Ʈ Ȳ�� ���ָӴ� �˾�
#define UPG_CMK_GET_NEWYEARATTENDANCE_POPUP "UPG_CMK_GET_NEWYEARATTENDANCE_POPUP" // �� �̺�Ʈ �⼮ �˾�
#define UPG_CMK_CREATE_NEWYEARATTENDANCE_POPUP "UPG_CMK_CREATE_NEWYEARATTENDANCE_POPUP" // �� �̺�Ʈ �⼮ �˾�
#define UPG_CMK_SHOW_NEWYEARATTENDANCE_POPUP "UPG_CMK_SHOW_NEWYEARATTENDANCE_POPUP" // �� �̺�Ʈ �⼮ �˾�
#define UPG_CMK_GET_GOLDMILEAGE_POPUP "UPG_CMK_GET_GOLDMILEAGE_POPUP" // ��� ���ϸ��� �˾�
#define UPG_CMK_CREATE_GOLDMILEAGE_POPUP "UPG_CMK_CREATE_GOLDMILEAGE_POPUP" // ��� ���ϸ��� �˾�
#define UPG_CMK_SHOW_GOLDMILEAGE_POPUP "UPG_CMK_SHOW_GOLDMILEAGE_POPUP" // ��� ���ϸ��� �˾�
#define UPG_CMK_GET_GOLDSUPPORTFUND_POPUP "UPG_CMK_GET_GOLDSUPPORTFUND_POPUP" // ����������˾�
#define UPG_CMK_CREATE_GOLDSUPPORTFUND_POPUP "UPG_CMK_CREATE_GOLDSUPPORTFUND_POPUP" // ����������˾�
#define UPG_CMK_SHOW_GOLDSUPPORTFUND_POPUP "UPG_CMK_SHOW_GOLDSUPPORTFUND_POPUP" // ����������˾�
#define UPG_CMK_GET_GOLDATTENDANCEBOOK_POPUP "UPG_CMK_GET_GOLDATTENDANCEBOOK_POPUP" // ����⼮���˾�
#define UPG_CMK_CREATE_GOLDATTENDANCEBOOK_POPUP "UPG_CMK_CREATE_GOLDATTENDANCEBOOK_POPUP" // ����⼮���˾�
#define UPG_CMK_SHOW_GOLDATTENDANCEBOOK_POPUP "UPG_CMK_SHOW_GOLDATTENDANCEBOOK_POPUP" // ����⼮���˾�

#define UPG_CMK_GET_GOLDWITHDRAW_POPUP "UPG_CMK_GET_GOLDWITHDRAW_POPUP" // ��� ���� ���� �˾�
#define UPG_CMK_SHOW_GOLDWITHDRAW_POPUP "UPG_CMK_SHOW_GOLDWITHDRAW_POPUP" // ��� ���� ���� �˾�

#define UPG_CMK_CHANGE_PROMOTION_LOBBY "UPG_CMK_CHANGE_PROMOTION_LOBBY"
#define UPG_CMK_ROOMLISTHEADER_CHANGE "UPG_CMK_ROOMLISTHEADER_CHANGE"
#define UPG_CMK_SET_FUNDEVENT_ENABLE "UPG_CMK_SET_FUNDEVENT_ENABLE"

#define UPG_CMK_GET_ENDPOPUP_PROMOTION		"UPG_CMK_GET_ENDPOPUP_PROMOTION"
#define UPG_CMK_INIT_ENDPOPUP_PROMOTION		"UPG_CMK_INIT_ENDPOPUP_PROMOTION"
#define UPG_CMK_PAINT_ENDPOPUP_PROMOTION	"UPG_CMK_PAINT_ENDPOPUP_PROMOTION"
#define UPG_CMK_CLICK_ENDPOPUP_PROMOTION	"UPG_CMK_CLICK_ENDPOPUP_PROMOTION"
#define UPG_CMK_DESTROY_ENDPOPUP_PROMOTION	"UPG_CMK_DESTROY_ENDPOPUP_PROMOTION"

#define UPG_CMK_CLEAR_JACKPOTMONEY			"UPG_CMK_CLEAR_JACKPOTMONEY"
#define UPG_CMK_SET_JACKPOTEFFECT_SHOW		"UPG_CMK_SET_JACKPOTEFFECT_SHOW"

#define UPG_CMK_SHOW_LUCKYWHEEL_POPUP		"UPG_CMK_SHOW_LUCKYWHEEL_POPUP"		//��Ű�� ���������� ����Ʈ ��ġ�� �ʰ�.

#define UPG_CMK_SHOW_POPUP_ENDDING_UPP_LV3		"UPG_CMK_SHOW_POPUP_ENDDING_UPP_LV3"

#define UPG_CMK_RESET_WARNINGTIMER			"UPG_CMK_RESET_WARNINGTIMER"
#define UPG_CMK_RESET_GAMEMONEY				"UPG_CMK_RESET_GAMEMONEY"  //�Ӵ� ������ �κ� UI����
#define UPG_CMK_REFRESH_MYMONEY				"UPG_CMK_REFRESH_MYMONEY"

#define UPG_CMK_LOBBY_PLAY_TOGETHER				"UPG_CMK_LOBBY_PLAY_TOGETHER"			//���Ӱ����ϱ�/ ���������� (�κ����μ���)
#define UPG_CMK_LOBBY_PLAY_TOGETHER_COMMUNITY	"UPG_CMK_LOBBY_PLAY_TOGETHER_COMMUNITY"	//���Ӱ����ϱ�/ ���������� (�κ����μ���) - Ŀ�´�Ƽ
#define UPG_CMK_LOBBY_PLAY_TOGETHER_CAFE		"UPG_CMK_LOBBY_PLAY_TOGETHER_CAFE"		//���Ӱ����ϱ�/ ���������� (�κ����μ���) - ī�俬�չ�

#define UPG_CMK_NOTIFY_ACTIVE_PCPROMOTION	"UPG_CMK_NOTIFY_ACTIVE_PCPROMOTION"			//PC�� ��� ���θ���� Active or non-Active ���θ� ���ǿ��� NOTY����

#define UPG_CMK_SET_URL_LOBBY_EVENT			"UPG_CMK_SET_URL_LOBBY_EVENT"				//���� �̺�Ʈ ��� ���� URL ��ü
#define UPG_CMK_GET_LOBBY_WINDOW			"UPG_CMK_GET_LOBBY_WINDOW"

#define UPG_CMK_SEND_CREATE_CHATTING		"UPG_CMK_SEND_CREATE_CHATTING"

#define UPG_CHECK_SHOW_COMMUNITY_WINDOW		"UPG_CHECK_SHOW_COMMUNITY_WINDOW"
#define UPG_ADD_CHATTING_WHISPER_LIST		"UPG_ADD_CHATTING_WHISPER_LIST"

//���̷ο� ����
#define UPG_CMK_GET_LOW_JOKBO_NAME			"UPG_CMK_GET_LOW_JOKBO_NAME"			// �ο������� ��������
#define UPG_CMK_GET_MY_LOWJOKBO_NAME		"UPG_CMK_GET_MY_LOWJOKBO_NAME"			// �� �ο� ������ ��������

#define UPG_CMK_SET_USER_LIST_TAB_TYPE		"UPG_CMK_SET_USER_LIST_TAB_TYPE"
#define UPG_CMK_GET_USER_LIST_TAB_TYPE		"UPG_CMK_GET_USER_LIST_TAB_TYPE"
#define UPG_CMK_SET_USER_LIST_TAB_ACTIVE	"UPG_CMK_SET_USER_LIST_TAB_ACTIVE"

#define UPG_CMK_SHOW_ADDUSERSELECTDLG				"UPG_CMK_SHOW_ADDUSERSELECTDLG"
#define UPG_CMK_CLOSE_ADDUSERSELECTDLG				"UPG_CMK_CLOSE_ADDUSERSELECTDLG"
#define UPG_CMK_IS_SHOW_LOADING_USERLIST			"UPG_CMK_IS_SHOW_LOADING_USERLIST"
#define UPG_CMK_ADDUSERSELECTDLG_SELCHANGE_GAME		"UPG_CMK_ADDUSERSELECTDLG_SELCHANGE_GAME"
#define UPG_CMK_ADDUSERSELECTDLG_SELCHANGE_MONEY	"UPG_CMK_ADDUSERSELECTDLG_SELCHANGE_MONEY"

#define UPG_CMK_CHANGE_MYUSERINFO					"UPG_CMK_CHANGE_MYUSERINFO"

#define UPG_WEB_CALL_LIST                           "UPG_WEB_CALL_LIST"
#define UPG_MEMO_CALL_LIST                          "UPG_MEMO_CALL_LIST"
#define UPG_MEMO_CHECK_START						"UPG_MEMO_CHECK_START"

#define UPG_CMK_SET_PLAY_RAKEBACK_GOLD				"UPG_CMK_SET_PLAY_RAKEBACK_GOLD"
#define UPG_CMK_SET_PLAY_RAKEBACK_GOLD_EVENT		"UPG_CMK_SET_PLAY_RAKEBACK_GOLD_EVENT"

#define UPG_CMK_SHOW_GOLD_OWNLIMIT_POPUP			"UPG_CMK_SHOW_GOLD_OWNLIMIT_POPUP"

#define UPG_CMK_SHOW_CAPTCHA_DLG						"UPG_CMK_SHOW_GOLD_CAPTCHA_DLG"
#define UPG_CMK_CLOSE_CAPTCHA_DLG						"UPG_CMK_CLOSE_GOLD_CAPTCHA_DLG"
#define UPG_CMK_GET_CAPTCHA_DLG							"UPG_CMK_GET_GOLD_CAPTCHA_DLG"
#define UPG_CMK_SET_AUTHENTICODE_CAPTCHA			"UPG_CMK_SET_AUTHENTICODE_CAPTCHA"
#define UPG_CMK_GET_AUTHENTICODE_CAPTCHA			"UPG_CMK_GET_AUTHENTICODE_CAPTCHA"
#define UPG_CMK_SET_NEED_CAPTCHA						"UPG_CMK_SET_NEED_CAPTCHA"
#define UPG_CMK_GET_NEED_CAPTCHA						"UPG_CMK_GET_NEED_CAPTCHA"

#define UPG_CMK_GET_CHUSEOK_ATTENDANCE_DLG		"UPG_CMK_GET_CHUSEOK_ATTENDANCE_DLG"
#define UPG_CMK_SHOW_CHUSEOK_ATTENDANCE_DLG		"UPG_CMK_SHOW_CHUSEOK_ATTENDANCE_DLG"
#define UPG_CMK_GET_CHUSEOK_POPUP "UPG_CMK_GET_CHUSEOK_POPUP"
#define UPG_CMK_SHOW_CHUSEOK_POPUP "UPG_CMK_SHOW_CHUSEOK_POPUP"
#define UPG_CMK_SHOW_CHUSEOK_ATTENDANCE_BTN		"UPG_CMK_SHOW_CHUSEOK_ATTENDANCE_BTN"
#define UPG_CMK_SHOW_CHUSEOK_GOLDCOIN				"UPG_CMK_SHOW_CHUSEOK_GOLDCOIN"

#define UPG_CMK_GET_LOBBY_NOTIFY_DLG			"UPG_CMK_GET_LOBBY_NOTIFY_DLG"
#define UPG_CMK_SHOW_LOBBY_NOTIFY_DLG			"UPG_CMK_SHOW_LOBBY_NOTIFY_DLG"

#define UPG_CMK_SHOW_MESSAGE_MODELESS_DLG			"UPG_CMK_SHOW_MESSAGE_MODELESS_DLG"

#define NORMAL_GAME  0
#define CHOICE_GAME  1
#define BOMB_GAME	 2
#define JOKER_GAME	 3

enum ENUM_SHOW_USERLIST_TYPE
{
	ESUT_CONNECT_USER = 0,
	ESUT_MY_BUDDY,
	ESUT_CLUB,
	ESUT_CAFE,
	ESUT_BLACK_BUDDY
};

enum ENUM_PROFILE_OPEN_KIND
{
	EPOK_NONE = 0,
	EPOK_LOBBY_MY_AVATAR,
	EPOK_ROOM_CLICK_AVATAR
};

enum ENUM_TIMER_ID
{
	// CChattingRoomInviteDlg
	TIMER_CHATROOMINVITE_PROGRESS,
	// CComboBoxImageDlg
	TIMER_COMBOIMG_DRAW,
	// CEndPopup
	TIMER_ENDPOPUP_DRAW_PROGBAR,
	// CGameGradeDlg
	TIMER_GAMEGRADE_COMMAND,
	// CMainFrame
	TIMER_MAINFRAME_BADUSER,
	TIMER_MAINFRAME_CEHCK_LONGTIMEPLAY,
	TIMER_MAINFRAME_CEHCK_PROCESS,
	TIMER_MAINFRAME_REFRESH_LOGIN_COOKIE,
	TIMER_MAINFRAME_WEBCALL,
	TIMER_MAINFRAME_LOGIN_POPUP,
	// CLoadingDlg
	TIMER_LODING_MSGBOX_SELFDESTROY,
	// CMessageNotificationPopup
	TIMER_MSGNOTIPOP_SLIDE,
	// CMsgBoxDlg
	TIMER_MSGBOX_CANCEL,
	// CNewGameItemDlg
	TIMER_NEWGAMEITEM_RESIZE,
	// CTimeNotifierDlg
	TIMER_TIMENOTIFIERDLG,
	// UPGDlgBase
	TIMER_UPGDLGBASE_REDRAW = 10000,
	// CChipAttendPopup
	TIMER_CHIPATTEND_WHEEL_ANI = 20000,
	TIMER_CHIPATTEND_RESULT_DLG,
	// CChipAttendSubPopup
	TIMER_CHIPATTEND_CLOSE_DLG,
};

class CAddUserSelectDlg;

//UPG_COMMON_EXPORT extern NMBASE::SOCK::CSockMan		NMBASE::SOCK::GSOCKMAN();		// ���� �޴��� Ŭ����
UPG_COMMON_EXPORT extern NMBASE::UTIL::CNoChat			NoChat;
UPG_COMMON_EXPORT extern CAdminMessageDlg	*g_pAMes; // [������ ��� �۾�]

//UPG_COMMON_EXPORT extern CTodayTipDlg		g_TipManDlg;			// �� 

UPG_COMMON_EXPORT extern CString			g_ServIP;		// ���� ������ IP
UPG_COMMON_EXPORT extern UINT				g_ServPort;		// ���� ������ ��Ʈ��ȣ
UPG_COMMON_EXPORT extern CString			g_AuthCookie;	// ���� ��Ű						[SSO �۾�]
UPG_COMMON_EXPORT extern CString			g_DataCookie;	// ������ ��Ű						[SSO �۾�]
UPG_COMMON_EXPORT extern CString			g_CpCookie;		// CP��Ű							[SSO �۾�]
UPG_COMMON_EXPORT extern CString			g_SpareParam;	// ������ ����(CP���ӵ�� ���)	[SSO �۾�]
UPG_COMMON_EXPORT extern CString			g_LoginID;		// �ڵ� �α��� ID
UPG_COMMON_EXPORT extern CString			g_LoginPass;	// �ڵ� �α��� �н�����
UPG_COMMON_EXPORT extern int				g_LoginGroup;	// �α��ν� ��û�� �׷�
UPG_COMMON_EXPORT extern int				g_LoginGameCode;	// �α��ν� ��û�� �����ڵ�
UPG_COMMON_EXPORT extern CString			g_MyUniqNo;		// �� ����ũ �ѹ�

UPG_COMMON_EXPORT extern Ms_DestServerInfo	g_DestSvInfo;	// ������ ������ ���� ����
UPG_COMMON_EXPORT extern int 				g_NowGroupNo;	// ������ �׷� ��ȣ
UPG_COMMON_EXPORT extern IDX_GAMERECORD		g_NowGameRecord;//������ ä�� ����
UPG_COMMON_EXPORT extern IDX_GROUPCLASS		g_NowGroupClass;//������ �׷챺(�������� ���� ����)

UPG_COMMON_EXPORT extern int 				g_NowChanNo;	// ������ ���� ä�� ��ȣ
UPG_COMMON_EXPORT extern CString			g_NowChanName;	// ������ ä�� �̸�
UPG_COMMON_EXPORT extern CString			g_strSrvInfo;	// ä�� ����

UPG_COMMON_EXPORT extern int				g_MSCode;
UPG_COMMON_EXPORT extern NMBASE::UTIL::CStringManager			g_StrMan;

UPG_COMMON_EXPORT extern int				g_MasterSID;	// ������ ������ ���� ��ȣ
UPG_COMMON_EXPORT extern int				g_MainSrvSID;	// ���� ������ ���� ��ȣ

UPG_COMMON_EXPORT extern CString			g_strMasterServerName;	// ������ ������ ���� ��ȣ
UPG_COMMON_EXPORT extern CString			g_strConnectecMasterServerName;	// ������ ������ ���� ��ȣ
UPG_COMMON_EXPORT extern CString			g_strMainServerName;	// ���� ������ ���� ��ȣ


UPG_COMMON_EXPORT extern CRect				g_OrgMainDlgRect;	// ���� ���̾�α��� ���� ũ��
UPG_COMMON_EXPORT extern BOOL				g_bNoGo;
UPG_COMMON_EXPORT extern BOOL				g_bChannel;
UPG_COMMON_EXPORT extern int				g_MyPN;				// ���� �÷��̾� ��ȣ(0-2)
UPG_COMMON_EXPORT extern int				g_MastPN;			// ������ �÷��̾� ��ȣ(0-2)
UPG_COMMON_EXPORT extern ROOMINFO			g_RI;				// �� ���� ����ü
UPG_COMMON_EXPORT extern NMBASE::GRAPHICGDI::CDisplay			Display;			// ���÷��� ��� ���� Ŭ����

UPG_COMMON_EXPORT extern BOOL				g_bMyObserver;
UPG_COMMON_EXPORT extern BOOL				g_bAdminUser;		// [������ ��� �۾�]
UPG_COMMON_EXPORT extern BOOL				g_bConfirmation;	//[������ ���� Ȯ��]

//////////// [���� ������ �۾�] ////////////
UPG_COMMON_EXPORT extern GAMEITEM_LIST		g_MyGameItem;		// �� ���� ������
UPG_COMMON_EXPORT extern COleDateTimeSpan	g_CSTimeGap;		// ������ Ŭ���̾�Ʈ�� �ð���

UPG_COMMON_EXPORT extern DWORD				g_nStartTick;		// �������� �ð� ���� ��
UPG_COMMON_EXPORT extern DWORD				g_DebugStartTick;	// �������� �ð� ���� ��
UPG_COMMON_EXPORT extern bool				g_bLobyItemFirstNavigation;	// [������ ����ȭ ����]

////////////////////////////////////////////
UPG_COMMON_EXPORT extern POKERGAME	g_Poker;
UPG_COMMON_EXPORT extern CString	g_TargetID;
/////////////////////////
UPG_COMMON_EXPORT extern INT64		g_MaxServerMoney;		//���� ���� �� �ִ� �ִ� �Ӵ� (�α��ν� �������� ����)
UPG_COMMON_EXPORT extern INT64		g_DisplayMaxMoney[MAX_CODE_NEW_FAMILY]; //�йи� ȸ���� �ִ� �����Ӵ� (�α��ν� �������� ����)
/////////////////////////
UPG_COMMON_EXPORT extern INT64		g_MaxServerGold;		//���� ���� �� �ִ� �ִ� ��� (�α��ν� �������� ����)
UPG_COMMON_EXPORT extern INT64		g_DisplayMaxGold[MAX_CODE_NEW_FAMILY]; //�йи� ȸ���� �ִ� ������� (�α��ν� �������� ����)
UPG_COMMON_EXPORT extern BOOL		g_bNewlySignedUp;//���̵� ����� ó�� �α����ߴ��� ����
/////////////////////////
UPG_COMMON_EXPORT extern INT64		g_MaxServerGoldBank;		//���� ���� �� �ִ� �ִ� ���� ���(�α��ν� �������� ����)
UPG_COMMON_EXPORT extern INT64		g_DisplayMaxGoldBank[MAX_CODE_NEW_FAMILY]; //�йи� ȸ���� �ִ� ���� ���� ��� (�α��ν� �������� ����)

UPG_COMMON_EXPORT extern CHAT_MACRO	g_ChatMacro; // ��ũ��(ä�� ����)
UPG_COMMON_EXPORT extern float		g_fRaceBetRatio;// ���̽� ���� ����  % 
UPG_COMMON_EXPORT extern GAMESPEED	g_GameSpeed;

UPG_COMMON_EXPORT extern BOOL		g_bJumpItemBuying;
UPG_COMMON_EXPORT extern BOOL		g_bRealAdmissionBuying; //���� ä�� �����

//�� ����â���� ������ ��� �Ѵٰ� ������ ���³�
UPG_COMMON_EXPORT extern BOOL		g_bAskContinue; 

UPG_COMMON_EXPORT extern MYGAMEINOF	g_MyGameInfo;	//���� ��� �ɼ� �� ���� �Ѵ�.
UPG_COMMON_EXPORT extern PREMOPTION	g_PremOption;	//�����̾� �ɼ�
UPG_COMMON_EXPORT extern jackpot::JP_MONEYINFO g_JackPotMoneyInfo;	// ���̱ݾ� ����


UPG_COMMON_EXPORT extern const int	REFRESH_FPS;
UPG_COMMON_EXPORT extern const int	HEIGHT_CAPTION_LOBBY;
UPG_COMMON_EXPORT extern CONFIG		g_Config;
/////////////////////////////////////////////////////////////////////////////////////

UPG_COMMON_EXPORT double	GetRadian(int x1, int y1, int x2, int y2);
UPG_COMMON_EXPORT float	GetAngle(int x1, int y1, int x2, int y2);
UPG_COMMON_EXPORT float	GetDistance(int x1, int y1, int x2, int y2);
UPG_COMMON_EXPORT POINT	GetMovePoint(int sx, int sy, float angle, float distance);
UPG_COMMON_EXPORT float	GetCrossAngle(float sangle, float rangle) ;

UPG_COMMON_EXPORT void	_LOADSPRITEDEFINE( NMBASE::GRAPHICGDI::xSprite &spr,	string &dir, char *pname, bool bForceLoad = true );
UPG_COMMON_EXPORT bool	_LOADSPRITEDEFINE( NMBASE::GRAPHICGDI::xSprite *pSpr,	string &dir, char* pname, bool bForceLoad = true ); //< CYS 100525 >
UPG_COMMON_EXPORT bool	_LOADSPRITEDEFINE( NMBASE::GRAPHICGDI::xSprite &pSpr,	string &pFilePath, bool bForceLoad = true ); //< CYS 100525 >

UPG_COMMON_EXPORT BOOL	OnMacroCheck(BOOL bclose = TRUE); //FALSE ��� TRUE ���� ����
UPG_COMMON_EXPORT BOOL	CheckCompareString(CString *dest_str, CString *compare_str);

UPG_COMMON_EXPORT void	SSO_OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
UPG_COMMON_EXPORT CString GetPostedData( CString sMacroString, LPCTSTR lpszPostedData );
UPG_COMMON_EXPORT CString GetPostedData1( CString sMacroString, LPCTSTR lpszPostedData );
UPG_COMMON_EXPORT CString g_GetLevelName(int nLevel);
UPG_COMMON_EXPORT IDX_GAME get_GameMode(int groupcode);		//���� ��带 ������ ��

UPG_COMMON_EXPORT INT64		get_BetMoney( int index , int GroupNo = 5);
UPG_COMMON_EXPORT CString	get_Numberstring(INT64 number);
UPG_COMMON_EXPORT CString	get_str_AutoBetRule(int autokind ,int game , int groupclass, BOOL bName = FALSE );
UPG_COMMON_EXPORT BOOL		g_Ask_RealChannel_JoinRoomMsg();			//è�Ǿ� ä�ο��� ����� ���� �Ӵ� üũ �� ����  ó��
UPG_COMMON_EXPORT int		g_GetURLChannelNo(); //�� URL ���� �����  ä�ι�ȣ ���ϱ�
UPG_COMMON_EXPORT CString	get_GroupName(int group);
UPG_COMMON_EXPORT CString	GetMACAddress(const char *strIP);

UPG_COMMON_EXPORT BOOL IsValidObserverChat();

//�˾� �޽��� ���� �Լ���
UPG_COMMON_EXPORT LRESULT ShowMainMessageDlg(const char *pMsg);					//���� �޽���â�� ����.
UPG_COMMON_EXPORT LRESULT ShowMsgChkDlg(const char *pMsg, int nType = 0);		//üũ�ڽ��� ���Ե� �޽��� â�� ����. nType�� 1�̸� üũ�ڽ� ����.
UPG_COMMON_EXPORT LRESULT ShowMsgModelessDlg(HWND parentHWnd, LPCTSTR msg);
UPG_COMMON_EXPORT void CloseMainMessageDlg(void);								//���� �޽���â�� �ݴ´�.
UPG_COMMON_EXPORT void ClearMainMessageDlg(void);								//���� �޽���â�� �޽����� ��� �����.
UPG_COMMON_EXPORT bool IsVisibleMainMessageDlg(void);							//���� �޽���â�� ���ִ����� �����Ѵ�.
UPG_COMMON_EXPORT void ShowTipManDlg(void);										//������ �� ���̾˷α׸� �����ش�.
UPG_COMMON_EXPORT void ShowDirectGoDlg(void);
UPG_COMMON_EXPORT void ShowSelectStartDlg(void);
UPG_COMMON_EXPORT void ShowResetSelectStartDlg(void);
UPG_COMMON_EXPORT LRESULT ShowQuickJoinerDlg(void);						//������� �ٷ����� ����â�� �����ش�.
//������� �ٷΰ��� ��Ŷ�� ������.
//������� �ٷΰ��Ⱑ �����Ǿ� ���� �ʴٸ� false�� �����Ѵ�.
UPG_COMMON_EXPORT bool SendQuickJoiner(void);							//������� �ٷ������� Ȱ��ȭ �Ǿ� �ִ��� �����ϴ� �Լ�
UPG_COMMON_EXPORT void CloseLoadingDlg(bool bCancle = true);			//�ε�â�� �ݴ´�.

//�� ���ӻ��� ���� �Լ���
UPG_COMMON_EXPORT  void SetAccessRoomState(bool bAccessRoomState);		//������ ���¸� �����Ѵ�.
UPG_COMMON_EXPORT  bool IsAccessRoomState(void);						//������ ���θ� �����Ѵ�.

//Ǯ��ũ�� ���� �Լ���
UPG_COMMON_EXPORT  void SetFullScreenState(bool bFullScreen);			//Ǯ��ũ�� ���¸� �����Ѵ�.
UPG_COMMON_EXPORT  bool IsFullScreenState(void);						//���� Ǯ��ũ�� �������� ���θ� �����Ѵ�.

UPG_COMMON_EXPORT SMALLUSERINFO *GetUserSmallInfo(const char *pID);		//���� ������ ���ؿ��� �Լ�

UPG_COMMON_EXPORT void ClearLoadImageList(void);								//�̹��� �ε�â���� �̹��� ����Ʈ�� �ʱ�ȭ �Ѵ�.
																				//�̹��� �ε�â���� �о�帱 �̹����� ����Ѵ�.
UPG_COMMON_EXPORT LRESULT AddLoadImageList(NMBASE::GRAPHICGDI::xSprite *pSprite, const char *pPath, bool bRegisterGameSprite = false);
UPG_COMMON_EXPORT void LoadingImageList(DWORD dwMinShowTime, BOOL bBasedOnTime);//�̹��� �ε�â�� ��ϵ� �̹����� �ε��Ѵ�.

UPG_COMMON_EXPORT LRESULT SetChangeWhere(IDX_GAMEWHERE nView, bool bRefresh);	//������ ���� ��ġ�� �����Ѵ�. bRefresh�� ���Ƿ� �̵��� ���� ����Ʈ�� �ٽ� ������ ���θ� �����Ѵ�.
UPG_COMMON_EXPORT IDX_GAMEWHERE GetCurrentWhere();	 //������ ���� ��ġ�� ���Ѵ�.
UPG_COMMON_EXPORT LRESULT SaveGameConfig(LPCTSTR lpID);					//���� �ɼ��� �����Ѵ�
UPG_COMMON_EXPORT LRESULT LoadGameConfig(LPCTSTR lpID);					//���� �ɼ��� �����Ѵ�
UPG_COMMON_EXPORT CObserverDlg *GetObserverDlg(void);					//������ ���̾˷α� ��������
UPG_COMMON_EXPORT CInviteDlg *GetInviteDlg(void);						//�ʴ��ϱ� ���̾˷α� ��������
UPG_COMMON_EXPORT CEndEventPopupDlg *GetEndEventDlg(void);				//(�̺�Ʈ)���� ���̾˷α� ��������
UPG_COMMON_EXPORT CProfileDlg *GetProfileDlg(void);						//������ ���̾˷α� ��������
UPG_COMMON_EXPORT CWhisperDlg *GetWhisperDlg(void);						//�Ӹ��ϱ� ���̾˷α� ��������
UPG_COMMON_EXPORT CReceiveMessageDlg *GetReceiveMessageDlg(void);		//�������� ���̾˷α� ��������
UPG_COMMON_EXPORT CReceiveAdminMessageDlg *GetReceiveAdminMessageDlg(void);	//���� ��� ���� ���̾˷α� ��������
UPG_COMMON_EXPORT CSendMessageDlg *GetSendMessageDlg(void);				//���������� ���̾˷α� ��������
UPG_COMMON_EXPORT CMessageBox *GetMessageBoxDlg(void);					//������ ���̾˷α� ��������
UPG_COMMON_EXPORT void BanishKickOut(const char *pTargetID);			//���� ó��
UPG_COMMON_EXPORT void RefreshGameItemDlg_MyItem(void);					//���� ������ ���̾˷α׿��� �� ������ ����Ʈ�� �����Ѵ�.
UPG_COMMON_EXPORT void CloseGameItemDlg(void);							//���� ������ ���̾˷α׸� �ݴ´�.
UPG_COMMON_EXPORT void ShowGameItemDlg(bool bForceClose);				//���� ������ ���̾˷α׸� �����ش�.
UPG_COMMON_EXPORT void ExitGameClient(bool bShowEndPopup);				//Ŭ���̾�Ʈ�� �����Ѵ�.
UPG_COMMON_EXPORT void SetMainWindowText(LPCTSTR lpText);				//������������ �ؽ�Ʈ�� �����Ѵ�.
UPG_COMMON_EXPORT CWnd *GetLobbyWindow(void);							//�κ� ������ ��������

UPG_COMMON_EXPORT void AddLobbyHideChatText(CString *pMsg);				//�ҷ����� �Ű� ���� Hide ä��â�� �޽��� ���
UPG_COMMON_EXPORT void AddLobbyChatViewEdit(const char *str, COLORREF crNewColor=RGB(20,20,20));	//�κ� ä��â�� �޽��� ���
UPG_COMMON_EXPORT void AddGameInfoViewEdit(const char *str, COLORREF crNewColor=RGB(0,0,0), bool bUserChat = false);	//���� ä��â�� �޽��� ���
UPG_COMMON_EXPORT void AddGameChatViewEdit(const char *str, COLORREF crNewColor=RGB(0,0,0), bool bUserChat = false);	//���� ä��â�� �޽��� ���

UPG_COMMON_EXPORT void ResetBadUserTimer(void);		//�ҷ����� Ÿ�̸� �ʱ�ȭ
UPG_COMMON_EXPORT void ExitGameRequest(bool bUserAFK= false);		//���ӹ� ������ ��û
UPG_COMMON_EXPORT void CloseClient(void);			//Ŭ���̾�Ʈ ����
UPG_COMMON_EXPORT bool ChangeDisplayMode(void);		//Ǯ��ũ��/â��� ��ȭ�� ��û�Ѵ�.
UPG_COMMON_EXPORT void SaveScreenCapture(void);		//��ũ���� ĸ���Ͽ� �����Ѵ�.
UPG_COMMON_EXPORT bool CheckBadUserReport(void);	//�ֱ� 1�е��� BadUser �Ű� �߾������� �˻��Ѵ�.
UPG_COMMON_EXPORT LRESULT ShowNewAskChargeDlg(NEW_ASK_CHARGE_DLG_TYPE type, LPCTSTR lpMessage);
UPG_COMMON_EXPORT BOOL UPGIsGameItemUsing(int itemcode, BOOL *pExist);
UPG_COMMON_EXPORT int UPGCheckPremiumState(USERINFO* pUI);
UPG_COMMON_EXPORT GAMEITEM *UPGNotUseGameItem(int itemcode);
UPG_COMMON_EXPORT CString UPGGetAutoBettingRuleName(IDX_GAME nGame,  int nAutoBet, BETRULE_KIND eBetRuleKind = BETRULE_KIND_REAL, RULETYPE ruleType=RULETYPE_7POKER);
UPG_COMMON_EXPORT void DrawGameText(CDC &dc, IDX_GAME idxGame, LPRECT rt, UINT nFormat);		//�� ���Ӹ��� ���� �°� �׸���.

UPG_COMMON_EXPORT  void ShowChanceBtn(void);							//������ư�� ������ �ش�.
UPG_COMMON_EXPORT void OpenGameManualPage(void);
UPG_COMMON_EXPORT void OpenPCRoomAdvantagePage(int nPCRoomCode);
UPG_COMMON_EXPORT void AskProfileToServer(UINT64 llUserKey, const char* pID, const char *pUniqNo = "");

UPG_COMMON_EXPORT void ShowProfileDialog(USERINFO *pUserinfo, CWnd *pParent, ENUM_PROFILE_OPEN_KIND enumProfileOpenKind = EPOK_NONE);
UPG_COMMON_EXPORT void CloseProfileDialog(void);

UPG_COMMON_EXPORT void ShowWhisperDialog(SMALLUSERINFO *pInfo, CWnd *pParent);
UPG_COMMON_EXPORT void CloseWhisperDialog(void);

UPG_COMMON_EXPORT void ShowReceiveMessageDialog(Camel::Dar2::SS::Memo *pMemo, CWnd *pParent);
UPG_COMMON_EXPORT void CloseReceiveMessageDialog(void);

UPG_COMMON_EXPORT void ShowSendMessageDialog( LPCTSTR lpFriendNickName, bool bGroup = false );
UPG_COMMON_EXPORT void CloseSendMessageDialog(void);

UPG_COMMON_EXPORT void CloseMessageBoxDialog(void);

UPG_COMMON_EXPORT void SetUserListTabType(ENUM_SHOW_USERLIST_TYPE nType);
UPG_COMMON_EXPORT ENUM_SHOW_USERLIST_TYPE GetUserListTabType(void);

UPG_COMMON_EXPORT void SendCreateChatting(std::list<std::string> *pListInviteID, char *pNotConnectMessage);

UPG_COMMON_EXPORT bool CheckShowCommunityWindow(void);
UPG_COMMON_EXPORT void AddChattingWhisperList(LPCTSTR lpNickName, bool bSetCurSel, bool bStartAnimation, bool bAllUser);

UPG_COMMON_EXPORT LSY::CObjectMan *CreateObjectMan(CWnd *pWnd, CWnd *pMainWnd);
UPG_COMMON_EXPORT void ReleaseObjectMan(void);
UPG_COMMON_EXPORT LSY::CObjectMan *GetObjectMan(void);
UPG_COMMON_EXPORT void AllDrawObjectMan(void);

UPG_COMMON_EXPORT void AddSelectUserDlg(CAddUserSelectDlg *pDlg);
UPG_COMMON_EXPORT void SubSelectUserDlg(CAddUserSelectDlg *pDlg);
UPG_COMMON_EXPORT bool IsShowLoadingUserList(void);
UPG_COMMON_EXPORT void SelChangeGameAddSelectUserDlg(IDX_GAME nGame);
UPG_COMMON_EXPORT void SelChangeMoneyAddSelectUserDlg(int nIndex);

UPG_COMMON_EXPORT bool DebugLog(LPCTSTR szLogFile, LPCTSTR szLogMsg, ...);
UPG_COMMON_EXPORT char* ANSIToUTF8(const char * pszCode);
UPG_COMMON_EXPORT char* URLencode(const char *str);


inline COleDateTime g_GetSyncTime()		// ����ȭ�� ���� �ð��� ������ 
{
	// ����ȭ�� ���� �ð��� ������ 
	return COleDateTime::GetCurrentTime() - COleDateTimeSpan(0,0,0, (int)g_CSTimeGap.GetTotalSeconds());
}

//�����̸�
inline char *g_GetInsuName(int InsuCode)
{
	static char* InsuName[]={"?", "?", "?", "���", "����", "����", "����", "����","?","?","?" };	

	//6:����, 7:����
	if (InsuCode<TOP_INSURANCE || InsuCode>BOTTOM_INSURANCE)
		return InsuName[0];

	return InsuName[InsuCode];
}
inline char *g_GetFormName(IDX_GAME idxGame, int nFormName)
{
	static char* FormName[]={"", "�Ϲ�", "���̽�", "��ź", "��Ŀ", "2��", "3��", "�Ϲ�", "��Ŀ", "����", "?", "?", "?", "?", "?"};
	
	if (nFormName<0 || nFormName>4)
		return FormName[0];

	if (idxGame==IDX_GAME_BD)
		return FormName[0];

	if(idxGame==IDX_GAME_SD)
	{		
		return FormName[nFormName+5];
	}

	if(idxGame==IDX_GAME_HA)
	{		
		return FormName[nFormName+6];
	}

	return FormName[nFormName+1];

}

inline char *g_GetGameKindName(IDX_GAME idxGame, int nGameKind)
{
	static char* szGameKindName[]={"", "�Ϲ�", "����Off", "����On", "?", "?", "?", "?"};

	if (nGameKind<0 || nGameKind>3)
		return szGameKindName[0];


	if(idxGame==IDX_GAME_HA)
	{		
		return szGameKindName[nGameKind+1];
	}

	return szGameKindName[0];

}


/// �������� 1: 7��Ŀ(RULETYPE_7POKER),  2: 5��Ŀ(RULETYPE_5POKER)
inline char *g_GetRuleTypeName(int nRuleType) //����Ŀ ��Ÿ��
{
	
	static char* RuleTypeName[]={"?", "7��Ŀ", "������", "?", "?", "?", "?", "?"};

	if (nRuleType<1 || nRuleType>2)
		return RuleTypeName[0];

	return RuleTypeName[nRuleType];

}

inline CString g_NumberToOrientalString( INT64 number, int nCount )
{
	CString strrtn; // �ѱ�ǥ��
	CString strtemp;
	INT64 t = number;

	BOOL bMinus = FALSE;
	if( t < 0 ) {
		bMinus = TRUE;
		t *= -1;
	}

	int nCnt = 0 ; 

	INT64 v = t / (INT64)10000000000000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)10000000000000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;

		nCnt++ ;
	}

	if(nCnt >= nCount)
		return strrtn ;

	v = t / (INT64)1000000000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)1000000000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;

		nCnt++ ;
	}

	if(nCnt >= nCount)
		return strrtn ;

	v = t / (INT64)100000000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)100000000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;

		nCnt++ ;
	}

	if(nCnt >= nCount)
		return strrtn ;

	v = t / (INT64)10000;	// �� ����
	if(v > 0) {
		t = t - v*(INT64)10000;
		strtemp.Format("%I64d��", v);
		strrtn += strtemp;

		nCnt++ ;
	}

	if(nCnt >= nCount)
		return strrtn ;

	if( t > 0 ) {
		strtemp.Format("%I64d", t);
		strrtn += strtemp;

		nCnt++ ;
	}

	if(nCnt >= nCount)
		return strrtn ;

	if( number == 0 )
	{
		strrtn = "0";
	}

	if( bMinus )
	{
		strrtn.Insert(0,"-");
		//CString temp;
		//temp.Format( "-%s", strrtn );
		//strrtn = temp;
	}
	return strrtn;
}

inline CString g_GetRoomName(IDX_GAME idxGame, SMALLROOMINFO *pSRI)
{
	CString strRoomName;
	CString strRoomType;

	if (pSRI==NULL)
		return strRoomName;

	strRoomType.Format(strChip_Count[pSRI->ChipKind]);

	if ( pSRI->bIsMaxRoom )
	{
		if( idxGame == IDX_GAME_SP ){
			
			
			if(pSRI->llEnterLimitMoney != 0)
			{
				if(2<=pSRI->cMemberRoom){
					if(9900000000000ll == pSRI->llEnterLimitMoney)
					{
						strRoomName.Format("%s%s", NMBASE::UTIL::g_MakeShortMoneyStrEx(10000000000000ll), strRoomType.GetString());
					}
					else{
						strRoomName.Format("%s%s", NMBASE::UTIL::g_MakeShortMoneyStrEx(pSRI->llEnterLimitMoney), strRoomType.GetString());
					}
				}
				else{
					strRoomName.Format("%s%s", NMBASE::UTIL::g_MakeShortMoneyStrEx(pSRI->llEnterLimitMoney), strRoomType.GetString());
				}
			}else
			{
				strRoomName.Format("MAX��");
			}
		}else{
			strRoomName.Format("MAX��");
		}
	}
	else if ( pSRI->llEnterLimitMoney == 0 )
	{
		if ( pSRI->bIsFriendRoom )
		{
			strRoomName.Format("ģ��");
		}
		else
		{
			strRoomName.Format("���Ѿ���");
		}
	}
	else
	{
		if(2<=pSRI->cMemberRoom){
			if(9900000000000ll == pSRI->llEnterLimitMoney)
			{
				strRoomName.Format("%s%s", NMBASE::UTIL::g_MakeShortMoneyStrEx(10000000000000ll), strRoomType.GetString());
			}
			else{
				strRoomName.Format("%s%s", NMBASE::UTIL::g_MakeShortMoneyStrEx(pSRI->llEnterLimitMoney), strRoomType.GetString());
			}
		}
		else{
			strRoomName.Format("%s%s", NMBASE::UTIL::g_MakeShortMoneyStrEx(pSRI->llEnterLimitMoney), strRoomType.GetString());
		}
	}
	return strRoomName;
}

inline CString g_GetSeedMoneyToText( IDX_GAME idxGame, INT64 llSeedMoney )
{
	if ( idxGame == IDX_GAME_HA )
	{
		CString strTemp[4];
		CString str;
		
		if ( llSeedMoney == 300000000000)
		{
			str.Format( "3000��-6000��-9000��-1.2��" );
		}
		else if ( llSeedMoney == 400000000000)
        {
            str.Format( "4000��-4000��-4000��-4000��" );
        }
		else
		{
			for ( int i = 0 ; i < 4 ; i++ )
			{
				strTemp[i] = NMBASE::UTIL::g_NumberToHangul( llSeedMoney * ( i + 1 ) );
			}
			str.Format( "%s-%s-%s-%s", strTemp[0], strTemp[1], strTemp[2], strTemp[3] );
		}
		return str;		
	}
	return NMBASE::UTIL::g_NumberToHangul(llSeedMoney);
}

/// <summary>
/// �� �ڸ� �� ���� �޸� ����ֱ�
/// <summary>
/// <param name="nType"></param>
/// <returns></returns>
inline void g_GetNumberWithComma(CString parm_data, CString &parm_string)
{
	// ������� ��Ȯ�ϰ� �����ϱ� ���ؼ� ',' ó������� ����� ���ڿ� ������ �ʱ�ȭ�Ѵ�.
	if (parm_string.GetLength())
		parm_string.Format("");

	// ���ڿ��� ���̸� ���Ѵ�.
	int count = parm_data.GetLength();

	for (int i = 0; i < count; i++)
	{
		// 3�� ����� �� "," �� ���ڿ��� �߰��Ѵ�.
		if (i && !(i % 3))
			parm_string = "," + parm_string;

		// parm_data ���� parm_string���� �����Ѱ��� ���������� �����Ѵ�.
		parm_string = CString(parm_data[count - 1 - i]) + parm_string;
	}
}

/// <summary>
/// �� �ڸ� �� ���� �޸� ����ֱ�
/// <summary>
/// <param name="nType"></param>
/// <returns></returns>
inline CString g_GetNumberWithComma(INT64 param_int)
{
	CString parm_data;
	CString return_string;

	// �Ű������� CString ���·� ��ȯ.
	parm_data.Format("%I64d", param_int);

	// ������� ��Ȯ�ϰ� �����ϱ� ���ؼ� ',' ó������� ����� ���ڿ� ������ �ʱ�ȭ�Ѵ�.
	if (return_string.GetLength())
		return_string.Format("");

	// ���ڿ��� ���̸� ���Ѵ�.
	int count = parm_data.GetLength();

	for (int i = 0; i < count; i++)
	{
		// 3�� ����� �� "," �� ���ڿ��� �߰��Ѵ�.
		if (i && !(i % 3))
			return_string = "," + return_string;

		// parm_data ���� parm_string���� �����Ѱ��� ���������� �����Ѵ�.
		return_string = CString(parm_data[count - 1 - i]) + return_string;
	}

	return return_string;
}

// 4 �̻� ���� ���ڴ� ****�� ǥ���ϱ�
// 3 �̻� ���� �ѱ� ���ڿ� ���� ������ ***�� ǥ���ϱ�
inline std::string g_ConverNumberToAsterisk(std::string strParam)
{
	std::size_t start_n = strParam.find_first_of("0123456789");
	std::size_t start_k = strParam.find_first_of("���̻�����ĥ�ȱ���");
	int checkSize = 4;				// ���� ����
	int n_length = 0;				// ���Ե� ���� ����
	int k_length = 0;				// ���Ե��ѱ� ���� ����
	std::size_t replaceIndex = 0;	// ���� ���ڿ����� �����Ǿ� �ݿ��� ��ġ
	std::string retStr = "";		// ���� ���ڿ�
	do 
	{
		// �˻��� ���ڰ� ���̻� ���Ե��� �ʾ����� ������ ���� �����ϰ� ����.
		if(start_n == start_k)
		{
			retStr.append(strParam.substr(replaceIndex, strParam.length()-replaceIndex));
			replaceIndex = strParam.length();
			break;
		}

		// ���ο� ���� �˻�� �� ���ڵ� ���� ���ڿ���
		std::size_t end = 0;
		if(n_length + k_length == 0)
		{
			retStr.append(strParam.substr(replaceIndex, min(start_k,start_n)-replaceIndex));
			replaceIndex = min(start_k,start_n);
		}

		if (start_n > start_k)		// �ѱ� ���ڰ� �տ� ������
		{
			checkSize = 3;
			end = strParam.find_first_not_of("���̻�����ĥ�ȱ���", start_k);
			k_length += (end - start_k)/2;
			start_k = strParam.find_first_of("���̻�����ĥ�ȱ���", end);

			// �ѱ� ���� �ڿ� �ٷ� ���ڰ� ����
			if (start_n == end)
			{
				continue;
			}
		}
		else if (start_n < start_k)	// ���ڰ� �տ� ������
		{
			end = strParam.find_first_not_of("0123456789", start_n);
			n_length += end - start_n;
			start_n = strParam.find_first_of("0123456789", end);

			// ���� �ڿ� �ٷ� �ѱ� ���ڰ� ����
			if (start_k == end)
			{
				continue;
			}
		}

		// �ѱ� ���ڰ� ���� ������ ���� ���̺��� ������ *�� ����
		if (n_length + k_length >= checkSize)
		{
			for(int i = 0; i < n_length + k_length; i++)
			{
				retStr.append("*");
			}
			replaceIndex += n_length + k_length * 2;
		}	
		// �˻��� �κб��� ���� ���ڿ��� ����.
		retStr.append(strParam.substr(replaceIndex, end-replaceIndex));

		replaceIndex = end;
		n_length = 0;
		k_length = 0;

		start_k = strParam.find_first_of("���̻�����ĥ�ȱ���", end);
		start_n = strParam.find_first_of("0123456789", end);
	} while (1);
	retStr.append("\0");
	return retStr;
}

inline INT64 g_CheckMarkedExceedingLimitHaveMoney(INT64 userMarkHaveMoney, int premMenuNo)
{
	if(premMenuNo > CODE_NEW_FAMILY_GOLD)
	{
		if(userMarkHaveMoney > 15 * ONE_HUNDRED_TRILLION)
		{
			userMarkHaveMoney = 15 * ONE_HUNDRED_TRILLION;
		}
	}
	else if(premMenuNo > CODE_NEW_FAMILY_SILVER)
	{
		if(userMarkHaveMoney > 5 * ONE_HUNDRED_TRILLION)
		{
			userMarkHaveMoney = 5 * ONE_HUNDRED_TRILLION;
		}
	}
	else
	{
		if(userMarkHaveMoney > 2 * ONE_HUNDRED_TRILLION)
		{
			userMarkHaveMoney = 2 * ONE_HUNDRED_TRILLION;
		}
	}

	return userMarkHaveMoney;
}

inline SYSTEMTIME operator-(const SYSTEMTIME& pSr, const SYSTEMTIME& pSl)
{
	SYSTEMTIME t_res;
	FILETIME v_ftime;
	ULARGE_INTEGER v_ui;
	__int64 v_right, v_left, v_res;

	SystemTimeToFileTime(&pSr,&v_ftime);

	v_ui.LowPart = v_ftime.dwLowDateTime;
	v_ui.HighPart = v_ftime.dwHighDateTime;
	v_right = v_ui.QuadPart;

	SystemTimeToFileTime(&pSl, &v_ftime);
	v_ui.LowPart = v_ftime.dwLowDateTime;
	v_ui.HighPart = v_ftime.dwHighDateTime;
	v_left = v_ui.QuadPart;

	v_res = v_right - v_left;

	v_ui.QuadPart = v_res;
	v_ftime.dwLowDateTime = v_ui.LowPart;
	v_ftime.dwHighDateTime = v_ui.HighPart;
	FileTimeToSystemTime(&v_ftime, &t_res);

	return t_res;
}

inline CString g_AnsiToUTF8(CString Str)
{
	if (Str.GetLength() <= 0 || Str.Trim() == TEXT("")) return TEXT("");

	WCHAR szUniCode[50000];
	memset(&szUniCode, 0, sizeof(szUniCode));

	char szUTF8[50000];
	memset(&szUTF8, 0, sizeof(szUTF8));

	CString strConvert;
	char *szSrc = (LPSTR)(LPCSTR)Str;

	char szRetVal[50000];
	memset(&szRetVal, 0, sizeof(szRetVal));

	int UniCodeSize = MultiByteToWideChar(CP_ACP, 0, szSrc, (int)strlen(szSrc), szUniCode, sizeof(szUniCode));
	int UTF8Size = WideCharToMultiByte(CP_UTF8, 0, szUniCode, UniCodeSize, szUTF8, sizeof(szUTF8), NULL, NULL);

	memcpy(szRetVal, szUTF8, UTF8Size);
	strConvert = szRetVal;

	return strConvert;
}

inline BYTE g_ToHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}

inline CString g_URLEncode(CString sIn)
{
	if (sIn.GetLength() <= 0 || sIn.Trim() == TEXT("")) return TEXT("");

	CString sOut;
	const int nLen = sIn.GetLength() + 1;
	LPBYTE pOutTmp = NULL;
	LPBYTE pOutBuf = NULL;
	LPBYTE pInTmp = NULL;
	LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);

	//alloc out buffer
	pOutBuf = (LPBYTE)sOut.GetBuffer(nLen*3);

	if(pOutBuf)
	{
		pInTmp   = pInBuf;
		pOutTmp = pOutBuf;
		// do encoding
		while (*pInTmp)
		{
			if(isalnum(*pInTmp) || '-'==*pInTmp || '_'==*pInTmp || '.'==*pInTmp)
				*pOutTmp++ = *pInTmp;
			else if(isspace(*pInTmp))
				*pOutTmp++ = '+';
			else
			{
				*pOutTmp++ = '%';
				*pOutTmp++ = g_ToHex(*pInTmp>>4);
				*pOutTmp++ = g_ToHex(*pInTmp%16);
			}
			pInTmp++;
		}
		*pOutTmp = '\0';
		sOut.ReleaseBuffer();
	}
	sIn.ReleaseBuffer();

	return sOut;
}

inline CString g_GetMoneyString(INT64 nMoney)
{
	CString str = ""; // �ѱ�ǥ��
	CString str1 = "";
	INT64 Money = nMoney;

	BOOL bMinus = FALSE;
	if (nMoney < 0) {
		bMinus = TRUE;
		Money = nMoney * -1;
	}

	int num[4] = { 0, };
	NMBASE::UTIL::g_NumberToParse(Money, num[3], num[2], num[1], num[0]);

	if (num[3] > 0) {
		str1.Format("%d��", num[3]);
		str += str1;
		if (num[2] > 0) {
			str1.Format("%d��", num[2]);
			str += str1;
		}
	}
	else if (num[2] > 0) {
		str1.Format("%d��", num[2]);
		str += str1;
		if (num[1] > 0) {
			str1.Format("%d��", num[1]);
			str += str1;
		}
	}
	else if (num[1] > 0) {
		str1.Format("%d��", num[1]);
		str += str1;
		if (num[0] > 0) {
			str1.Format("%d", num[0]);
			str += str1;
		}
	}
	else if (num[0] > 0) {
		str1.Format("%d", num[0]);
		str += str1;
	}
	else {
		str = "0";
	}

	return str;
}
