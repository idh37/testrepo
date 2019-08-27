#pragma once

class CBuddyGroup;
class CBuddyFriend;

// 카페원 정보 구조체
typedef struct
{
	std::string sName; // 리스트 아이템 텍스트(그룹 외에는 유저 아이디)
	CafeSystem::CafeKind::Type typeGroup;
	CafeSystem::MemberGrade::Type memGrade; // 0: 비회원 1: 준회원 2: 정회원 3: 카페장, 4: 연합원(카페장 포함) 5: 연합장 
	int nRoomNum; // 게임중인 경우 방 번호. 게임중이 아니면 0
	bool bBuddy;
	bool bGroup;
	bool bExpend;
} CAFEMEMBERINFO;

class UPG_LOBBY_EXPORT CCafeListItem : public LSY::CListItem
{
protected:
	// LSY Control
	LSY::CGroup *m_pGrpCafe;
	LSY::CCheckButton *m_pChkBtnCafeExpandGroup;

	LSY::CGroup *m_pGrpCafeSub;
	LSY::CText *m_pTxtCafeGrpName;

	LSY::CGroup *m_pGrpCafeMem;
	LSY::CImage *m_pImgCafeOnOffline;
	LSY::CText *m_pTxtCafeMemName;
	LSY::CText *m_pTxtCafeOffline;
	
	// 사용자 정의 변수
	std::list<CAFEMEMBERINFO *> m_listMemInfo;
	bool m_bExpFnd;

public:
	CCafeListItem(void);
	virtual ~CCafeListItem(void);

	// 가상 함수
	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);
	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);

	// 이벤트 함수
	LRESULT OnCheckChangeCafeGroupExpand(LSY::CMessage *pMsg);
	LRESULT OnClickButton(LSY::CMessage *pMsg);

	// 사용자 정의 함수
	void AllAddCafe(LSY::CList *pList);
	void UpdateCafeGroup(CAFEMEMBERINFO *pMemInfo);
	void UpdateCafeMember(CAFEMEMBERINFO *pMemInfo);
	void ChangeExpendList(CafeSystem::CafeKind::Type groupType, bool bExpend);

public:
	friend class CCafeListGroup;
};
