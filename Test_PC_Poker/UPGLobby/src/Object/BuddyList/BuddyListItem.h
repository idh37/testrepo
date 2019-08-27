#pragma once

class CBuddyGroup;
class CBuddyFriend;

class UPG_LOBBY_EXPORT CBuddyListItem : public LSY::CListItem
{
protected:
	//친구 그룹 관련 객체 변수들
	LSY::CGroup			*m_pGrpBuddyGroup;
	LSY::CCheckButton	*m_pExpandGroup;
	//친구 그룹 관련 객체 변수들(일반)
	LSY::CGroup			*m_pGrpBuddyGroupShow;
	LSY::CText			*m_pTxtGroupName;
	LSY::CButton		*m_pBtnGroupNameChange;
	LSY::CButton		*m_pBtnGroupDelete;
	//친구 그룹 관련 객체 변수들(편집)
	LSY::CGroup			*m_pGrpBuddyGroupEdit;
	LSY::CImage			*m_pImgBuddyGroupEditBack;
	LSY::CEdit			*m_pEditBuddyGroupName;
	LSY::CButton		*m_pBtnGroupNameEditOK;
	LSY::CButton		*m_pBtnGroupNameEditCancel;
	
	LSY::CGroup			*m_pGrpBuddy;
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
	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnEditFocusOut(LSY::CMessage *msg);

	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);
protected:
	virtual void UpdateBuddyGroup(CBuddyGroup *pBuddyGroup);
	virtual void UpdateBuddy(CBuddyFriend *pBuddy);

public:
	friend class CBuddyListGroup;
};