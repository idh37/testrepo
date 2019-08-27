#pragma once

class UPG_LOBBY_EXPORT CBlackBuddyListGroup : public LSY::CWindows
{
protected:
	LSY::CButton		*m_pBtnBuddyAdd;
	LSY::CButton		*m_pBtnBuddyDelete;
	LSY::CList			*m_pListBuddy;
public:
	CBlackBuddyListGroup();
	virtual ~CBlackBuddyListGroup();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnListItemRClick(LSY::CMessage *msg);
	LRESULT OnListItemDbClick(LSY::CMessage *msg);

	void OnReceiveBlackBuddyList(void);
};