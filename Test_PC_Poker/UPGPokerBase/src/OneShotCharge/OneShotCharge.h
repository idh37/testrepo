#pragma once

#define IDM_ONESHOT_CHARGE	(WM_USER+6400)
#define IDM_ONESHOT_ONCE	(WM_USER+6401)
#define IDM_ONESHOT_TWICE	(WM_USER+6402)
#define IDM_ONESHOT_CLOSE	(WM_USER+6403)

class CUIInfoData;
class CSeatKeeper;

class UPG_GAMEBASE_EXPORT COneShotCharge
{
protected:
	NMBASE::GRAPHICGDI::xSprite		m_sprOneShotCharge;
	NMBASE::GRAPHICGDI::xSprite		m_sprOneShotChargeBtn;
	NMBASE::GRAPHICGDI::xSprite		*m_pCloseBtnSpr;

	NMBASE::SKINGDI::CGraphButton	m_btnOneShotCharge;
	NMBASE::SKINGDI::CGraphButton	m_btnOnceCharge;		//한번에 충전
	NMBASE::SKINGDI::CGraphButton	m_btnTwiceCharge;		//두번에 나누워서 충전
	NMBASE::SKINGDI::CGraphButton	m_btnClose;			//두번에 나누워서 충전

	CUIInfoData		*m_pUIInfoData;
	CGameProcess	*m_pGameProcess;
	CGameViewBase	*m_pGameView;
	NMBASE::GRAPHICGDI::CPage			*m_pPage;
	CSeatKeeper		*m_pSeatKeeper;

	bool			m_bShowAskDlg;
	bool			m_bWaitResult;
	bool			m_bShowResultDlg;
	bool			m_bRevervation;
	bool			m_bFail;

	int				m_nRemainCnt;
	int				m_nItemKind;
	INT64			m_i64RecvMoney;
	CString			m_strItemName;

	CRect			m_rtRect;
	NMBASE::SKINGDI::CNText			*m_pPossibilityMoney;
	NMBASE::SKINGDI::CNText			*m_pOnceMoney;
	NMBASE::SKINGDI::CNText			*m_pTwiceMoney;
	NMBASE::SKINGDI::CNText			*m_pErrorMsg;
	bool			m_bShowBtn;

	NMBASE::SKINGDI::CNText			*m_pSuccessMsg1;
	NMBASE::SKINGDI::CNText			*m_pSuccessMsg2;

	NMBASE::SKINGDI::CNText			*m_pReservationMsg1;
	NMBASE::SKINGDI::CNText			*m_pReservationMsg2;
	NMBASE::SKINGDI::CNText			*m_pReservationMsg3;
public:
	COneShotCharge();
	virtual ~COneShotCharge();

	virtual void LoadImage(void);

	virtual void Init(CUIInfoData *pUIInfoData, CGameProcess *pProcess, CSeatKeeper *pSeatKeeper, NMBASE::GRAPHICGDI::xSprite *pCloseBtnSpr);

	virtual BOOL OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData);
	virtual void OnClickOneShotCharge();
	virtual void OnClickOneShotOnceCharge();
	virtual void OnClickOneShotTwiceCharge();
	virtual void OnClickOneShotClose();
	virtual void CheckStatus(void);
	virtual void OnEnterRoom(void);

	virtual INT64 GetCompareMoney(void);
	virtual void GetJekiOptionName( const char* OptionGoodsCode );
	virtual void OnChargeMoney(CSV_CHANGE_MONEY& msg);

	virtual void SetError(LPCTSTR strErrMsg);
	virtual void SetReservation(void);
	virtual void SetUseItem(INT64 i64Money);

	virtual BOOL OnLButtonDown(int x , int y);
	virtual BOOL OnLButtonUp(int x , int y);
	virtual BOOL OnMouseMove(int x , int y);

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void Draw(CDC *pDC);
};
