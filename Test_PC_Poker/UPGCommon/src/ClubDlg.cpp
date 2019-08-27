#include "StdAfx.h"
#include "ClubDlg.h"
#include "clubdlgid.h"

#include <sstream>
#include <string>
#include <math.h>

CClubDlg::CClubDlg(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{

}

CClubDlg::~CClubDlg(void)
{

}

//BEGIN_MESSAGE_MAP(CClubDlg, LSY::CLSYLibDialogBase)
//END_MESSAGE_MAP()

INT_PTR CClubDlg::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_CLUBDLG);
}


bool CClubDlg::OnCreatedProject(void)
{
	// LSY Control 연결
	InitLSYControl();
	// 데이터 세팅
	InitData();
	// 유저 상태에 맞는 다이얼로그 보이기
	bool bCanCreate = GM().GetClubEnableCreate();
	InitShow(bCanCreate);

	return TRUE;
}

void CClubDlg::RefreshDlg(void)
{
	// 데이터 세팅
	InitData();
	// 유저 상태에 맞는 다이얼로그 보이기
	bool bCanCreate = GM().GetClubEnableCreate();
	InitShow(bCanCreate);
	
	LSY::CTabControl * tabControl= (LSY::CTabControl *)GetObject(ID_CLUBDLG_TAB_CLUB);
	tabControl->SetCurSelFromIndex(0);
}

void CClubDlg::CloseDlg(void)
{
	OnCancel();
}

void CClubDlg::OnCancel() 
{
	GM().SetIsShowingClubDlg(false);
	LSY::CLSYLibDialogBase::OnCancel();
}

LSY::CObjectBase *CClubDlg::OnCallCreateObject(const int &nID, const std::string &strName)
{
	switch(nID){
	case ID_CLUBDLG_LISTITEM_MYCLUB_BOSS_MEMLIST:
	case ID_CLUBDLG_LISTITEM_MYCLUB_MEMBER_MEMLIST:
		return (LSY::CObjectBase*)(new CMyClubMemberListItem());
	case ID_CLUBDLG_LISTITEM_CLUBMANAGE_NONMEM_JOINREQUEST_CLUB:
		return (LSY::CObjectBase*)(new CMyClubRequestListItem());
	case ID_CLUBDLG_LISTITEM_CLUBMANAGE_NONMEM_ACCEPT_CLUB:
		return (LSY::CObjectBase*)(new CMyClubInvitedListItem());
	case ID_CLUBDLG_LISTITEM_CLUBMANAGE_BOSS_INVITE_INVITELIST:
		return (LSY::CObjectBase*)(new CClubManageInviteListItem());
	case ID_CLUBDLG_LISTITEM_CLUBMANAGE_BOSS_ACCEPT_USER:
		return (LSY::CObjectBase*)(new CClubManageAcceptListItem());	
	case ID_CLUBDLG_LISTITEM_CLUBMANAGE_BOSS_DEPORT_USER:
		return (LSY::CObjectBase*)(new CClubManageMemberListItem());
	}
	return NULL;
}

/// <summary>
/// 버튼 클릭 이벤트. 대부분 서버에 요청 보내는 기능.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CClubDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_CLUBDLG_BTN_CLUB_CLOSE:		// 닫기 버튼
		CloseDlg();
		break;
	case ID_CLUBDLG_BTN_MYCLUB_NONMEMCREATE_CREATECLUB:		// 클럽 생성 버튼
		{
			CCL_CLUB_CREATE msg;
			msg.Set();
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case ID_CLUBDLG_BTN_CLUBMANAGE_MEMBER_WITHDRAW:			// 클럽 탈퇴 버튼
	case ID_CLUBDLG_BTN_CLUBMANAGE_BOSS_DISPERSE:			// 클럽 해산 버튼
		{
			CCL_CLUB_WITHDRAW msg;
			msg.Set();
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case ID_CLUBDLG_BTN_CLUBMANAGE_BOSS_INVITE:				// 초대하기 버튼
		{
			CCL_CLUB_INVITE msg;

			CString strText = m_pEditClubManage_boss_invite_id->GetText().c_str();
			if(strText.GetLength() < 1 || strText == "닉네임을 입력하세요") 
				return FALSE;

			m_pEditClubManage_boss_invite_id->SetText("");

			msg.Set(strText.GetString());
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case ID_CLUBDLG_TXTBTN_MYCLUB_BOSS_GOLDCHANGE:
		{
			//MM().Call(UPG_CMK_SHOW_GOLDBIGWHEEL_DIALOG);
		}
		break;
	}

	return TRUE;
}

LRESULT CClubDlg::OnEditFocusIn(LSY::CMessage *msg)
{
	LSY::CMO_FocusIn *pMsg = (LSY::CMO_FocusIn *)msg;
	LSY::CEdit *pEdit = (LSY::CEdit *)pMsg->GetObject();
	if(pEdit->GetText() == "닉네임을 입력하세요")
	{
		m_pEditClubManage_boss_invite_id->SetText("");
	}

	return NULL;
}

LRESULT CClubDlg::OnEditReturn(LSY::CMessage *msg)
{
	LSY::CMO_Return *pMsg = (LSY::CMO_Return *)msg;
	LSY::CEdit *pEdit = (LSY::CEdit *)pMsg->GetObject();
	std::string strText = pEdit->GetText();
	if(strText == "" || strText == "닉네임을 입력하세요") 
		return FALSE;

	pEdit->SetText("");

	CCL_CLUB_INVITE cMsg;
	cMsg.Set(strText.c_str());
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), cMsg.pData, cMsg.GetTotalSize());

	return NULL;
}

