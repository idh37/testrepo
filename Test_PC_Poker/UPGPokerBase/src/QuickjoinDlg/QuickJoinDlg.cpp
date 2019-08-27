// QuickJoinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QuickJoinDlg.h"
#include "../UIInfoData.h"
#include "Define_Sutda.h"
//#include "RoomOptionMan.h"
//#include "CreateRoomDlg.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define QJ_MYID			(GM().GetMyInfo()->UI.ID)			// 내 아이디
#define QJ_MYNICKNAME	(GM().GetMyInfo()->UI.NickName)		// 내 닉네임 
#define QJ_MYMONEY		(GM().GetMyInfo()->UI.GetMoney(IsTournament()))	// 내 보유머니 
#define CURRENT_GROUP	(g_GetGroupGrade(g_NowGroupNo))		// 현재 그룹 번호 

//베팅룰에 따라 시드머니 선택이 의존적인가? 섯다를 제외한 나머지게임은 베팅룰설정에 따라 시드머니 활성/비활성화됨 .
#define DoesSeedMoneyDependsOnBetRule(eDialogType) (eDialogType!=EQJ_SUTDA)			


CQJConditionBase::CQJConditionBase()
{
	m_pParentWnd = NULL;
}

CQJConditionBase::~CQJConditionBase()
{
	OnDestory();
}

// 컨트롤 생성
BOOL CQJConditionBase::Create(QUICKJOINDLGTYPE eQJDlgType, PID_QUICKJOIN_ITEM eQJItemID, CWnd *pParentWnd, int nItemNumber)
{
	m_eDialogType			= eQJDlgType;	
	m_pParentWnd			= pParentWnd;
	m_nItemNumber			= nItemNumber;
	m_eQuickJoinItemID		= eQJItemID;
	m_sQuickJoinData.m_IDQJ = eQJItemID;

	return TRUE;
}

// 컨트롤 삭제
void CQJConditionBase::OnDestory()
{
	for(int i = 0; i < static_cast<int>(m_vecControls.size()); ++i)
	{
		delete m_vecControls[i].pContol;
	}

	m_vecControls.clear();
}

// 컨트롤 Enable
void CQJConditionBase::EnableControl(BOOL bEnable)
{
	for(int i = 0; i< static_cast<int>(m_vecControls.size()); ++i)
	{		
		m_vecControls[i].pContol->EnableWindow(bEnable);
	}
}

