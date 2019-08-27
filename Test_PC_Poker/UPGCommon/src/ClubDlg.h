#pragma once

typedef struct
{
	//클럽원 정보 구조체 이다.
	std::string MemberID;
	INT64 PMoney;
	INT64 LastLoginTime;
} CLUBMEMBERINFO;

typedef struct 
{
	//초대 및 가입신청관련 구조체이다.
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
	//일반 유저들이 보낸 클럽 가입 요청 구조체 이다.
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
	
	// 전체적인 화면
	LSY::CButton *m_pBtnClose;
	LSY::CImage *m_pImgMyClub_nonMem;
	LSY::CImage *m_pImgMyClub_nonMemCreate;
	LSY::CImage *m_pImgMyClub_member;
	LSY::CImage *m_pImgMyClub_boss;
	LSY::CTabControl *m_pTabClubManage_nonMem;
	LSY::CTabControl *m_pTabClubManage_member;
	LSY::CTabControl *m_pTabClubManage_boss;
	
	//// 내클럽
	// 미가입 -> 컨트롤 없음
	// 미가입 + 클럽 생성 가능
	LSY::CButton *m_pBtnMyClub_nonmemcreate; // 클럽 생성 버튼
	// 맴버
	LSY::CText *m_pTxtMyClub_member_clubName; // 가입 클럽명 텍스트
	LSY::CText *m_pTxtMyClub_member_maxNum; // 클럽 최대 인원 텍스트
	LSY::CText *m_pTxtMyClub_member_benefitEx; // 맴버가 되면서 받게되는 추가 적립 비율(%) 설명 텍스트
	LSY::CList *m_pListMyClub_member_memList; // 클럽원 리스트
	LSY::CText *m_pListMyClub_member_memNum; // 클럽원 수 텍스트
	// 보스
	LSY::CText *m_pTxtMyClub_boss_gradeVip; // VIP 등급 텍스트
	LSY::CText *m_pTxtMyClub_boss_maxNum; // 클럽 최대 인원 텍스트
	LSY::CText *m_pTxtMyClub_boss_gradeEx; // 다음 등급에서 가질수 있는 클럽 최대 인원 설명 텍스트
	LSY::CText *m_pTxtMyClub_boss_goldAmount; // 보유 골드 텍스트
	LSY::CText *m_pTxtMyClub_boss_goldPlus; // 전일 보유 골드 텍스트
	LSY::CText *m_pTxtMyClub_boss_goldEx; // 보스의 패이백 비율(%단위) 설명 텍스트
	LSY::CList *m_pListMyClub_boss_memList; // 클럽원 리스트
	LSY::CText *m_pListMyClub_boss_memNum; // 클럽원 수 텍스트
	LSY::CTextButton *m_pTxtBtnMyClub_boss_goldChange; // 골드 교환 팝업 텍스트버튼
	
	////// 관리
	//// 미가입
	// 클럽혜택
	LSY::CText *m_pTxtClubManage_nonMem_benefit; // 클럽 혜택(33.33%) 설명 텍스트
	// 가입요청
	LSY::CText *m_pTxtClubManage_nonMem_joinRequest_count; // 가입 요청한 클럽 수 텍스트
	LSY::CList *m_pListClubManage_nonMem_joinRequestList;
	// 초대수락
	LSY::CList *m_pListClubManage_nonMem_acceptList;
	//// 맴버
	LSY::CButton *m_pBtnClubManage_member_withdraw; // 클럽 탈퇴 버튼
	//// 보스
	// 초대
	LSY::CText *m_pTxtClubManage_boss_invite_count; // 금일 남은 횟수 텍스트
	LSY::CEdit *m_pEditClubManage_boss_invite_id; // 아이디 에디트
	LSY::CButton *m_pBtnClubManage_boss_invite; // 초대하기 버튼
	LSY::CList *m_pListClubManage_boss_inviteList; // 금일 초대한 리스트
	// 요청수락
	LSY::CList *m_pListClubManage_boss_acceptList;
	// 강제탈퇴
	LSY::CList *m_pListClubManage_boss_deportList;
	// 해산
	LSY::CButton *m_pBtnClubManage_boss_disperse; // 클럽 해산 버튼
	
public:
	// 생성자, 소멸자
	CClubDlg(CWnd* pParent = NULL);
	~CClubDlg(void);
	
	// 윈도우
	virtual INT_PTR DoModal(CWnd *pParent);
	
	// 이벤트
	virtual bool OnCreatedProject(void);
	virtual LSY::CObjectBase *OnCallCreateObject(const int &nID, const std::string &strName);
	
	void RefreshDlg(void);
	void CloseDlg(void);
	
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnEditFocusIn(LSY::CMessage *msg);
	LRESULT OnEditReturn(LSY::CMessage *msg);
	
	virtual void OnCancel();
	
protected:
	//// 사용자 정의
	// LSY Control 연결
	void InitLSYControl();
	// 유저 상태에 맞는 다이얼로그 보이기
	void InitShow(bool bCanCreate);
	// 데이터 세팅
	void InitData();
	// 골드를 00000000 -> 0000만0000 형태로 형변환
	std::string ConvertIntForm(int nParam);
	// 클럽원 리스트 관련
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