/// <summary>
/// LSY Control 연결
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CClubDlg::InitLSYControl()
{
	CString str;
	//// 전체적인 화면
	// 닫기 버튼
	m_pBtnClose = (LSY::CButton *)GetObject(ID_CLUBDLG_BTN_CLUB_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubDlg::OnClickButton));
	// 유저가 클럽 미가입 상태일때 화면
	m_pImgMyClub_nonMem = (LSY::CImage *)GetObject(ID_CLUBDLG_IMG_MYCLUB_NONMEM);
	m_pTabClubManage_nonMem = (LSY::CTabControl *)GetObject(ID_CLUBDLG_TAB_CLUBMANAGE_NONMEM);
	// 유저가 클럽 미가입 + 클럽 만들기 가능 상태일때 화면
	m_pImgMyClub_nonMemCreate = (LSY::CImage *)GetObject(ID_CLUBDLG_IMG_MYCLUB_NONMEMCREATE);
	// 유저가 클럽원 일때 화면
	m_pImgMyClub_member = (LSY::CImage *)GetObject(ID_CLUBDLG_IMG_MYCLUB_MEMBER);
	m_pTabClubManage_member = (LSY::CTabControl *)GetObject(ID_CLUBDLG_TAB_CLUBMANAGE_MEMBER);
	// 유저가 클럽 보스 일때 화면
	m_pImgMyClub_boss = (LSY::CImage *)GetObject(ID_CLUBDLG_IMG_MYCLUB_BOSS);
	m_pTabClubManage_boss = (LSY::CTabControl *)GetObject(ID_CLUBDLG_TAB_CLUBMANAGE_BOSS);
	
	//// 내클럽
	// 미가입 -> 컨트롤 없음
	// 미가입 + 클럽 생성 가능
	m_pBtnMyClub_nonmemcreate = (LSY::CButton *)GetObject(ID_CLUBDLG_BTN_MYCLUB_NONMEMCREATE_CREATECLUB);	// 클럽 생성 버튼
	m_pBtnMyClub_nonmemcreate->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubDlg::OnClickButton));
	// 맴버
	m_pTxtMyClub_member_clubName = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_MEMBER_CLUBNAME);		// 클럽 이름 텍스트
	m_pTxtMyClub_member_maxNum = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_MEMBER_MAXNUM);			// 클럽 최대 인원 텍스트
	m_pTxtMyClub_member_benefitEx = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_MEMBER_BENEFITEX);	// 클럽 혜택 설명 텍스트
	m_pListMyClub_member_memList = (LSY::CList *)GetObject(ID_CLUBDLG_LIST_MYCLUB_MEMBER_MEMLIST);		// 클럽원 리스트
	m_pListMyClub_member_memNum = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_MEMBER_MEMNUM);	
	// 보스
	m_pTxtMyClub_boss_gradeVip = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_BOSS_GRADEVIP);		// VIP 등급 텍스트
	m_pTxtMyClub_boss_maxNum = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_BOSS_MAXNUM);			// 클럽 최대 인원 텍스트
	m_pTxtMyClub_boss_gradeEx = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_BOSS_GRADEEX);		// VIP 등급에 따른 클럽원 수 설명 텍스트
	m_pTxtMyClub_boss_goldAmount = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_BOSS_GOLDAMOUNT);	// 보유 골드 텍스트
	m_pTxtMyClub_boss_goldPlus = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_BOSS_GOLDPLUS);		// 전일 보유 골드 텍스트
	m_pTxtMyClub_boss_goldPlus->SetText("0");
	m_pTxtMyClub_boss_goldEx = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_BOSS_GOLDEX);			// 골드 적립 설명 텍스트
	m_pListMyClub_boss_memList = (LSY::CList *)GetObject(ID_CLUBDLG_LIST_MYCLUB_BOSS_MEMLIST);		// 클럽원 리스트
	m_pListMyClub_boss_memNum = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_BOSS_MEMNUM);	
	m_pTxtBtnMyClub_boss_goldChange = (LSY::CTextButton *)GetObject(ID_CLUBDLG_TXTBTN_MYCLUB_BOSS_GOLDCHANGE);	// 골드 교환 팝업 버튼
	m_pTxtBtnMyClub_boss_goldChange->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubDlg::OnClickButton));
	
	////// 관리
	//// 미가입
	// 클럽혜택
	m_pTxtClubManage_nonMem_benefit = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_CLUBMANAGE_NONMEM_BENEFIT_EXPLAIN);			// 클럽 혜택(33.33%) 설명 텍스트
	str.Format("클럽에 가입하면\n매 판 최대 %.2f%%의 %s이 추가로 적립됩니다.", GM().GetMemberAddGetRate(), strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	m_pTxtClubManage_nonMem_benefit->SetText(str.GetString());
	
	// 가입요청
	m_pTxtClubManage_nonMem_joinRequest_count = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_CLUBMANAGE_NONMEM_JOINREQUEST_COUNT);	// 가입 요청한 클럽 수 텍스트
	m_pListClubManage_nonMem_joinRequestList = (LSY::CList *)GetObject(ID_CLUBDLG_LIST_CLUBMANAGE_NONMEM_JOINREQUEST_CLUB);
	// 초대수락
	m_pListClubManage_nonMem_acceptList = (LSY::CList *)GetObject(ID_CLUBDLG_LIST_CLUBMANAGE_NONMEM_ACCEPT_CLUB);
	// 맴버
	m_pBtnClubManage_member_withdraw = (LSY::CButton *)GetObject(ID_CLUBDLG_BTN_CLUBMANAGE_MEMBER_WITHDRAW);			// 클럽 탈퇴 버튼
	m_pBtnClubManage_member_withdraw->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubDlg::OnClickButton));
	//// 보스
	// 초대
	m_pTxtClubManage_boss_invite_count = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_CLUBMANAGE_BOSS_INVITE_COUNT);		// 금일 남은 횟수 텍스트
	m_pEditClubManage_boss_invite_id = (LSY::CEdit *)GetObject(ID_CLUBDLG_EDIT_CLUBMANAGE_BOSS_INVITE_ID);		// 아이디 에디트
	m_pEditClubManage_boss_invite_id->SetText("닉네임을 입력하세요");
	m_pEditClubManage_boss_invite_id->AddHandler(LSY::EM_O_FOCUSIN, LSY::Fnt(this, &CClubDlg::OnEditFocusIn));
	m_pEditClubManage_boss_invite_id->AddHandler(LSY::EM_O_RETURN, LSY::Fnt(this, &CClubDlg::OnEditReturn));
	
	m_pBtnClubManage_boss_invite = (LSY::CButton *)GetObject(ID_CLUBDLG_BTN_CLUBMANAGE_BOSS_INVITE);			// 초대하기 버튼
	m_pBtnClubManage_boss_invite->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubDlg::OnClickButton));
	m_pListClubManage_boss_inviteList = (LSY::CList *)GetObject(ID_CLUBDLG_LIST_CLUBMANAGE_BOSS_INVITE_INVITELIST);
	// 요청수락
	m_pListClubManage_boss_acceptList = (LSY::CList *)GetObject(ID_CLUBDLG_LIST_CLUBMANAGE_BOSS_ACCEPT_USER);
	// 강제탈퇴
	m_pListClubManage_boss_deportList = (LSY::CList *)GetObject(ID_CLUBDLG_LIST_CLUBMANAGE_BOSS_DEPORT_USER);
	// 해산
	m_pBtnClubManage_boss_disperse = (LSY::CButton *)GetObject(ID_CLUBDLG_BTN_CLUBMANAGE_BOSS_DISPERSE);		// 클럽 해산 버튼
	m_pBtnClubManage_boss_disperse->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubDlg::OnClickButton));

	LSY::CText *textObj = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_NONMEMCREATE_EXPLAIN);
	str.Format("* 클럽은 보스가 다이아몬드 회원 및 VIP등급이 3이상\n되어야 유지됩니다.\n\n* 클럽 생성시 클럽 보스가 되면,\n클럽원이 획득한 재화의 %.1f%%를 %s으로 환산하여 적립받으실 수 있습니다.", GM().GetBossPayBackRate(), strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	textObj->SetText(str.GetString());
	textObj = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_NONMEM_QUAL);
	str.Format("VIP3 등급 이상의 다이아몬드 회원만 가능합니다.");
	textObj->SetText(str.GetString());
	textObj = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_NONMEM_BENEFIT);
	str.Format("클럽을 생성해서 보스가 되면\n매일 클럽원이 획득한 재화의 일정비율을\n%s으로 환산하여 적립받으실 수 있습니다.", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	textObj->SetText(str.GetString());
}

