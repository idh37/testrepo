// UIInfoData.h: interface for the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>

enum ENUM_POSTYPE
{
	EPT_UNKNOWN = -1,
	EPT_ME,
	EPT_LEFT,
	EPT_RIGHT,
};

enum ENUM_BUTTON_TYPE
{
	EBT_UNKNOWN = -1,
	EBT_PING_N,
	EBT_DADANG_N,
	EBT_CHECK_N,
	EBT_HALF_N,
	EBT_CALL_N,
	EBT_DIE_N,
	EBT_PING_H,
	EBT_DADANG_H,
	EBT_CHECK_H,
	EBT_HALF_H,
	EBT_CALL_H,
	EBT_DIE_H,
	EBT_PING_P,
	EBT_DADANG_P,
	EBT_CHECK_P,
	EBT_HALF_P,
	EBT_CALL_P,
	EBT_DIE_P,
	EBT_QUARTER_P,
	EBT_PING_V,
	EBT_DADANG_V,
	EBT_CHECK_V,
	EBT_HALF_V,
	EBT_CALL_V,
	EBT_DIE_V,
	EBT_GAME_TIP,
	EBT_CAPTURE,
	EBT_OPTION,
	EBT_MINIMIZE,
	EBT_CHANGEDISPLAY,
	EBT_TOP_EXIT,
	EBT_USER_GRADE,
	EBT_INVITE,
	EBT_OBSERVERVIEW,
	EBT_CARD_OPEN, // ���� ī�� ���� ��ư
	EBT_SINOUT,	// ���� ��ư
	EBT_BOTTOM_EXIT, // ������ ��ư
	EBT_JOKBOHELP_ON,
	EBT_JOKBOHELP_OFF,
	EBT_JOKBOHELP_USE,
	EBT_JOKBOHELP_INFO,
	EBT_CHAT,
	EBT_JOKBO,
	EBT_EVENT,
	EBT_BADUSER,
	EBT_WHISPERLIST,
	EBT_SIDEPOT,
	EBT_SEATKEEPER,
	EBT_ONESHOTCHARGE,
	EBT_POKERSHOP,
	EBT_LOSSMONEYWARNING,
	EBT_LOSSMONEYWARNINGCLOSE,
	EBT_WAITUSER, // ����ư
	EBT_COMPLAIN,
	EBT_GIVEUP, // ����ϱ� ��ư
	EBT_MAX, // ???
};

enum ENUM_CHAT_COLOR
{
	ECC2_UNKNOWN = -1,
	ECC2_NOTIFY,			//����,����,112 �Ű���� �˸� �޽���
	ECC2_USEITEM,			//�ƹ�Ÿ ����, ����, �����, ������ ���
	ECC2_CHAT,				//�Ϲ� ä�� ����
	ECC2_SERVMSG,			//�������� ���� �޽���
	ECC2_WHISPER_SEND,		//�Ӹ� ������
	ECC2_WHISPER_RECEIVE,	//�Ӹ� �ޱ�
	ECC2_RESULT,			//��� �Ϲ�
	ECC2_RESULT_WINNER,		//��� ���� ���̵�
	ECC2_RESULT_WINMONEY,	//��� ���� ȹ��ݾ�
	ECC2_RESULT_JOKBO,		//��� ���� ����
	ECC2_RESULT_LOSER,		//��� ���� ���̵�
	ECC2_RESULT_LOSEMONEY,	//��� ���� ���� �ݾ�
	ECC2_RESULT_JACKPOT,	//����
	ECC2_MAX,
};

typedef struct _tagQUICKJOINAPPLYBUT
{
	CPoint ptButton[2];	//���� / ���
}SQUICKJOINAPPLYBUT;

class UPG_GAMEBASE_EXPORT CUIInfoData
{
public:
	bool					m_bObserverChat;				//�����ڳ����� ä���ϵ��� �Ѵ�.

	CRect					m_rtJokoName;					//���� �̸��� ǥ���� ����
	COLORREF				m_clrJokboNameShadowColor;		//���� �̸��� ǥ���Ҷ��� �׸��ڻ�
	COLORREF				m_clrJokboNameColor;			//���� �̸��� ǥ���Ҷ��� ���ڻ�

