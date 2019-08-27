#include "StdAfx.h"
#include "JackpotCurrentInfoDlg.h"
#include "../../WidgetID.h"

CJackpotCurrentInfoDlg::CJackpotCurrentInfoDlg()
:	m_pSelGame(NULL),
	m_pBtnClose(NULL),
	m_pBtnDetail(NULL)
{
	ZeroMemory(&m_stJackpotInfo, sizeof(jackpot::JP_MONEYINFO));
}

CJackpotCurrentInfoDlg::~CJackpotCurrentInfoDlg()
{
}

void CJackpotCurrentInfoDlg::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_COMBO_JACKPOT_GAME:
		{
			m_pSelGame				= (LSY::CCombo *)pObject;
			pObject->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CJackpotCurrentInfoDlg::OnSelChangeGame));
		}
		break;
	case ID_LOBBY_BTN_JACKPOT_CLOSE:	m_pBtnClose			= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_BTN_JACKPOT_INFO:		m_pBtnDetail		= (LSY::CButton *)pObject;		bAddChild = true; break;
	}

	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CJackpotCurrentInfoDlg::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

void CJackpotCurrentInfoDlg::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	OnCreateChild(pProject->GetObject(ID_LOBBY_BTN_JACKPOT_INFO));
	SetMiddleCount(0);


	//콤보박스(게임) 채우기
	int nCount = 1;
	int idxGame = 0;
	for(idxGame=0; idxGame<MAX_GAME; idxGame++ )
	{		
		if (GetArrayGameIndex((IDX_GAME)idxGame) < 0) 
			continue;

		CGame *pGame = GM().GetGame((IDX_GAME) idxGame);
		if (pGame)
		{			
			int nIndex = m_pSelGame->AddText(pGame->GetGameName().GetString());
			++nCount;

			m_pSelGame->SetItemData(nIndex, idxGame);
		}
	}
	m_pSelGame->SetEnable(false);
#if defined(_VIPJACKPOT)
#ifndef _DEBUG
	m_pSelGame->SetEnable(false);
#endif
#endif

	m_pSelGame->SetTotalHeightUseLineHeight(nCount);

}

LRESULT CJackpotCurrentInfoDlg::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	CWnd *pWnd = GetObjectMan()->GetWnd();
	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_JACKPOT_CLOSE:
		SetShow(false);
		break;
	case ID_LOBBY_BTN_JACKPOT_INFO:
		pWnd->SendMessage(LOBBY_BUTTON_ID_JACKPOT_DISPLAY_DIALOG_DETAIL);
		break;
	}
	return TRUE;
}

LRESULT CJackpotCurrentInfoDlg::OnSelChangeGame(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;

	int nCurSel = m_pSelGame->GetCurSelectItem();

	if (nCurSel<0)
		return FALSE;

	IDX_GAME eIdxGame = (IDX_GAME) m_pSelGame->GetItemData(nCurSel);

	if (GetArrayGameIndex(eIdxGame)<0)
		return FALSE;

 	SetShowDlg(eIdxGame);

	return TRUE;
}

void CJackpotCurrentInfoDlg::SetJackpotDisplyInfo( jackpot::JP_MONEYINFO *pJackpotMoneyInfo)
{
	if(pJackpotMoneyInfo == NULL)
	{
		ZeroMemory(&m_stJackpotInfo, sizeof(jackpot::JP_MONEYINFO));
		SetMiddleCount(0);
		return;
	}

	memcpy(&m_stJackpotInfo, pJackpotMoneyInfo, sizeof(jackpot::JP_MONEYINFO));

	if(IsShow())
	{
		int nCurSel = m_pSelGame->GetCurSelectItem();
		if(nCurSel>=0)
		{
			IDX_GAME eIdxGame = (IDX_GAME) m_pSelGame->GetItemData(nCurSel);			
			SetShowDlg(eIdxGame);						
		}
	}
}


int CJackpotCurrentInfoDlg::GetArrayGameIndex(const IDX_GAME &nGame)
{
	//게임인덱스로 서버에서 날라오는 구조체 m_stJackpotInfo.stJPBase 배열의 인덱스를 반환한다.

	switch(nGame)
	{
	case IDX_GAME_SP:
		return 0;
	case IDX_GAME_BD:
		return 1;
	case IDX_GAME_NP:
		return 2;
	case IDX_GAME_HL:
		return 3;
	case IDX_GAME_HA:
		return 4;
	}

	return -1;
}

