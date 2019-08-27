#include "StdAfx.h"
#include "NMGameProcess.h"
#include "NMGameView.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace FRAME
	{

		CNMGameProcess::CNMGameProcess()
		{
			m_pNMGameView = NULL;
		}


		CNMGameProcess::CNMGameProcess(CNMGameView* pGameView)
		{
			m_pNMGameView = pGameView;	
		}


		CNMGameProcess::~CNMGameProcess()
		{
			if(m_pNMGameView)
			{
				delete m_pNMGameView;
			}
		}


		BOOL CNMGameProcess::OnAttached()
		{
			CNMGameView* pWnd = dynamic_cast<CNMGameView*>(m_pNMGameView);
			if(pWnd)
			{
				pWnd->m_cOnClickExitGameBtn	= UTIL::Fnt(this, &CNMGameProcess::OnClickExitGameBtn);
			}

			return TRUE;
		}


		BOOL CNMGameProcess::OnDetached()
		{
			delete this;
			return TRUE;
		}

		LRESULT CNMGameProcess::OnClickExitGameBtn(WPARAM& wParam, LPARAM& lParam)
		{
			//return	UTIL::MM().Call(CONTROL_OnClickJoinBtn, wParam, lParam);
			return TRUE;
		}

	}//namespace FRAME

}//namespace NMBASE