	CPoint					m_ptBonusCardBack;				//���ʽ� ī�尡 ��ġ�� �ٴ��� ��� ��ġ
	CPoint					m_ptBonusCardGab;				//���ʽ� ī�尡 ��ġ�� ���ʽ�ī�� �ٴ� ���� ����
	CPoint					m_ptBonusOpen;					//���ʽ� ī�尡 ���µ� ��ġ
	CPoint					m_ptRuleKind;					//�� ���� ��������Ʈ ��ġ
	CPoint					m_ptDisCardGab;					//���� ī���� ��ġ, �ڽ��� ī�� �������� ������ ��� ��ġ

	COLORREF				m_clrResultJokboNameWinColor;	//��� ��½� �� �÷��̾ �¸� ������ ǥ���� ��
	COLORREF				m_clrResultJokboNameLoseColor;	//��� ��½� �� �÷��̾ �й� ������ ǥ���� ��
	COLORREF				m_clrResultMoneyWinColor;		//��� ��½� �� �÷��̾ �� �Ӵϸ� ǥ���� ��
	COLORREF				m_clrResultMoneyLoseColor;		//��� ��½� �� �÷��̾ ���� �Ӵϸ� ǥ���� ��

	CRect					m_rtResultJokboYouSize;			//��� ��½� ��� �÷��̾��� ����
	CRect					m_rtResultMoneyYouSize;			//��� ��½� ��� �÷��̾ �Ұų� �� �Ӵϸ� ǥ���� ������ ũ��
	CPoint					m_ptResultBackYouGab;			//��� ��½� ��� �÷��̾��� ��� �ٴ��� ��ġ, ī����������� ������ ��밪

	CRect					m_rtResultJokboMySize;			//��� ��½� �� ����
	CRect					m_rtResultMoneyMySize;			//��� ��½� ���� �Ұų� �� �Ӵϸ� ǥ���� ������ ũ��
	CPoint					m_ptResultBackMyGab;			//��� ��½� �� ��� �ٴ��� ��ġ, ī����������� ������ ��밪

	CPoint					m_ptResultJokboNameGab;			//��� ��½� �� �÷��̾��� ���� ǥ�� ��ġ, ����ٴ����� ������ ��밪
	CPoint					m_ptResultMoneyGab;				//��� ��½� �� �÷��̾��� �Ӵ� ǥ�� ��ġ, ����ٴ����� ������ ��밪

	CPoint					m_ptTimer;						//�ٸ� ������ Ÿ�̸��� ��ġ
	
	std::vector<CPoint>		m_ptlistBossMarkGab;			//���� ��ũǥ�� - (�ڽ�,����,������) (ī���� �������� ���������� ��� ��ġ)
	std::vector<CPoint>		m_ptlistDealerMarkGab;			//���� ��ũǥ�� - (�ڽ�,����,������) (ī���� �������� ���������� ��� ��ġ)
	std::vector<CPoint>		m_ptlistHostMarkGab;			//���� ��ũǥ�� - (�ڽ�,����,������) (�ƹ�Ÿ ���� ��� ��ġ)

	std::vector<CPoint>		m_listCardStartPos;				//�� �÷��̾��� ī���� ���� ��ġ�� ������ �ִ� ����Ʈ
	std::vector<CPoint>		m_listAvatarPos;				//�� �÷��̾��� �ƹ�Ÿ�� ���� ��ġ�� ������ �ִ� ����Ʈ
	std::vector<CRect>		m_listUserMoneyRect;			//�� �÷��̾��� �����Ӵ� ǥ�� ��ġ
	int						m_nCardGab;						//�� ī���� ������ ��Ÿ��
	int						m_nMyCardGab;					//���� ���� �� ī���� ������ ��Ÿ��
	CPoint					m_ptOverHaveMoneyGab;			//�÷��̾ ������ ������ �ִ� �Ӵϸ� ǥ���ϴ� ���� ��ġ, �����Ӵ� ǥ�� ��� �߾��� ���������� ��� ��ġ

