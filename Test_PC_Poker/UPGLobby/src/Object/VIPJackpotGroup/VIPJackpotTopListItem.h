#pragma once

class UPG_LOBBY_EXPORT CVIPJackpotTopListItem : public LSY::CListItem
{
protected:
	LSY::CTextNumber			*m_pNum;
	LSY::CTextNumber			*m_pMoney;
	LSY::CText					*m_pID;
	LSY::CText					*m_pGame;
	LSY::CText					*m_pDate;

public:
	CVIPJackpotTopListItem();
	virtual ~CVIPJackpotTopListItem();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);

	void SetBarType(int nStartIndex);
};