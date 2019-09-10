#pragma once
#include "UserStruct.h"
#include <memory>

#define MODE_OPTION		0
#define MODE_MACRO		1
#define MODE_DSEVENT	2

#define IDC_EDIT_MACRO					1511

//���ù�ư Ÿ��
enum CONFIG_BETBTN_TYPE
{
	CONFIG_BETBTN_TYPE_N,
	CONFIG_BETBTN_TYPE_H,
	CONFIG_BETBTN_TYPE_P
};

// �ɼ� ������
typedef struct
{
	char BGMVolume;						// ����� ����
	BOOL bMusicON;						// ����� On/Off
	char FxVolume;						// ȿ���� ����
	BOOL bSndFX;						// ȿ���� On/Off
	CONFIG_BETBTN_TYPE eBetBtnType;		// ���ù�ư Ÿ��(���� ����)	
	BOOL bCutStyle;
	int  nAutoUpType;					// �ٵ��� �ڵ��ø� üũ // 0 : ������ 1: ��� 2 : ���� ��
	int  nAutoSortType;					// �ٵ��� �ڵ����� üũ // 0 : ������ 1: ��� 2 : ���� ��
	BOOL bNoObserverChat;				// ���� ä��  0 ���� 1 �Ⱥ���
	BOOL bNotificationConnectBuddy;		// ģ�� ���� �˸�
	BOOL bCardArray;					// FALSE = �⺻��, TRUE = ������
	int  nAutoBet;						// 0 = off 1 = 4  2 = 5  3 = 6
	BOOL bAvataAni;						// 0 = off 1 = on
	BOOL bShowTip;
	int  nSKinKind;						// 0 ��� ��Ų , 1 �׷��� ��Ų
	BOOL nbShowHidden;
	BOOL bAutoHidden;					// ���� �� �ڵ� ���� (��� true, ������ false)
	int  SndFxKind;						// ���� ���� (0, 1)

}CONFIG;


enum CONFIG_GAME_TYPE
{
	CONFIG_GAME_TYPE_ALLGAME = 0,		// �⺻ ���Ӽ���
	CONFIG_GAME_TYPE_SUTDA,				// ���� ���Ӽ���
	CONFIG_GAME_TYPE_HOOLA,				// �Ƕ� ���Ӽ���
};

class COptionFrameBase;

class UPG_COMMON_EXPORT COptionDlg : public LSY::CLSYLibDialogBase
{
private:
	shared_ptr<COptionFrameBase> m_OptionFrame;	// ���� Ÿ�� �� �ɼ� ������ ��ü

private:
	shared_ptr<COptionFrameBase> CreateOptionFrame(CONFIG_GAME_TYPE gameType);

public:
	CONFIG *m_pMyConfig;					// �ɼ� ������
	bool m_bGameOptionTab;					// �ɼ� ��
	CONFIG_GAME_TYPE m_eConfigGameType;		// ���Ӽ���â Ÿ��
	bool m_bSex;							// false ����, true ����
	int m_nBGMVolumeBackup;					// ���� ���
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