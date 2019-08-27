#include "StdAfx.h"
#include "NMChannelProcess.h"
#include "NMChannelView.h"
#include "CommandDefine.h"	// Command Define


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace FRAME
	{

		CNMChannelProcess::CNMChannelProcess()
		{
			m_pNMChannelView = NULL;
		}


		CNMChannelProcess::CNMChannelProcess(CNMChannelView* pChannelView)
		{
			m_pNMChannelView = pChannelView;	
		}


		CNMChannelProcess::~CNMChannelProcess()
		{
			if(m_pNMChannelView)
			{
				delete m_pNMChannelView;
			}
		}


		BOOL CNMChannelProcess::OnAttached()
		{

			//////////////////////////////////////////////////////////////////////////
			UTIL::MM().AddMsgH(MSG_VIEW_ALLCHANNELINFO,	UTIL::Fnt(this, &CNMChannelProcess::OnAllChannelInfo));	



			CNMChannelView* pWnd = dynamic_cast<CNMChannelView*>(m_pNMChannelView);
			if(pWnd)
			{
				pWnd->m_cOnClickRefreshBtn	= UTIL::Fnt(this, &CNMChannelProcess::OnClickRefreshBtn);
				pWnd->m_cOnClickJoinBtn		= UTIL::Fnt(this, &CNMChannelProcess::OnClickJoinBtn);
			}

			return TRUE;
		}


		BOOL CNMChannelProcess::OnDetached()
		{
			delete this;
			return TRUE;
		}


		LRESULT CNMChannelProcess::OnClickRefreshBtn(WPARAM& wParam, LPARAM& lParam)
		{
			//return	UTIL::MM().Call(CONTROL_OnClickRefreshBtn, wParam, lParam);
			return TRUE;
		}


		LRESULT CNMChannelProcess::OnClickJoinBtn(WPARAM& wParam, LPARAM& lParam)
		{
			//return	UTIL::MM().Call(CONTROL_OnClickJoinBtn, wParam, lParam);
			return TRUE;
		}



		LRESULT CNMChannelProcess::OnAllChannelInfo(WPARAM& wParam, LPARAM& lParam)
		{
			// 종환복구
			//뷰에 보유머니구간 지우고 다시 세팅			
			//m_pLobbyView->SetDataRoomOptionList(pDataRoomOptionList);
			//방옵션정보창이 열려있으면 갱신한다.
			//m_pLobbyView->RefreshRoomOptionInfoDlg();

			return TRUE;
		}

	}//namespace FRAME

}//namespace NMBASE