void CQJConditionBase::ShowControl(BOOL bShow)
{
	for(int i = 0; i< static_cast<int>(m_vecControls.size()); ++i)
	{		
		m_vecControls[i].pContol->ShowWindow(bShow?SW_SHOW:SW_HIDE);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////


CQJCondition::CQJCondition()
{
	m_pFirstRadioBtn = NULL;
	m_eRuleType = RULETYPE_7POKER;
}	

CQJCondition::~CQJCondition()
{
}

// 존재 여부 체크
BOOL CQJCondition::DoesExist()
{
	return (ptRadioButton[m_eDialogType][m_eQuickJoinItemID][0] != CPoint(0,0));
}


// 실제 컨트롤을생성하는 함수( 하나의 아이템 그룹에 해당하는 컨트롤만 생성을 한다. )
// 1. 기본 라디오버튼 생성 2. 우선순위 라디오 버튼 생성 3. 콤보박스 생성
BOOL CQJCondition::Create(QUICKJOINDLGTYPE eQJDlgType,  PID_QUICKJOIN_ITEM eQJItemID, CWnd *pParentWnd, int nItemNumber)
{		
	CQJConditionBase::Create(eQJDlgType, eQJItemID, pParentWnd, nItemNumber);

	if(!DoesExist()) 
	{
		return FALSE;
	}

	string RADIOON_PATH = "./PokerCommon/image/QuickJoin/radio02.bmp";		// ON

	//1. 라디오 버튼 생성
	for(int i = 0; i < MAX_RADIO; ++i)
	{			
		// 0으로 세팅이 되어 있으면 라디오 버튼을 생성 안 한다.
		if( CPoint(0,0) == ptRadioButton[eQJDlgType][m_eQuickJoinItemID][i])
		{
			continue;
		}

		// 크기를 세팅한다.
		RECT rt = { ptRadioButton[eQJDlgType][m_eQuickJoinItemID][i].x,										// left
					ptRadioButton[eQJDlgType][m_eQuickJoinItemID][i].y,										// top
					ptRadioButton[eQJDlgType][m_eQuickJoinItemID][i].x + WIDTH_RADIOBUTTON,					// right
					ptRadioButton[eQJDlgType][m_eQuickJoinItemID][i].y + HEIGHT_RADIOBUTTON};				// buttom

		NMBASE::SKINGDI::CBmpRadioBtn* pRadioBtn = new NMBASE::SKINGDI::CBmpRadioBtn;
		pRadioBtn->Create( "Button", WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, rt, pParentWnd, IDC_QUICKJOIN_RADIO);
		pRadioBtn->Init(rt.left, rt.top, const_cast<LPSTR>(RADIOON_PATH.c_str()), NULL, pParentWnd);

		SCONTROL sControlInfo;
		sControlInfo.pContol     = pRadioBtn;
		sControlInfo.TypeControl = TYPE_CONTROL_RADIO;

		//2. 자식으로 콤보 박스가 있으면 생성을 한다.
		if( CPoint(0,0) != ptComboBox[eQJDlgType][m_eQuickJoinItemID][i])	
		{					
			RECT rtCombo = {ptComboBox[eQJDlgType][m_eQuickJoinItemID][i].x, 
							ptComboBox[eQJDlgType][m_eQuickJoinItemID][i].y,
							ptComboBox[eQJDlgType][m_eQuickJoinItemID][i].x + WIDTH_COMBOBOX,
							ptComboBox[eQJDlgType][m_eQuickJoinItemID][i].y + HEIGHT_COMBOBOX};

			CComboBox* pCmbBox = new CComboBox;
			pCmbBox->Create( WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, rtCombo, pParentWnd, (WM_USER + 114680 + m_eQuickJoinItemID + i));			
			pCmbBox->SetFont(pParentWnd->GetFont());

			SCONTROL controlcombo;
			controlcombo.pContol	= pCmbBox;
			controlcombo.TypeControl= TYPE_CONTROL_COMBO;
			m_vecControls.push_back(controlcombo);

			sControlInfo.pChildControl = pCmbBox;
		}

		m_vecControls.push_back(sControlInfo);	

		//첫번째 라디오 버튼 저장 
		if(m_pFirstRadioBtn == NULL) 
		{
			m_pFirstRadioBtn = pRadioBtn;
			m_pFirstRadioBtn->SetCheck(TRUE);			
		}
		else
		{
			//라디오 버튼 그룹화
			m_pFirstRadioBtn->AddToGroup(pRadioBtn);
		}	
	}	

	if(static_cast<int>(m_vecControls.size()) == 0)
	{
		return FALSE;
	}

	return TRUE;
}	


int CQJCondition::GetRadioComboIndex(int &idxCombo)
{
	//현재 선택된 라디오 인덱스 반환
	idxCombo = -1;

	int idxRadio = -1;
	for(int i = 0; i < static_cast<int>(m_vecControls.size()); ++i)
	{
		if(m_vecControls[i].TypeControl != TYPE_CONTROL_RADIO)
		{
			continue;
		}

		idxRadio++;
		if( ((NMBASE::SKINGDI::CBmpRadioBtn*)m_vecControls[i].pContol)->GetCheck() )
		{
			if(m_vecControls[i].pChildControl) ///
			{
				//자식 콤보박스가 있다.
				idxCombo = ((CComboBox*)m_vecControls[i].pChildControl)->GetCurSel();
			}
			return idxRadio;
		}
	}
	return -1;			
}

int CQJCondition::GetCurSelRadioButton(void)
{
	int nRadioButCount = 0;		// 라디오버튼

	BOOL bRadioCheck = FALSE;	// 라디오버튼

	int nControlCount = static_cast<int>(m_vecControls.size());
	TYPE_CONTROL eControlType;

	for(int i = 0; i < nControlCount; ++i)
	{	
		eControlType = m_vecControls[i].TypeControl;

		// 라디오 버튼 ( 0번 )
		if(eControlType == TYPE_CONTROL_RADIO &&! bRadioCheck)
		{				
			if(((NMBASE::SKINGDI::CBmpRadioBtn*)m_vecControls[i].pContol)->GetCheck())
			{
				bRadioCheck = TRUE;				
			}
			else
			{
				++nRadioButCount;
			}			
		}
	}

	return nRadioButCount;
}

CComboBox* CQJCondition::GetComboBox(int idxCombo)
{
	//idxCombo(zero-based) 번 째 콤보박스를 가져온다.
	for(int i = 0, cnt = 0; i < static_cast<int>(m_vecControls.size()); ++i)
	{
		if(m_vecControls[i].TypeControl != TYPE_CONTROL_COMBO)
		{
			continue;
		}

		if(idxCombo == cnt++)
		{
			return (CComboBox*)m_vecControls[i].pContol;
		}
	}

	return NULL;
}

void CQJCondition::RefreshConditonButton()
{		
	//라디오버튼이 CHECK FALSE라면  자식콘트롤은 Disable 시킨다.
	for(int i = 0; i < static_cast<int>(m_vecControls.size()); ++i)
	{
		//라디오버튼만
		if(m_vecControls[i].TypeControl != TYPE_CONTROL_RADIO)
		{
			continue;
		}

		if(m_vecControls[i].pChildControl)
		{
			m_vecControls[i].pChildControl->EnableWindow(((NMBASE::SKINGDI::CBmpRadioBtn*)m_vecControls[i].pContol)->GetCheck());
		}
	}		
}


BOOL CQJCondition::SaveData(const char *pStrSection)
{
	////콤보박스가 있는 조건 항목은 채널 종속적이라고 본다. 채널에 따라 콤보박스 내용이 틀려지기 때문..

	//// 
	////PID_QUICKJOIN_ITEM_MONEY,			// 보유머니	
	////PID_QUICKJOIN_ITEM_GAMEMODE,		// 게임모드
	////PID_QUICKJOIN_ITEM_GAMERULE,		// 게임종류
	////PID_QUICKJOIN_ITEM_BETRULE,		// 베팅룰
	////PID_QUICKJOIN_ITEM_SEEDMONEY,		// 시드머니	

	//int nPriorityCount = 0;	// 우선순위
	//int nRadioButCount = 0;	// 라디오버튼
	//BOOL bWrite = FALSE;	// 우선순위

	//for(int i = 0; i < static_cast<int>(m_vecControls.size()); ++i)
	//{			
	//	CString strKey;		
	//	strKey.Format("%d_%d", m_eQuickJoinItemID, m_vecControls[i].TypeControl);	// Group타입 / 컨트롤 타입(라디오/콤보/우선순위)

	//	// 라디오버튼 0
	//	
	//	// 콤보       1
	//	
	//	// 우선순위	  2
	//	
	//	CString strData;

	//	// 콤보박스 내용 ( 2번 )
	//	if(m_vecControls[i].TypeControl == TYPE_CONTROL_COMBO)
	//	{
	//		int nSel = ((CComboBox*)m_vecControls[i].pContol)->GetCurSel();
	//		if(nSel < 0)
	//		{
	//			continue;
	//		}

	//		/*CString strComboData;
	//		((CComboBox*)m_vecControls[i].pContol)->GetLBText(nSel, strComboData);
	//		strData.Format("%s", strComboData.GetBuffer());*/

	//		strData.Format("%d", nSel);
	//		bWrite = TRUE;
	//	}
	//	// 라디오 버튼 ( 1번 )
	//	else if(m_vecControls[i].TypeControl == TYPE_CONTROL_RADIO)
	//	{				
	//		if(((NMBASE::SKINGDI::CBmpRadioBtn*)m_vecControls[i].pContol)->GetCheck())
	//		{
	//			strData.Format("%d", nRadioButCount);
	//			bWrite = TRUE;
	//		}
	//		++nRadioButCount;
	//	}
	//	// 우선 순위 ( 0 번 )
	//	else if(m_vecControls[i].TypeControl == TYPE_CONTROL_BUTTON_PRIORITY)
	//	{
	//		if(((NMBASE::SKINGDI::CMyButton*)m_vecControls[i].pContol)->IsWindowEnabled())
	//		{
	//			strData.Format("%d", nPriorityCount);
	//			bWrite = TRUE;
	//		}

	//		++nPriorityCount;
	//	}

	//	// 저장
	//	if(bWrite)
	//	{
	//		NMBASE::UTIL::WriteProfile_string(pStrSection, strKey, strData);
	//		bWrite = FALSE;
	//	}
	//}

	return TRUE;
}

BOOL CQJCondition::LoadData(const char *pSection, BOOL bUseFlag)
{		
	//if(m_pFirstRadioBtn==NULL)
	//{
	//	return TRUE;
	//}

	//if(!_LoadData(pSection))
	//{
	//	//데이타가 하나라도 읽기 실패하면 기본 버튼으로 세팅  
	//	SetPriority(0);

	//	m_pFirstRadioBtn->SetCheck(TRUE);
	//	return FALSE;
	//}

	return TRUE;
}

// 데이터를 로드한다.
BOOL CQJCondition::_LoadData(const char *pSection)
{
	//// 콤보박스가 있는 조건 항목은 채널 종속적이라고 본다. 채널에 따라 콤보박스 내용이 틀려지기 때문..

	////int nPriorityBtnCnt =0;

	//int nPriorityCount = 0;	// 우선순위
	//int nRadioButCount = 0;	// 라디오버튼

	//for(int i = 0; i < static_cast<int>(m_vecControls.size()); ++i)
	//{		
	//	CString strKey;
	//	strKey.Format("%d_%d",  m_eQuickJoinItemID, m_vecControls[i].TypeControl);

	//	CString strData = NMBASE::UTIL::GetProfile_string(pSection, strKey, "__NULL__");

	//	if(strData.Compare("__NULL__")==0)
	//	{
	//		//데이타가 하나라도 읽기 실패하면 return FALSE;				
	//		return FALSE;
	//	}

	//	// 콤보 박스
	//	if(m_vecControls[i].TypeControl == TYPE_CONTROL_COMBO)
	//	{
	//		int nSel = atoi(strData);

	//		/*int nSel = ((CComboBox*)m_vecControls[i].pContol)->FindStringExact(0, strData);
	//		if(nSel < 0) 
	//		{
	//			return FALSE;				
	//		}*/

	//		if(nSel < 0 || nSel >= ((CComboBox*)m_vecControls[i].pContol)->GetCount())
	//		{
	//			nSel = 0;
	//		}			

	//		((CComboBox*)m_vecControls[i].pContol)->SetCurSel(nSel);				
	//	}
	//	// 라디오버튼
	//	else if(m_vecControls[i].TypeControl == TYPE_CONTROL_RADIO)
	//	{			
	//		int nPos = atoi(strData);

	//		if(nRadioButCount == nPos)
	//		{
	//			((NMBASE::SKINGDI::CBmpRadioBtn*)m_vecControls[i].pContol)->SetCheck(TRUE);
	//		}
	//		else
	//		{
	//			((NMBASE::SKINGDI::CBmpRadioBtn*)m_vecControls[i].pContol)->SetCheck(FALSE);
	//		}

	//		++nRadioButCount;
	//	}
	//	// 우선순위
	//	else if(m_vecControls[i].TypeControl == TYPE_CONTROL_BUTTON_PRIORITY)
	//	{
	//		int nPos = atoi(strData);

	//		if(nPriorityCount == nPos)
	//		{
	//			((NMBASE::SKINGDI::CMyButton*)m_vecControls[i].pContol)->EnableWindow(TRUE);

	//			if(GetPriority() == 0)
	//			{
	//				SetPriority(nPos);				
	//			}
	//		}
	//		else
	//		{
	//			((NMBASE::SKINGDI::CMyButton*)m_vecControls[i].pContol)->EnableWindow(FALSE);
	//		}

	//		/*BOOL bEnable = (atoi(strData) != 0);
	//		((NMBASE::SKINGDI::CMyButton*)m_vecControls[i].pContol)->EnableWindow(bEnable);
	//		if(bEnable)
	//		{
	//			if(GetPriority() == 0)
	//			{
	//				SetPriority(nPriorityBtnCnt);				
	//			}
	//		}*/

	//		++nPriorityCount;
	//		//++nPriorityBtnCnt;
	//	}		
	//}

	return TRUE;
}

BOOL CMyCQJCondition::Create(QUICKJOINDLGTYPE eQJDlgType, PID_QUICKJOIN_ITEM eQJItemID, CWnd *pParentWnd, int nItemNumber, INT64&	i64LImitMoney)
{
	if(!CQJCondition::Create(eQJDlgType, eQJItemID, pParentWnd, nItemNumber))
	{
		return FALSE;	
	}

	m_eItemType = eQJItemID;				// 아이템 타입

	// 베팅룰과 시드머니는 콤보박스가 있는 항목으로 Add한다. ( 일단 나중에 수정 )
	if(eQJItemID == PID_QUICKJOIN_ITEM_BETRULE)
	{
		CComboBox* pComboBox = GetComboBox(0);
		if(pComboBox)
		{
			SetCombo_AutobetSeedMoney((IDX_GAME) eQJDlgType, pComboBox, i64LImitMoney);				
		}
	}

	return TRUE;	
}

BOOL CMyCQJCondition::ReloadCondition(IDX_GAME idxGame)
{
	//라디오버튼과 콤보박스 설정을 읽어서 Data에 세팅해주는 부분이다.
	// nRadio : 선택된 라디오 버튼 인덱스 
	// nCmBox : 선택된 라디오 버튼 옆의 콤보박스에서  선택된  항목 인덱스 

	int nRadio = 0,  nSelectComboBox = 0;
	nRadio = GetRadioComboIndex(nSelectComboBox);	

	if(nRadio == 0)		// 0일 경우 설정OFF이다 
	{
		//m_sQuickJoinData.m_nPriority = 0;		//설정 OFF는 우선순위 0
		return FALSE;	//우선순위 OFF이다.
	}
	else
	{				
		//여기서 각 라디오버튼+ 인덱스, 콤보박스 인덱스에 따라서 세팅을 해준다. 
		switch(m_eQuickJoinItemID)
		{
		// 3. 게임모드
		case PID_QUICKJOIN_ITEM_GAMEMODE:	
			{
				if(nRadio == 1)	 
				{
					m_sQuickJoinData.union_data.data_mode.FormKind = FORMKIND_CHOICE;
				}
				else
				{
					m_sQuickJoinData.union_data.data_mode.FormKind = FORMKIND_NORMAL;
				}
			}
			break;

		// 4. 입장제한
		case PID_QUICKJOIN_ITEM_BETRULE:
			{					
				m_sQuickJoinData.union_data.data_betrule.nAutoBetKind = 0;			

				if(nRadio == 1) 
				{					
					int nGameIndex = g_RI.nGameIndex;
					m_sQuickJoinData.union_data.data_betrule.nAutoBetKind = nRadio;
				}					
			}
			break;
		// 5. 베팅룰/시드머니
		case PID_QUICKJOIN_ITEM_SEEDMONEY:
			{
				m_sQuickJoinData.union_data.data_seedmoney.idxSeedMoney = nSelectComboBox;
			} 
			break;
		}

		return TRUE; //우선순위가 설정되어야한다.
	}
}



void CMyCQJCondition::SetCheckOff()
{		
	for(int i = 0; i <static_cast<int>(m_vecControls.size()); ++i)
	{		
		if(m_vecControls[i].TypeControl == TYPE_CONTROL_RADIO)
		{
			((NMBASE::SKINGDI::CBmpRadioBtn*)m_vecControls[i].pContol)->SetCheck(m_vecControls[i].pContol == m_pFirstRadioBtn);				
		}
	}

	RefreshConditonButton();
}

void CMyCQJCondition::SetCombo_AutobetSeedMoney(IDX_GAME idxGame, CComboBox *pComboBox, INT64 i64LImitMoney)
{
	if(pComboBox == NULL) 
	{
		return;	
	}

	// 시드머니 변경에 의해 AutoBetSeed가 바뀌기 때문에 이곳에서 값을 저장하고 있는다.
	m_pComboAutobetSeedMoney = pComboBox;
	m_eGameType = idxGame;

	//이전 선택된 스트링을 기억해 뒀다가  내용을 바꾼다음 다시 선택시켜준다.
	CString strPrevText;
	int iPrevSel = pComboBox->GetCurSel();
	if(iPrevSel >= 0)
	{
		pComboBox->GetLBText(iPrevSel, strPrevText);
	}

	// SetAutobetSeedMoneyValue()함수 내부에서 호출
	//pComboBox->ResetContent();

	// 시드머니 값을 채운다.
	SetAutobetSeedMoneyValue();

	//이전 선택된 스트링을 기억해 뒀다가  내용을 바꾼다음 다시 선택시켜준다.
	if(strPrevText.IsEmpty())
	{
		pComboBox->SetCurSel(0);
	}
	else
	{
		int nSelect = pComboBox->FindStringExact(0, strPrevText);
		if(nSelect >= 0)
		{
			pComboBox->SetCurSel(nSelect);
		}
		else
		{
			pComboBox->SetCurSel(0);
		}
	}	
}


BOOL CMyCQJCondition::SetAutobetSeedMoneyValue( void )
{		
	m_pComboAutobetSeedMoney->AddString("큰방");
	m_pComboAutobetSeedMoney->SetItemData(0, 0);

	m_pComboAutobetSeedMoney->AddString("기본방");
	m_pComboAutobetSeedMoney->SetItemData(1, 1);

	m_pComboAutobetSeedMoney->AddString("작은방");
	m_pComboAutobetSeedMoney->SetItemData(2, 2);

	m_pComboAutobetSeedMoney->SetCurSel(0);

	return TRUE;
}

BOOL CMyCQJCondition::SaveData(const char *pStrSection)
{
	//콤보박스가 있는 조건 항목은 채널 종속적이라고 본다. 채널에 따라 콤보박스 내용이 틀려지기 때문..
	int nRadioButCount = 0;		// 라디오버튼
	int nSelectCombo   = 0;

	BOOL bRadioCheck = FALSE;	// 라디오버튼
	BOOL bComboCheck = FALSE;	// 콤보

	int nControlCount = static_cast<int>(m_vecControls.size());
	TYPE_CONTROL eControlType;

	for(int i = 0; i < nControlCount; ++i)
	{	
		eControlType = m_vecControls[i].TypeControl;

		// 라디오 버튼 ( 0번 )
		if(eControlType == TYPE_CONTROL_RADIO &&! bRadioCheck)
		{				
			if(((NMBASE::SKINGDI::CBmpRadioBtn*)m_vecControls[i].pContol)->GetCheck())
			{
				bRadioCheck = TRUE;				
			}
			else
			{
				++nRadioButCount;
			}			
		}
		// 콤보박스 내용 ( 1번 )
		else if(eControlType == TYPE_CONTROL_COMBO && !bComboCheck)
		{
			nSelectCombo = ((CComboBox*)m_vecControls[i].pContol)->GetCurSel();
			if(nSelectCombo < 0)
			{
				continue;
			}

			bComboCheck = TRUE;
		}	
	}

	// 값을 저장한다. 
	CString strKey;	
	CString strData;
	// Group타입(보유머니,게임모드,게임종류,입장제한,{베팅룰,시드머니}) / 컨트롤 타입(우선순위/라디오/콤보)

	// 1. 라디오 버튼
	{
		strKey.Format("%d_%d", m_eQuickJoinItemID, TYPE_CONTROL_RADIO);	

		if(bRadioCheck)
			strData.Format("%d", nRadioButCount);
		else
			strData.Format("%d", -1);
		NMBASE::UTIL::WriteProfile_string(pStrSection, strKey, strData);	
	}

	// 2. 콤보 버튼
	{
		strKey.Format("%d_%d", m_eQuickJoinItemID, TYPE_CONTROL_COMBO);	

		// 선택한 콤보 위치
		strData.Format("%d", nSelectCombo);
		NMBASE::UTIL::WriteProfile_string(pStrSection, strKey, strData);	

		if(bComboCheck)
		{
			if(m_eQuickJoinItemID == PID_QUICKJOIN_ITEM_SEEDMONEY)	// 베팅룰 /시드머니
			{
				DWORD_PTR dwItemData = m_pComboAutobetSeedMoney->GetItemData(nSelectCombo);
				// 2. 베팅룰 저장
				CString strTempKey;		

				// 3. 시드머니 저장					
				strTempKey.Format("%d_%d_1", m_eQuickJoinItemID, TYPE_CONTROL_COMBO);	// Group타입 / 컨트롤 타입(라디오/콤보/우선순위)
				strData.Format("%d", dwItemData);

				NMBASE::UTIL::WriteProfile_string(pStrSection, strTempKey, strData);	
			}
		}	
		else 
		{	
			strData.Format("%d", -1);
			NMBASE::UTIL::WriteProfile_string(pStrSection, strKey, strData);	
		}
	}	

	return TRUE;
}

BOOL CMyCQJCondition::LoadData(const char *pSection, BOOL bUseFlag)
{		
	if(m_pFirstRadioBtn == NULL)
	{
		return TRUE;
	}

	// 사용안함이면 
	if(!bUseFlag)
	{
		//데이타가 하나라도 읽기 실패하면 기본 버튼으로 세팅  
		m_pFirstRadioBtn->SetCheck(TRUE);

		return FALSE;
	}

	if(!_LoadData(pSection))
	{
		//데이타가 하나라도 읽기 실패하면 기본 버튼으로 세팅  
		m_pFirstRadioBtn->SetCheck(TRUE);

		return FALSE;
	}

	return TRUE;
}

// 데이터를 로드한다.
BOOL CMyCQJCondition::_LoadData(const char *pSection)
{
	int nPriorityCount = 0;	// 우선순위
	int nPrioritySelect= -1;	// 우선순위
	int nRadioButCount = 0;	// 라디오버튼
	int nComboPos      = 0; // 콤보박스

	CString strKey;
	CString strData; 

	// 값을 미리 얻어 온다. 
	// 1. 라디오 버튼
	strKey.Format("%d_%d", m_eQuickJoinItemID, TYPE_CONTROL_RADIO);		
	strData = NMBASE::UTIL::GetProfile_string(pSection, strKey, "__NULL__");

	if(strData.Compare("__NULL__")==0)
		nRadioButCount = -1;
	else 
		nRadioButCount = atoi(strData);

	// 2. 콤보박스
	strKey.Format("%d_%d", m_eQuickJoinItemID, TYPE_CONTROL_COMBO);		
	strData = NMBASE::UTIL::GetProfile_string(pSection, strKey, "__NULL__");

	if(strData.Compare("__NULL__")==0)
		nComboPos = -1;
	else 
		nComboPos = atoi(strData);


	int nPriorityStep = 0;
	int nRadioButStep = 0;

	for(int i = 0; i < static_cast<int>(m_vecControls.size()); ++i)
	{		
		if(m_vecControls[i].TypeControl == TYPE_CONTROL_RADIO)
		{	
			if(nRadioButCount == nRadioButStep)
			{
				((NMBASE::SKINGDI::CBmpRadioBtn*)m_vecControls[i].pContol)->SetCheck(TRUE);
			}				
			else
			{
				((NMBASE::SKINGDI::CBmpRadioBtn*)m_vecControls[i].pContol)->SetCheck(FALSE);
			}

			++nRadioButStep;
		}
		// 콤보 박스
		else if(m_vecControls[i].TypeControl == TYPE_CONTROL_COMBO)
		{
			if(nRadioButCount >= 1)
			{
				if(nComboPos < 0 || nComboPos >= ((CComboBox*)m_vecControls[i].pContol)->GetCount())
				{
					nComboPos = 0;
				}			

				((CComboBox*)m_vecControls[i].pContol)->SetCurSel(nComboPos);		
			}					
		}			
	}

	return TRUE;
}

CMyCQJCondition* CQuickJoinControls::GetQJCondition(PID_QUICKJOIN_ITEM eQJItemID)
{
	for(int i = 0; i < m_nQuickJoinItemCreateCount; ++i)
	{
		if(m_QuickJoinItemGroup[i].m_eQuickJoinItemID == eQJItemID)
		{
			return &m_QuickJoinItemGroup[i];
		}
	}

	return NULL;
}

void CQuickJoinControls::RefreshCondition()
{
	for( int i = 0; i < m_nQuickJoinItemCreateCount; ++i)
	{
		m_QuickJoinItemGroup[i].RefreshConditonButton();				

		if(m_QuickJoinItemGroup[i].ReloadCondition(m_eSelectGameID))
		{
			switch(m_QuickJoinItemGroup[i].GetQuickjoinItemType())
			{
			case PID_QUICKJOIN_ITEM_BETRULE:
				{
					if (DoesSeedMoneyDependsOnBetRule(m_eDialogType))
					{
						CMyCQJCondition *pCondition = GetQJCondition(PID_QUICKJOIN_ITEM_SEEDMONEY);
						pCondition->EnableControl(TRUE);
					}
				}
				break;
			}
		}
		else
		{
			// 시드머니가 설정안함으로 셋팅되면 변경되면
			switch(m_QuickJoinItemGroup[i].GetQuickjoinItemType())
			{
			case PID_QUICKJOIN_ITEM_BETRULE:
				{
					if (DoesSeedMoneyDependsOnBetRule(m_eDialogType))
					{
						m_QuickJoinItemGroup[i].EnableControl(TRUE);
						CMyCQJCondition *pCondition = GetQJCondition(PID_QUICKJOIN_ITEM_SEEDMONEY);
						pCondition->EnableControl(FALSE);

						pCondition->SetCheckOff();

						CComboBox *pCombo = pCondition->GetComboBox(0);
						pCombo->SetCurSel(0);
					}
				}
				break;
			case PID_QUICKJOIN_ITEM_SEEDMONEY:
				{
					CComboBox *pCombo = m_QuickJoinItemGroup[i].GetComboBox(0);
					pCombo->SetCurSel(0);
				}
				break;
			}
		}
	}
}

BOOL CQuickJoinControls::Create(CWnd *pParentWnd, QUICKJOINDLGTYPE eQJDlgType)
{
	// 총 조건 항목  갯수
	m_nQuickJoinItemCreateCount = 0;
	m_eDialogType = eQJDlgType;

	// 내맘대로 바로입장 설정 항목만큼 컨트롤을 생성 한다. ( 내부에서 컨트롤을 생성할때 좌표가 0이면 컨트롤을 생성 안 한다.
	for(int i = 0; i < PMAX_QUICKJOIN_ITEM; ++i)
	{		
		// 아이템에 해당하는 컨트롤을 생성한다.
		if(!m_QuickJoinItemGroup[m_nQuickJoinItemCreateCount].Create(m_eDialogType, (PID_QUICKJOIN_ITEM)i, pParentWnd, m_nQuickJoinItemCreateCount, m_i64SelectLImitMoney))
		{	
			continue;
		}

		m_QuickJoinItemGroup[m_nQuickJoinItemCreateCount].RefreshConditonButton();

		// 컨트롤이 생성이 된 만큰 증가 시킨다.
		++m_nQuickJoinItemCreateCount;
	}

	return TRUE;
}

void CQuickJoinControls::SetCheckOff()
{
	for(int i = 0; i < m_nQuickJoinItemCreateCount; ++i)
	{
		m_QuickJoinItemGroup[i].SetCheckOff();			
	}	
}

BOOL CQuickJoinControls::OnCommand(UINT CmdID)
{
	//버튼이 눌러졌을 때 
	BOOL bClickedAny = FALSE;

	if(CmdID == IDC_QUICKJOIN_RADIO)
	{
		//눌러진 라디오 버튼이 게임종류일 경우 게임에 따라 자동베팅룰, 시드머니종류를 다시 세팅해줘야한다.
		bClickedAny =TRUE;
	}

	RefreshCondition();

	return bClickedAny;
}

void CQuickJoinControls::ResetComboBox(IDX_GAME idxGame)
{
	//게임에 따라 콤보박스 (시드머니, 베팅룰)을 다시 세팅해준다. 
	CMyCQJCondition* pQJC = GetQJCondition(PID_QUICKJOIN_ITEM_SEEDMONEY);
	if(pQJC)
	{
		pQJC->SetCombo_AutobetSeedMoney(idxGame, pQJC->GetComboBox(0), m_i64SelectLImitMoney);
	}

	m_eSelectGameID = idxGame;
}

BOOL CQuickJoinControls::SaveData()
{
	// 데이터를 Config에 저장한다. 

	CString strSection;
	strSection.Format("QJ%02d_%s_%d", QJ_VER, QJ_MYID, m_eDialogType);	 // QJ_(Version)_(ID)_(GameCode)

	for(int i = 0; i < m_nQuickJoinItemCreateCount; ++i)
	{
		if(m_QuickJoinItemGroup[i].DoesExist() == FALSE)
		{
			continue;
		}
		m_QuickJoinItemGroup[i].SaveData(strSection);	
		m_QuickJoinItemGroup[i].ReloadCondition(m_eSelectGameID);
	}

	/*int datasize = GetQjDataSize();
	if(datasize>0)
	{
		BYTE *pDataBuf = new BYTE[datasize];		
		GetQjDataBinary(pDataBuf);

		CString strData = BinaryToString(pDataBuf, datasize);
		NMBASE::UTIL::WriteProfile_string(strSection, "QJDataBin", strData);			
		delete[] pDataBuf;
	}*/

	return TRUE;
}

BOOL CQuickJoinControls::LoadData(BOOL bUseFlag)
{
	CString strSection;
	strSection.Format("QJ%02d_%s_%d", QJ_VER, QJ_MYID, m_eDialogType);	// QJ_(Version)_(ID)_(GameCode)

	for(int i = 0; i < m_nQuickJoinItemCreateCount; ++i)
	{		
		m_QuickJoinItemGroup[i].LoadData(strSection, bUseFlag);			
	}		

	RefreshCondition();

	return TRUE;
}

BOOL CQuickJoinControls::EnableControl(BOOL bEnable)
{

	for(int i = 0; i < m_nQuickJoinItemCreateCount; ++i)
	{
		m_QuickJoinItemGroup[i].EnableControl(bEnable);			
	}

	return TRUE;
}

int CQuickJoinControls::GetQjDataSize()
{
	int cnt = 0;

	for(int i = 0; i < m_nQuickJoinItemCreateCount; ++i)
	{			
		++cnt;
	}

	return cnt * sizeof(QJData);
}

int CQuickJoinControls::GetQjDataBinary(BYTE *pBuffer)
{
	/// 우선순위 순으로 항목데이터를 차레 대로 pBuffer에 채운다. (우선순위 0 은 제외한다.)
	int cnt = 0;
	for(int i = 0; i < m_nQuickJoinItemCreateCount; ++i)
	{
		int buf_offset = cnt * sizeof(QJData);
		memcpy(pBuffer + buf_offset, &m_QuickJoinItemGroup[i].m_sQuickJoinData, sizeof(QJData));
		cnt++;
	}

	return cnt * sizeof(QJData);
}

CString CQuickJoinControls::BinaryToString(const BYTE *pBuffer, int nSize)
{
	CString strRet;
	char buf[3] = {0, };

	for(int i = 0; i < nSize; ++i)
	{
		wsprintf(buf, "%02x", pBuffer[i]);
		strRet += buf;
	}		

	return strRet;
}

BOOL CQuickJoinControls::SendQJData()
{
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// CQuickJoinDlg dialog

CQuickJoinDlg::CQuickJoinDlg(QUICKJOINDLGTYPE eDlgType, CUIInfoData* pUIInfoData, CWnd* pParent /*=NULL*/)
: CUPGDlgBase(CQuickJoinDlg::IDD, pParent),
m_eDialogType(eDlgType),
m_pUIInfoData(pUIInfoData)
{
	//{{AFX_DATA_INIT(CQuickJoinDlg)
	//}}AFX_DATA_INIT

	// 기본7Poker
	//m_eDialogType = EQJ_7POKER;
}


void CQuickJoinDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CQuickJoinDlg)
	DDX_Control(pDX, IDC_BUTTON_SEARCHON, m_RadioSearchOn);	// 검색기능 사용/사용안함
	DDX_Control(pDX, IDC_BUTTON_SEARCHOFF, m_RadioSearchOff);

	DDX_Control(pDX, IDC_BUTTON_MODE1, m_rdMode1);
	DDX_Control(pDX, IDC_BUTTON_MODE2, m_rdMode2);
	DDX_Control(pDX, IDC_BUTTON_MODE3, m_rdMode3);
	DDX_Control(pDX, IDC_BUTTON_MODE4, m_rdMode4);
	DDX_Control(pDX, IDC_BUTTON_MODE5, m_rdMode5);
	DDX_Control(pDX, IDC_BUTTON_MODE6, m_rdMode6);
	DDX_Control(pDX, IDC_BUTTON_MODE7, m_rdMode7);
	DDX_Control(pDX, IDC_BUTTON_MODE8, m_rdMode8);

	//규제안 관련하여 카드세팅 옵션 라디오버튼 추가
	DDX_Control(pDX, IDC_BUTTON_MODE9, m_rdMode9);
	DDX_Control(pDX, IDC_BUTTON_MODE10, m_rdMode10);

	DDX_Control(pDX, IDC_COMBO_ROOMTYPE, m_cbRoomType);
	DDX_Control(pDX, IDC_COMBO_CARDSETING, m_cbCardSeting); //규제안 관련 하여 카드세팅 추가 2014.01
	

	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_BtnApply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuickJoinDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CQuickJoinDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHON, OnButtonOn)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHOFF, OnButtonOff)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
