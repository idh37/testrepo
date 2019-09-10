#pragma once

class CBuddyGroup;
class CBuddyFriend;

// ī��� ���� ����ü
typedef struct
{
	std::string sName; // ����Ʈ ������ �ؽ�Ʈ(�׷� �ܿ��� ���� ���̵�)
	CafeSystem::CafeKind::Type typeGroup;
	CafeSystem::MemberGrade::Type memGrade; // 0: ��ȸ�� 1: ��ȸ�� 2: ��ȸ�� 3: ī����, 4: ���տ�(ī���� ����) 5: ������ 
	int nRoomNum; // �������� ��� �� ��ȣ. �������� �ƴϸ� 0
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
	
	// ����� ���� ����
	std::list<CAFEMEMBERINFO *> m_listMemInfo;
	bool m_bExpFnd;

public:
	CCafeListItem(void);
	virtual ~CCafeListItem(void);

	// ���� �Լ�
	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);
	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);

	// �̺�Ʈ �Լ�
	LRESULT OnCheckChangeCafeGroupExpand(LSY::CMessage *pMsg);
	LRESULT OnClickButton(LSY::CMessage *pMsg);

	// ����� ���� �Լ�
	void AllAddCafe(LSY::CList *pList);
	void UpdateCafeGroup(CAFEMEMBERINFO *pMemInfo);
	void UpdateCafeMember(CAFEMEMBERINFO *pMemInfo);
	void ChangeExpendList(CafeSystem::CafeKind::Type groupType, bool bExpend);

public:
	friend class CCafeListGroup;
};
