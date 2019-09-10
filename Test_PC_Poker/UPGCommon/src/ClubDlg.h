#pragma once

typedef struct
{
	//Ŭ���� ���� ����ü �̴�.
	std::string MemberID;
	INT64 PMoney;
	INT64 LastLoginTime;
} CLUBMEMBERINFO;

typedef struct 
{
	//�ʴ� �� ���Խ�û���� ����ü�̴�.
	INT64 InviteSeq;
	int ClubID;
	std::string ClubName;
	int MemberMax;
	int MemberCount;
	int StatusCode;
	int RequestCode;
} CLUBINVITEDINFO;

typedef struct
{
	//�Ϲ� �������� ���� Ŭ�� ���� ��û ����ü �̴�.
	std::string UserID;
	INT64 JoinSeq;
	INT64 UserPMoney;
	int StatusCode;
} CLUBJOININFO;

class UPG_COMMON_EXPORT CClubDlg : public LSY::CLSYLibDialogBase
{
private:
	
public:
	int m_nClubMemberGrade;
	
	// ��ü���� ȭ��
	LSY::CButton *m_pBtnClose;
	LSY::CImage *m_pImgMyClub_nonMem;
	LSY::CImage *m_pImgMyClub_nonMemCreate;
	LSY::CImage *m_pImgMyClub_member;
	LSY::CImage *m_pImgMyClub_boss;
	LSY::CTabControl *m_pTabClubManage_nonMem;
	LSY::CTabControl *m_pTabClubManage_member;
	LSY::CTabControl *m_pTabClubManage_boss;
	
	//// ��Ŭ��
	// �̰��� -> ��Ʈ�� ����
	// �̰��� + Ŭ�� ���� ����
	LSY::CButton *m_pBtnMyClub_nonmemcreate; // Ŭ�� ���� ��ư
	// �ɹ�
	LSY::CText *m_pTxtMyClub_member_clubName; // ���� Ŭ���� �ؽ�Ʈ
	LSY::CText *m_pTxtMyClub_member_maxNum; // Ŭ�� �ִ� �ο� �ؽ�Ʈ
	LSY::CText *m_pTxtMyClub_member_benefitEx; // �ɹ��� �Ǹ鼭 �ްԵǴ� �߰� ���� ����(%) ���� �ؽ�Ʈ
	LSY::CList *m_pListMyClub_member_memList; // Ŭ���� ����Ʈ
	LSY::CText *m_pListMyClub_member_memNum; // Ŭ���� �� �ؽ�Ʈ
	// ����
	LSY::CText *m_pTxtMyClub_boss_gradeVip; // VIP ��� �ؽ�Ʈ
	LSY::CText *m_pTxtMyClub_boss_maxNum; // Ŭ�� �ִ� �ο� �ؽ�Ʈ
	LSY::CText *m_pTxtMyClub_boss_gradeEx; // ���� ��޿��� ������ �ִ� Ŭ�� �ִ� �ο� ���� �ؽ�Ʈ
	LSY::CText *m_pTxtMyClub_boss_goldAmount; // ���� ��� �ؽ�Ʈ
	LSY::CText *m_pTxtMyClub_boss_goldPlus; // ���� ���� ��� �ؽ�Ʈ
	LSY::CText *m_pTxtMyClub_boss_goldEx; // ������ ���̹� ����(%����) ���� �ؽ�Ʈ
	LSY::CList *m_pListMyClub_boss_memList; // Ŭ���� ����Ʈ
	LSY::CText *m_pListMyClub_boss_memNum; // Ŭ���� �� �ؽ�Ʈ
	LSY::CTextButton *m_pTxtBtnMyClub_boss_goldChange; // ��� ��ȯ �˾� �ؽ�Ʈ��ư
	
	////// ����
	//// �̰���
	// Ŭ������
	LSY::CText *m_pTxtClubManage_nonMem_benefit; // Ŭ�� ����(33.33%) ���� �ؽ�Ʈ
	// ���Կ�û
	LSY::CText *m_pTxtClubManage_nonMem_joinRequest_count; // ���� ��û�� Ŭ�� �� �ؽ�Ʈ
	LSY::CList *m_pListClubManage_nonMem_joinRequestList;
	// �ʴ����
	LSY::CList *m_pListClubManage_nonMem_acceptList;
	//// �ɹ�
	LSY::CButton *m_pBtnClubManage_member_withdraw; // Ŭ�� Ż�� ��ư
	//// ����
	// �ʴ�
	LSY::CText *m_pTxtClubManage_boss_invite_count; // ���� ���� Ƚ�� �ؽ�Ʈ
	LSY::CEdit *m_pEditClubManage_boss_invite_id; // ���̵� ����Ʈ
	LSY::CButton *m_pBtnClubManage_boss_invite; // �ʴ��ϱ� ��ư
	LSY::CList *m_pListClubManage_boss_inviteList; // ���� �ʴ��� ����Ʈ
	// ��û����
	LSY::CList *m_pListClubManage_boss_acceptList;
	// ����Ż��
	LSY::CList *m_pListClubManage_boss_deportList;
	// �ػ�
	LSY::CButton *m_pBtnClubManage_boss_disperse; // Ŭ�� �ػ� ��ư
	
public:
	// ������, �Ҹ���
	CClubDlg(CWnd* pParent = NULL);
	~CClubDlg(void);
	