/// <summary>
/// 데이터 세팅
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CClubDlg::InitData()
{
	// 서버 데이터들 컨트롤에 맞게 형변환
	int nMyGold = GM().GetMyGold();		// 보유 골드
	std::string sMyGold = NMBASE::UTIL::g_NumberToHangul(nMyGold);
	CString clubName = GM().GetClubName();		// 클럽 이름(MyInfo, Detail)
	
	//// 내클럽
	// 미가입 -> 컨트롤 없음
	// 미가입 + 클럽 생성 가능
	// 맴버
	m_pTxtMyClub_member_clubName->SetText(clubName.GetString());		// 클럽 이름 텍스트(MyInfo, Detail)
	// 보스
	m_pTxtMyClub_boss_goldAmount->SetText(sMyGold);		// 보유 골드 텍스트(MyInfo)
	
	//// 기타
	m_nClubMemberGrade = GM().GetClubMemberGrade();	// 맴버 등급 설정(Detail)
}

/// <summary>
/// 유저 상태에 맞는 다이얼로그 보이기.
/// <summary>
/// <param name="bCanCreate">false : 클럽 미가입, true : 클럽 미가입 + 클럽 만들기 가능</param>
/// <returns></returns>
void CClubDlg::InitShow(bool bCanCreate)
{
	// 유저가 클럽 미가입 상태일때 다이얼로그
	m_pImgMyClub_nonMem->SetShow(false);
	m_pTabClubManage_nonMem->SetShow(false);
	// 유저가 클럽 미가입 + 클럽 만들기 가능 상태일때 다이얼로그
	m_pImgMyClub_nonMemCreate->SetShow(false);
	// 유저가 클럽원 일때 다이얼로그
	m_pImgMyClub_member->SetShow(false);
	m_pTabClubManage_member->SetShow(false);
	// 유저가 클럽 보스 일때 다이얼로그
	m_pImgMyClub_boss->SetShow(false);
	m_pTabClubManage_boss->SetShow(false);
	
	switch ((CLUB_MEMBER_GRADE)m_nClubMemberGrade)
	{
	case CLUB_MEMBER_GRADE_NONE:
		if (bCanCreate)
			m_pImgMyClub_nonMemCreate->SetShow(true);
		else
			m_pImgMyClub_nonMem->SetShow(true);
		
		m_pTabClubManage_nonMem->SetShow(true);
		break;
	case CLUB_MEMBER_GRADE_NORMAL:
		m_pImgMyClub_member->SetShow(true);
		m_pTabClubManage_member->SetShow(true);
		break;
	case CLUB_MEMBER_GRADE_BOSS:
		m_pImgMyClub_boss->SetShow(true);
		m_pTabClubManage_boss->SetShow(true);
		break;
	}
}

/// <summary>
/// 골드를 00000000 -> 0000만0000 형태로 형변환
/// <summary>
/// <param name=""></param>
/// <returns></returns>
std::string CClubDlg::ConvertIntForm(int nParam)
{
	CString cs;
	std::string s;
	
	// 네자리수 이상일때 '~만' 으로 출력
	if (nParam >= 10000)
	{
		int a = nParam / 10000;
		int b = nParam % 10000;
		cs.Format("%d만%d", a, b);
	}
	else
	{
		cs.Format("%d", nParam);
	}
	
	s = (LPCTSTR)cs;
	
	return s;
}


