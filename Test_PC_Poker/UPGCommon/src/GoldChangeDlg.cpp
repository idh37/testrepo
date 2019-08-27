#include "stdafx.h"
#include "GoldChangeDlg.h"
#include "goldchangedlgid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CGoldChangeDlg::CGoldChangeDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
m_pBtn10(NULL),
m_pBtn100(NULL),
m_pBtn1000(NULL),
m_pBtn10000(NULL),
m_pBtn100000(NULL),
m_pBtnRemove(NULL),
m_pBtnMax(NULL),
m_pBtnChange(NULL),
m_pBtnClose(NULL),
m_pTxtCurMoney(NULL),
m_pTxtCurGold(NULL),
m_pTxtChangeGold(NULL),
m_pTxtResultMoney(NULL)
{
	m_nChangeGold = 0;
}

BEGIN_MESSAGE_MAP(CGoldChangeDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CGoldChangeDlg::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_GOLDCHANGEDLG);
}

LRESULT CGoldChangeDlg::OnInit(WPARAM &wParam, LPARAM &lParam)
{
	Init();

	return (LRESULT)wParam;
}

void CGoldChangeDlg::Init()
{
	m_nChangeGold = 0;
	m_pTxtCurMoney->SetNumber(GM().GetMyInfo()->UI.GetMoney());
	m_pTxtCurGold->SetNumber(GM().GetMyGold());

	m_nMaxGold = min((g_MaxServerMoney - GM().GetMyInfo()->UI.GetMoney()) / GM().GetGoldValue(), GM().GetMyGold());
	SetChangeGold(0ll);
	//SetChangeGold(0);
}

bool CGoldChangeDlg::OnCreatedProject(void)
{
	m_pBtn10 = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_10);
	m_pBtn10->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtn100 = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_100);
	m_pBtn100->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtn1000 = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_1000);
	m_pBtn1000->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtn10000 = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_10000);
	m_pBtn10000->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtn100000 = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_100000);
	m_pBtn100000->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtnRemove = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_REMOVE);
	m_pBtnRemove->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtnMax = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_MAX);
	m_pBtnMax->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtnChange = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_CHANGE);
	m_pBtnChange->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtnClose = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pTxtCurMoney = (LSY::CTextNumber *)GetObject(ID_GOLDCHANGEDLG_TEXT_CUR_MONEY);
	m_pTxtCurGold = (LSY::CTextNumber *)GetObject(ID_GOLDCHANGEDLG_TEXT_CUR_GOLD);
	m_pTxtChangeGold = (LSY::CEdit *)GetObject(ID_GOLDCHANGEDLG_TEXT_CHANGE_GOLD);
	// 텍스트 박스의 값을 키보드 입력으로 변경했을때 OnKeyUpEdit 함수를 호출시킴
	m_pTxtChangeGold->AddHandler(LSY::EM_O_KEYUP, LSY::Fnt(this, &CGoldChangeDlg::OnKeyUpEdit));
	m_pTxtChangeGold->SetText("0");
	m_pTxtResultMoney = (LSY::CTextNumber *)GetObject(ID_GOLDCHANGEDLG_TEXT_RESULT_MONEY);
	
	Init();
	
	return TRUE;
}

/// <summary>
/// 교환 골드, 변경 보유머니 값을 세팅한다.
/// m_nChangeGold 값에 의해 세팅되므로 m_nChangeGold 값을 세팅한 후 호출해야함.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldChangeDlg::SetChangeGold(INT64 nChangeGold)
{
	// 캡슐화 - 교환 골드에 따라 변경 보유 머니가 세팅 되야함.
	
	// 변수 세팅(형변환)
	m_nChangeGold = nChangeGold;						// 전역 변수 세팅
	CString csChangeGold;								// CString형 교환 골드
	csChangeGold.Format("%d", nChangeGold);
	std::string strChangeGold((LPCTSTR)csChangeGold);	// string형 교환 골드
	
	// 교환 골드(텍스트 박스 컨트롤) 세팅
	m_pTxtChangeGold->SetText(strChangeGold);
	
	// 변경 보유 머니(숫자형 텍스트 컨트롤) 세팅
	m_pTxtResultMoney->SetNumber(GM().GetMyInfo()->UI.GetMoney() + m_nChangeGold * GM().GetGoldValue());
}