	// ������
	virtual INT_PTR DoModal(CWnd *pParent);
	
	// �̺�Ʈ
	virtual bool OnCreatedProject(void);
	virtual LSY::CObjectBase *OnCallCreateObject(const int &nID, const std::string &strName);
	
	void RefreshDlg(void);
	void CloseDlg(void);
	
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnEditFocusIn(LSY::CMessage *msg);
	LRESULT OnEditReturn(LSY::CMessage *msg);
	
	virtual void OnCancel();
	
protected:
	//// ����� ����
	// LSY Control ����
	void InitLSYControl();
	// ���� ���¿� �´� ���̾�α� ���̱�
	void InitShow(bool bCanCreate);
	// ������ ����
	void InitData();
	// ��带 00000000 -> 0000��0000 ���·� ����ȯ
	std::string ConvertIntForm(int nParam);
	// Ŭ���� ����Ʈ ����
	void RemoveAllMember(void);
	void RemoveAllMyInvited(void);
	void RemoveAllMyJoin(void);
	void RemoveAllManageInvite(void);
	void RemoveAllManageJoin(void);
	
public:
	void AddMember(CLUB_MEMBER_INFO *pMemInfo);
	void AddMyInvited(CLUB_INVITED_INFO *pUserInfo);
	void AddMyJoin(CLUB_INVITED_INFO *pUserInfo);
	void AddManageInvite(CLUB_JOIN_INFO *pJoinInfo);
	void AddManageJoin(CLUB_JOIN_INFO *pJoinInfo);
	
	void ShowPopupResultMsg(int Signal, int TotSize, char *lpData);
	void HandleResponse(int Signal, int TotSize, char *lpData);
	
	static LPCTSTR MinuteToDHM(INT64 min);
	
	//DECLARE_MESSAGE_MAP()
};

class UPG_COMMON_EXPORT CMyClubMemberListItem : public LSY::CListItem
{
protected:
	LSY::CText *m_pTxtNick;
	LSY::CText *m_pTxtMoney;
	LSY::CText *m_pTxtLastLogin;
public:
	CMyClubMemberListItem();
	virtual ~CMyClubMemberListItem();
	
	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	
	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);
};

class UPG_COMMON_EXPORT CMyClubRequestListItem : public LSY::CListItem
{
protected:
	LSY::CText *m_pTxtClubName;
	LSY::CText *m_pTxtClubMember;
	LSY::CText *m_pTxtStatus;
public:
	CMyClubRequestListItem();
	virtual ~CMyClubRequestListItem();
	
	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	
	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);
};

class UPG_COMMON_EXPORT CMyClubInvitedListItem : public LSY::CListItem
{
protected:
	LSY::CText *m_pTxtClubName;
	LSY::CText *m_pTxtClubMember;
	LSY::CButton *m_pBtnAccept;
	LSY::CButton *m_pBtnRefuse;
	LSY::CText *m_pTxtStatus;
public:
	CMyClubInvitedListItem();
	virtual ~CMyClubInvitedListItem();
	
	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	
	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);
	LRESULT OnClickButton(LSY::CMessage *pMsg);
};

class UPG_COMMON_EXPORT CClubManageInviteListItem : public LSY::CListItem
{
protected:
	LSY::CText *m_pTxtNick;
	LSY::CText *m_pTxtMoney;
	LSY::CText *m_pTxtStatus;
public:
	CClubManageInviteListItem();
	virtual ~CClubManageInviteListItem();
	
	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	
	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);
};

class UPG_COMMON_EXPORT CClubManageAcceptListItem : public LSY::CListItem
{
protected:
	LSY::CText *m_pTxtNick;
	LSY::CText *m_pTxtMoney;
	LSY::CButton *m_pBtnAccept;
	LSY::CButton *m_pBtnRefuse;
	LSY::CText *m_pTxtStatus;
public:
	CClubManageAcceptListItem();
	virtual ~CClubManageAcceptListItem();
	
	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	
	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);
	LRESULT OnClickButton(LSY::CMessage *pMsg);
};

class UPG_COMMON_EXPORT CClubManageMemberListItem : public LSY::CListItem
{
protected:
	LSY::CText *m_pTxtNick;
	LSY::CText *m_pTxtLastLogin;
	LSY::CButton *m_pBtnWithdraw;
public:
	CClubManageMemberListItem();
	virtual ~CClubManageMemberListItem();
	
	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	
	virtual LSY::CObjectBase *NewObject(void);
	virtual void UpdateData(const LPARAM &lParam);
	LRESULT OnClickButton(LSY::CMessage *pMsg);
};