/// <summary>
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CClubDlg::ShowPopupResultMsg(int Signal, int TotSize, char *lpData)
{
	CString str;
	switch(Signal)
	{
	case SV_CLUB_CREATE_RESULT:
		{
			//0.성공, -1000.내부에러, -1001.이미다른클럽에 가입중, -1002.당일생성불가
			CSV_CLUB_CREATE_RESULT msg;
			msg.Get(lpData, TotSize);
			if (*msg.m_Result==0)
			{
				CCL_CLUB_REQUEST_MYSELF_JOIN_LIST rMsg;
				rMsg.Set(-1);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), rMsg.pData, rMsg.GetTotalSize());

				CCL_CLUB_REQUEST_DETAIL_INFO dMsg;
				dMsg.Set();
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), dMsg.pData, dMsg.GetTotalSize());

				str.Format( "%s 클럽 개설에 성공하였습니다.", msg.m_Club_Name );
				ShowMainMessageDlg(str);
				RefreshDlg();
			}
			else if (*msg.m_Result==-1001)
			{
				str.Format("이미 다른 클럽에 가입중입니다.");
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1002)
			{
				str.Format("오늘은 더이상 생성할 수 없습니다.");
				ShowMainMessageDlg(str);
			}
		}
		break;
	case SV_CLUB_ACCEPT_INVITE_RESULT:
		{
			//0.성공, -1000.내부에러, -1001.존재하지 않는 초대, -1002.클럽정보오류, -1003.가입제한인원 초과
			CSV_CLUB_ACCEPT_INVITE_RESULT msg;
			msg.Get(lpData, TotSize);
			if (*msg.m_Result==0)
			{
				CCL_CLUB_REQUEST_INVITED_LIST rMsg;
				rMsg.Set(-1);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), rMsg.pData, rMsg.GetTotalSize());

				if (*msg.m_ActionCode==CLUB_ACTION_ALLOW)
				{
					CCL_CLUB_REQUEST_DETAIL_INFO dMsg;
					dMsg.Set();
					NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), dMsg.pData, dMsg.GetTotalSize());

					str.Format( "%s 클럽의 초대 요청을 수락하였습니다.", msg.m_ClubName );
					ShowMainMessageDlg(str);
					RefreshDlg();
				}
				else if (*msg.m_ActionCode==CLUB_ACTION_DENY)
				{
					str.Format( "%s 클럽의 초대 요청을 거절하였습니다.", msg.m_ClubName );
					ShowMainMessageDlg(str);
				}
			}
			else if (*msg.m_Result==-1002)
			{
				str.Format( "잘못된 클럽 정보입니다." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1003)
			{
				str.Format( "%s 클럽 인원이 가득 찼습니다.", msg.m_ClubName );
				ShowMainMessageDlg(str);
			}
			else
			{
				str.Format( "클럽 초대 요청 처리를 실패하였습니다." );
				ShowMainMessageDlg(str);
			}
		}
		break;
	case SV_CLUB_WITHDRAW_RESULT:
		{
			//0.성공, -1000.내부에러, -1001.클럽정보오류, -1002.재가입한 클럽에 당일 탈퇴불가
			CSV_CLUB_WITHDRAW_RESULT msg;
			msg.Get(lpData, TotSize);
			if (*msg.m_Result==0)
			{
				CCL_CLUB_REQUEST_DETAIL_INFO dMsg;
				dMsg.Set();
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), dMsg.pData, dMsg.GetTotalSize());

				if(m_nClubMemberGrade == CLUB_MEMBER_GRADE_NORMAL)
				{
					ShowMainMessageDlg("클럽을 탈퇴하였습니다.");
				}
				else if(m_nClubMemberGrade == CLUB_MEMBER_GRADE_BOSS)
				{
					ShowMainMessageDlg("클럽을 해산하였습니다.");
				}
				RefreshDlg();
			}
			else if (*msg.m_Result==-1001)
			{
				str.Format( "잘못된 클럽 정보입니다." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1002)
			{
				str.Format( "재가입한 클럽은 당일에 탈퇴할 수 없습니다." );
				ShowMainMessageDlg(str);
			}
			else
			{
				if(m_nClubMemberGrade == CLUB_MEMBER_GRADE_NORMAL)
				{
					ShowMainMessageDlg("클럽 탈퇴를 실패하였습니다.");
				}
				else if(m_nClubMemberGrade == CLUB_MEMBER_GRADE_BOSS)
				{
					ShowMainMessageDlg("클럽 해산을 실패하였습니다.");
				}
			}
		}
		break;
	case SV_CLUB_MEMBER_FIRE_RESULT:
		{
			//0.성공, -1000.내부에러, -1001.클럽정보오류, -1002.존재하지 않는 회원, -1003.금일 탈퇴 한도 초과(1일1회)
			CSV_CLUB_MEMBER_FIRE_RESULT msg;
			msg.Get(lpData, TotSize);
			if (*msg.m_Result==0)
			{
				CCL_CLUB_REQUEST_DETAIL_INFO rMsg;
				rMsg.Set();
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), rMsg.pData, rMsg.GetTotalSize());

				str.Format( "클럽원 %s을(를) 강퇴시켰습니다.", msg.m_FireID );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1001)
			{
				str.Format( "잘못된 클럽 정보입니다." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1002)
			{
				str.Format( "존재하지 않는 회원입니다." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1003)
			{
				str.Format( "강제탈퇴는 1일1회만 가능 합니다." );
				ShowMainMessageDlg(str);
			}
			else
			{
				str.Format( "클럽원 강퇴를 실패하였습니다" );
				ShowMainMessageDlg(str);
			}
		}
		break;
	case SV_CLUB_MEMBER_FIRE_NOTIFY:
		{
			//탈퇴이유.. enum CLUB_FIRE_REASON 데이터가 들어감. CLUB_FIRE_REASON_BOSS_KICK, CLUB_FIRE_REASON_CLOSE
			/*
			CSV_CLUB_MEMBER_FIRE_NOTIFY msg;
			msg.Get(lpData, TotSize);
			if ((CLUB_FIRE_REASON)(*msg.m_ReasonCode)==CLUB_FIRE_REASON_BOSS_KICK)
			{
				str.Format( "%s 클럽에서 탈퇴 당하셨습니다", msg.m_ClubName );
				ShowMainMessageDlg(str);
			}
			else if ((CLUB_FIRE_REASON)(*msg.m_ReasonCode)==CLUB_FIRE_REASON_CLOSE)
			{
				str.Format( "%s 클럽이 해산되었습니다.", msg.m_ClubName );
				ShowMainMessageDlg(str);
			}
			*/
		}
		break;
	case SV_CLUB_ACCEPT_JOIN_RESULT:
		{
			//0.성공, -1000.내부에러,-1001.클럽정보오류, -1002.가입제한인원초과, -1003.가입자가 다른클럽 보스임, -1004.가입자가 다른 클럽에 가입중
			CSV_CLUB_ACCEPT_JOIN_RESULT msg;
			msg.Get(lpData, TotSize);
			if (*msg.m_Result==0)
			{
				CCL_CLUB_REQUEST_JOIN_LIST rMsg;
				rMsg.Set(-1);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), rMsg.pData, rMsg.GetTotalSize());

				if(*msg.m_ActionCode == CLUB_ACTION_ALLOW)
				{
					CCL_CLUB_REQUEST_DETAIL_INFO dMsg;
					dMsg.Set();
					NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), dMsg.pData, dMsg.GetTotalSize());

					str.Format( "%s님의 클럽 가입 요청을 수락했습니다.", msg.m_JoinID );
				}
				else
				{
					str.Format( "%s님의 클럽 가입 요청을 거절했습니다.", msg.m_JoinID );
				}
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1001)
			{
				str.Format( "잘못된 클럽 정보입니다." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1002)
			{
				str.Format( "클럽 총원을 초과 하였습니다. 클럽원을 줄이거나 VIP등급을 상향하고 다시 시도해주세요" );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1003)
			{
				str.Format( "%s님은 다른 클럽의 보스입니다.", msg.m_JoinID);
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1004)
			{
				str.Format( "%s님은 다른 클럽에 가입중입니다.", msg.m_JoinID );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1005)
			{
				str.Format( "오늘 클럽에서 탈퇴한 유저입니다. 00시 이후에 다시 시도해주세요." );
				ShowMainMessageDlg(str);
			}
			else
			{
				str.Format( "클럽 가입 요청 처리를 실패했습니다." );
				ShowMainMessageDlg(str);
			}
		}
		break;
	}
}