	CPoint					m_ptUserNameGab;				//�� �÷��̾��� �̸��� ǥ���� �ƹ�Ÿ ���������� ������ �����ǥ
	CRect					m_rtUserName;					//�� �÷��̾��� �̸��� ǥ���� ������ ũ��
	COLORREF				m_clrUserNameShadowColor;		//�� �÷��̾��� �̸��� ǥ�ö� ����� �׸��� ��
	COLORREF				m_clrUserNameColor;				//�� �÷��̾��� �̸��� ǥ�ö� ����� ���ڻ�

	std::vector<CPoint>		m_listLevelGab;					//�� �÷��̾��� ����� ǥ���� ���� �ƹ�Ÿ ��ġ�� ������ �����ǥ
	CSize					m_szAvatar;						//�ƹ�Ÿ�� ũ��
	CPoint					m_ptReservation;				//������/�������� ǥ�� ��ġ, �ƹ�Ÿ ������ġ�� ������ �����ǥ

	CRect					m_rtMyTotalBettingMoney;		//�ڽ��� �� ���� �ݾ� ǥ�� ���� ũ��
	CPoint					m_ptMyTotalBettingMoney;		//�ڽ��� �� ���� �ݾ� ǥ�� ��ġ
	COLORREF				m_clrMyTotalBettingMoneyText;	//�ڽ��� �� ���� �ݾ� ǥ�� ��
	COLORREF				m_clrMyTotalBettingMoneyOutline;//�ڽ��� �� ���� �ݾ� ǥ�� �ܰ��� ��

	CRect					m_rtBettingMoney;				//���� ���� �ݾ� ǥ�� ���� ũ��
	COLORREF				m_clrBettingMoneyText;			//���� ���� �ݾ� ǥ�� ��
	COLORREF				m_clrBettingMoneyOutline;		//���� ���� �ݾ� ǥ�� �ܰ��� ��
	std::vector<CPoint>		m_listBettingMoneyPos;			//���� ���� �ݾ� ǥ�� ��ġ, �� ����ں� ��ġ

	CRect					m_rtBackTotalMoney;				//������ ������ �� ���� �ݾ��� ǥ���� ����
	CRect					m_rtBackCallMoney;				//������ ������ �� �Ҷ� �ʿ��� �ݾ��� ǥ���� ����

	CPoint					m_ptJacpotBack;					//���� �ٴ��� �׷��� ��ġ
	CPoint					m_ptJacpotNumberStart;			//���� ���� ǥ�� ���� ��ġ
	int						m_nJacpotNumberWidth;			//���� ������ ��

	std::vector<CPoint>		m_listButtonPos;				//��ư�� ��ġ�� ������ �ִ� ����Ʈ
	CRect					m_rtBetBtnCombo;				//���� ��ư Ÿ�� ���� �޺��ڽ��� ��Ʈ

	CRect					m_ptBtnMoneyDrawRect;			//��ư�� Ŭ�������� ���ҵǴ� �Ӵϸ� ǥ���ϴ� ��Ʈ
	std::vector<CPoint>		m_ptBtnMonyDrawGab;				//��ư�� Ŭ�������� ���ҵǴ� �Ӵϸ� ǥ���ϴ� ��ġ, ��ư �������� ���������� �����ǥ(N, H, P��)
	
	COLORREF				m_clrBtnBetText;				//��ư�� ���� �ݾ� ǥ�� ���ڻ�
	COLORREF				m_clrBtnBetTextShadow;			//��ư�� ���� �ݾ� ǥ�� ������ �׸��ڻ�
	CRect					m_rtBtnBetText;					//��ư�� ���� �ݾ� ǥ�� ���� ũ��
	CRect					m_rtObserverCount;				//�����ڼ� ǥ�� ������ ũ��
	COLORREF				m_clrObserverCount;				//�����ڼ� ǥ�� ���ڻ�
	CPoint					m_ptObserverCountGab;			//�����ڼ� ǥ�� ���� ��ġ, �����ں��� ��ư������ �����ġ

