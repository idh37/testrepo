#include "StdAfx.h"
#include "NMLobbyProcess.h"
#include "NMLobbyView.h"
#include "CommandDefine.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace FRAME
	{

		CNMLobbyProcess::CNMLobbyProcess()
		{
			m_pNMLobbyView = NULL;
		}


		CNMLobbyProcess::CNMLobbyProcess(CNMLobbyView* pLobbyView)
		{
			m_pNMLobbyView = pLobbyView;	
		}


		CNMLobbyProcess::~CNMLobbyProcess()
		{
			if(m_pNMLobbyView)
			{
				delete m_pNMLobbyView;
			}
		}


		BOOL CNMLobbyProcess::OnAttached()
		{
			//////////////////////////////////////////////////////////////////////////
			UTIL::MM().AddMsgH(MSG_VIEW_ACCEPTLOGIN,	UTIL::Fnt(this, &CNMLobbyProcess::OnGameServerAcceptLogInOk));	

			CNMLobbyView* pWnd = dynamic_cast<CNMLobbyView*>(m_pNMLobbyView);
			if(pWnd)
			{
				pWnd->m_cOnClickRefreshBtn	= UTIL::Fnt(this, &CNMLobbyProcess::OnClickRefreshBtn);
				pWnd->m_cOnClickJoinBtn		= UTIL::Fnt(this, &CNMLobbyProcess::OnClickJoinBtn);
				pWnd->m_cOnClickExitGameBtn	= UTIL::Fnt(this, &CNMLobbyProcess::OnClickExitGameBtn);	
			}

			return TRUE;
		}


		BOOL CNMLobbyProcess::OnDetached()
		{
			delete this;
			return TRUE;
		}


		LRESULT CNMLobbyProcess::OnClickRefreshBtn(WPARAM& wParam, LPARAM& lParam)
		{
			//return	UTIL::MM().Call(CONTROL_OnClickRefreshBtn, wParam, lParam);
			return TRUE;
		}


		LRESULT CNMLobbyProcess::OnClickJoinBtn(WPARAM& wParam, LPARAM& lParam)
		{
			//return	UTIL::MM().Call(CONTROL_OnClickJoinBtn, wParam, lParam);
			return TRUE;
		}


		LRESULT CNMLobbyProcess::OnClickExitGameBtn(WPARAM& wParam, LPARAM& lParam)
		{
			//return	UTIL::MM().Call(CONTROL_OnClickJoinBtn, wParam, lParam);
			return TRUE;
		}


		LRESULT CNMLobbyProcess::OnGameServerAcceptLogInOk(WPARAM& wParam, LPARAM& lParam)
		{

			// 종환복구
			//m_bLogined = true;
			//m_pLobbyView->EnableWindow(TRUE);	

			//INT64 llMyPMoney = GM().GetMyInfo()->UI.GetMoney();

			////현재탭 설정
			//m_pLobbyView->SetGameTab(GM().GetCurrentGameType());

			////탭설정		
			//ChangeProperRoomTab();

			////입장머니 저장
			//m_llOldMoney = m_llLoginMoney = llMyPMoney;	

			////내정보 세팅
			//m_pLobbyView->SetMyUserInfo(&GM().GetMyInfo()->UI, TRUE);	


			return TRUE;
		}

	}//namespace FRAME

}//namespace NMBASE
