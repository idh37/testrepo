#pragma once

class UPG_LOBBY_EXPORT CJackpotCurrentInfoDlg : public LSY::CWindows
{
protected:
	LSY::CCombo				*m_pSelGame;	
	LSY::CButton			*m_pBtnClose;
	LSY::CButton			*m_pBtnDetail;
	jackpot::JP_MONEYINFO	m_stJackpotInfo;
	
public:
	CJackpotCurrentInfoDlg();
	virtual ~CJackpotCurrentInfoDlg();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnSelChangeGame(LSY::CMessage *msg);

	void SetShowDlg(void);
	void SetJackpotDisplyInfo( jackpot::JP_MONEYINFO *pJackpotMoneyInfo);
protected:	
	void SetShowDlg(const IDX_GAME &nGame);

	int GetArrayGameIndex(const IDX_GAME &nGame);
};