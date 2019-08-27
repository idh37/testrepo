#pragma once

class UPG_LOBBY_EXPORT CVIPJackpotWinListItem : public LSY::CListItem
{
protected:
	LSY::CTextNumber			*m_pNum;
	LSY::CTextNumber			*m_pMoney;
	LSY::CText					*m_pID;
	LSY::CText					*m_pGame;
	LSY::CText					*m_pRound;

public:
	CVIPJackpotWinListItem();
	virtual ~CVIPJackpotWinListItem();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);

	void SetBarType(int nStartIndex);
};