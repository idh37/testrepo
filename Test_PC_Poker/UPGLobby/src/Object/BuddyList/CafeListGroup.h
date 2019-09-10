#pragma once

class UPG_LOBBY_EXPORT CCafeListGroup : public LSY::CWindows
{
protected:
	// LSY Control
	LSY::CList *m_pListCafe;
	LSY::CButton *m_pBtnAddFnd;
	LSY::CCheckBox *m_pChkExpFnd;

	// ���� ����(SV_CAFE_MEMBERLIST) ������
	CafeSystem::CafeKind::Type m_cafeKind;
	int m_cafeID;
	int m_memInfoCnt;
	CafeSystem::MemberInfo *m_memInfoList;

	// ����� ����
	CCafeListItem *m_pListItem;

public:
	CCafeListGroup(void);
	virtual ~CCafeListGroup(void);

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnListItemRClick(LSY::CMessage *msg);
	LRESULT OnCheckChange(LSY::CMessage *pMsg);

	// ����� ����
	//void AddMember(CAFEMEMBERINFO *pMemInfo);
	void HandleResponse(int Signal, int TotSize, char *lpData); // ���� ���� �ڵ鸵
	void SelectedCafeTab(int Signal, int TotSize, char *lpData);
	void SetListCafeMember();
};
