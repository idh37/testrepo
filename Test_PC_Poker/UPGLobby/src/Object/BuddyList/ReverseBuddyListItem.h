#pragma once

class UPG_LOBBY_EXPORT CReverseBuddyListItem : public LSY::CListItem
{
protected:
	LSY::CText			*m_pTxtBuddyNickName;

public:
	CReverseBuddyListItem();
	virtual ~CReverseBuddyListItem();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);
public:
	friend class CReverseBuddyListGroup;
};