void CJackpotCurrentInfoDlg::SetShowDlg(void)
{
	if(IsShow()) return;

	IDX_GAME nGame = GM().GetCurrentGameType();
	int nArrayGameIndex = GetArrayGameIndex(nGame);
// 	if ( nGame == IDX_GAME_HA )
// 	{
// 		CString strErrMsg;
// 		strErrMsg.Format(" %s의 잭팟금액은 게임방에서 확인할 수 있습니다.", GM().GetCurrentGame()->GetGameName());
// 		ShowMainMessageDlg(strErrMsg);
// 		return;
// 	}
// 	else 
	if(nArrayGameIndex<0)
	{
		CString strErrMsg;
		strErrMsg.Format("%s 게임에서는 잭팟서비스가 제공되지 않습니다", GM().GetCurrentGame()->GetGameName());
		ShowMainMessageDlg(strErrMsg);
		return;
	}

	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	
	if (nGame == IDX_GAME_HA)
	{
		// 훌라 게임이면 당첨자보기 버튼을 보여주지 않는다.
		LSY::CImage *pImage = (LSY::CImage*)(pProject->GetObject(ID_LOBBY_IMG_JACKPOT_BOTTOM));
		LSY::CButton *pButton = (LSY::CButton*)(pProject->GetObject(ID_LOBBY_BTN_JACKPOT_INFO));

		pImage->SetIndex(53);
		pButton->SetShow(false);
	}
	else
	{
		// 그 외의 다른 게임이면 당첨자보기 버튼을 보여준다.
		LSY::CImage *pImage = (LSY::CImage*)(pProject->GetObject(ID_LOBBY_IMG_JACKPOT_BOTTOM));
		LSY::CButton *pButton = (LSY::CButton*)(pProject->GetObject(ID_LOBBY_BTN_JACKPOT_INFO));

		pImage->SetIndex(3);
		pButton->SetShow(true);
	}
	
	m_pSelGame->SetCurSelectItem(nArrayGameIndex);	
	SetShowDlg(nGame);
	SetShow(true);
}

void CJackpotCurrentInfoDlg::SetShowDlg(const IDX_GAME &nGame)
{
	int nArrayGameIndex = GetArrayGameIndex(nGame);
	if (nArrayGameIndex<0)
		return;

	SetMiddleCount(m_stJackpotInfo.stJPBase[nArrayGameIndex].sMoneyRangeCount);
	int nCount = GetMiddleCount();

	LSY::CImage *pImage = NULL;
	LSY::CImageNumber *pJackpotMoney = NULL;
	LSY::CImageNumber *pMinimumMoney = NULL;
	bool bShowJackpotMoney = false;

	for(int i=0;i<nCount;++i)
	{
		pImage = GetMiddleImage(i);
		bShowJackpotMoney = true;
		//잭팟 대방출
		if( m_stJackpotInfo.stJPBase[ nArrayGameIndex ].bQuadrillionTime[i])
		{
			bShowJackpotMoney = false;
			pImage->SetIndex(51);
		}
		//1000조 일경우
		else if( m_stJackpotInfo.stJPBase[ nArrayGameIndex ].llJackPotMoney[i] == 1000000000000000 )
		{
			pImage->SetIndex(50);
		}
		else
		{
			pImage->SetIndex(i%2==0?1:2);
		}

		pJackpotMoney = (LSY::CImageNumber *)pImage->GetFirstChildFromID(ID_LOBBY_NUMBER_JACKPOT_BAR_MONEY);
		if(bShowJackpotMoney)
		{
			pJackpotMoney->SetNumber(m_stJackpotInfo.stJPBase[ nArrayGameIndex ].llJackPotMoney[i]);
			pJackpotMoney->SetShow(true);
		}
		else
		{
			pJackpotMoney->SetShow(false);
		}

		pMinimumMoney = (LSY::CImageNumber *)pImage->GetFirstChildFromID(ID_LOBBY_NUMBER_JACKPOT_BAR_ROOM);
		pMinimumMoney->SetNumber(LIMITMONEY_INT_INT64(m_stJackpotInfo.stJPBase[ nArrayGameIndex ].nLimitMoney[i]));
	}

	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);

	if (nGame == IDX_GAME_HA)
	{
		// 훌라 게임이면 당첨자보기 버튼을 보여주지 않는다.
		LSY::CImage *pImage = (LSY::CImage*)(pProject->GetObject(ID_LOBBY_IMG_JACKPOT_BOTTOM));
		LSY::CButton *pButton = (LSY::CButton*)(pProject->GetObject(ID_LOBBY_BTN_JACKPOT_INFO));

		pImage->SetIndex(53);
		pButton->SetShow(false);
	}
	else
	{
		// 그 외의 다른 게임이면 당첨자보기 버튼을 보여준다.
		LSY::CImage *pImage = (LSY::CImage*)(pProject->GetObject(ID_LOBBY_IMG_JACKPOT_BOTTOM));
		LSY::CButton *pButton = (LSY::CButton*)(pProject->GetObject(ID_LOBBY_BTN_JACKPOT_INFO));

		pImage->SetIndex(3);
		pButton->SetShow(true);
	}
}