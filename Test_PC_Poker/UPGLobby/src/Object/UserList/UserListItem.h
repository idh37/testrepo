#pragma once

class UPG_LOBBY_EXPORT CUserListItem : public LSY::CListItem
{
protected:
	LSY::CText			*m_pNickName;
	LSY::CTextNumber	*m_pHaveMoney;
	LSY::CText			*m_pGameWhere;
	LSY::CText			*m_pRoomNumber;
public:
	CUserListItem();
	virtual ~CUserListItem();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);
};