	int						m_nMaxJokbo;					//��������� �ִ� ǥ�� ������
	std::vector<std::string> m_listJokboName;				//���� �̸�
	CRect					m_rtJokboHelpPercent;			//��������� ������ ���� Ȯ��ǥ�� ũ��
	CPoint					m_ptJokboHelpPercent;			//��������� ���� Ȯ��ǥ�� ������ġ
	int						m_nJokboHelpHightGab;			//������ ���� Ȯ��ǥ���� ���� ����
	CPoint					m_ptJokboHelpCompleteGab;		//�ϼ��� ������ �н��ϱ� ���� ������ ���� Ȯ��ǥ�� ������ġ������ ����
	COLORREF				m_clrJokboHelpNoPossibilityName;//������ �̹� �ϼ��Ȱ� ���� ���� �����̰ų� ���ɼ��� ���� �����̸� ǥ�� ��
	COLORREF				m_clrJokboHelpPossibilityName;	//������ ������ �����̸� ǥ�� ��
	COLORREF				m_clrJokboHelpMyJokboName;		//���� ����̿��� �ڽ��� ������ ��
	COLORREF				m_clrJokboHelpPercent;			//���� Ȯ�� ǥ�� ��
	CPoint					m_ptJokboHelpPercentGab;		//������ ������ Ȯ��ǥ�� ��ġ, m_ptJokboHelpPercent������ ��� ��ǥ �̴�
	CPoint					m_ptJokboHelpBack;				//����� ��� ��ġ
	CPoint					m_ptJokboHelpUseDlg;			//����� ��� ��ư�� Ŭ�������� ��Ÿ�� ���̾˷α� ��ġ
	CPoint					m_ptJokboHelpUseDlgUseBtnGab;	//����� ���â���� ����ϱ� ��ư�� ��ġ, â�� �����ǥ
	CPoint					m_ptJokboHelpUseDlgCancelBtnGab;//����� ���â���� ��� ��ư�� ��ġ, â�� �����ǥ
	CPoint					m_ptJokboHelpUseDlgOK;			//����� ��� ���â���� Ȯ�� ��ư�� ��ġ, â�� �����ǥ
	CPoint					m_ptJokboArrowGab;				//�� ������� ���� ǥ�ÿ� ��Ÿ���� ȭ��ǥ�� ��ǥ, ī��� ������ ��� ��ǥ
	CPoint					m_ptJokboInfo;					//���� ������ ǥ�õ� ��ġ

	std::vector<CPoint>		m_listUserChangeGameInfoBackPos;//ENUM_POSTYPE�� ������ �����ݾ�/�����Ǽ� ǥ�� ��� �̹��� ��ġ
	CRect					m_rtUserChangeGameInfoMoney;	//����Ӵ� ǥ�� ����, �� �÷��̾��� �����ݾ�/�����Ǽ� ǥ�� ��� �̹��������� ��� ��ǥ
	CRect					m_rtUserChangeGameInfoInfo;		//�����Ǽ� ǥ�� ����, �� �÷��̾��� �����ݾ�/�����Ǽ� ǥ�� ��� �̹��������� ��� ��ǥ
	CRect					m_rtUserChangeGameInfoLimit;	//�ִ뺣�� ǥ�� ����, �� �÷��̾��� �����ݾ�/�����Ǽ� ǥ�� ��� �̹��������� ��� ��ǥ
	COLORREF				m_clrUserChangeGameInfoMoneyWin;//���ݾ��� ǥ���� ��
	COLORREF				m_clrUserChangeGameInfoMoneyLose;//�����Ӵ� ǥ���� ��
	COLORREF				m_clrUserChangeGameInfoInfo;	//�����Ǽ� ǥ���� ��
	COLORREF				m_clrUserChangeGameInfoLimitMoney;//���ݾ��� ǥ���� ��

	CPoint					m_ptCardDeck;					//ī�嵦�� ��ġ
	CRect					m_rtRoomTitle;					//���� ��� ũ��
	CPoint					m_ptRoomTitle;					//���� ��� ��ġ
	COLORREF				m_clrRoomTitle;					//���� ��� ��
	COLORREF				m_clrRoomTitleOutLine;			//���� ��� �ƿ����λ�

