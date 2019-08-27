#pragma once

#define UM_GH_RULE_ON			(WM_USER+8050)
#define UM_GH_RULE_OFF			(WM_USER+8051)
#define UM_GH_USE				(WM_USER+8052)
#define UM_GH_USEDLG_USE		(WM_USER+8053)
#define UM_GH_USEDLG_CANCEL		(WM_USER+8054)
#define UM_GH_USEDLG_OK			(WM_USER+8055)

class CUIInfoData;
class CJokboProbability;
class COtherUserJokboCheckMan;

class UPG_GAMEBASE_EXPORT CGameHelperMan  
{
protected:
	enum ENUM_GAMEHELPER_STATUS
	{
	EGHS_NOITEM = -1,		//아이템이 없음
	EGHS_USEITEM_ON,		//아이템이 사용중이고 ON 상태임
	EGHS_USEITEM_OFF,		//아이템이 사용중이고 OFF 상태임
	EGHS_NOTUSEITEM,		//아이템을 가지고 있지만 사용중이지 않음
	EGHS_USEDLG,			//아이템을 사용하기 위한 창을 띄웠음
	};

	CUIInfoData		*m_pUIInfoData;
	CGameViewBase	*m_pGameView;
	CGameProcess	*m_pGameProcess;

	NMBASE::GRAPHICGDI::xSprite			m_sprBtn;
	NMBASE::GRAPHICGDI::xSprite			m_sprJBTable;
	NMBASE::GRAPHICGDI::xSprite			m_sprUseHelper;

	NMBASE::GRAPHICGDI::xSprite			m_sprArrow;
	NMBASE::GRAPHICGDI::xSprite			m_sprUseBtn;
	NMBASE::GRAPHICGDI::xSprite			m_sprUseDlg;
	NMBASE::GRAPHICGDI::xSprite			m_sprUseCancelBtn;
	NMBASE::GRAPHICGDI::xSprite			m_sprUseOkBtn;
	NMBASE::GRAPHICGDI::xSprite			m_sprOnOff;
	NMBASE::GRAPHICGDI::xSprite			m_sprInfo;
	CRect			m_rtUseDlg;

	ENUM_GAMEHELPER_STATUS	m_nStatus;

	NMBASE::SKINGDI::CGraphButton    m_btnOn;
	NMBASE::SKINGDI::CGraphButton    m_btnOff;
	NMBASE::SKINGDI::CGraphButton    m_btnUse;
	NMBASE::SKINGDI::CGraphButton    m_btnUseDlgUseBtn;
	NMBASE::SKINGDI::CGraphButton    m_btnUseDlgCancelBtn;
	NMBASE::SKINGDI::CGraphButton    m_btnUseDlgOKBtn;

	COtherUserJokboCheckMan		*m_pOtherUserJokboCheckMan;
	CJokboProbability			*m_pJokboProbability;

	int				m_nMyCurJokboIndex;
	CRect			*m_plistProbabilityRect;

	bool			m_bUseResultDlg;
	bool			m_bBuyInfoDlg;

	int				m_nMaxRule;
	int				m_nMaxPlayer;
	int				m_nInfo;

	virtual void CheckItemHaveAndUse(void);
public:
	CGameHelperMan();
	virtual ~CGameHelperMan();

	virtual void Init(CUIInfoData *pUIInfoData, CGame *pGame, int nMaxRule, COtherUserJokboCheckMan *pOtherUserJokboCheckMan, CJokboProbability *pJokboProbability);
	virtual void Clear();
	virtual void ClearProbability();
	virtual void Reset();
	virtual void CheckUseItemState();
	virtual void LoadImage(LPCTSTR lpJokboBtnPath, LPCTSTR lpDoumiPath, LPCTSTR lpTablePath, LPCTSTR lpJokboInfo);
	
	virtual BOOL IsValid() { return (m_nStatus == EGHS_USEITEM_ON)?TRUE:FALSE;};
	virtual void OnEnterRoom(void);

	virtual void Draw( CDC *pDC = NULL , NMBASE::GRAPHICGDI::CPage *pPage = NULL );

	virtual void ClearInfo(void){m_nInfo = -1;}
	virtual void DrawProbability( CDC *pDC = NULL , NMBASE::GRAPHICGDI::CPage *pPage = NULL );
	virtual void DrawOtherUserJokboArrow( CDC *pDC = NULL , NMBASE::GRAPHICGDI::CPage *pPage = NULL );
	virtual void DrawUseDlg( CDC *pDC = NULL , NMBASE::GRAPHICGDI::CPage *pPage = NULL );
	virtual void DrawUseResultDlg( CDC *pDC = NULL , NMBASE::GRAPHICGDI::CPage *pPage = NULL );
	virtual void DrawBuyInfoDlg( CDC *pDC = NULL , NMBASE::GRAPHICGDI::CPage *pPage = NULL );
	
	virtual void CalculationProbability();
	virtual void CalculationMyJokbo();
	virtual void CheckOtherJokboCheck(int pnum = -1 );

	virtual void SendUsePacket(void);
	virtual void OnUseGameHelper(bool bUse);

	virtual void GameHelperOnOff(void);
	virtual void SetAllUserCardInfo();

	LRESULT OnUseItem(WPARAM &wParam, LPARAM &lParam);

	virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );

	virtual BOOL OnLButtonDown( int x, int y);
	virtual BOOL OnLButtonUp( int x, int y );
	virtual BOOL OnMouseMove( int x, int y );

	virtual BOOL OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData);
};