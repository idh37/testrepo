#include "StdAfx.h"
#include "PromotionUPP.h"
#include "PromotionManager.h"
#include "../GameButtonMan/GameViewButtonMan.h"

#define _TESTCODE_ 1
#define ID_BUTTON_CLICK (1111)



CPromotionUPP::CPromotionUPP( CPromotionManager *pManager, int nType )
: CPromotionBase(pManager, nType)
{

	string strDir = ".\\PokerCommon\\data\\Event\\upp\\";
	_LOADSPRITEDEFINE(&m_sprBack, strDir, "upp.spr");
	
	m_rtRemainTime = CRect(903, 475, 903 + 68, 475 + 12);
	m_rtRemainPan = CRect(903, 491, 903 + 68, 491 + 12);
	//�ӽ�
//	SetActive(true);

	m_nLevel = 0; // UPP ���� 
	m_bAgree = FALSE; // UPP ��å�� agree �ߴ��� ���� 
	m_nWarning = FALSE; // ��� â ���� ( �ʴ��� ) 
	m_nExit = FALSE;// ���� ���� ( �ʴ��� )

	m_nRemainTimeSec = 0;
	m_nRemainRound = 0;	

	m_bForceExitSubscript = FALSE;
	m_eExitType = eExit_None;

	MM().AddMsgH(UPG_CMK_SHOW_POPUP_ENDDING_UPP_LV3, Fnt(this, &CPromotionUPP::OnShowEndingPopup));
}

CPromotionUPP::~CPromotionUPP(void)
{
	MM().RemoveMsgH(UPG_CMK_SHOW_POPUP_ENDDING_UPP_LV3);
}


void CPromotionUPP::Accept_OutRoom()
{
// 	if (!IsActive())
// 	{
// 		return;
// 	}

	if (m_bForceExitSubscript)
	{ 
		//m_bForceExitSubscript exit�޽����� �޾����� ���̵ȴ�. ������ �����̵Ǽ� �濡�� �����Ұ��
		//1����̸� �κ� �������� �ʰ� ���� �����ϰ� �׿ܿ��� EndPopup�� ȣ��ȴ�.
		if ( m_eExitType == eExit_Force )
		{
			ExitGameClient(false);
		}
		else if ( m_eExitType == eExit_Timer )
		{
			MM().Call(UPG_CMK_CLICK_LOBBY_EXIT_BUTTON,1,(LPARAM)0);
		}
	}
	else if( IsActive() )
	{
		//���� �� ������ ��� Ȱ��ȭ �� ��� �κ񿡼� �ش� �� ǥ��
		CUPPInfoPopup dlg(GM().GetMainWnd());
		dlg.SetRemainTimeRound(m_nRemainTimeSec,m_nRemainRound);
		dlg.DoModal(TYPE_UPPPOPUP_GRADE1_STEP2);
	}
	
}

void CPromotionUPP::Accept_CreateRoom()
{
// 	if (GM().GetCurrentGameType()==IDX_GAME_BD)
// 		SetActive(true);
// 	else
// 		SetActive(false);
	
}

void CPromotionUPP::Accept_EnterRoom()
{
// 	if (GM().GetCurrentGameType()==IDX_GAME_BD)
// 		SetActive(true);
// 	else
// 		SetActive(false);
}



void CPromotionUPP::DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if( !IsActive() ) return;
}

void CPromotionUPP::DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if( !IsActive() ) return;

	
}

//�Ǽ� �� �ð� �׸���
void CPromotionUPP::DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
	if( !IsActive() ) return;	

	int nHour = m_nRemainTimeSec / 3600;
	int nMin = ( m_nRemainTimeSec % 3600 ) / 60;

	if (!(m_nRemainRound == 30 || m_nRemainRound == 10 || m_nRemainRound <= 5 || ( nHour == 1 && nMin == 0 ) 
		|| ( nHour == 0 && nMin == 30 ) || ( nHour == 0 && nMin == 10 ) || ( nHour == 0 && nMin <= 5 ) ))
	{
		return;
	}

	pPage->PutSprAuto( 806, 469, &m_sprBack, 0, AB );

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 0, 0));
	pDC->SelectObject(NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_BOLD));

	CString str;
	str.Format("%02d�ð�%02d��", nHour, nMin);
	pDC->DrawText(str, &m_rtRemainTime, DT_VCENTER|DT_SINGLELINE|DT_LEFT);
	str.Format("%02d��", m_nRemainRound);
	pDC->DrawText(str, &m_rtRemainPan, DT_VCENTER|DT_SINGLELINE|DT_LEFT);
}