	std::vector<COLORREF>	m_listChatColor;				//ä��â�� ��µ� ENUM_CHAT_COLOR ���º� ��
	CPoint					m_ptChat;						//ä��â�� ���� ��ġ
	CSize					m_szChat;						//ä��â�� ũ��
	CPoint					m_ptInfo;						//����â�� ���� ��ġ
	CSize					m_szInfo;						//����â�� ũ��
	CPoint					m_ptEvent;						//�̺�Ʈâ�� ���� ��ġ
	CPoint					m_ptEdit;						//����Ʈâ�� ���� ��ġ
	CSize					m_szEdit;						//����Ʈâ�� ũ��
	CPoint					m_ptInfoScroll;					//��ũ�ѹ��� ���� ��ġ
	int						m_nInfoScrollHeight;			//��ũ�ѹ��� ����
	CPoint					m_ptChatScroll;					//��ũ�ѹ��� ���� ��ġ
	int						m_nChatScrollHeight;			//��ũ�ѹ��� ����
	CPoint					m_ptEventPage;					//�̺�Ʈ ������ ���� ǥ�� ������ ������ ������ġ, �̺�Ʈâ ���� ��ġ�κ����� ��밪
	int						m_nEventPageGab;				//�̺�Ʈ ������ ������ ���� ǥ�� X�� ����

	std::vector<CPoint>		m_listPlayerBackPos;			//�� �÷��̾��� ���� ��ġ

	CPoint					m_ptBackChipCenter;				//�ٴ� Ĩ�� ���� ��ġ
	std::vector<std::vector<CPoint> > m_listBackChipGabPos;	//�ٴ� Ĩ�� ��ġ ����
	CPoint					m_ptChipEndGab;					//���ڿ��� Ĩ�� ���� �� ������ ������ ����� ���� ��ġ ����, �� ������ ī���� �������� �������� �Ѵ�. Ĩ�� �ټ����� �⺻���� �ؼ� ��ġ ���̸� ����
	DWORD					m_dwChipMoveDelayTime;			//��� ��½� Ĩ�� �����̱��� ��� �ð�

	CRect					m_rtDisplayBoard;				//�������� ũ��
	CPoint					m_ptDisplayBoard;				//�������� ��ġ
	COLORREF				m_clrDisplayNormal;				//�������� �Ϲ� �ؽ�Ʈ ��
	COLORREF				m_clrDisplayJokbo;				//�������� ���� ��
	COLORREF				m_clrDisplayMoney;				//�������� �Ӵ� ��

	CPoint					m_ptBettingCmd;					//������ ������ ������ ǥ�����ִ� ��������Ʈ�� ��ġ

	CRect					m_rtSidePotMoneyName;			//���̵��� �̸��� ǥ�� ���� ũ�⸦ ����
	CPoint					m_ptSidePotMoneyNameGab;		//���̵��� �̸��� ǥ�� ��ġ, �� ����Ʈ���� ����� �̹����� ���������� ������ ��밪
	COLORREF				m_clrSidePotMoneyName;			//���̵��� �̸��� ��
	CRect					m_rtSidePotMoney;				//�� ���̵����� �Ӵ��� ǥ�� ������ ũ�⸦ ����
	CPoint					m_ptSidePotMoneyGab;			//�� ���̵����� �Ӵ��� ǥ�� ��ġ, �� ����Ʈ���� ����� �̹����� ���������� ������ ��밪
	COLORREF				m_clrSidePotMoney;				//���̵��� �Ӵ��� ��
	CPoint					m_ptSidePotResult;				//���̵��� ���ǥ�� ��ġ
	CRect					m_rtSidePotResultVS;			//���̵��� ����� �� ���� ��/�� ���� ǥ�� ������ ũ��
	CPoint					m_ptSidePotResultVSGab;			//���̵��� ����� �� ���� ��/�� ���� ǥ�� ������ ��ġ
	int						m_nSidePotResultGab;			//���̵��� ����� �� ���� ��/�� ���� ǥ�� ������ ������
	CRect					m_rtSidePotResultMoney;			//���̵��� ����� �� ���� �Ӵ� ǥ�� ������ ũ��
	CPoint					m_ptSidePotResultMoneyGab;		//���̵��� ����� �� ���� �Ӵ� ǥ�� ������ ��ġ
	CRect					m_rtSidePotResultTotalMoney;	//���̵��� ����� ��ü ȹ�� �Ӵ� ǥ�� ������ ũ��
	CPoint					m_ptSidePotResultTotalMoneyGab;	//���̵��� ����� ��ü ȹ�� �Ӵ� ǥ�� ������ ��ġ