/// <summary>
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CClubDlg::HandleResponse(int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
	case SV_CLUB_RESPONSE_DETAIL_INFO:
		{
			RemoveAllMember();
			
			CSV_CLUB_RESPONSE_DETAIL_INFO msg;
			msg.Get(lpData, TotSize);
			
			int nMemMax = *msg.m_Club_MemberMax; // 클럽 최대 인원
			std::ostringstream osMemMax;
			osMemMax << nMemMax;
			std::string sMemMax("클럽원 최대 " + osMemMax.str() + "명");
			int nBossLevel = *msg.m_Club_BossLevel;	// VIP 등급
			std::ostringstream osBossLevel;
			osBossLevel << nBossLevel;
			std::string sBossLevel("VIP 등급 : " + osBossLevel.str() + "등급");
			CString strBenefitEx;
			strBenefitEx.Format("클럽 가입 혜택으로 %s이 %.2f%% 추가적립 됩니다.", strChip_Name[(int)ROOM_CHIPKIND_GOLD], *msg.m_Member_AddGet_Rate);
			int nGradeEx = *msg.m_Club_Next_MemberMax;	// 다음 등급에서 가질수 있는 클럽 최대 인원
			CString strVIPGrade;
			if(nBossLevel<11)
			{
				strVIPGrade.Format("VIP 1등급 상향시 클럽정원이 %d명으로 증가됩니다.", nGradeEx);
			}
			else
			{
				strVIPGrade.Format("현재 VIP 최고 등급입니다.");
			}
			CString strGoldEx;
			strGoldEx.Format("클럽원이 획득한 재화의 %.1f%%가 %s으로 적립됩니다.", *msg.m_Boss_PayBack_Rate, strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
			CString strMemNum;
			strMemNum.Format("(%d/%d)", *msg.m_Club_MemberCount, *msg.m_Club_MemberMax);
			
			
			//// 내클럽
			// 미가입 -> 컨트롤 없음
			// 미가입 + 클럽 생성 가능
			// 맴버
			m_pTxtMyClub_member_maxNum->SetText(sMemMax); // 클럽 최대 인원 텍스트
			m_pTxtMyClub_member_benefitEx->SetText(strBenefitEx.GetString()); // 맴버가 되면서 받게되는 추가 적립 비율(%) 설명 텍스트
			// 보스
			m_pTxtMyClub_boss_gradeVip->SetText(sBossLevel); // VIP 등급 텍스트
			m_pTxtMyClub_boss_maxNum->SetText(sMemMax); // 클럽 최대 인원 텍스트
			m_pTxtMyClub_boss_gradeEx->SetText(strVIPGrade.GetString()); // 다음 등급에서 가질수 있는 클럽 최대 인원 설명 텍스트
			m_pTxtMyClub_boss_goldEx->SetText(strGoldEx.GetString()); // 보스의 패이백 비율(%단위) 설명 텍스트
			m_pTxtMyClub_boss_goldPlus->SetText(NMBASE::UTIL::g_NumberToHangul(*msg.m_Club_Boss_GetGold_Yesterday)); // 전일 보유 골드 텍스트
			m_pListMyClub_member_memNum->SetText(strMemNum.GetString());
			m_pListMyClub_boss_memNum->SetText(strMemNum.GetString());
			
			for(int i=0; i<*msg.m_Club_MemberCount; i++)					
			{
				if (msg.m_ArrayClubMemberInfo == NULL)
				{
					ShowMainMessageDlg("정보를 가져올 수 없습니다.");
					
					continue;
				}
				AddMember(&msg.m_ArrayClubMemberInfo[i]); // 클럽원 리스트
			}
		}
		break;
	case SV_CLUB_RESPONSE_MYSELF_INVITED_LIST:
		{
			RemoveAllMyInvited();
			
			CSV_CLUB_RESPONSE_MYSELF_INVITED_LIST msg;
			msg.Get(lpData, TotSize);
			
			CString str;
			str.Format("(%d/%d)", *msg.m_TodayCount, *msg.m_TodayMax);
			m_pTxtClubManage_nonMem_joinRequest_count->SetText(str.GetString());
			for(int i=0; i<*msg.m_Count; i++)
			{
				if (msg.m_ArrayInvitedInfo == NULL)
				{
					ShowMainMessageDlg("정보를 가져올 수 없습니다.");
					
					continue;
				}
				AddMyInvited(&msg.m_ArrayInvitedInfo[i]);
			}
		}
		break;
	case SV_CLUB_RESPONSE_INVITED_LIST:
		{
			RemoveAllMyJoin();

			CSV_CLUB_RESPONSE_INVITED_LIST msg;
			msg.Get(lpData, TotSize);

			for(int i=0; i<*msg.m_Count; i++)					
			{
				if (msg.m_ArrayInvitedInfo == NULL)
				{
					ShowMainMessageDlg("정보를 가져올 수 없습니다.");

					continue;
				}
				AddMyJoin(&msg.m_ArrayInvitedInfo[i]);
			}
		}
		break;
	case SV_CLUB_RESPONSE_MYSELF_JOIN_LIST:
		{
			RemoveAllManageInvite();

			CSV_CLUB_RESPONSE_MYSELF_JOIN_LIST msg;
			msg.Get(lpData, TotSize);

			CString str;
			str.Format("(금일 남은 횟수 : %d건)", *msg.m_TodayMax - *msg.m_TodayCount);
			m_pTxtClubManage_boss_invite_count->SetText(str.GetString());
			for(int i=0; i<*msg.m_Count; i++)					
			{
				if (msg.m_ArrayJoinInfo == NULL)
				{
					ShowMainMessageDlg("정보를 가져올 수 없습니다.");

					continue;
				}
				AddManageInvite(&msg.m_ArrayJoinInfo[i]);
			}
		}
		break;
	case SV_CLUB_RESPONSE_JOIN_LIST:
		{
			RemoveAllManageJoin();

			CSV_CLUB_RESPONSE_JOIN_LIST msg;
			msg.Get(lpData, TotSize);

			for(int i=0; i<*msg.m_Count; i++)					
			{
				if (msg.m_ArrayJoinInfo == NULL)
				{
					ShowMainMessageDlg("정보를 가져올 수 없습니다.");

					continue;
				}
				AddManageJoin(&msg.m_ArrayJoinInfo[i]);
			}
		}
	case SV_TOTAL_REQUEST_FAIL_RESULT:
		{
			RefreshDlg();
		}
		break;
	}
}

/// <summary>
/// 클럽원 리스트 전체 삭제
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CClubDlg::RemoveAllMember(void)
{
	m_pListMyClub_member_memList->DeleteAllItem();
	m_pListMyClub_boss_memList->DeleteAllItem();
	m_pListClubManage_boss_deportList->DeleteAllItem();
}

void CClubDlg::RemoveAllMyInvited(void)
{
	m_pListClubManage_nonMem_joinRequestList->DeleteAllItem();
}

void CClubDlg::RemoveAllMyJoin(void)
{
	m_pListClubManage_nonMem_acceptList->DeleteAllItem();
}

void CClubDlg::RemoveAllManageInvite(void)
{
	m_pListClubManage_boss_inviteList->DeleteAllItem();
}

void CClubDlg::RemoveAllManageJoin(void)
{
	m_pListClubManage_boss_acceptList->DeleteAllItem();
}

/// <summary>
/// 클럽원 리스트에 컬럼 추가
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CClubDlg::AddMember(CLUB_MEMBER_INFO *pMemInfo)
{
	CLUBMEMBERINFO* memInfo = new CLUBMEMBERINFO();
	memInfo->MemberID = pMemInfo->MemberID;
	memInfo->PMoney = pMemInfo->PMoney;
	memInfo->LastLoginTime = pMemInfo->LastLoginTime;
	if(GM().GetClubMemberGrade() == CLUB_MEMBER_GRADE_NORMAL)
	{
		m_pListMyClub_member_memList->AddItem((LPARAM)memInfo);
	}
	else if(GM().GetClubMemberGrade() == CLUB_MEMBER_GRADE_BOSS)
	{
		m_pListMyClub_boss_memList->AddItem((LPARAM)memInfo);
		m_pListClubManage_boss_deportList->AddItem((LPARAM)memInfo);
	}
}

