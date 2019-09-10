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
	// LSY Control ����
	InitLSYControl();
	// ������ ����
	InitData();
	// ���� ���¿� �´� ���̾�α� ���̱�
	bool bCanCreate = GM().GetClubEnableCreate();
	InitShow(bCanCreate);

	return TRUE;
}

void CClubDlg::RefreshDlg(void)
{
	// ������ ����
	InitData();
	// ���� ���¿� �´� ���̾�α� ���̱�
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
/// ��ư Ŭ�� �̺�Ʈ. ��κ� ������ ��û ������ ���.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CClubDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_CLUBDLG_BTN_CLUB_CLOSE:		// �ݱ� ��ư
		CloseDlg();
		break;
	case ID_CLUBDLG_BTN_MYCLUB_NONMEMCREATE_CREATECLUB:		// Ŭ�� ���� ��ư
		{
			CCL_CLUB_CREATE msg;
			msg.Set();
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case ID_CLUBDLG_BTN_CLUBMANAGE_MEMBER_WITHDRAW:			// Ŭ�� Ż�� ��ư
	case ID_CLUBDLG_BTN_CLUBMANAGE_BOSS_DISPERSE:			// Ŭ�� �ػ� ��ư
		{
			CCL_CLUB_WITHDRAW msg;
			msg.Set();
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case ID_CLUBDLG_BTN_CLUBMANAGE_BOSS_INVITE:				// �ʴ��ϱ� ��ư
		{
			CCL_CLUB_INVITE msg;

			CString strText = m_pEditClubManage_boss_invite_id->GetText().c_str();
			if(strText.GetLength() < 1 || strText == "�г����� �Է��ϼ���") 
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
	if(pEdit->GetText() == "�г����� �Է��ϼ���")
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
	if(strText == "" || strText == "�г����� �Է��ϼ���") 
		return FALSE;

	pEdit->SetText("");

	CCL_CLUB_INVITE cMsg;
	cMsg.Set(strText.c_str());
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), cMsg.pData, cMsg.GetTotalSize());

	return NULL;
}

/// <summary>
/// LSY Control ����
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CClubDlg::InitLSYControl()
{
	CString str;
	//// ��ü���� ȭ��
	// �ݱ� ��ư
	m_pBtnClose = (LSY::CButton *)GetObject(ID_CLUBDLG_BTN_CLUB_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubDlg::OnClickButton));
	// ������ Ŭ�� �̰��� �����϶� ȭ��
	m_pImgMyClub_nonMem = (LSY::CImage *)GetObject(ID_CLUBDLG_IMG_MYCLUB_NONMEM);
	m_pTabClubManage_nonMem = (LSY::CTabControl *)GetObject(ID_CLUBDLG_TAB_CLUBMANAGE_NONMEM);
	// ������ Ŭ�� �̰��� + Ŭ�� ����� ���� �����϶� ȭ��
	m_pImgMyClub_nonMemCreate = (LSY::CImage *)GetObject(ID_CLUBDLG_IMG_MYCLUB_NONMEMCREATE);
	// ������ Ŭ���� �϶� ȭ��
	m_pImgMyClub_member = (LSY::CImage *)GetObject(ID_CLUBDLG_IMG_MYCLUB_MEMBER);
	m_pTabClubManage_member = (LSY::CTabControl *)GetObject(ID_CLUBDLG_TAB_CLUBMANAGE_MEMBER);
	// ������ Ŭ�� ���� �϶� ȭ��
	m_pImgMyClub_boss = (LSY::CImage *)GetObject(ID_CLUBDLG_IMG_MYCLUB_BOSS);
	m_pTabClubManage_boss = (LSY::CTabControl *)GetObject(ID_CLUBDLG_TAB_CLUBMANAGE_BOSS);
	
	//// ��Ŭ��
	// �̰��� -> ��Ʈ�� ����
	// �̰��� + Ŭ�� ���� ����
	m_pBtnMyClub_nonmemcreate = (LSY::CButton *)GetObject(ID_CLUBDLG_BTN_MYCLUB_NONMEMCREATE_CREATECLUB);	// Ŭ�� ���� ��ư
	m_pBtnMyClub_nonmemcreate->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubDlg::OnClickButton));
	// �ɹ�
	m_pTxtMyClub_member_clubName = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_MEMBER_CLUBNAME);		// Ŭ�� �̸� �ؽ�Ʈ
	m_pTxtMyClub_member_maxNum = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_MEMBER_MAXNUM);			// Ŭ�� �ִ� �ο� �ؽ�Ʈ
	m_pTxtMyClub_member_benefitEx = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_MEMBER_BENEFITEX);	// Ŭ�� ���� ���� �ؽ�Ʈ
	m_pListMyClub_member_memList = (LSY::CList *)GetObject(ID_CLUBDLG_LIST_MYCLUB_MEMBER_MEMLIST);		// Ŭ���� ����Ʈ
	m_pListMyClub_member_memNum = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_MEMBER_MEMNUM);	
	// ����
	m_pTxtMyClub_boss_gradeVip = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_BOSS_GRADEVIP);		// VIP ��� �ؽ�Ʈ
	m_pTxtMyClub_boss_maxNum = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_BOSS_MAXNUM);			// Ŭ�� �ִ� �ο� �ؽ�Ʈ
	m_pTxtMyClub_boss_gradeEx = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_BOSS_GRADEEX);		// VIP ��޿� ���� Ŭ���� �� ���� �ؽ�Ʈ
	m_pTxtMyClub_boss_goldAmount = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_BOSS_GOLDAMOUNT);	// ���� ��� �ؽ�Ʈ
	m_pTxtMyClub_boss_goldPlus = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_BOSS_GOLDPLUS);		// ���� ���� ��� �ؽ�Ʈ
	m_pTxtMyClub_boss_goldPlus->SetText("0");
	m_pTxtMyClub_boss_goldEx = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_BOSS_GOLDEX);			// ��� ���� ���� �ؽ�Ʈ
	m_pListMyClub_boss_memList = (LSY::CList *)GetObject(ID_CLUBDLG_LIST_MYCLUB_BOSS_MEMLIST);		// Ŭ���� ����Ʈ
	m_pListMyClub_boss_memNum = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_BOSS_MEMNUM);	
	m_pTxtBtnMyClub_boss_goldChange = (LSY::CTextButton *)GetObject(ID_CLUBDLG_TXTBTN_MYCLUB_BOSS_GOLDCHANGE);	// ��� ��ȯ �˾� ��ư
	m_pTxtBtnMyClub_boss_goldChange->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubDlg::OnClickButton));
	
	////// ����
	//// �̰���
	// Ŭ������
	m_pTxtClubManage_nonMem_benefit = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_CLUBMANAGE_NONMEM_BENEFIT_EXPLAIN);			// Ŭ�� ����(33.33%) ���� �ؽ�Ʈ
	str.Format("Ŭ���� �����ϸ�\n�� �� �ִ� %.2f%%�� %s�� �߰��� �����˴ϴ�.", GM().GetMemberAddGetRate(), strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	m_pTxtClubManage_nonMem_benefit->SetText(str.GetString());
	
	// ���Կ�û
	m_pTxtClubManage_nonMem_joinRequest_count = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_CLUBMANAGE_NONMEM_JOINREQUEST_COUNT);	// ���� ��û�� Ŭ�� �� �ؽ�Ʈ
	m_pListClubManage_nonMem_joinRequestList = (LSY::CList *)GetObject(ID_CLUBDLG_LIST_CLUBMANAGE_NONMEM_JOINREQUEST_CLUB);
	// �ʴ����
	m_pListClubManage_nonMem_acceptList = (LSY::CList *)GetObject(ID_CLUBDLG_LIST_CLUBMANAGE_NONMEM_ACCEPT_CLUB);
	// �ɹ�
	m_pBtnClubManage_member_withdraw = (LSY::CButton *)GetObject(ID_CLUBDLG_BTN_CLUBMANAGE_MEMBER_WITHDRAW);			// Ŭ�� Ż�� ��ư
	m_pBtnClubManage_member_withdraw->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubDlg::OnClickButton));
	//// ����
	// �ʴ�
	m_pTxtClubManage_boss_invite_count = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_CLUBMANAGE_BOSS_INVITE_COUNT);		// ���� ���� Ƚ�� �ؽ�Ʈ
	m_pEditClubManage_boss_invite_id = (LSY::CEdit *)GetObject(ID_CLUBDLG_EDIT_CLUBMANAGE_BOSS_INVITE_ID);		// ���̵� ����Ʈ
	m_pEditClubManage_boss_invite_id->SetText("�г����� �Է��ϼ���");
	m_pEditClubManage_boss_invite_id->AddHandler(LSY::EM_O_FOCUSIN, LSY::Fnt(this, &CClubDlg::OnEditFocusIn));
	m_pEditClubManage_boss_invite_id->AddHandler(LSY::EM_O_RETURN, LSY::Fnt(this, &CClubDlg::OnEditReturn));
	
	m_pBtnClubManage_boss_invite = (LSY::CButton *)GetObject(ID_CLUBDLG_BTN_CLUBMANAGE_BOSS_INVITE);			// �ʴ��ϱ� ��ư
	m_pBtnClubManage_boss_invite->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubDlg::OnClickButton));
	m_pListClubManage_boss_inviteList = (LSY::CList *)GetObject(ID_CLUBDLG_LIST_CLUBMANAGE_BOSS_INVITE_INVITELIST);
	// ��û����
	m_pListClubManage_boss_acceptList = (LSY::CList *)GetObject(ID_CLUBDLG_LIST_CLUBMANAGE_BOSS_ACCEPT_USER);
	// ����Ż��
	m_pListClubManage_boss_deportList = (LSY::CList *)GetObject(ID_CLUBDLG_LIST_CLUBMANAGE_BOSS_DEPORT_USER);
	// �ػ�
	m_pBtnClubManage_boss_disperse = (LSY::CButton *)GetObject(ID_CLUBDLG_BTN_CLUBMANAGE_BOSS_DISPERSE);		// Ŭ�� �ػ� ��ư
	m_pBtnClubManage_boss_disperse->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CClubDlg::OnClickButton));

	LSY::CText *textObj = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_NONMEMCREATE_EXPLAIN);
	str.Format("* Ŭ���� ������ ���̾Ƹ�� ȸ�� �� VIP����� 3�̻�\n�Ǿ�� �����˴ϴ�.\n\n* Ŭ�� ������ Ŭ�� ������ �Ǹ�,\nŬ������ ȹ���� ��ȭ�� %.1f%%�� %s���� ȯ���Ͽ� ���������� �� �ֽ��ϴ�.", GM().GetBossPayBackRate(), strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	textObj->SetText(str.GetString());
	textObj = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_NONMEM_QUAL);
	str.Format("VIP3 ��� �̻��� ���̾Ƹ�� ȸ���� �����մϴ�.");
	textObj->SetText(str.GetString());
	textObj = (LSY::CText *)GetObject(ID_CLUBDLG_TXT_MYCLUB_NONMEM_BENEFIT);
	str.Format("Ŭ���� �����ؼ� ������ �Ǹ�\n���� Ŭ������ ȹ���� ��ȭ�� ����������\n%s���� ȯ���Ͽ� ���������� �� �ֽ��ϴ�.", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	textObj->SetText(str.GetString());
}

