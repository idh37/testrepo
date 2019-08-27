#pragma once

class UPG_LOBBY_EXPORT CVipJackpotListItem : public LSY::CListItem
{
protected:
	LSY::CText *m_pTxtNum;
	LSY::CText *m_pTxtMoney;
	LSY::CText *m_pTxtId;
	LSY::CText *m_pTxtGameName;
	LSY::CText *m_pTxtTime;
public:
	CVipJackpotListItem();
	virtual ~CVipJackpotListItem();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);
	string ChangeID(char* pID);

	virtual void OnMouseOver(void);
	virtual void OnMouseLeave(void);


};

typedef struct tagVipJackpotUser{
	long long llMoney;
	std::string sId;
	int nGameType;
	int nTime;
	SYSTEMTIME sTime;
public:
	tagVipJackpotUser(void){ Clear(); }
	void Clear(void){ llMoney=0ll; sId.clear(); nGameType=0; nTime=0; memset(&sTime, 0, sizeof(SYSTEMTIME)); }
}tVipJackpotUser;

class CVipJackpotDlg : public LSY::CWindows
{
protected:
	LSY::CButton		*m_pBtnClose;
	LSY::CTabControl    *m_pTabControl;
	LSY::CGroup			*m_pGrpInfo;
	LSY::CGroup			*m_pGrpRanking;
	std::vector<tVipJackpotUser*> m_cvtTodayUser;
	std::vector<tVipJackpotUser*> m_cvtMonthUser;
public:
	CVipJackpotDlg(void);
	virtual ~CVipJackpotDlg(void);
	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);
	virtual bool OnClickOutside(CObjectBase *pFocus);
	virtual void SetShow(const bool &bShow);
public:
	void SetInfoState(void);
	void SetInfoState(int nStage, int nState);
	void AddTodayItem(tVipJackpotUser* ptVipJackpotUser);
	void DelTodayItem(void);
	void AddMonthItem(tVipJackpotUser* ptVipJackpotUser);
	void DelMonthItem(void);
public:	
	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnTabSelChange(LSY::CMessage *msg);
	void SetShowDlg();
};