void CClubDlg::AddMyInvited(CLUB_INVITED_INFO *pUserInfo)
{
	CLUBINVITEDINFO* userInfo = new CLUBINVITEDINFO();
	userInfo->InviteSeq = pUserInfo->InviteSeq;
	userInfo->ClubID = pUserInfo->ClubID;
	userInfo->ClubName = pUserInfo->ClubName;
	userInfo->MemberMax = pUserInfo->MemberMax;
	userInfo->MemberCount = pUserInfo->MemberCount;
	userInfo->StatusCode = pUserInfo->StatusCode;
	userInfo->RequestCode = pUserInfo->RequestCode;

	m_pListClubManage_nonMem_joinRequestList->AddItem((LPARAM)userInfo);
}

void CClubDlg::AddMyJoin(CLUB_INVITED_INFO *pUserInfo)
{
	CLUBINVITEDINFO* userInfo = new CLUBINVITEDINFO();
	userInfo->InviteSeq = pUserInfo->InviteSeq;
	userInfo->ClubID = pUserInfo->ClubID;
	userInfo->ClubName = pUserInfo->ClubName;
	userInfo->MemberMax = pUserInfo->MemberMax;
	userInfo->MemberCount = pUserInfo->MemberCount;
	userInfo->StatusCode = pUserInfo->StatusCode;
	userInfo->RequestCode = pUserInfo->RequestCode;

	m_pListClubManage_nonMem_acceptList->AddItem((LPARAM)userInfo);
}

void CClubDlg::AddManageInvite(CLUB_JOIN_INFO *pJoinInfo)
{
	CLUBJOININFO* joinInfo = new CLUBJOININFO();
	joinInfo->UserID = pJoinInfo->UserID;
	joinInfo->JoinSeq = pJoinInfo->JoinSeq;
	joinInfo->UserPMoney = pJoinInfo->UserPMoney;
	joinInfo->StatusCode = pJoinInfo->StatusCode;

	if(joinInfo->StatusCode != 2)
		m_pListClubManage_boss_inviteList->AddItem((LPARAM)joinInfo);
}

void CClubDlg::AddManageJoin(CLUB_JOIN_INFO *pJoinInfo)
{
	CLUBJOININFO* joinInfo = new CLUBJOININFO();
	joinInfo->UserID = pJoinInfo->UserID;
	joinInfo->JoinSeq = pJoinInfo->JoinSeq;
	joinInfo->UserPMoney = pJoinInfo->UserPMoney;
	joinInfo->StatusCode = pJoinInfo->StatusCode;

	m_pListClubManage_boss_acceptList->AddItem((LPARAM)joinInfo);
}

// 분을 일, 시, 분으로
LPCTSTR CClubDlg::MinuteToDHM(INT64 min)
{
	static CString s_strTemp; // 한글표시
	s_strTemp = "";
	if(min <= 0)
	{
		s_strTemp.Format("접속중");
	}
	else if(min < 60)
	{
		s_strTemp.Format("%I64d분전", min);
	}
	else
	{
		INT64 hour = min/60ll;
		if(hour < 24)
		{
			s_strTemp.Format("%I64d시간%I64d분전", hour, min%60ll);
		}
		else
		{
			s_strTemp.Format("%d일전", hour/24);
		}
	}

	return (LPCTSTR)s_strTemp;
}

////////////////////////////////////////////////////////////////
// 리스트 아이템 정의
//
// 클럽원 보기
CMyClubMemberListItem::CMyClubMemberListItem()
:
m_pTxtNick(NULL),
m_pTxtMoney(NULL),
m_pTxtLastLogin(NULL)
{
}

CMyClubMemberListItem::~CMyClubMemberListItem()
{
}

void CMyClubMemberListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	switch(pObject->GetID())
	{	
	case ID_CLUBDLG_TXT_MYCLUB_BOSS_MEMLIST_ID: 
	case ID_CLUBDLG_TXT_MYCLUB_MEMBER_MEMLIST_ID: 
		m_pTxtNick = (LSY::CText *)pObject;		
		break;		
	case ID_CLUBDLG_TXT_MYCLUB_BOSS_MEMLIST_MONEY:
	case ID_CLUBDLG_TXT_MYCLUB_MEMBER_MEMLIST_MONEY:  
		m_pTxtMoney = (LSY::CText *)pObject;		
		break;		
	case ID_CLUBDLG_TXT_MYCLUB_BOSS_MEMLIST_LAST: 
	case ID_CLUBDLG_TXT_MYCLUB_MEMBER_MEMLIST_LAST: 
		m_pTxtLastLogin = (LSY::CText *)pObject;		
		break;		
	}

	__super::OnCreateChild(pObject);
}

LSY::CObjectBase *CMyClubMemberListItem::NewObject(void)
{
	return new CMyClubMemberListItem;
}

void CMyClubMemberListItem::UpdateData(const LPARAM &lParam)
{
	LSY::CListItemData *pListItemData = (LSY::CListItemData*)GetData();
	if (pListItemData==NULL) return;

	CLUBMEMBERINFO *pMemberInfo = (CLUBMEMBERINFO*)pListItemData->GetItemData();
	if (pMemberInfo == NULL) return;
	
	m_pTxtNick->SetText(pMemberInfo->MemberID);
	m_pTxtMoney->SetText(NMBASE::UTIL::g_MakeShortMoneyStrEx(pMemberInfo->PMoney));
	m_pTxtLastLogin->SetText(CClubDlg::MinuteToDHM(pMemberInfo->LastLoginTime));
}

// 비멤버 - 보낸 가입요청 리스트
CMyClubRequestListItem::CMyClubRequestListItem()
:
m_pTxtClubName(NULL),
m_pTxtClubMember(NULL),
m_pTxtStatus(NULL)
{
}

CMyClubRequestListItem::~CMyClubRequestListItem()
{
}

void CMyClubRequestListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	switch(pObject->GetID())
	{	
	case ID_CLUBDLG_TXT_CLUBMANAGE_NONMEM_JOINREQUEST_CLUB_NAME: 
		m_pTxtClubName = (LSY::CText *)pObject;		
		break;		
	case ID_CLUBDLG_TXT_CLUBMANAGE_NONMEM_JOINREQUEST_CLUB_NUM:
		m_pTxtClubMember = (LSY::CText *)pObject;		
		break;	
	case ID_CLUBDLG_TXT_CLUBMANAGE_NONMEM_JOINREQUEST_CLUB_STATUS:
		m_pTxtStatus = (LSY::CText *)pObject;		
		break;	
	}

	__super::OnCreateChild(pObject);
}

LSY::CObjectBase *CMyClubRequestListItem::NewObject(void)
{
	return new CMyClubRequestListItem;
}