/// <summary>
/// ������ ����
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CClubDlg::InitData()
{
	// ���� �����͵� ��Ʈ�ѿ� �°� ����ȯ
	int nMyGold = GM().GetMyGold();		// ���� ���
	std::string sMyGold = NMBASE::UTIL::g_NumberToHangul(nMyGold);
	CString clubName = GM().GetClubName();		// Ŭ�� �̸�(MyInfo, Detail)
	
	//// ��Ŭ��
	// �̰��� -> ��Ʈ�� ����
	// �̰��� + Ŭ�� ���� ����
	// �ɹ�
	m_pTxtMyClub_member_clubName->SetText(clubName.GetString());		// Ŭ�� �̸� �ؽ�Ʈ(MyInfo, Detail)
	// ����
	m_pTxtMyClub_boss_goldAmount->SetText(sMyGold);		// ���� ��� �ؽ�Ʈ(MyInfo)
	
	//// ��Ÿ
	m_nClubMemberGrade = GM().GetClubMemberGrade();	// �ɹ� ��� ����(Detail)
}

/// <summary>
/// ���� ���¿� �´� ���̾�α� ���̱�.
/// <summary>
/// <param name="bCanCreate">false : Ŭ�� �̰���, true : Ŭ�� �̰��� + Ŭ�� ����� ����</param>
/// <returns></returns>
void CClubDlg::InitShow(bool bCanCreate)
{
	// ������ Ŭ�� �̰��� �����϶� ���̾�α�
	m_pImgMyClub_nonMem->SetShow(false);
	m_pTabClubManage_nonMem->SetShow(false);
	// ������ Ŭ�� �̰��� + Ŭ�� ����� ���� �����϶� ���̾�α�
	m_pImgMyClub_nonMemCreate->SetShow(false);
	// ������ Ŭ���� �϶� ���̾�α�
	m_pImgMyClub_member->SetShow(false);
	m_pTabClubManage_member->SetShow(false);
	// ������ Ŭ�� ���� �϶� ���̾�α�
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
/// ��带 00000000 -> 0000��0000 ���·� ����ȯ
/// <summary>
/// <param name=""></param>
/// <returns></returns>
std::string CClubDlg::ConvertIntForm(int nParam)
{
	CString cs;
	std::string s;
	
	// ���ڸ��� �̻��϶� '~��' ���� ���
	if (nParam >= 10000)
	{
		int a = nParam / 10000;
		int b = nParam % 10000;
		cs.Format("%d��%d", a, b);
	}
	else
	{
		cs.Format("%d", nParam);
	}
	
	s = (LPCTSTR)cs;
	
	return s;
}


/// <summary>
/// �������� ������ ��Ŷ(����) ó��
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
			//0.����, -1000.���ο���, -1001.�̴̹ٸ�Ŭ���� ������, -1002.���ϻ����Ұ�
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

				str.Format( "%s Ŭ�� ������ �����Ͽ����ϴ�.", msg.m_Club_Name );
				ShowMainMessageDlg(str);
				RefreshDlg();
			}
			else if (*msg.m_Result==-1001)
			{
				str.Format("�̹� �ٸ� Ŭ���� �������Դϴ�.");
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1002)
			{
				str.Format("������ ���̻� ������ �� �����ϴ�.");
				ShowMainMessageDlg(str);
			}
		}
		break;
	case SV_CLUB_ACCEPT_INVITE_RESULT:
		{
			//0.����, -1000.���ο���, -1001.�������� �ʴ� �ʴ�, -1002.Ŭ����������, -1003.���������ο� �ʰ�
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

					str.Format( "%s Ŭ���� �ʴ� ��û�� �����Ͽ����ϴ�.", msg.m_ClubName );
					ShowMainMessageDlg(str);
					RefreshDlg();
				}
				else if (*msg.m_ActionCode==CLUB_ACTION_DENY)
				{
					str.Format( "%s Ŭ���� �ʴ� ��û�� �����Ͽ����ϴ�.", msg.m_ClubName );
					ShowMainMessageDlg(str);
				}
			}
			else if (*msg.m_Result==-1002)
			{
				str.Format( "�߸��� Ŭ�� �����Դϴ�." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1003)
			{
				str.Format( "%s Ŭ�� �ο��� ���� á���ϴ�.", msg.m_ClubName );
				ShowMainMessageDlg(str);
			}
			else
			{
				str.Format( "Ŭ�� �ʴ� ��û ó���� �����Ͽ����ϴ�." );
				ShowMainMessageDlg(str);
			}
		}
		break;
	case SV_CLUB_WITHDRAW_RESULT:
		{
			//0.����, -1000.���ο���, -1001.Ŭ����������, -1002.�簡���� Ŭ���� ���� Ż��Ұ�
			CSV_CLUB_WITHDRAW_RESULT msg;
			msg.Get(lpData, TotSize);
			if (*msg.m_Result==0)
			{
				CCL_CLUB_REQUEST_DETAIL_INFO dMsg;
				dMsg.Set();
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), dMsg.pData, dMsg.GetTotalSize());

				if(m_nClubMemberGrade == CLUB_MEMBER_GRADE_NORMAL)
				{
					ShowMainMessageDlg("Ŭ���� Ż���Ͽ����ϴ�.");
				}
				else if(m_nClubMemberGrade == CLUB_MEMBER_GRADE_BOSS)
				{
					ShowMainMessageDlg("Ŭ���� �ػ��Ͽ����ϴ�.");
				}
				RefreshDlg();
			}
			else if (*msg.m_Result==-1001)
			{
				str.Format( "�߸��� Ŭ�� �����Դϴ�." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1002)
			{
				str.Format( "�簡���� Ŭ���� ���Ͽ� Ż���� �� �����ϴ�." );
				ShowMainMessageDlg(str);
			}
			else
			{
				if(m_nClubMemberGrade == CLUB_MEMBER_GRADE_NORMAL)
				{
					ShowMainMessageDlg("Ŭ�� Ż�� �����Ͽ����ϴ�.");
				}
				else if(m_nClubMemberGrade == CLUB_MEMBER_GRADE_BOSS)
				{
					ShowMainMessageDlg("Ŭ�� �ػ��� �����Ͽ����ϴ�.");
				}
			}
		}
		break;
	case SV_CLUB_MEMBER_FIRE_RESULT:
		{
			//0.����, -1000.���ο���, -1001.Ŭ����������, -1002.�������� �ʴ� ȸ��, -1003.���� Ż�� �ѵ� �ʰ�(1��1ȸ)
			CSV_CLUB_MEMBER_FIRE_RESULT msg;
			msg.Get(lpData, TotSize);
			if (*msg.m_Result==0)
			{
				CCL_CLUB_REQUEST_DETAIL_INFO rMsg;
				rMsg.Set();
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), rMsg.pData, rMsg.GetTotalSize());

				str.Format( "Ŭ���� %s��(��) ������׽��ϴ�.", msg.m_FireID );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1001)
			{
				str.Format( "�߸��� Ŭ�� �����Դϴ�." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1002)
			{
				str.Format( "�������� �ʴ� ȸ���Դϴ�." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1003)
			{
				str.Format( "����Ż��� 1��1ȸ�� ���� �մϴ�." );
				ShowMainMessageDlg(str);
			}
			else
			{
				str.Format( "Ŭ���� ���� �����Ͽ����ϴ�" );
				ShowMainMessageDlg(str);
			}
		}
		break;
	case SV_CLUB_MEMBER_FIRE_NOTIFY:
		{
			//Ż������.. enum CLUB_FIRE_REASON �����Ͱ� ��. CLUB_FIRE_REASON_BOSS_KICK, CLUB_FIRE_REASON_CLOSE
			/*
			CSV_CLUB_MEMBER_FIRE_NOTIFY msg;
			msg.Get(lpData, TotSize);
			if ((CLUB_FIRE_REASON)(*msg.m_ReasonCode)==CLUB_FIRE_REASON_BOSS_KICK)
			{
				str.Format( "%s Ŭ������ Ż�� ���ϼ̽��ϴ�", msg.m_ClubName );
				ShowMainMessageDlg(str);
			}
			else if ((CLUB_FIRE_REASON)(*msg.m_ReasonCode)==CLUB_FIRE_REASON_CLOSE)
			{
				str.Format( "%s Ŭ���� �ػ�Ǿ����ϴ�.", msg.m_ClubName );
				ShowMainMessageDlg(str);
			}
			*/
		}
		break;
	case SV_CLUB_ACCEPT_JOIN_RESULT:
		{
			//0.����, -1000.���ο���,-1001.Ŭ����������, -1002.���������ο��ʰ�, -1003.�����ڰ� �ٸ�Ŭ�� ������, -1004.�����ڰ� �ٸ� Ŭ���� ������
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

					str.Format( "%s���� Ŭ�� ���� ��û�� �����߽��ϴ�.", msg.m_JoinID );
				}
				else
				{
					str.Format( "%s���� Ŭ�� ���� ��û�� �����߽��ϴ�.", msg.m_JoinID );
				}
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1001)
			{
				str.Format( "�߸��� Ŭ�� �����Դϴ�." );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1002)
			{
				str.Format( "Ŭ�� �ѿ��� �ʰ� �Ͽ����ϴ�. Ŭ������ ���̰ų� VIP����� �����ϰ� �ٽ� �õ����ּ���" );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1003)
			{
				str.Format( "%s���� �ٸ� Ŭ���� �����Դϴ�.", msg.m_JoinID);
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1004)
			{
				str.Format( "%s���� �ٸ� Ŭ���� �������Դϴ�.", msg.m_JoinID );
				ShowMainMessageDlg(str);
			}
			else if (*msg.m_Result==-1005)
			{
				str.Format( "���� Ŭ������ Ż���� �����Դϴ�. 00�� ���Ŀ� �ٽ� �õ����ּ���." );
				ShowMainMessageDlg(str);
			}
			else
			{
				str.Format( "Ŭ�� ���� ��û ó���� �����߽��ϴ�." );
				ShowMainMessageDlg(str);
			}
		}
		break;
	}
}

