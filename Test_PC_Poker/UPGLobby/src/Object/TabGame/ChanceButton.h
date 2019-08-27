#pragma once

class CChanceButton :public LSY::CButton
{
public:
	CChanceButton(void);
	~CChanceButton(void);

	virtual void		OnCreateChild(LSY::CObjectBase *pObject);
	virtual void		OnCreated(void);
	virtual void		Logic(const DWORD &dwTime);

	void				SetChanceInfo(int nRewardItemType, int nRewardItemCode, COleDateTime &timeExpire );
	void				OnExpire(void);
	void				MakeSetText(void);
	void				ClearChance(void);
	int					GetRewartItemCode();
	int					GetRewartItemType();
	
protected:

	LSY::CText			*m_pText;
	int					m_nRewardItemCode;
	int					m_nRewardItemType;
	
	COleDateTime		m_timeExpire;
	DWORD				m_dwLastUpdateTime;
	BOOL				m_bEventChanceActive;
};