	CRect					m_rtHighJokboUserName;			//4ī�� �̻� ������ �޼������� ��Ÿ���� ����Ʈ�� ǥ���� ���� �̸� ���� ũ��
	CPoint					m_ptHighJokboUserName;			//4ī�� �̻� ������ �޼������� ��Ÿ���� ����Ʈ�� ǥ���� ���� �̸� ��ġ
	COLORREF				m_clrHighJokboUserName;			//4ī�� �̻� ������ �޼������� ��Ÿ���� ����Ʈ�� ǥ���� ���� �̸� ��
	CRect					m_rtHighJokboName;				//4ī�� �̻� ������ �޼������� ��Ÿ���� ����Ʈ�� ǥ���� ���� �̸� ���� ũ��
	CPoint					m_ptHighJokboName;				//4ī�� �̻� ������ �޼������� ��Ÿ���� ����Ʈ�� ǥ���� ���� �̸� ��ġ
	COLORREF				m_clrHighJokboName;				//4ī�� �̻� ������ �޼������� ��Ÿ���� ����Ʈ�� ǥ���� ���� �̸� ��
	CRect					m_rtHighJokboWinMoney;			//4ī�� �̻� ������ �޼������� ��Ÿ���� ����Ʈ�� ǥ���� ȹ�� �Ӵ� ���� ũ��
	CPoint					m_ptHighJokboWinMoney;			//4ī�� �̻� ������ �޼������� ��Ÿ���� ����Ʈ�� ǥ���� ȹ�� �Ӵ� ��ġ
	COLORREF				m_clrHighJokboWinMoney;			//4ī�� �̻� ������ �޼������� ��Ÿ���� ����Ʈ�� ǥ���� ȹ�� �Ӵ� ��
	CRect					m_rtHighJokboJacpotMoney;		//���̽� ȹ�� �Ӵ� ���� ũ��
	CPoint					m_ptHighJokboJacpotMoney;		//���̽� ȹ�� �Ӵ� ��ġ
	COLORREF				m_clrHighJokboJacpotMoney;		//���̽� ȹ�� �Ӵ� ��

	CPoint					m_ptObserver;					//������ �ǳ� ǥ�� ��ġ
	CPoint					m_ptObserverBtnGab;				//������ �����ϱ�/������� ��ư�� ������ �ȳڷ� ������ ��� ��ġ
	CPoint					m_ptObserverReservePriorityGab;	//������ �ǳڿ��� ��������� ������� ǥ�� ��ġ

	std::vector<CPoint>		m_listSeatKeeperWinPos;			//�ڸ���Ŵ�� ������ ��ġ��(0, 1, 2, 3, 4) ->�ƹ�Ÿ�� ���������� ��� ��ǥ
	std::vector<CPoint>		m_listTurnDisplayPos;			//�ڸ���Ŵ�� �����츦 ���������� �����ǥ(�����츦 ���������� �����ǥ)
	CPoint					m_TurnTextPos;					//�ڸ���Ŵ�� �ϼ��� ��� ��ġ(�����츦 ���������� �����ǥ)
	CPoint					m_SeatKeeperJoinButPos;			//�ڸ���Ŵ�� �������� ��ư(�����츦 ���������� �����ǥ)
	CRect					m_rtSeatKeeperWeb;				//�ڸ���Ŵ�� ��(�����츦 ���������� �����ǥ)
	COLORREF				m_clrSeatKeeperTurn;			//�ڸ���Ŵ�� �� ��

	DWORD					m_dwBombCardOpenMoveTime;		//��źī�带 ���� or ������ ���ڸ��� ������ ���ؼ� �Ͽ� �̵��ϴ� �ð� 
	CPoint					m_ptBombEffectGab;				//��ź����Ʈ ��� ��ġ, ��ź ī��� ������ ��� ��ǥ
	CPoint					m_ptJokerEffectGab;				//��Ŀ����Ʈ ��� ��ġ, ��ź ī��� ������ ��� ��ǥ
	DWORD					m_dwBombEffectGab;				//��ź����Ʈ ����� �� �������� ����ð�
	DWORD					m_dwStartGameDealingTimeGab;	//���� ī��(3,4��) ������ ���� �ð���
	float					m_fStartGameDealingCardSpeed;	//���� ī��(3,4��) ������ ���� ī���� ��� ���
	DWORD					m_dwDealingTimeGab;				//ī�� ������ �� ī�庰 ���� �ð���
	float					m_fDealingCardSpeed;			//ī�� ������ �� ī�庰 ������ ���� ī���� ��� ���

	std::vector<CPoint>		m_listSameIPGab;				//ENUM_POSTYPE�� ���� ������ ǥ�� ��ġ, �ƹ�Ÿ�� ���������� ��� ��ǥ
	std::vector<bool>		m_listSameIPMarkRight;			//ENUM_POSTYPE�� ���� ������ ǥ�� ��ġ, �ƹ�Ÿ�� �����ʿ� ǥ�õǴ����� ����

	CPoint					m_ptAutoBettingPos;				//�ڵ����� ��ġ

	CPoint					m_ptOneShotDlg;						//�������� ���̾˷α� ��� ��ġ
	CPoint					m_ptOneShotDlg_btnOnceGab;			//�������� ���̾˷α��� �ѹ��� �ޱ� ��ư ��ġ(��� ��ġ)
	CPoint					m_ptOneShotDlg_btnTwiceGab;			//�������� ���̾˷α��� ������ �ޱ� ��ư ��ġ(��� ��ġ)
	CPoint					m_ptOneShotDlg_btnCloseGab;			//�������� ���̾˷α��� �ݱ� ��ư ��ġ(��� ��ġ)
	CRect					m_rtOneShotPossibilityMoneySize;	//�������� ���ް��� �ݾ� ǥ�� ũ��
	CPoint					m_ptOneShotPossibilityMoneyGab;		//���� ���� �ݾ� ǥ�� ��ġ, ���̾˷α� ���� ��ġ�κ����� ��� ��ǥ
	COLORREF				m_clrPossibilityMoneyOutline;		//���� ���� �ݾ� ǥ�� �ܰ�����
	COLORREF				m_clrPossibilityMoney;				//���� ���� �ݾ� ǥ�� ��
	CRect					m_rtOneShotOnceTextSize;			//�ѹ��� ���� �ݾ� ǥ�� ũ��
	CPoint					m_ptOneShotOnceTextGab;				//�ѹ��� ���� �ݾ� ǥ�� ��ġ, ���̾˷α� ���� ��ġ�κ����� ��� ��ǥ
	CRect					m_rtOneShotTwiceTextSize;			//������ ���� �ݾ� ǥ�� ũ��
	CPoint					m_ptOneShotTwiceTextGab;			//������ ���� �ݾ� ǥ�� ��ġ, ���̾˷α� ���� ��ġ�κ����� ��� ��ǥ
	COLORREF				m_clrTextOutline;					//�ѹ���/������ ���� �׾� ǥ�� �ܰ�����
	COLORREF				m_clrText;							//�ѹ���/������ ���� �׾� ǥ��	��
	CRect					m_rtOneShotError;					//���� ���� ���н� �������� ���� ���� ũ��
	CPoint					m_ptOneShotErrorGab;				//���� ���� ���н� �������� ���� ���� ��ġ
	COLORREF				m_clrResultOutline;					//��� ��½� ���� �ܰ��� ��
	COLORREF				m_clrResultEmphasis;				//��� ��½� ���� ���� �ؽ�Ʈ ��
	COLORREF				m_clrResultNormal;					//��� ��½� �Ϲ� ���� �ؽ�Ʈ ��
	CRect					m_rtMsgSize;						//����/����� �޽��� ǥ�� ������ ũ��
	CPoint					m_ptSuccessMoneyGab;				//���޽� ������������ ���޵Ǵ� �Ӵ�ǥ�� ǥ�� ������ ��ġ
	int						m_nLineGab;							//����/���� �޽��� ǥ�ý� ������ ���� ��ġ ����
	CPoint					m_ptReservationGab;					//����� �޽��� ��� ��ġ, �����ǥ

