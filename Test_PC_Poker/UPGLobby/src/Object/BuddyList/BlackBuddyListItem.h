#pragma once

class UPG_LOBBY_EXPORT CBlackBuddyListItem : public LSY::CListItem
{
protected:
	LSY::CText			*m_pTxtBuddyNickName;

public:
	CBlackBuddyListItem();
	virtual ~CBlackBuddyListItem();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);

public:
	friend class CBlackBuddyListGroup;
};