void CPromotionUPP::DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart)
{

}
LRESULT  CPromotionUPP::OnShowEndingPopup(WPARAM &wParam, LPARAM &lParam)
{
	if (m_nLevel == 3)
	{
		CUPPInfoPopup dlg(GM().GetMainWnd());
		dlg.DoModal(TYPE_UPPPOPUP_GRADE3_STEP1);
	}
	return TRUE;
}

BOOL CPromotionUPP::OnLButtonDownMiddle(int &x , int &y)
{
// 	m_BtnClose.OnLButtonDown( x, y ); 
// 	m_BtnOpen.OnLButtonDown( x, y );
	
	return FALSE;
}

BOOL CPromotionUPP::OnLButtonUpMiddle(int &x , int &y)
{
// 	m_BtnClose.OnLButtonUp( x, y );
// 	m_BtnOpen.OnLButtonUp( x, y );
	
	return FALSE;
}

BOOL CPromotionUPP::OnMouseMoveMiddle(int &x , int &y)
{
// 	m_BtnClose.OnMouseMove( x, y );
// 	m_BtnOpen.OnMouseMove( x, y );
	
	return FALSE;
}

CString Hex2Str(PBYTE pHex, int nHexSize)
{  
	CString strHex;

	for (int i=0 ; i<nHexSize ; i++)
	{
		CString strTemp;
		strTemp.Format("%02X ", *(pHex+i));
		strHex += strTemp;
	}   
	return strHex;
}

#ifdef _DEBUG

#define TRACE_PACKET_CASE(sig) case sig: {	\
	CString strFile; \
	strFile.Format("log_%s.txt", GM().GetMyInfo()->UI.ID); \
	TRACE("Tournament Packet [%s]", #sig);\
	char *buf = new char[TotSize];\
	memcpy(buf, lpData, TotSize);\
	CCommMsg::DecodeAll(buf);\
	DebugLog(strFile, "Packet Received [%s] : %s ", #sig, Hex2Str( (PBYTE)buf+sizeof(PACKETHEADER), TotSize-sizeof(PACKETHEADER) ) ) ;\
	delete buf;\
}

#else
#define TRACE_PACKET_CASE(sig) case sig:
#endif
BOOL CPromotionUPP::OnCommand(WPARAM &wParam, LPARAM &lParam)
{
	switch(wParam)
	{
	case IDM_EXIT:
	case IDM_EXIT_BOTTOM:		
		if (m_bForceExitSubscript && GM().GetCurrentGameView()->m_bExitSubscript)
			return TRUE;
		break;
	}
	return FALSE;
}


BOOL CPromotionUPP::OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData)
{
	switch(Signal)
	{
	case SV_UPPTIMEINFO:
		{
			CSV_UPPTIMEINFO msg;
			msg.Get(lpData, TotSize);

			static BOOL bFirst = TRUE;
			if ( GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME && bFirst)
			{
				bFirst = FALSE;
				if ( m_bAgree == FALSE )
				{
					CheckUPPAgreement();
				}
				else
				{
					CUPPInfoPopup dlg(GM().GetMainWnd());
 				dlg.SetRemainTimeRound(*msg.m_remainTimeSec, *msg.m_nRoundLimit - *msg.m_nRound);
					dlg.DoModal(TYPE_UPPPOPUP_GRADE1_STEP2);
				}				
			}
			int nRemainTimeSec = *msg.m_remainTimeSec;
			int nRemainRound = *msg.m_nRoundLimit - *msg.m_nRound;



			if ( nRemainTimeSec <= 3600 || nRemainRound <= 30)
				SetActive(TRUE);

			if ( (nRemainRound == 1) || (nRemainTimeSec <= 60))
			{
				CString str;
				str.Format( "������ ���Դϴ�.\n" );
				AddGameInfoViewEdit(str.GetString(), RGB(255,0,0));
			}

			m_nRemainTimeSec = *msg.m_remainTimeSec;
			m_nRemainRound = *msg.m_nRoundLimit - *msg.m_nRound;
// 			else
// 				SetActive(FALSE);

		}break;
	case SV_UPPEVENT:
		{
 			CSV_UPPEVENT msg;
			msg.Get(lpData, TotSize);

			switch(*msg.m_cmd)
			{
			case CSV_UPPEVENT::WARNING:
				break;
			case CSV_UPPEVENT::EXIT:
			case CSV_UPPEVENT::EXITFORCE:
				if( GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME )
				{
					m_bForceExitSubscript = TRUE; 
					// ### [ ������� ] ###
					CCL_EXITSUBSCRIPT esmsg;
					// ������ �߰�.
					/*esmsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, 1);*/
					esmsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, 1, 0);	

					// ���μ������� ������ ���� ���¸� �˸�
					NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), esmsg.pData, esmsg.GetTotalSize());
				}
				else
				{
					MM().Call(UPG_CMK_CLICK_LOBBY_EXIT_BUTTON,1,(LPARAM)0);
				}

				switch(m_eExitType)
				{
					case eExit_None:	
						if ( *msg.m_cmd == CSV_UPPEVENT::EXIT )
						{
							m_eExitType = eExit_Timer;
						}
						else if (  *msg.m_cmd == CSV_UPPEVENT::EXITFORCE )
						{
							m_eExitType = eExit_Force;
						}
						break;
					case eExit_Timer:	break;
						if ( *msg.m_cmd == CSV_UPPEVENT::EXIT )
						{
//							m_eExitType = eExit_Timer;
						}
						else if (  *msg.m_cmd == CSV_UPPEVENT::EXITFORCE )
						{
							m_eExitType = eExit_Force;
						}
						break;
					case eExit_Force:	
						if ( *msg.m_cmd == CSV_UPPEVENT::EXIT )
						{
//							m_eExitType = eExit_Timer;
						}
						else if (  *msg.m_cmd == CSV_UPPEVENT::EXITFORCE )
						{
							m_eExitType = eExit_Force;
						}
						break;
				}
				break;
			case CSV_UPPEVENT::DENYLOGIN:
				CUPPInfoPopup dlg(GM().GetMainWnd());
				if(dlg.DoModal(TYPE_UPPPOPUP_GRADE1_STEP1) == IDOK)
				{
					ExitGameClient(false);
				}
				break;			
			}
		}break;
	}

	return FALSE;
}
void CPromotionUPP::Test()
{
	m_bForceExitSubscript = TRUE; 
	m_eExitType = eExit_Force;
	// ### [ ������� ] ###
	CCL_EXITSUBSCRIPT esmsg;	
	// ������ �߰�.
	/*esmsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, 1);*/
	esmsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, 1, 0);			

	// ���μ������� ������ ���� ���¸� �˸�
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), esmsg.pData, esmsg.GetTotalSize());
}
void CPromotionUPP::SetActive(const bool &bActive)
{
	CPromotionBase::SetActive(bActive);
}