void CMyClubRequestListItem::UpdateData(const LPARAM &lParam)
{
	LSY::CListItemData *pListItemData = (LSY::CListItemData*)GetData();
	if (pListItemData==NULL) return;

	CLUBINVITEDINFO *pClubInfo = (CLUBINVITEDINFO*)pListItemData->GetItemData();
	if (pClubInfo == NULL) return;

	CString str;
	m_pTxtClubName->SetText(pClubInfo->ClubName);
	str.Format("%d/%d", pClubInfo->MemberCount, pClubInfo->MemberMax);
	m_pTxtClubMember->SetText(str.GetString());
	if(pClubInfo->StatusCode==0)
	{
		m_pTxtStatus->SetText("수락대기");
	}
	else if(pClubInfo->StatusCode==1)
	{
		m_pTxtStatus->SetText("거절");
	}
}

// 비멤버 - 받은 초대 리스트
CMyClubInvitedListItem::CMyClubInvitedListItem()
:
m_pTxtClubName(NULL),
m_pTxtClubMember(NULL),
m_pBtnAccept(NULL),
m_pBtnRefuse(NULL),
m_pTxtStatus(NULL)
{
}

CMyClubInvitedListItem::~CMyClubInvitedListItem()
{
}

void CMyClubInvitedListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	switch(pObject->GetID())
	{	
	case ID_CLUBDLG_TXT_CLUBMANAGE_NONMEM_ACCEPT_CLUB_NAME: 
		m_pTxtClubName = (LSY::CText *)pObject;		
		break;		
	case ID_CLUBDLG_TXT_CLUBMANAGE_NONMEM_ACCEPT_CLUB_NUM:
		m_pTxtClubMember = (LSY::CText *)pObject;		
		break;	
	case ID_CLUBDLG_BTN_CLUBMANAGE_NONMEM_ACCEPT_CLUB_ACCEPT:
		m_pBtnAccept = (LSY::CButton *)pObject;	
		m_pBtnAccept->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMyClubInvitedListItem::OnClickButton));	
		break;	
	case ID_CLUBDLG_BTN_CLUBMANAGE_NONMEM_ACCEPT_CLUB_REJECT:
		m_pBtnRefuse = (LSY::CButton *)pObject;		
		m_pBtnRefuse->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMyClubInvitedListItem::OnClickButton));
		break;
	case ID_CLUBDLG_TXT_CLUBMANAGE_NONMEM_ACCEPT_CLUB_STATUS:
		m_pTxtStatus = (LSY::CText *)pObject;		
		break;	
	}

	__super::OnCreateChild(pObject);
}

LSY::CObjectBase *CMyClubInvitedListItem::NewObject(void)
{
	return new CMyClubInvitedListItem;
}

void CMyClubInvitedListItem::UpdateData(const LPARAM &lParam)
{
	LSY::CListItemData *pListItemData = (LSY::CListItemData*)GetData();
	if (pListItemData==NULL) return;

	CLUBINVITEDINFO *pClubInfo = (CLUBINVITEDINFO*)pListItemData->GetItemData();
	if (pClubInfo == NULL) return;

	CString str;
	m_pTxtClubName->SetText(pClubInfo->ClubName);
	str.Format("%d/%d", pClubInfo->MemberCount, pClubInfo->MemberMax);
	m_pTxtClubMember->SetText(str.GetString());

	m_pBtnAccept->SetUserData(0, (LPARAM)pClubInfo->InviteSeq);
	m_pBtnAccept->SetUserData(1, (LPARAM)pClubInfo->ClubID);
	m_pBtnRefuse->SetUserData(0, (LPARAM)pClubInfo->InviteSeq);
	m_pBtnRefuse->SetUserData(1, (LPARAM)pClubInfo->ClubID);

	if(pClubInfo->StatusCode == 1)
	{
		m_pBtnAccept->SetShow(FALSE);
		m_pBtnRefuse->SetShow(FALSE);
		m_pTxtStatus->SetShow(TRUE);
		m_pTxtStatus->SetText("거절");
	}
	else
	{
		m_pBtnAccept->SetShow(TRUE);
		m_pBtnRefuse->SetShow(TRUE);
		m_pTxtStatus->SetShow(FALSE);
	}
}

LRESULT CMyClubInvitedListItem::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_CLUBDLG_BTN_CLUBMANAGE_NONMEM_ACCEPT_CLUB_ACCEPT:	
		{
			INT64 seq = msg->GetObject()->GetUserData(0);
			int id = msg->GetObject()->GetUserData(1);
			CCL_CLUB_ACCEPT_INVITE msg;
			msg.Set(GM().GetMyInfo()->UI.ID, seq, id, CLUB_ACTION_ALLOW);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case ID_CLUBDLG_BTN_CLUBMANAGE_NONMEM_ACCEPT_CLUB_REJECT:		
		{
			INT64 seq = msg->GetObject()->GetUserData(0);
			int id = msg->GetObject()->GetUserData(1);
			CCL_CLUB_ACCEPT_INVITE msg;
			msg.Set(GM().GetMyInfo()->UI.ID, seq, id, CLUB_ACTION_DENY);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	}

	return TRUE;
}

// 보스 - 내가 초대한 리스트
CClubManageInviteListItem::CClubManageInviteListItem()
:
m_pTxtNick(NULL),
m_pTxtMoney(NULL),
m_pTxtStatus(NULL)
{
}

CClubManageInviteListItem::~CClubManageInviteListItem()
{
}

void CClubManageInviteListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	switch(pObject->GetID())
	{	
	case ID_CLUBDLG_TXT_CLUBMANAGE_BOSS_INVITE_INVITELIST_ID: 
		m_pTxtNick = (LSY::CText *)pObject;		
		break;		
	case ID_CLUBDLG_TXT_CLUBMANAGE_BOSS_INVITE_INVITELIST_MONEY:
		m_pTxtMoney = (LSY::CText *)pObject;		
		break;	
	case ID_CLUBDLG_TXT_CLUBMANAGE_BOSS_INVITE_INVITELIST_STATE:
		m_pTxtStatus = (LSY::CText *)pObject;		
		break;	
	}

	__super::OnCreateChild(pObject);
}

LSY::CObjectBase *CClubManageInviteListItem::NewObject(void)
{
	return new CClubManageInviteListItem;
}

void CClubManageInviteListItem::UpdateData(const LPARAM &lParam)
{
	LSY::CListItemData *pListItemData = (LSY::CListItemData*)GetData();
	if (pListItemData==NULL) return;

	CLUBJOININFO *pJoinInfo = (CLUBJOININFO*)pListItemData->GetItemData();
	if (pJoinInfo == NULL) return;

	m_pTxtNick->SetText(pJoinInfo->UserID);
	m_pTxtMoney->SetText(NMBASE::UTIL::g_MakeShortMoneyStrEx(pJoinInfo->UserPMoney));
	if(pJoinInfo->StatusCode==0)
	{
		m_pTxtStatus->SetText("수락대기");
	}
	else if(pJoinInfo->StatusCode==1)
	{
		m_pTxtStatus->SetText("거절");
	}
}