/// <summary>
/// �������� ������ ��Ŷ(����) ó��
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
			
			int nMemMax = *msg.m_Club_MemberMax; // Ŭ�� �ִ� �ο�
			std::ostringstream osMemMax;
			osMemMax << nMemMax;
			std::string sMemMax("Ŭ���� �ִ� " + osMemMax.str() + "��");
			int nBossLevel = *msg.m_Club_BossLevel;	// VIP ���
			std::ostringstream osBossLevel;
			osBossLevel << nBossLevel;
			std::string sBossLevel("VIP ��� : " + osBossLevel.str() + "���");
			CString strBenefitEx;
			strBenefitEx.Format("Ŭ�� ���� �������� %s�� %.2f%% �߰����� �˴ϴ�.", strChip_Name[(int)ROOM_CHIPKIND_GOLD], *msg.m_Member_AddGet_Rate);
			int nGradeEx = *msg.m_Club_Next_MemberMax;	// ���� ��޿��� ������ �ִ� Ŭ�� �ִ� �ο�
			CString strVIPGrade;
			if(nBossLevel<11)
			{
				strVIPGrade.Format("VIP 1��� ����� Ŭ�������� %d������ �����˴ϴ�.", nGradeEx);
			}
			else
			{
				strVIPGrade.Format("���� VIP �ְ� ����Դϴ�.");
			}
			CString strGoldEx;
			strGoldEx.Format("Ŭ������ ȹ���� ��ȭ�� %.1f%%�� %s���� �����˴ϴ�.", *msg.m_Boss_PayBack_Rate, strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
			CString strMemNum;
			strMemNum.Format("(%d/%d)", *msg.m_Club_MemberCount, *msg.m_Club_MemberMax);
			
			
			//// ��Ŭ��
			// �̰��� -> ��Ʈ�� ����
			// �̰��� + Ŭ�� ���� ����
			// �ɹ�
			m_pTxtMyClub_member_maxNum->SetText(sMemMax); // Ŭ�� �ִ� �ο� �ؽ�Ʈ
			m_pTxtMyClub_member_benefitEx->SetText(strBenefitEx.GetString()); // �ɹ��� �Ǹ鼭 �ްԵǴ� �߰� ���� ����(%) ���� �ؽ�Ʈ
			// ����
			m_pTxtMyClub_boss_gradeVip->SetText(sBossLevel); // VIP ��� �ؽ�Ʈ
			m_pTxtMyClub_boss_maxNum->SetText(sMemMax); // Ŭ�� �ִ� �ο� �ؽ�Ʈ
			m_pTxtMyClub_boss_gradeEx->SetText(strVIPGrade.GetString()); // ���� ��޿��� ������ �ִ� Ŭ�� �ִ� �ο� ���� �ؽ�Ʈ
			m_pTxtMyClub_boss_goldEx->SetText(strGoldEx.GetString()); // ������ ���̹� ����(%����) ���� �ؽ�Ʈ
			m_pTxtMyClub_boss_goldPlus->SetText(NMBASE::UTIL::g_NumberToHangul(*msg.m_Club_Boss_GetGold_Yesterday)); // ���� ���� ��� �ؽ�Ʈ
			m_pListMyClub_member_memNum->SetText(strMemNum.GetString());
			m_pListMyClub_boss_memNum->SetText(strMemNum.GetString());
			
			for(int i=0; i<*msg.m_Club_MemberCount; i++)					
			{
				if (msg.m_ArrayClubMemberInfo == NULL)
				{
					ShowMainMessageDlg("������ ������ �� �����ϴ�.");
					
					continue;
				}
				AddMember(&msg.m_ArrayClubMemberInfo[i]); // Ŭ���� ����Ʈ
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
					ShowMainMessageDlg("������ ������ �� �����ϴ�.");
					
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
					ShowMainMessageDlg("������ ������ �� �����ϴ�.");

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
			str.Format("(���� ���� Ƚ�� : %d��)", *msg.m_TodayMax - *msg.m_TodayCount);
			m_pTxtClubManage_boss_invite_count->SetText(str.GetString());
			for(int i=0; i<*msg.m_Count; i++)					
			{
				if (msg.m_ArrayJoinInfo == NULL)
				{
					ShowMainMessageDlg("������ ������ �� �����ϴ�.");

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
					ShowMainMessageDlg("������ ������ �� �����ϴ�.");

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
/// Ŭ���� ����Ʈ ��ü ����
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
/// Ŭ���� ����Ʈ�� �÷� �߰�
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

// ���� ��, ��, ������
LPCTSTR CClubDlg::MinuteToDHM(INT64 min)
{
	static CString s_strTemp; // �ѱ�ǥ��
	s_strTemp = "";
	if(min <= 0)
	{
		s_strTemp.Format("������");
	}
	else if(min < 60)
	{
		s_strTemp.Format("%I64d����", min);
	}
	else
	{
		INT64 hour = min/60ll;
		if(hour < 24)
		{
			s_strTemp.Format("%I64d�ð�%I64d����", hour, min%60ll);
		}
		else
		{
			s_strTemp.Format("%d����", hour/24);
		}
	}

	return (LPCTSTR)s_strTemp;
}

////////////////////////////////////////////////////////////////
// ����Ʈ ������ ����
//
// Ŭ���� ����
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

// ���� - ���� ���Կ�û ����Ʈ
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
		m_pTxtStatus->SetText("�������");
	}
	else if(pClubInfo->StatusCode==1)
	{
		m_pTxtStatus->SetText("����");
	}
}

// ���� - ���� �ʴ� ����Ʈ
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
		m_pTxtStatus->SetText("����");
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

// ���� - ���� �ʴ��� ����Ʈ
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
		m_pTxtStatus->SetText("�������");
	}
	else if(pJoinInfo->StatusCode==1)
	{
		m_pTxtStatus->SetText("����");
	}
}

// ���� - ���Կ�û ���� ����Ʈ
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
		m_pTxtStatus->SetText("����");
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

// ���� - Ŭ���� ���� ����Ʈ
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