	CPoint					m_ptOwnLimitMarkGab;				//�����ѵ� �ʰ� ��ũ ǥ�� ��ġ, �ƹ�Ÿ�� ������ ��� ��ġ
	CPoint					m_ptOwnLimitTipGab;					//�����ѵ� �ʰ� ���� ��ġ, ��ũ�κ����� �����ǥ
	CRect					m_rtOwnLimitMoneySize;				//�����ѵ� �ʰ� �ݾ� ǥ�ÿ��� ũ��
	CPoint					m_ptOwnLimitMoneyGab;				//�����ѵ� �ʰ� �ݾ� ǥ����ġ, �����κ����� �����ǥ
	CPoint					m_ptOwnLimitGoldGab;				//�����ѵ� �ʰ� ��� ǥ����ġ, �ʰ� �ӴϷκ����� �����ǥ	
	COLORREF				m_clrOwnLimitMoney;					//�����ѵ� �Ұ� �ݾ� ǥ�û�

	CPoint					m_ptChipStartGab;					//�ڽ��� ������ Ĩ�� ��ġ, ī���� ���� ��ġ�� ������ ��� ��ǥ

	CPoint					m_ptSelectCardFirstCard_Choice;		//���̽� ī�� ���� â�� ��ġ
	CPoint					m_ptSelectCardFirstCard_Normal;		//�Ϲ� ī�� ���� â�� ��ġ
	CRect					m_rtSelectCardFirstCard_Choice;		//���̽� ī�� ���� â���� ù��° ī���� ����, â�� ������ġ������ ��� ��ǥ
	CRect					m_rtSelectCardFirstCard_Normal;		//�Ϲ� ī�� ���� â���� ù��° ī���� ����, â�� ������ġ������ ��� ��ǥ
	int						m_nSelectCardCardGab;				//ī�� ���� â���� �� ī���� ��ġ ��
	CPoint					m_ptSlectCardSelectMarkGab;			//����ī��/����ī�� ���� ��ũ ������ ��Ÿ����. ��ī�忡���� ��� ��ǥ

	CPoint					m_ptQuickJoinDlgSearchON;			// ������� �ٷ����� ���/ ������
	CPoint					m_ptQuickJoinDlgSearchOFF;			
	std::vector<SQUICKJOINAPPLYBUT>	m_listQuickJoinDlgApplayBut;// ������ư
																
	std::vector<CPoint>		m_listPokerTimeEffectPos;			//�� ������ ��ĿŸ�� �̺�Ʈ ��û ����Ʈ ��ġ, ������ ������ ī�� ���� ��ġ���� ����
	CRect					m_rtPokerTimeEffect;				//����Ʈ ǥ�� ������ ũ��

	std::map<IDX_GAME, CRect>	m_listRoomInfoMinimumMoney;		//������, �������� �Ӵ� ǥ��
	std::map<IDX_GAME, CRect>	m_listRoomInfoRule;				//���� ��Ŀ�� ǥ�� ����
	std::map<IDX_GAME, COLORREF>	m_listRoomInfoRuleColor;	//���� ��Ŀ�� ǥ�� �ؽ�Ʈ ��

	COLORREF				m_clrRoomInfoMinimumMoney;			//���� ���� �Ӵ� �ؽ�Ʈ ��
	COLORREF				m_clrRoomInfoOutline;				//�ܰ��� ��
	CRect					m_rtRoomInfoPokerKind;				//����Ŀ���� �������� ǥ�� ����

	CPoint					m_ptWaitAlertPos;					//������ : ����ϱ� ���� �̹��� ��ġ

	CRect					m_ptGold;							//�Ϲ� ����� ��� ��ǥ
	COLORREF				m_clrGold;							//�Ϲ� ����� ��� �ؽ�Ʈ ����
	COLORREF				m_clrGoldDiff;						//�Ϲ� ����� ��� ���� ����	
	COLORREF				m_clrGoldReward;					//��� ������ ����	
	//yoo 2015.01 ������
	std::vector<CPoint>		m_listLimitIcon[2];					//�սǸӴ� ��� ǥ�� ������
public:
	CUIInfoData();
	virtual ~CUIInfoData();

	virtual void Init(void);

	const char *GetJokboName(int nKind);
};