// 보스 - 가입요청 받은 리스트
CClubManageAcceptListItem::CClubManageAcceptListItem()
:
m_pTxtNick(NULL),
m_pTxtMoney(NULL),
m_pBtnAccept(NULL),
m_pBtnRefuse(NULL),
m_pTxtStatus(NULL)
{
}

CClubManageAcceptListItem::~CClubManageAcceptListItem()
{
}

void CClubManageAcceptListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	switch(pObject->GetID())
	{	
	case ID_CLUBDLG_TXT_CLUBMANAGE_BOSS_ACCEPT_USER_ID: 
		m_pTxtNick = (LSY::CText *)pObject;		
		break;		
	case ID_CLUBDLG_TXT_CLUBMANAGE_BOSS_ACCEPT_USER_MONEY:
		m_pTxtMoney = (LSY::CText *)pObject;		
		break;	
	case ID_CLUBDLG_BTN_CLUBMANAGE_BOSS_ACCEPT_USER_ACCEPT:
		m_pBtnAccept = (LSY::CButton *)pObject;	
		m_pBtnAccept->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubManageAcceptListItem::OnClickButton));	
		break;	
	case ID_CLUBDLG_BTN_CLUBMANAGE_BOSS_ACCEPT_USER_REJECT:
		m_pBtnRefuse = (LSY::CButton *)pObject;		
		m_pBtnRefuse->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubManageAcceptListItem::OnClickButton));
		break;	
	case ID_CLUBDLG_TXT_CLUBMANAGE_BOSS_ACCEPT_USER_STATUS:
		m_pTxtStatus = (LSY::CText *)pObject;		
		break;	
	}

	__super::OnCreateChild(pObject);
}

LSY::CObjectBase *CClubManageAcceptListItem::NewObject(void)
{
	return new CClubManageAcceptListItem;
}

void CClubManageAcceptListItem::UpdateData(const LPARAM &lParam)
{
	LSY::CListItemData *pListItemData = (LSY::CListItemData*)GetData();
	if (pListItemData==NULL) return;

	CLUBJOININFO *pJoinInfo = (CLUBJOININFO*)pListItemData->GetItemData();
	if (pJoinInfo == NULL) return;

	m_pTxtNick->SetText(pJoinInfo->UserID);
	m_pTxtMoney->SetText(NMBASE::UTIL::g_MakeShortMoneyStrEx(pJoinInfo->UserPMoney));

	m_pBtnAccept->SetUserData(0, (LPARAM)pJoinInfo->JoinSeq);
	m_pBtnAccept->SetUserData(1, (LPARAM)&pJoinInfo->UserID);
	m_pBtnRefuse->SetUserData(0, (LPARAM)pJoinInfo->JoinSeq);
	m_pBtnRefuse->SetUserData(1, (LPARAM)&pJoinInfo->UserID);

	if(pJoinInfo->StatusCode == 1)
	{
		m_pBtnAccept->SetShow(FALSE);
		m_pBtnRefuse->SetShow(FALSE);
		m_pTxtStatus->SetShow(TRUE);
		m_pTxtStatus->SetText("거절");
	}
	else
	{
		m_pBtnAccept->SetShow(TRUE);
		m_pBtnRefuse->SetShow(TRUE);
		m_pTxtStatus->SetShow(FALSE);
	}
}

LRESULT CClubManageAcceptListItem::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_CLUBDLG_BTN_CLUBMANAGE_BOSS_ACCEPT_USER_ACCEPT:	
		{
			INT64 seq = msg->GetObject()->GetUserData(0);
			LPARAM lParam = (LPARAM)msg->GetObject()->GetUserData(1);
			std::string *pUserID = (std::string *)lParam;	
			CCL_CLUB_ACCEPT_JOIN msg;
			msg.Set(pUserID->c_str(), GM().GetClubName().GetString(), seq, CLUB_ACTION_ALLOW);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case ID_CLUBDLG_BTN_CLUBMANAGE_BOSS_ACCEPT_USER_REJECT:		
		{
			INT64 seq = msg->GetObject()->GetUserData(0);
			LPARAM lParam = (LPARAM)msg->GetObject()->GetUserData(1);
			std::string *pUserID = (std::string *)lParam;	
			CCL_CLUB_ACCEPT_JOIN msg;
			msg.Set(pUserID->c_str(), GM().GetClubName().GetString(), seq, CLUB_ACTION_DENY);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	}

	return TRUE;
}

// 보스 - 클럽원 관리 리스트
CClubManageMemberListItem::CClubManageMemberListItem()
:
m_pTxtNick(NULL),
m_pTxtLastLogin(NULL),
m_pBtnWithdraw(NULL)
{
}

CClubManageMemberListItem::~CClubManageMemberListItem()
{
}

void CClubManageMemberListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	switch(pObject->GetID())
	{	
	case ID_CLUBDLG_TXT_CLUBMANAGE_BOSS_DEPORT_USER_ID: 
		m_pTxtNick = (LSY::CText *)pObject;		
		break;	
	case ID_CLUBDLG_TXT_CLUBMANAGE_BOSS_DEPORT_USER_LAST:
		m_pTxtLastLogin = (LSY::CText *)pObject;		
		break;		
	case ID_CLUBDLG_BTN_CLUBMANAGE_BOSS_DEPORT_USER_STATE:
		m_pBtnWithdraw = (LSY::CButton *)pObject;
		m_pBtnWithdraw->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubManageMemberListItem::OnClickButton));
		break;
	}

	__super::OnCreateChild(pObject);
}

LSY::CObjectBase *CClubManageMemberListItem::NewObject(void)
{
	return new CClubManageMemberListItem;
}

void CClubManageMemberListItem::UpdateData(const LPARAM &lParam)
{
	LSY::CListItemData *pListItemData = (LSY::CListItemData*)GetData();
	if (pListItemData==NULL) return;

	CLUBMEMBERINFO *pMemberInfo = (CLUBMEMBERINFO*)pListItemData->GetItemData();
	if (pMemberInfo == NULL) return;

	m_pTxtNick->SetText(pMemberInfo->MemberID);
	m_pTxtLastLogin->SetText(CClubDlg::MinuteToDHM(pMemberInfo->LastLoginTime));

	m_pBtnWithdraw->SetUserData(0, (LPARAM)&pMemberInfo->MemberID);
}

LRESULT CClubManageMemberListItem::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_CLUBDLG_BTN_CLUBMANAGE_BOSS_DEPORT_USER_STATE:
		{
			LPARAM lParam = (LPARAM)msg->GetObject()->GetUserData(0);
			std::string *pMemberID = (std::string *)lParam;	
			CCL_CLUB_MEMBER_FIRE msg;
			msg.Set(pMemberID->c_str(), "");
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	}

	return TRUE;
}