LRESULT CPromotionUPP::OnIsActive(WPARAM &wParam, LPARAM &lParam)
{
	return IsActive();
}

void CPromotionUPP::SetUPPData(CSV_UPPINFO* msg)
{
	m_nLevel = *msg->m_level; 
	m_bAgree = *msg->m_agree ; 
	m_nWarning = *msg->m_warning;
	m_nExit = *msg->m_exit;	
}

BOOL CPromotionUPP::CheckUPPAgreement()
{
	if (m_bAgree)
	{	
		// ��å�� ������ ���
		return TRUE;
	}
	if ( m_nLevel == 1 )
	{	//1���
		CUPPPopup dlg(GM().GetMainWnd());
		if (dlg.DoModal(1)==IDCANCEL)//�˾� Ÿ���� 1
		{
			ExitGameClient(false);
		}
		//1��� ���� �˾� ����
		dlg.DoModal(3); //�˾� Ÿ���� 3
	}
	else if ( m_nLevel == 2 )
	{	//2���
		CUPPPopup dlg(GM().GetMainWnd());

		if (dlg.DoModal(2)==IDCANCEL)//�˾� Ÿ���� 2
		{
			ExitGameClient(false);
		}
	}
	return FALSE;
}
void CPromotionUPP::ShowUPPInfoDlg()
{
	CUPPInfoPopup dlg(GM().GetMainWnd());
	switch(m_nLevel)
	{
// 	case 1:
// 		{
// 			dlg.DoModal(TYPE_UPPPOPUP_GRADE1_STEP2);
// 		}break;
	case 2:
		{
			dlg.DoModal(TYPE_UPPPOPUP_GRADE2);
		}break;
	case 3:
		{
			dlg.DoModal(TYPE_UPPPOPUP_GRADE3_STEP2);
		}break;
	}
}