//	ON_CBN_SELCHANGE(IDC_COMBO_CARDSETING, &CQuickJoinDlg::OnCbnSelchangeComboCardseting)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuickJoinDlg message handlers


//QUICKJOINDLGTYPE CQuickJoinDlg::GetQJDlgIndex() 
//{
//	//현재 그룹에 맞는 다이얼로그 종류를 찾는다.	
//	//IDX_GAME idxGame =g_GetGameFromGroup(g_NowGroupNo);
//	/*IDX_GAME idxGame = IDX_GAME_SP;
//	
//	if(idxGame == IDX_GAME_NULL)
//	{		
//		return  QJ_IDX_DLG_CM;
//	}
//	else
//	{
//		return  (QUICKJOINDLGTYPE)idxGame;		
//	}*/
//
//	return m_eDialogType;	
//}


BOOL CQuickJoinDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();

	string RADIOON_PATH		= "./PokerCommon/image/QuickJoin/radio02.bmp";		// ON
	string APPLY_PATH		= "./PokerCommon/image/QuickJoin/Apply.bmp";		// 설정
	string CANCEL_PATH		= "./PokerCommon/image/QuickJoin/cancel.bmp";		// 취소


	// 배경이미지
	const char* pBGName[EQJ_MAXDLG] = {"7poker.bmp", "lowbd.bmp", "newpoker.bmp", "sutda.bmp", "hoola.bmp"};
	CString strPathBG;
	strPathBG.Format("./PokerCommon/image/QuickJoin/%s", pBGName[m_eDialogType]);

	if(!m_cBackImg.LoadBitmapFile(strPathBG))
	{
		return FALSE;
	}

	m_cRgn.SetRgn( (HBITMAP)m_cBackImg.GetSafeHandle(), this, RGB( 255, 0, 255 ), &m_cBackImg);
	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, (HBITMAP)m_cBackImg.GetSafeHandle());

	strPathBG.Format("./PokerCommon/image/QuickJoin/gamescale.bmp");
	if(!m_cGameScaleImage.LoadBitmapFile(strPathBG))
	{
		return FALSE;
	}

	// 검색기능 사용 라디오 버튼
	m_RadioSearchOn.Init(m_pUIInfoData->m_ptQuickJoinDlgSearchON.x, m_pUIInfoData->m_ptQuickJoinDlgSearchON.y,  const_cast<LPSTR>(RADIOON_PATH.c_str()), NULL, this);
	m_RadioSearchOff.Init(m_pUIInfoData->m_ptQuickJoinDlgSearchOFF.x, m_pUIInfoData->m_ptQuickJoinDlgSearchOFF.y,  const_cast<LPSTR>(RADIOON_PATH.c_str()), NULL, this);
	m_RadioSearchOn.AddToGroup(&m_RadioSearchOff);

	//// 검색기능사용 사용여부를 체크한다.
	//if(IsEnableQJ(m_eDialogType))
	//{
	//	m_RadioSearchOn.SetCheck(TRUE);
	//	m_bUseGameMode = TRUE;
	//}
	//else
	//{
	//	m_RadioSearchOff.SetCheck(TRUE);
	//}

	CString strSection;
	strSection.Format("QJ%02d_%s_%d", QJ_VER, GM().GetMyInfo()->UI.ID, m_eDialogType);
	
	LoadData(strSection, &m_bUseGameMode, &m_nGameMode, &m_nRoomType, &m_nMaxUser, &m_nComboSel, &m_bUseCardSeting , &m_nCardSet, &m_nBetMoneyMulti) ;

	if(m_bUseGameMode)
	{
		m_RadioSearchOn.SetCheck(TRUE);
	}
	else
	{
		m_RadioSearchOff.SetCheck(TRUE);
	}

	// 적용
	m_BtnApply.Init(m_pUIInfoData->m_listQuickJoinDlgApplayBut[m_eDialogType].ptButton[0].x, m_pUIInfoData->m_listQuickJoinDlgApplayBut[m_eDialogType].ptButton[0].y, APPLY_PATH.c_str(), 4);
	// 취소	
	m_BtnCancel.Init(m_pUIInfoData->m_listQuickJoinDlgApplayBut[m_eDialogType].ptButton[1].x, m_pUIInfoData->m_listQuickJoinDlgApplayBut[m_eDialogType].ptButton[1].y, CANCEL_PATH.c_str(), 4);

	CRect rt = CRect(0,0,105,100);
	m_nGame = GM().GetCurrentGameType() ;

	m_cbRoomType.EnableWindow(FALSE) ;


	//규제안
	m_cbRoomType.AddString("기본방");
	m_cbRoomType.SetItemData(0, 0);
	m_cbRoomType.SetCurSel(0);

	//규제안 : 기본구간으로 설정
	/*
	m_cbRoomType.AddString("큰방");
	m_cbRoomType.SetItemData(0, 0);

	m_cbRoomType.AddString("기본방");
	m_cbRoomType.SetItemData(1, 1);

	m_cbRoomType.AddString("작은방");
	m_cbRoomType.SetItemData(2, 2);

	m_cbRoomType.SetCurSel(m_nComboSel);
	*/

	m_ptRoomType = CPoint(25,153) ;

	//규제안 관련해서 카드세팅 콤보박스추가 2014.01
	m_cbCardSeting.EnableWindow(TRUE);

	/*
		7포커, 하이로우는 llSeedMoney 1조 이상인 경우 기본&5장, 1조 미만인 경우 기본&6장만 표시
		뉴포커는 기본&6장만 표시(자유방 무시)
	*/
	
	CDataRoomOptionList *pDataRoomOptionList = GM().GetDataRoomOptionList();
	if(pDataRoomOptionList == NULL) 
		return TRUE;

	ROOM_CREATEOPTION *pRoomCreationOption = NULL;
	pRoomCreationOption = pDataRoomOptionList->GetRoomCreationOption();
	if(pRoomCreationOption == NULL) 
		return FALSE;

	ROOMOPTION *pDefaultRoomOption = pDataRoomOptionList->GetDefaultRoomOption();
	if(pDefaultRoomOption == NULL) 
		return TRUE;

	if (m_nGame != IDX_GAME_HA && m_nGame !=  IDX_GAME_SD)
	{
		if (m_nGame == IDX_GAME_NP){
			m_cbCardSeting.AddString("수동");
			m_cbCardSeting.SetItemData(0, 5);
		}else{
			m_cbCardSeting.AddString("수동");
			m_cbCardSeting.SetItemData(0, 4);
		}

		//m_cbCardSeting.AddString(GetCardSettingName(m_nGame, pDefaultRoomOption->cCardSetting ));
		//m_cbCardSeting.SetItemData(1, pDefaultRoomOption->cCardSetting);

		// 상 하위방의 설정가능 값이 다름.
		if(pDefaultRoomOption->llEnterLimitMoney < 1000000000000){

			m_cbCardSeting.AddString("6구");
			m_cbCardSeting.SetItemData(1, 6);
		}
		else
		{
			if (m_nGame == IDX_GAME_NP){
				m_cbCardSeting.AddString("6구");
				m_cbCardSeting.SetItemData(1, 6);
			}else{
				m_cbCardSeting.AddString("5구");
				m_cbCardSeting.SetItemData(1, 5);
			}		
		}
	}

	//콤보박스 선택
	if (GM().GetCurrentGameType() == IDX_GAME_SD)
	{
	} 
	else if(GM().GetCurrentGameType() == IDX_GAME_NP)
	{
		// 4는 0번 아이템, 기본(4)보다 크면(5,6)은 1번 아이템
		if (m_nCardSet > 5)
		{
			m_nCardSet = 1; 
		} 
		else 
		{
			m_nCardSet = 0;
		}
		m_cbCardSeting.SetCurSel(m_nCardSet);
	}
	else 
	{
		// 4는 0번 아이템, 기본(4)보다 크면(5,6)은 1번 아이템
		if (m_nCardSet > 4)
		{
			m_nCardSet = 1; 
		} 
		else 
		{
			m_nCardSet = 0;
		}
		m_cbCardSeting.SetCurSel(m_nCardSet);
	}
	
	CRect rtForCardSeting; //카드세팅 위치 설정 Rect

	CRect rtApplyBtn; //적용버튼 위치
	CRect rtCancelBtn; //취소버튼 위치

	//게임마다의 위치 조정 
	switch(m_nGame)
	{	//라디오 버튼 위치 지정
	case IDX_GAME_HA:
		m_rdMode1.ShowWindow(SW_SHOW);
		m_rdMode2.ShowWindow(SW_SHOW);
		m_rdMode3.ShowWindow(SW_SHOW);
		m_rdMode4.ShowWindow(SW_SHOW);
		m_rdMode5.ShowWindow(SW_SHOW);
		m_rdMode6.ShowWindow(SW_SHOW);
		m_rdMode7.ShowWindow(SW_SHOW);
		m_rdMode8.ShowWindow(SW_SHOW);

		m_rdMode1.Init(36,179,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode2.Init(126,179,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode3.Init(216,179,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode1.AddToGroup(&m_rdMode2);
		m_rdMode1.AddToGroup(&m_rdMode3);

		m_rdMode4.Init(36,234,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode5.Init(126,234,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode4.AddToGroup(&m_rdMode5);

		m_rdMode6.Init(36,124,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode7.Init(126,124,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode8.Init(216,124,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode6.AddToGroup(&m_rdMode7);
		m_rdMode6.AddToGroup(&m_rdMode8);

		rt.OffsetRect(185,232);
		m_ptRoomType = CPoint(25,206) ;

		if(m_nGameMode == 0 )
			m_rdMode1.SetCheck(TRUE);
		else if(m_nGameMode == 1 )
			m_rdMode2.SetCheck(TRUE);
		else if(m_nGameMode == 2 )
			m_rdMode3.SetCheck(TRUE);

		if(m_nRoomType == 0 )
			m_rdMode4.SetCheck(TRUE);
		else if(m_nRoomType == 1 ) 
		{
			m_rdMode5.SetCheck(TRUE);
			m_cbRoomType.EnableWindow(TRUE) ;
		}

		if(m_nMaxUser == 0 )
			m_rdMode6.SetCheck(TRUE);
		else if(m_nMaxUser == 1 )
			m_rdMode7.SetCheck(TRUE);
		else if(m_nMaxUser == 2 )
			m_rdMode8.SetCheck(TRUE);

		//적용/취소버튼 위치
		rtApplyBtn.SetRect(0,0,80,31);	//크기
		rtApplyBtn.OffsetRect(80,330);	

		rtCancelBtn.SetRect(0,0,80,31);
		rtCancelBtn.OffsetRect(171, 330);


		m_cbCardSeting.ShowWindow(SW_HIDE);
		m_cbCardSeting.EnableWindow(FALSE);

		break;

	case IDX_GAME_SP:
	case IDX_GAME_HL:
	case IDX_GAME_NP:
		m_rdMode1.ShowWindow(SW_SHOW);
		m_rdMode2.ShowWindow(SW_SHOW);
		m_rdMode3.ShowWindow(SW_SHOW);
		m_rdMode4.ShowWindow(SW_SHOW);
		m_rdMode5.ShowWindow(SW_SHOW);
		m_rdMode6.ShowWindow(SW_HIDE);
		m_rdMode7.ShowWindow(SW_HIDE);
		m_rdMode8.ShowWindow(SW_HIDE);

		m_rdMode1.Init(36,124,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode2.Init(126,124,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode3.Init(216,124,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode1.AddToGroup(&m_rdMode2);
		m_rdMode1.AddToGroup(&m_rdMode3);

		m_rdMode4.Init(36,179,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode5.Init(126,179,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode4.AddToGroup(&m_rdMode5);

		m_rdMode6.Init(184,91,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode7.Init(184,91,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode8.Init(184,91,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode6.AddToGroup(&m_rdMode7);
		m_rdMode6.AddToGroup(&m_rdMode8);

		//규제안 라디오 버튼 추가 (카드세팅)
		m_rdMode9.ShowWindow(SW_SHOW);
		m_rdMode10.ShowWindow(SW_SHOW);
		m_rdMode9.Init(36,284,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode10.Init(126,284,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode10.AddToGroup(&m_rdMode9);

		rt.OffsetRect(185,177);

		if(m_nGameMode == 0 )  
			m_rdMode1.SetCheck(TRUE);
		else if(m_nGameMode == 1 )
			m_rdMode2.SetCheck(TRUE);
		else if(m_nGameMode == 2 )
			m_rdMode3.SetCheck(TRUE);

		if(m_nRoomType == 0 )
			m_rdMode4.SetCheck(TRUE);
		else if(m_nRoomType == 1 )
		{
			m_rdMode5.SetCheck(TRUE);
			m_cbRoomType.EnableWindow(TRUE) ;
		}

		m_cbCardSeting.ShowWindow(SW_SHOW);

		//규제안 카드세팅 설정값
		if (m_bUseCardSeting == false)
		{
			m_rdMode9.SetCheck(TRUE);
			m_cbCardSeting.EnableWindow(FALSE);
		}else{
			m_rdMode10.SetCheck(TRUE);
			m_cbCardSeting.EnableWindow(TRUE);
		}

		//규제안 관련하여 추가된 카드세팅 위치 설정 
		rtForCardSeting.SetRect(0,0,102,17);	//크기
		rtForCardSeting.OffsetRect(185,282);	//위치
		

		//적용/취소버튼 위치
		rtApplyBtn.SetRect(0,0,80,31);	//크기
		rtApplyBtn.OffsetRect(80,379);	

		rtCancelBtn.SetRect(0,0,80,31);
		rtCancelBtn.OffsetRect(171, 379);
		break;
	case IDX_GAME_SD:
		m_rdMode1.ShowWindow(SW_SHOW);
		m_rdMode2.ShowWindow(SW_SHOW);
		m_rdMode3.ShowWindow(SW_SHOW);
		m_rdMode4.ShowWindow(SW_SHOW);
		m_rdMode5.ShowWindow(SW_SHOW);
		m_rdMode6.ShowWindow(SW_HIDE);
		m_rdMode7.ShowWindow(SW_HIDE);
		m_rdMode8.ShowWindow(SW_HIDE);

		m_rdMode1.Init(36,124,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode2.Init(126,124,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode3.Init(216,124,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode1.AddToGroup(&m_rdMode2);
		m_rdMode1.AddToGroup(&m_rdMode3);

		m_rdMode4.Init(36,179,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode5.Init(126,179,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode4.AddToGroup(&m_rdMode5);

		m_rdMode6.Init(184,91,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode7.Init(184,91,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode8.Init(184,91,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode6.AddToGroup(&m_rdMode7);
		m_rdMode6.AddToGroup(&m_rdMode8);

		rt.OffsetRect(185,177);
		m_ptRoomType = CPoint(25,150) ;

		if(m_nGameMode == 0 )
			m_rdMode1.SetCheck(TRUE);
		else if(m_nGameMode == 1 )
			m_rdMode2.SetCheck(TRUE);
		else if(m_nGameMode == 2 )
			m_rdMode3.SetCheck(TRUE);

		if(m_nRoomType == 0 )
			m_rdMode4.SetCheck(TRUE);
		else if(m_nRoomType == 1 )
		{
			m_rdMode5.SetCheck(TRUE);
			m_cbRoomType.EnableWindow(TRUE) ;
		}

		// 참가비 영역 모두 제거.
		//규제안 라디오 버튼 추가 (참가비)
		m_rdMode9.ShowWindow(SW_HIDE);
		m_rdMode10.ShowWindow(SW_HIDE);
		m_rdMode9.Init(36,284,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode10.Init(126,284,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode10.AddToGroup(&m_rdMode9);

		m_cbCardSeting.ShowWindow(SW_HIDE);
		//규제안 참가비 설정값
		if (m_bUseCardSeting == false)
		{
			m_rdMode9.SetCheck(TRUE);
			m_cbCardSeting.EnableWindow(FALSE);
		}else{
			m_rdMode10.SetCheck(TRUE);
			m_cbCardSeting.EnableWindow(TRUE);
		}

		//규제안 : 참가비 위치 설정
		rtForCardSeting.SetRect(0,0,102,17);	//크기
		rtForCardSeting.OffsetRect(185, 282);

		//적용/취소버튼 위치
		rtApplyBtn.SetRect(0,0,80,31);	//크기
		rtApplyBtn.OffsetRect(80,274);	

		rtCancelBtn.SetRect(0,0,80,31);
		rtCancelBtn.OffsetRect(171, 274);
		break;
	case IDX_GAME_BD:
		m_rdMode1.ShowWindow(SW_HIDE);
		m_rdMode2.ShowWindow(SW_HIDE);
		m_rdMode3.ShowWindow(SW_HIDE);
		m_rdMode4.ShowWindow(SW_SHOW);
		m_rdMode5.ShowWindow(SW_SHOW);
		m_rdMode6.ShowWindow(SW_HIDE);
		m_rdMode7.ShowWindow(SW_HIDE);
		m_rdMode8.ShowWindow(SW_HIDE);

		m_rdMode1.Init(36,124,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode2.Init(126,124,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode3.Init(216,124,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode1.AddToGroup(&m_rdMode2);
		m_rdMode1.AddToGroup(&m_rdMode3);

		m_rdMode4.Init(36,124,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode5.Init(126,124,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode4.AddToGroup(&m_rdMode5);

		m_rdMode6.Init(184,91,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode7.Init(184,91,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode8.Init(184,91,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
		m_rdMode6.AddToGroup(&m_rdMode7);
		m_rdMode6.AddToGroup(&m_rdMode8);

		rt.OffsetRect(185,122);
		m_ptRoomType = CPoint(25,98) ;

		if(m_nGameMode == 0 )
			m_rdMode1.SetCheck(TRUE);
		else if(m_nGameMode == 1 )
			m_rdMode2.SetCheck(TRUE);
		else if(m_nGameMode == 2 )
			m_rdMode3.SetCheck(TRUE);

		if(m_nRoomType == 0 )
			m_rdMode4.SetCheck(TRUE);
		else if(m_nRoomType == 1 )
		{
			m_rdMode5.SetCheck(TRUE);
			m_cbRoomType.EnableWindow(TRUE) ;
		}

		//규제안 : 카드세팅 위치 설정
		rtForCardSeting.OffsetRect(185, 228);

		//적용/취소버튼 위치
		rtApplyBtn.SetRect(0,0,80,31);	//크기
		rtApplyBtn.OffsetRect(80,219);	

		rtCancelBtn.SetRect(0,0,80,31);
		rtCancelBtn.OffsetRect(171, 219);

		m_cbCardSeting.ShowWindow(SW_HIDE);
		m_cbCardSeting.EnableWindow(FALSE);

		break;
	}

	//게임 규모 콤보박스 위치 설정
	m_cbRoomType.MoveWindow(rt);
	/*m_cbRoomType.ResetContent();*/

	//규제안 : 카드세팅 콤보위치 설정
	m_cbCardSeting.MoveWindow(rtForCardSeting);

	//확인 취소 버튼 위치 설정
	m_BtnApply.MoveWindow(rtApplyBtn);
	m_BtnCancel.MoveWindow(rtCancelBtn);

	////// 나머지컨트롤들을 생성한다.( 보유머니, 게임종류, 게임모드, 베팅룰, 시드머니 )
	//if(!m_cQuickJoinControls.Create(this, m_eDialogType))
	//{
	//	return FALSE;
	//}	

	//// ini파일의 값을 얻어 Dialog를 세팅한다.
	//m_cQuickJoinControls.LoadData(bUseFlag);
	CenterWindow();

	UpdateData(FALSE);

	return TRUE; 
}

void CQuickJoinDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));

	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

void CQuickJoinDlg::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();

	if(m_cBackImg.m_hObject) 
	{
		m_cBackImg.DeleteObject();
	}

	if(m_cGameScaleImage.m_hObject)
	{
		m_cGameScaleImage.DeleteObject();
	}
}

void CQuickJoinDlg::OnPaint() 
{
	CPaintDC dc(this);

	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) 
	{
		return;
	}

	CDC sdc;
	sdc.CreateCompatibleDC(&dc);
	if(sdc.m_hDC == NULL) 
	{
		return;
	}

	CBitmap* pOldBmp = cdc.SelectObject(&m_cBackImg);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();	

	//if(m_bShowRoomType)
	//{
	//	CBitmap* pOldBmp = sdc.SelectObject(&m_cGameScaleImage);

	//	dc.BitBlt(m_ptRoomType.x, m_ptRoomType.y, m_ptRoomType.x+m_cGameScaleImage.GetWidth(), m_ptRoomType.y+m_cGameScaleImage.GetHeight(), &sdc, 0, 0, SRCCOPY);

	//	sdc.SelectObject(pOldBmp);
	//	sdc.DeleteDC();	
	//}

	//if ( GM().GetCurrentGameType() == IDX_GAME_HA )
	//{
	//	CBitmap* pOldBmp = sdc.SelectObject(&m_cGameScaleImage);

	//	dc.BitBlt(ptGameScale[m_eDialogType].x, ptGameScale[m_eDialogType].y, ptGameScale[m_eDialogType].x+m_cGameScaleImage.GetWidth(), ptGameScale[m_eDialogType].y+m_cGameScaleImage.GetHeight(), &sdc, 0, 0, SRCCOPY);

	//	sdc.SelectObject(pOldBmp);
	//	sdc.DeleteDC();	
	//}
	//else if( DoesSeedMoneyDependsOnBetRule(m_eDialogType)  && m_cQuickJoinControls.GetQJCondition(PID_QUICKJOIN_ITEM_BETRULE)->GetCurSelRadioButton())
	//{
	//	CBitmap* pOldBmp = sdc.SelectObject(&m_cGameScaleImage);

	//	dc.BitBlt(ptGameScale[m_eDialogType].x, ptGameScale[m_eDialogType].y, ptGameScale[m_eDialogType].x+m_cGameScaleImage.GetWidth(), ptGameScale[m_eDialogType].y+m_cGameScaleImage.GetHeight(), &sdc, 0, 0, SRCCOPY);

	//	sdc.SelectObject(pOldBmp);
	//	sdc.DeleteDC();	
	//}

	CRect rtRect = rtGameScaleText;
	rtRect.OffsetRect(m_ptRoomType.x, m_ptRoomType.y);
	
	COLORREF clrOld = dc.SetTextColor(RGB(112, 112, 112));
	CFont *pOldFont = (CFont *)dc.SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
	int bkOldMode = dc.SetBkMode(TRANSPARENT);

	//CComboBox *pCombo = m_cQuickJoinControls.GetQJCondition(PID_QUICKJOIN_ITEM_SEEDMONEY)->GetComboBox(0);
	//CString strOutput;
	//if(pCombo)
	//{
	//	int nSel = pCombo->GetCurSel();
	//	switch(nSel)
	//	{
	//	case 0:
	//		strOutput = "입장 가능한 방 중 최상위 시드머니방으로 입장합니다.";
	//		break;
	//	case 1:
	//		strOutput = "입장 가능한 방 중 최상위, 최하위 시드머니를 제외한 기본방으로 입장입니다.";
	//		break;
	//	case 2:
	//		strOutput = "입장 가능한 방 중 최하위 시드머니방으로 입장합니다.";
	//		break;
	//	}
	//}

	//if(!strOutput.IsEmpty())
	//	dc.DrawText(strOutput, &rtRect, DT_WORDBREAK | DT_CENTER | DT_VCENTER);


	CString strOutput;
	if(m_cbRoomType)
	{
		int nSel = m_cbRoomType.GetCurSel();
		switch(nSel)
		{
		case 0:
			strOutput = "입장 가능한 방 중 최상위 시드머니방으로 입장합니다.";
			break;
		case 1:
			strOutput = "입장 가능한 방 중 최상위, 최하위 시드머니를 제외한 기본방으로 입장입니다.";
			break;
		case 2:
			strOutput = "입장 가능한 방 중 최하위 시드머니방으로 입장합니다.";
			break;
		}
	}

	if(!strOutput.IsEmpty())
		dc.DrawText(strOutput, &rtRect, DT_WORDBREAK | DT_CENTER | DT_VCENTER);

	

	dc.SetTextColor(clrOld);
	dc.SelectObject(pOldFont);
	dc.SetBkMode(bkOldMode);
}

BOOL CQuickJoinDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{		
	// 버튼이 눌러졌다면  ON시켜준다.
	if(wParam != IDC_BUTTON_APPLY && wParam != IDCANCEL)
	{
		m_RadioSearchOn.SetCheck(TRUE);
		m_RadioSearchOff.SetCheck(FALSE);

		m_bUseGameMode = true ;
	}

	switch(wParam)
	{
	case IDC_BUTTON_SEARCHOFF:
		{
			m_RadioSearchOn.SetCheck(FALSE);
			m_RadioSearchOff.SetCheck(TRUE);

			m_rdMode1.SetCheck(TRUE);
			m_rdMode2.SetCheck(FALSE);
			m_rdMode3.SetCheck(FALSE);
			m_rdMode4.SetCheck(TRUE);
			m_rdMode5.SetCheck(FALSE);
			m_rdMode6.SetCheck(TRUE);
			m_rdMode7.SetCheck(FALSE);
			m_rdMode8.SetCheck(FALSE);
			m_rdMode9.SetCheck(TRUE);
			m_rdMode10.SetCheck(FALSE);

			m_cbRoomType.EnableWindow(FALSE);

			m_bUseGameMode = false ;

			m_nGameMode = 0 ;
			m_nRoomType = 0 ;
			m_nMaxUser = 0 ;

			//규제안 : 설정 초기화
			m_bUseCardSeting = false ;
			m_cbCardSeting.EnableWindow(FALSE);
		}
		break ;
	case IDC_BUTTON_MODE1:

		m_nGameMode = 0 ;
		break;
	case IDC_BUTTON_MODE2:

		m_nGameMode = 1 ;
		break;
	case IDC_BUTTON_MODE3:

		m_nGameMode = 2 ;
		break;
	case IDC_BUTTON_MODE4:
		m_cbRoomType.EnableWindow(FALSE) ;
		m_nRoomType = 0 ;
		break;
	case IDC_BUTTON_MODE5:
		m_cbRoomType.EnableWindow(TRUE) ;
		m_nRoomType = 1 ;
		break;
	case IDC_BUTTON_MODE6:
		m_nMaxUser = 0 ;
		break;
	case IDC_BUTTON_MODE7:
		m_nMaxUser = 1 ;
		break;
	case IDC_BUTTON_MODE8:
		m_nMaxUser = 2 ;
		break;

		//규제안 카드세팅 버튼
	case IDC_BUTTON_MODE9:
		m_bUseCardSeting = false ;
		m_cbCardSeting.EnableWindow(FALSE);
		break;
	case IDC_BUTTON_MODE10:
		m_bUseCardSeting = true ;
		m_cbCardSeting.EnableWindow(TRUE);
		break;

	case IDC_COMBO_ROOMTYPE:
		{
		}
		break ;
	}
	Invalidate(FALSE);

	return CUPGDlgBase::OnCommand(wParam, lParam);
}

// Profile만 저장한다.
void CQuickJoinDlg::OnButtonApply() 
{
	#ifdef UPGRADE_10210619_ADD_CLICKLOG
		SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_ENTERBTN_AUTOENTER_OPTION_SET);
	#endif //UPGRADE_10210619_ADD_CLICKLOG

	//m_cQuickJoinControls.SaveData();

	BOOL bEnable = m_RadioSearchOn.GetCheck();

	CString strSection;
	strSection.Format("QJ%02d_%s_%d", QJ_VER, GM().GetMyInfo()->UI.ID, m_eDialogType);

	SaveData(strSection) ;

	/*if(m_eDialogType!=QJ_IDX_DLG_CM)
	{
	if(m_cQuickJoinControls.IsAllOff())
	{
	bEnable = FALSE;
	}
	}*/

	//CString strName;
	//strName.Format("bQJEnable%d", m_eDialogType);
	//NMBASE::UTIL::WriteProfile_int(QJ_MYID, strName, bEnable);

	//NMBASE::UTIL::WriteProfile_int(QJ_MYID, "QJFavoriteGame", GetSelectedGame());

	CUPGDlgBase::OnCancel();
}

// 취소
void CQuickJoinDlg::OnCancel() 
{
	#ifdef UPGRADE_10210619_ADD_CLICKLOG
		SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_ENTERBTN_AUTOENTER_OPTION_CANCEL);
	#endif //UPGRADE_10210619_ADD_CLICKLOG

	CUPGDlgBase::OnCancel();
}

bool CQuickJoinDlg::IsEnableQJ(QUICKJOINDLGTYPE eType)
{
	CString strName;
	strName.Format("bQJEnable%d", eType);

	return NMBASE::UTIL::GetProfile_int(QJ_MYID, strName, 0) > 0 ? true:false;
}

void CQuickJoinDlg::OnButtonOff() 
{
	#ifdef UPGRADE_10210619_ADD_CLICKLOG
		SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_ENTERBTN_AUTOENTER_OPTION_SEARCH_DISABLE);
	#endif //UPGRADE_10210619_ADD_CLICKLOG

	//m_cQuickJoinControls.SetCheckOff();
}


void CQuickJoinDlg::OnButtonOn() 
{
	#ifdef UPGRADE_10210619_ADD_CLICKLOG
		SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_ENTERBTN_AUTOENTER_OPTION_SEARCH_ENABLE);
	#endif //UPGRADE_10210619_ADD_CLICKLOG
	
	//m_cQuickJoinControls.SetCheckOff();
}

//IDX_GAME CQuickJoinDlg::GetSelectGame()
//{
//	//현재 조인한 그룹에 맞는 게임을 반환
//	//IDX_GAME idxGame =g_GetGameFromGroup(g_NowGroupNo);
//	IDX_GAME idxGame = IDX_GAME_SP;
//
//	// 이부분을 고민해봐야 한다.
//	//if(idxGame == IDX_GAME_NULL) //도신채널이라면 저장된 게임을 가져온다.
//	//{
//		idxGame = (IDX_GAME) NMBASE::UTIL::GetProfile_int(QJ_MYID, "QJSelectGame", IDX_GAME_SP);
//	//}
//
//	return idxGame;	
//}


IDX_GAME CQuickJoinDlg::GetSelectedGame()
{
	// 	if(m_Radio_SP.GetCheck()==TRUE)
	// 		return IDX_GAME_SP;
	// 	if(m_Radio_HL.GetCheck()==TRUE)
	// 		return IDX_GAME_HL;


	return IDX_GAME_SP;
}


BOOL CQuickJoinDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//OnPaint();
	return TRUE;
}


BOOL CQuickJoinDlg::StringToBinary(CString &str, BYTE *pBuffer, int nSize)
{
	int len = str.GetLength();

	if(len%2 != 0)
	{
		return FALSE;
	}

	for(int i = 0, cnt = 0; i < (len/2) ; ++i)
	{
		if(cnt>nSize) 
		{
			return FALSE;
		}

		int tempBYTE = 0;
		int nRetrn   = sscanf(str.Mid(i*2, 2), "%02x", &tempBYTE);
		pBuffer[cnt++] = (BYTE)tempBYTE;
	}

	return TRUE;
}


// 바로입장에 관련된 데이터를 추출한다.
BOOL CQuickJoinDlg::GetSearchCondition(QUICKJOINDLGTYPE eType, CString StrSection, SCONDITION* pInfo)
{
	//SGroupInfo sGroupInfo;
	//_LoadData(StrSection.GetBuffer(), 0, sGroupInfo);
	//int nType;

	//for(int i = 0; i < PMAX_QUICKJOIN_ITEM; ++i)
	//{
	//	_LoadData(StrSection.GetBuffer(), i, sGroupInfo);
	//	nType = i;

	//	switch(nType)
	//	{
	//	case PID_QUICKJOIN_ITEM_GAMEMODE:
	//		{				
	//			pInfo->m_bUseGameMode = sGroupInfo.m_bUsed;			// 게임모드 검색 사용여부

	//			if(EQJ_7POKER == eType)
	//			{
	//				int nPos = sGroupInfo.m_nRadioButPos - 1;
	//				if(nPos == 0)
	//				{
	//					pInfo->m_nGameMode = FORMKIND_CHOICE;	// 0:노멀, 1:초이스 ( 이미지하고 프로그램하고 처리하는 순서가 틀리다.)
	//				}
	//				else
	//				{
	//					pInfo->m_nGameMode = FORMKIND_NORMAL;	// 0:노멀, 1:초이스
	//				}
	//			}
	//			else if(EQJ_NEWPOKER == eType)
	//			{
	//				int nPos = sGroupInfo.m_nRadioButPos - 1;
	//				if(nPos == 0)
	//				{
	//					pInfo->m_nGameMode = FORMKIND_BOMB;	// 2:폭탄, 3: 조커 ( 이미지하고 프로그램하고 처리하는 순서가 틀리다.)
	//				}
	//				else
	//				{
	//					pInfo->m_nGameMode = FORMKIND_JOKER;	// 2:폭탄, 3: 조커
	//				}
	//			}
	//			else if(EQJ_SUTDA == eType)
	//			{
	//				int nPos = sGroupInfo.m_nRadioButPos - 1;
	//				if(nPos == 0)
	//				{
	//					pInfo->m_nGameMode = SD_2CARD;	//2장섯다
	//				}
	//				else
	//				{
	//					pInfo->m_nGameMode = SD_3CARD;	//3장섯다
	//				}
	//			}
	//			else if (EQJ_HOOLA == eType)
	//			{
	//				int nPos = sGroupInfo.m_nRadioButPos - 1;
	//				if(nPos == 0)
	//				{
	//					pInfo->m_nGameMode = 2;	//2인
	//				}
	//				else
	//				{
	//					pInfo->m_nGameMode = 5;	//5인
	//				}
	//			}

	//		}
	//		break;
	//	case PID_QUICKJOIN_ITEM_BETRULE:
	//		{
	//			pInfo->m_bUseBetRule = sGroupInfo.m_bUsed;								// 베팅룰 사용여부를 지정
	//			pInfo->m_bAutoBet = (sGroupInfo.m_nRadioButPos == 2)?true:false;		// 자동인지의 여부
	//			pInfo->m_sBetRuleKind = BETRULE_KIND_REAL;								//리미트머니
	//		}
	//		break;
	//	case PID_QUICKJOIN_ITEM_SEEDMONEY:
	//		{
	//			pInfo->m_bUseSeedMoney = sGroupInfo.m_bUsed;			// 게임규모 사용여부
	//			pInfo->m_nSeedType = sGroupInfo.m_nComboValue;			// 게임규모
	//		}
	//		break;
	//	case PID_QUICKJOIN_ITEM_CONDITION:
	//		{
	//			pInfo->m_bAutoEnterRoom = sGroupInfo.m_bUsed?false:true;			// 자동 입장 여부
	//		}
	//		break;
	//	}
	//}

	////모든 항목이 사용 안함이면 자동으로 입장한다.
	//if(!pInfo->m_bUseBetRule && !pInfo->m_bUseGameMode && !pInfo->m_bUseSeedMoney) return FALSE;

	//return TRUE;

	CString strSection;
	strSection.Format("QJ%02d_%s_%d", QJ_VER, GM().GetMyInfo()->UI.ID, eType);

	int	nMaxUser ;
	int	nRoomType ;
	int	nGameMode ;
	int	nComboSel ;
	int nCardSeting;
	float nBetMoneyMulti;

	//카드세팅, 참가비 사용여부
	bool bUseCardSeting;

	bool bUseGameMode;			// 게임모드 사용 여부

	LoadData(strSection, &bUseGameMode, &nGameMode, &nRoomType, &nMaxUser, &nComboSel , &bUseCardSeting, &nCardSeting , &nBetMoneyMulti) ;

	if(nGameMode == 0)
		pInfo->m_bUseGameMode = false;			// 게임모드 사용 여부
	else
		pInfo->m_bUseGameMode = true;			// 게임모드 사용 여부


	if(EQJ_7POKER == eType)
	{
		if(nGameMode == 1)
		{
			pInfo->m_nGameMode = FORMKIND_CHOICE;	// 0:노멀, 1:초이스 ( 이미지하고 프로그램하고 처리하는 순서가 틀리다.)
		}
		else
		{
			pInfo->m_nGameMode = FORMKIND_NORMAL;	// 0:노멀, 1:초이스
		}
	}
	else if(EQJ_NEWPOKER == eType)
	{
		if(nGameMode == 1)
		{
			pInfo->m_nGameMode = FORMKIND_BOMB;	// 2:폭탄, 3: 조커 ( 이미지하고 프로그램하고 처리하는 순서가 틀리다.)
		}
		else
		{
			pInfo->m_nGameMode = FORMKIND_JOKER;	// 2:폭탄, 3: 조커
		}
	}
	else if(EQJ_SUTDA == eType)
	{
		if(nGameMode == 1)
		{
			pInfo->m_nGameMode = SD_2CARD;	//2장섯다
		}
		else
		{
			pInfo->m_nGameMode = SD_3CARD;	//3장섯다
		}
	}
	else if (EQJ_HOOLA == eType)
	{
	//	//스톱on/off
	//	if (nGameMode == 0)
	//	{
	//		GM().m_scondition.m_bUseGameMode = false;
	//		GM().m_scondition.m_nRuleType = -1;
	//	} 
	//	else if(nGameMode == 1)
	//	{
	//		GM().m_scondition.m_bUseGameMode = true;
	//		GM().m_scondition.m_nRuleType  =  hoola::eRULE_TYPE_STOP;

	//	}else{
	//		GM().m_scondition.m_bUseGameMode = true;
	//		GM().m_scondition.m_nRuleType  =  hoola::eRULE_TYPE_WAR;
	//	}

	//	//인원수
	//	if( nMaxUser == 0 ){
	//		GM().m_scondition.m_bMaxPlayer = false;
	//		pInfo->m_nGameMode = -1;
	//	}
	//	else if(nMaxUser == 1)
	//	{
	//		GM().m_scondition.m_bMaxPlayer = true;
	//		pInfo->m_nGameMode = 2;	//2인
	//	}
	//	else
	//	{
	//		GM().m_scondition.m_bMaxPlayer = true;
	//		pInfo->m_nGameMode = 5;	//5인
	//	}
	}

	//pInfo->m_bUseBetRule = false ;			// 베팅룰 사용여부
	//pInfo->m_bAutoBet = true ;				// 자동여부

	//if(nRoomType == 0 )
	//{
	//	pInfo->m_bUseSeedMoney = false ;		// 게임규모 사용 여부
	//	GM().m_scondition.m_bUseSeedMoney = false;
	//	pInfo->m_nSeedType = 0 ;
	//}else
	//{
	//	pInfo->m_bUseSeedMoney = true ;
	//	GM().m_scondition.m_bUseSeedMoney = true;
	//	pInfo->m_nSeedType = nComboSel ;			// 게임규모(0: 큰방, 1: 일반방, 2:작은방)
	//}

	pInfo->m_bAutoEnterRoom = true ;		// 자동으로 입장
	pInfo->m_sBetRuleKind = false;			//리미트머니

			//규제안 작업 카드세팅
	if(EQJ_SUTDA == eType)
	{
		pInfo->m_nBetMulti = nBetMoneyMulti;
	}else{
		pInfo->m_nCardSeting = nCardSeting;
		
	}
	
	////규제안 : SCONDITION를 추가한다
	//GM().m_scondition.m_bUseBetRule = pInfo->m_bUseBetRule;
	//GM().m_scondition.m_bUseGameMode = pInfo->m_bUseGameMode;

	//
	//GM().m_scondition.m_nGameMode = pInfo->m_nGameMode;

	//GM().m_scondition.m_bAutoBet = pInfo->m_bAutoBet;
	//GM().m_scondition.m_bAutoEnterRoom = pInfo->m_bAutoEnterRoom;
	//GM().m_scondition.m_sBetRuleKind = pInfo->m_sBetRuleKind;

	//GM().m_scondition.m_bUseQuickOption = bUseGameMode;

	//GM().m_scondition.m_nSeedType = pInfo->m_nSeedType;


	////규제안 : 카드세팅을 사용하지 않을 경우 해당 값을 -1로 설정하고 방만들기에서 비교하여 기본값 또는 설정값을 사용한다.
	//if(bUseCardSeting == 0){
	//	//규제안 : 섯다 참가비 설정 추가
	//	GM().m_scondition.m_nBetMulti = -1;

	//	//규제안 관련 : 바로시작시에 사용할 방정보 옵션 추가
	//	GM().m_scondition.m_nCardSeting = -1;
	//	
	//}else{
	//	//규제안 : 섯다 참가비 설정 추가
	//	GM().m_scondition.m_nBetMulti = nBetMoneyMulti;

	//	//규제안 관련 : 바로시작시에 사용할 방정보 옵션 추가
	//	GM().m_scondition.m_nCardSeting = nCardSeting;
	//}

	if(nGameMode == 0 && !pInfo->m_bUseGameMode && !pInfo->m_bUseSeedMoney) return FALSE;

	return TRUE;
}

// 데이터를 로드한다.
BOOL CQuickJoinDlg::_LoadData(const char *pSection, int nItemID, SGroupInfo& sGroupInfo)
{
	memset(&sGroupInfo, 0, sizeof(SGroupInfo));

	int nRadioButCount = 0;
	int nPrioritySelect = 0;
	int nComboPos = 0;

	CString strKey;
	CString strData; 

	// 라디오 버튼
	strKey.Format("%d_%d", nItemID, TYPE_CONTROL_RADIO);		
	strData = NMBASE::UTIL::GetProfile_string(pSection, strKey, "__NULL__");

	if(strData.Compare("__NULL__")==0)
		nRadioButCount = -1;
	else 
		nRadioButCount = atoi(strData);

	if(nRadioButCount <= 0)
	{
		// 사용안함으로 설정이 되어 있어 아래값은 사용을 안 한다.
		return FALSE;
	}
	else
	{
		sGroupInfo.m_bUsed = TRUE;						// 설정체크
		sGroupInfo.m_nRadioButPos = nRadioButCount;		// 라디오버튼 위치
	}	

	// 3. 콤보박스
	strKey.Format("%d_%d", nItemID, TYPE_CONTROL_COMBO);		
	strData = NMBASE::UTIL::GetProfile_string(pSection, strKey, "__NULL__");

	if(strData.Compare("__NULL__")==0)
		nComboPos = -1;
	else 
		nComboPos = atoi(strData);

	if(nComboPos >= 0)
	{
		if(PID_QUICKJOIN_ITEM_SEEDMONEY == nItemID)
		{
			// 3. 시드머니 저장			
			CString strTempKey;	
			strTempKey.Format("%d_%d_1", nItemID, TYPE_CONTROL_COMBO);	// Group타입 / 컨트롤 타입(라디오/콤보/우선순위)
			strData = NMBASE::UTIL::GetProfile_string(pSection, strTempKey, "__NULL__");
			if(strData.Compare("__NULL__")==0)
				nComboPos = -1;
			else 
				sGroupInfo.m_nComboValue = atoi(strData);
		}
	}		

	return TRUE;
}

BOOL CQuickJoinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if ( pMsg->message == WM_KEYDOWN  )
	{
		if (pMsg->wParam!=VK_RETURN)
			return TRUE;
	}

	return CUPGDlgBase::PreTranslateMessage(pMsg);
}

void CQuickJoinDlg::LoadData(const char *pSection, bool* bUsed, int* nMode, int* nRoomType, int* nMaxUser, int* nCursel, bool* bUseCardOrBetMulti, int* nCardSet ,  float *nBetMoneyMulti)
{
	// 값을 불러온다. 
	CString strKey;
	CString strData;

	// 검색 사용 유무.
	strKey.Format("UseGameMode");		
	strData = NMBASE::UTIL::GetProfile_string(pSection, strKey, "__NULL__");

	if(strData.Compare("__NULL__")==0)
		*bUsed = 0;
	else 
		*bUsed = atoi(strData)==1;

	// 모드 사용 유무.
	strKey.Format("Gamemode");		
	strData = NMBASE::UTIL::GetProfile_string(pSection, strKey, "__NULL__");

	if(strData.Compare("__NULL__")==0)
		*nMode = 0;
	else 
		*nMode = atoi(strData);

	// 방 크기 설정 사용 유무.
	strKey.Format("RoomType");		
	strData = NMBASE::UTIL::GetProfile_string(pSection, strKey, "__NULL__");

	if(strData.Compare("__NULL__")==0)
		*nRoomType = 0;
	else 
		*nRoomType = atoi(strData);

	//훌라 2/5인 방 설정 사용.
	strKey.Format("MaxUser");		
	strData = NMBASE::UTIL::GetProfile_string(pSection, strKey, "__NULL__");

	if(strData.Compare("__NULL__")==0)
		*nMaxUser = 0;
	else 
		*nMaxUser = atoi(strData);

	//방 크기 종류 설정 사용.
	strKey.Format("ComboSel");		
	strData = NMBASE::UTIL::GetProfile_string(pSection, strKey, "__NULL__");

	if(strData.Compare("__NULL__")==0)
		*nCursel = 0;
	else 
		*nCursel = atoi(strData);


	
	if (GM().GetCurrentGameType() == IDX_GAME_SD)
	{
		//참가비 사용여부
		strKey.Format("BetMoneyMultiComboSel");		
		strData = NMBASE::UTIL::GetProfile_string(pSection, strKey, "__NULL__");

		if(strData.Compare("__NULL__")==0)
			*bUseCardOrBetMulti = 0;
		else 
			*bUseCardOrBetMulti = atoi(strData)==1;
		
	}else{
		//카드세팅 사용여부
		strKey.Format("CardSetingComboSel");		
		strData = NMBASE::UTIL::GetProfile_string(pSection, strKey, "__NULL__");

		if(strData.Compare("__NULL__")==0)
			*bUseCardOrBetMulti = 0;
		else 
			*bUseCardOrBetMulti = atoi(strData)==1;
	}
	

	//규제안 : 섯다 참가비 설정 로드
	strKey.Format("BetMoneyMulti");		
	strData = NMBASE::UTIL::GetProfile_string(pSection, strKey, "__NULL__");
	
	//참가비 설정 되지않은 유저 처리
	if(strData.Compare("__NULL__")==0)
	{
		*nBetMoneyMulti = 20.0; //맥스 20.0배로 설정
	}else 
	{
		*nBetMoneyMulti = (float)atof(strData);
	}


	//규제안 카드세팅 종류 설정 사용.
	strKey.Format("CardSeting");		
	strData = NMBASE::UTIL::GetProfile_string(pSection, strKey, "__NULL__");



	//로우바둑이는 바로가기 설정에서 카드 설정을 하지 않는다.
	if( GM().GetCurrentGameType() == IDX_GAME_BD){
		*nCardSet = 0;  //로우바둑이는 기본이 0이다
	}else{

		
		//카드설정이 되지않은 유저 처리
		if(strData.Compare("__NULL__")==0)
		{
			switch( GM().GetCurrentGameType() ){
				case IDX_GAME_SD:
					*nCardSet = 2; //섯다는 2가 기본 
					break;

				case IDX_GAME_SP:
				case IDX_GAME_HL:
					*nCardSet = 4; //섯다는 2가 기본 
					break;

				case IDX_GAME_NP:
					*nCardSet = 5; //뉴포커 5가 기본 
					break;
			}
		}else 
		{
			*nCardSet = atoi(strData);
		}
	}


	//test
	*nCardSet = *nCardSet;
	*nMode = *nMode;
	
}

BOOL CQuickJoinDlg::SaveData(const char *pStrSection)
{
	// 값을 저장한다. 
	CString strKey;	
	CString strData;

	// 검색 사용 유무.
	strKey.Format("UseGameMode");	
	strData.Format("%d", m_bUseGameMode);

	NMBASE::UTIL::WriteProfile_string(pStrSection, strKey, strData);

	// 모드 사용 유무.
	strKey.Format("Gamemode");	
	strData.Format("%d", m_nGameMode);
	
	NMBASE::UTIL::WriteProfile_string(pStrSection, strKey, strData);

	// 방 크기 설정 사용 유무.
	strKey.Format("RoomType");	
	strData.Format("%d", m_nRoomType);

	NMBASE::UTIL::WriteProfile_string(pStrSection, strKey, strData);	

	//훌라 2/5인 방 설정 사용.
	strKey.Format("MaxUser");	
	strData.Format("%d", m_nMaxUser);

	NMBASE::UTIL::WriteProfile_string(pStrSection, strKey, strData);	

	m_nComboSel = m_cbRoomType.GetCurSel() ;
	if(m_nComboSel == -1)
		m_nComboSel =0 ;

	//방 크기 종류 설정 사용.
	strKey.Format("ComboSel");	
	strData.Format("%d", m_nComboSel);

	NMBASE::UTIL::WriteProfile_string(pStrSection, strKey, strData);	

	//규제안 카드세팅/참가비 설정 사용.
	if(  GM().GetCurrentGameType() == IDX_GAME_SD){
		
		//참가비 사용야부 설정
		strKey.Format("BetMoneyMultiComboSel");	
		strData.Format("%d", m_bUseCardSeting);
		NMBASE::UTIL::WriteProfile_string(pStrSection, strKey, strData);	


		float multiNum = (float)m_cbCardSeting.GetItemData(m_cbCardSeting.GetCurSel());
		strKey.Format("BetMoneyMulti");	
		
		if (multiNum == 1)
		{
			multiNum = 1.0;
		} 
		else if (multiNum == 3)
		{
			multiNum = 3.5;
		}
		else if (multiNum == 8)
		{
			multiNum = 8.5;
		}
		else if (multiNum == 20)
		{
			multiNum = 20.0;
		}
		strData.Format("%f", multiNum);
		

		NMBASE::UTIL::WriteProfile_string(pStrSection, strKey, strData);	

	}
	else
	{		
		//카드설정 사용여부 설정
		strKey.Format("CardSetingComboSel");	
		strData.Format("%d", m_bUseCardSeting);
		NMBASE::UTIL::WriteProfile_string(pStrSection, strKey, strData);	

		//설정안하면 기본4장
		int cardNum = m_cbCardSeting.GetItemData(m_cbCardSeting.GetCurSel());
		strKey.Format("CardSeting");	
		strData.Format("%d", cardNum);

		NMBASE::UTIL::WriteProfile_string(pStrSection, strKey, strData);	
	}

	return TRUE;
}


//규제안 관련 카드세팅 추가
//void CQuickJoinDlg::OnCbnSelchangeComboCardseting()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}
