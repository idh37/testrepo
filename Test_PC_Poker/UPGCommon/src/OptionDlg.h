#pragma once
#include "UserStruct.h"
#include <memory>

#define MODE_OPTION		0
#define MODE_MACRO		1
#define MODE_DSEVENT	2

#define IDC_EDIT_MACRO					1511

//베팅버튼 타입
enum CONFIG_BETBTN_TYPE
{
	CONFIG_BETBTN_TYPE_N,
	CONFIG_BETBTN_TYPE_H,
	CONFIG_BETBTN_TYPE_P
};

// 옵션 데이터
typedef struct
{
	char BGMVolume;						// 배경음 볼륨
	BOOL bMusicON;						// 배경음 On/Off
	char FxVolume;						// 효과음 볼륨
	BOOL bSndFX;						// 효과음 On/Off
	CONFIG_BETBTN_TYPE eBetBtnType;		// 베팅버튼 타입(가로 세로)	
	BOOL bCutStyle;
	int  nAutoUpType;					// 바둑이 자동올림 체크 // 0 : 사용않함 1: 사용 2 : 선택 전
	int  nAutoSortType;					// 바둑이 자동정렬 체크 // 0 : 사용않함 1: 사용 2 : 선택 전
	BOOL bNoObserverChat;				// 관전 채팅  0 보기 1 안보기
	BOOL bNotificationConnectBuddy;		// 친구 접속 알림
	BOOL bCardArray;					// FALSE = 기본형, TRUE = 일자형
	int  nAutoBet;						// 0 = off 1 = 4  2 = 5  3 = 6
	BOOL bAvataAni;						// 0 = off 1 = on
	BOOL bShowTip;
	int  nSKinKind;						// 0 블루 스킨 , 1 그레이 스킨
	BOOL nbShowHidden;
	BOOL bAutoHidden;					// 섯다 패 자동 오픈 (사용 true, 사용안함 false)
	int  SndFxKind;						// 음성 종류 (0, 1)

}CONFIG;


enum CONFIG_GAME_TYPE
{
	CONFIG_GAME_TYPE_ALLGAME = 0,		// 기본 게임설정
	CONFIG_GAME_TYPE_SUTDA,				// 섯다 게임설정
	CONFIG_GAME_TYPE_HOOLA,				// 훌라 게임설정
};

class COptionFrameBase;

class UPG_COMMON_EXPORT COptionDlg : public LSY::CLSYLibDialogBase
{
private:
	shared_ptr<COptionFrameBase> m_OptionFrame;	// 게임 타입 별 옵션 프레임 객체

private:
	shared_ptr<COptionFrameBase> CreateOptionFrame(CONFIG_GAME_TYPE gameType);

public:
	CONFIG *m_pMyConfig;					// 옵션 데이터
	bool m_bGameOptionTab;					// 옵션 탭
	CONFIG_GAME_TYPE m_eConfigGameType;		// 게임설정창 타입
	bool m_bSex;							// false 여자, true 남자
	int m_nBGMVolumeBackup;					// 볼륨 백업
public:
	COptionDlg(CWnd* pParent = NULL);
	virtual ~COptionDlg();

public:
	virtual INT_PTR DoModal(IDX_GAME eGame, bool bMySex, bool bGameOptionTab = true);
	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnCurValueChange(LSY::CMessage *pMsg);
	LRESULT OnSelChangeOptionType(LSY::CMessage *msg);
	LRESULT OnSelChangeEffectSound(LSY::CMessage *msg);
	LRESULT OnSelChangeBetBtnType(LSY::CMessage *msg);
	LRESULT OnSelChangeHotKeyType(LSY::CMessage *msg);
	LRESULT OnSelChangeObserverChattingType(LSY::CMessage *msg);
	LRESULT OnSelChangeNotificationMemoType(LSY::CMessage *msg);
	LRESULT OnSelChangeNotificationConnectBuddyType(LSY::CMessage *msg);
	LRESULT OnSelChangeReceiveInvitationType(LSY::CMessage *msg);
	LRESULT OnSelChangeReceiveChatType(LSY::CMessage *msg);
	LRESULT OnSelChangeReceiveWhisperType(LSY::CMessage *msg);
	LRESULT OnSelChangeReceiveMemoType(LSY::CMessage *msg);
protected:
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
};