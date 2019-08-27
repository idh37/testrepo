#pragma once

typedef struct
{
	//클럽원 정보 구조체 이다.
	std::string MemberID;
	//INT64 PMoney;
	//INT64 LastLoginTime;
} CLUBMEMBERINFO2;

class UPG_LOBBY_EXPORT CReverseBuddyListGroup : public LSY::CWindows
{
protected:
	LSY::CButton *m_pBtnBuddyAdd;
	LSY::CCheckBox *m_pCheckBoxShowNotFriend;
	LSY::CList *m_pListBuddy;
	LSY::CRadio	*m_pRadioClub;
	
	SMALLUSERINFO m_stSelectUserInfo;
public:
	CReverseBuddyListGroup();
	virtual ~CReverseBuddyListGroup();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnListItemRClick(LSY::CMessage *msg);
	LRESULT OnListItemDbClick(LSY::CMessage *msg);
	LRESULT OnCheckChange(LSY::CMessage *msg);
	
	void OnReceiveReverseBuddyList(void);
	void SetShowIgnoreMyBuddy(void);
	
	// 사용자 정의
	void HandleResponse(int Signal, int TotSize, char *lpData);
	void RemoveAllMember(void);
	void AddMember(CLUB_MEMBER_INFO *pMemInfo);
};