/// <summary>
/// 교환 골드, 변경 보유머니 값을 세팅한다.
/// m_nChangeGold 값에 의해 세팅되므로 m_nChangeGold 값을 세팅한 후 호출해야함.
/// <summary>
/// <param name=""></param>
/// <returns>유효성 검사 결과</returns>
bool CGoldChangeDlg::ChangeGold(INT64 addValue)
{
	// 골드 변화가 없을 경우 리턴
	if (addValue == 0)
		return false;
	
	// 변수 세팅(형변환)
	CString csMaxGold;											// csMaxGold : CString형 최대 골드
	csMaxGold.Format("%d", m_nMaxGold);							// m_nMaxGold : 미리 세팅해논 최대 골드 전역 변수
	std::string strMaxGold((LPCTSTR)csMaxGold);					// strMaxGold : string형 최대 골드
	
	// 유저가 골드 한도를 초과하여 입력했을 경우
	if (m_nChangeGold + addValue > GM().GetMyGold())
	{
		//AfxMessageBox(_T("보유한 골드가 부족합니다. \n보유골드 내에서 교환해주세요."));
		
		// 입력한 골드가 교환 한도를 넘을경우 한도를 넘지 않도록 세팅
		//m_nChangeGold = min(m_nMaxGold, m_nChangeGold + addValue);
		m_nChangeGold = m_nMaxGold;
		SetChangeGold(m_nChangeGold);
		
		return false;
	}
	
	// 유저가 골드 한도를 초과하여 입력했을 경우
	if (m_nChangeGold + addValue > m_nMaxGold)
	{
		ShowMainMessageDlg("보유할수 있는 게임머니의 한도를 초과하였습니다.\n보유한도 내에서 교환해주세요.");
		
		// 입력한 골드가 교환 한도를 넘을경우 한도를 넘지 않도록 세팅
		//m_nChangeGold = min(m_nMaxGold, m_nChangeGold + addValue);
		m_nChangeGold = m_nMaxGold;
		SetChangeGold(m_nChangeGold);
		
		return false;
	}

	// 유효성 검사를 통과했을 경우 유저가 입력한 값을 교환 골드에 세팅
	m_nChangeGold += addValue;
	// 교환 골드, 변경 보유머니 값 세팅
	SetChangeGold(m_nChangeGold);
	
	return true;
}

/// <summary>
/// 유효성 검사 - 교환 타이밍
/// <summary>
/// <returns>요효성 검사 결과</returns>
bool CGoldChangeDlg::ValidationCheckChange()
{
	// 변수 세팅(형변환)
	CString csInputGold(m_pTxtChangeGold->GetText().c_str());
	INT64 nInputGold = _ttoi(csInputGold);

	if (nInputGold < 10)
	{
		CString csMsg;
		csMsg.Format("교환 최소 한도는 10%s 입니다.", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
		ShowMainMessageDlg(csMsg);

		return false;
	}
	
	return true;
}

/// <summary>
/// 골드 텍스트 박스의 값을 키보드 입력으로 변경했을때 호출되는 함수
/// <summary>
/// <param name="msg">결과 족보 코드(로티플, 스티플, 포카드 등등..)</param>
/// <returns></returns>
LRESULT CGoldChangeDlg::OnKeyUpEdit(LSY::CMessage *msg)
{
	// 변수 세팅(형변환)
	CString csInputGold(m_pTxtChangeGold->GetText().c_str());	// csInputGold : CString형 입력 골드, m_pTxtChangeGold : 변경 골드 에디트 박스
	INT64 nInputGold = _ttoi(csInputGold);						// nInputGold : INT64형 입력 골드
	LSY::CMO_KeyUp *pMsg = (LSY::CMO_KeyUp *)msg;

	if (csInputGold.IsEmpty())
	{
		SetChangeGold(0);
	}
	// 유저가 숫자가 아닌 값을 입력했을 경우
	// NumLock, 백스페이스, Del, 좌우방향키는 입력 가능
	else if (!((pMsg->GetValue()>=48 && pMsg->GetValue()<=57) || (pMsg->GetValue()>=96 && pMsg->GetValue()<=105)
		|| pMsg->GetValue()==8 || pMsg->GetValue()==46 || pMsg->GetValue()==37 || pMsg->GetValue()==39 || pMsg->GetValue()==144))
	{
		ShowMainMessageDlg("숫자만 입력 가능합니다.");
		SetChangeGold(m_nChangeGold);
		
		return FALSE;
	}
	
	ChangeGold(nInputGold - m_nChangeGold);
	
	//LSY::CMO_Return *pMsg = (LSY::CMO_Return *)msg;
	//LSY::CEdit *pEdit = (LSY::CEdit *)pMsg->GetObject();
	//switch(pEdit->GetID())
	//{
	//case ID_GOLDCHANGEDLG_TEXT_CHANGE_GOLD:
	//	FindUser();
	//	break;
	//}

	return TRUE;
}

LRESULT CGoldChangeDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_GOLDCHANGEDLG_BTN_10:
		ChangeGold(10ll);
		break;
	case ID_GOLDCHANGEDLG_BTN_100:
		ChangeGold(100ll);
		break;
	case ID_GOLDCHANGEDLG_BTN_1000:
		ChangeGold(1000ll);
		break;
	case ID_GOLDCHANGEDLG_BTN_10000:
		ChangeGold(10000ll);
		break;
	case ID_GOLDCHANGEDLG_BTN_100000:
		ChangeGold(100000ll);
		break;
	case ID_GOLDCHANGEDLG_BTN_REMOVE:
		ChangeGold(-m_nChangeGold);
		break;
	case ID_GOLDCHANGEDLG_BTN_MAX:
		{
			// 최대 값으로 세팅(유호성 검사 필요 없음)
			m_nChangeGold = m_nMaxGold;
			// 교환 골드, 변경 보유머니 값 세팅
			ChangeGold(m_nChangeGold);
			//ChangeGold(GM().GetMyGold() - m_nChangeGold);
		}
		break;
	case ID_GOLDCHANGEDLG_BTN_CHANGE:
		{
			if (ValidationCheckChange() == false)
				break;
			CCL_CHANGE_ETCCHIP msg;
			msg.Set("GD", m_nChangeGold);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case ID_GOLDCHANGEDLG_BTN_CLOSE:
		OnCancel();
		break;
	}
	return TRUE;
}