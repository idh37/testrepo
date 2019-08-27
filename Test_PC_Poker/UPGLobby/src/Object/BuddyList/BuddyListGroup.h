#pragma once

class UPG_LOBBY_EXPORT CBuddyListGroup : public LSY::CWindows
{
protected:
	LSY::CButton		*m_pBtnBuddyAdd;
	LSY::CButton		*m_pBtnBuddyDelete;
	LSY::CButton		*m_pBtnBuddyMoveGroup;
	LSY::CButton		*m_pBtnBuddySendMemo;
	LSY::CButton		*m_pBtnGroupAdd;
	LSY::CCheckBox		*m_pCheckShowConnectBuddy;
	LSY::CList			*m_pListBuddy;
public:
	CBuddyListGroup();
	virtual ~CBuddyListGroup();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnListItemDbClick(LSY::CMessage *msg);
	LRESULT OnListItemRClick(LSY::CMessage *msg);
	LRESULT OnCheckChange(LSY::CMessage *msg);

	void OnReceiveBuddyGroupEntryList(void);
	void OnReceiveBuddyGroupNameChange(void);
	void OnReceiveBuddyGroupAdd(void);
	void OnReceiveBuddyGroupRemove(void);
	void SetShowConnectBuddy(void);
};