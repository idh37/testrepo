#pragma once

class CBuddyGroup;
class CBuddyFriend;

class CBuddyListItem : public LSY::CListItem
{
protected:
	//친구 그룹 관련 객체 변수들
	LSY::CGroup			*m_pGrpBuddyGroup;
	LSY::CCheckButton	*m_pCheckGroupSelect;
	LSY::CText			*m_pTxtGroupName;
	
	LSY::CGroup			*m_pGrpBuddy;
	LSY::CCheckButton	*m_pCheckBuddySelect;
	LSY::CImage			*m_pImgBuddyOnOffline;
	LSY::CText			*m_pTxtBuddyNickName;
	LSY::CTextNumber	*m_pTxtnbBuddyHaveMoney;
	LSY::CImage			*m_pImgBuddyLocation;
	LSY::CText			*m_pTxtBuddyLocationGame;
	LSY::CText			*m_pTxtnbBuddyLocationRoomNumber;
	LSY::CText			*m_pTxtBuddyOffline;
public:
	CBuddyListItem();
	virtual ~CBuddyListItem();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	LRESULT OnCheckChangeBuddyGroupExpand(LSY::CMessage *msg);
	LRESULT OnCheckChange(LSY::CMessage *msg);

	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);
protected:
	virtual void UpdateBuddyGroup(CBuddyGroup *pBuddyGroup, LSY::CListItemData *pItemData);
	virtual void UpdateBuddy(CBuddyFriend *pBuddy, LSY::CListItemData *pItemData);

public:
	friend class CAddUserSelectDlg;
};