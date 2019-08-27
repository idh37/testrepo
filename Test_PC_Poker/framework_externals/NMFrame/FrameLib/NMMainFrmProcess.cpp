#include "StdAfx.h"
#include "NMMainFrmProcess.h"
#include "NMMainFrmView.h"
#include "CommandDefine.h"
#include "NMApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace FRAME
	{

		CNMMainFrmProcess::CNMMainFrmProcess(void)
		{
			m_pNMainFrmView = NULL;
		}

		CNMMainFrmProcess::~CNMMainFrmProcess(void)
		{
		}

		/*!
		\brief  Main frame Window Pointer를 얻는다.
		\param  void
		\return CF5MainFrmWnd* 
		*/
		CNMMainFrmView*	CNMMainFrmProcess::GetMainWnd() const
		{
			return static_cast<CNMMainFrmView*>(AfxGetMainWnd());
		}



		BOOL CNMMainFrmProcess::OnAttached()
		{
			_ASSERTE(GetMainWnd());

			if(GetMainWnd())
			{
				GetMainWnd()->m_cOnClickExitBtn		= UTIL::Fnt(this, &CNMMainFrmProcess::OnClickExitBtn);
				GetMainWnd()->m_cOnClickMiniBtn		= UTIL::Fnt(this, &CNMMainFrmProcess::OnClickMiniBtn);
				GetMainWnd()->m_cOnClickRestoreBtn	= UTIL::Fnt(this, &CNMMainFrmProcess::OnClickRestoreBtn);
				GetMainWnd()->m_cOnClickOptionBtn	= UTIL::Fnt(this, &CNMMainFrmProcess::OnOnClickOptionBtn);
				GetMainWnd()->m_cOnClickAltF4KeyDown= UTIL::Fnt(this, &CNMMainFrmProcess::OnOnClickAltF4KeyDown);		
			}

			///< Destroy
			UTIL::MM().AddMsgH( EVT_MAINFRAM_ONDESTROY, UTIL::Fnt( this, &CNMMainFrmProcess::OnWindowsDestroy) );

			return TRUE;
		}


		BOOL CNMMainFrmProcess::OnDetached()
		{
			delete this;

			return TRUE;
		}

		void CNMMainFrmProcess::OnCreate(  )
		{

		}


 		void CNMMainFrmProcess::OnDestroy( )
 		{
 
 		}



		LRESULT CNMMainFrmProcess::OnClickExitBtn(WPARAM& wParam, LPARAM& lParam)
		{

			return TRUE;
		}

		LRESULT CNMMainFrmProcess::OnClickMiniBtn(WPARAM& wParam, LPARAM& lParam)
		{

			return TRUE;
		}

		LRESULT CNMMainFrmProcess::OnClickRestoreBtn(WPARAM& wParam, LPARAM& lParam)
		{

			return TRUE;
		}

		LRESULT CNMMainFrmProcess::OnOnClickOptionBtn(WPARAM& wParam, LPARAM& lParam)
		{

			return TRUE;
		}

		LRESULT CNMMainFrmProcess::OnOnClickAltF4KeyDown(WPARAM& wParam, LPARAM& lParam)
		{
			UTIL::MM().Call( EVT_MAINFRAM_ONALTF4, wParam, lParam);		

			return TRUE;
		}

		LRESULT CNMMainFrmProcess::OnWindowsDestroy(WPARAM& wParam, LPARAM& lParam)
		{
			UNUSED_ALWAYS(wParam);
			UNUSED_ALWAYS(lParam);


			CNMApp* pApp = static_cast<CNMApp*>(AfxGetApp());
			if(pApp)
			{
				pApp->SetExitFlag(TRUE);
			}			

			if(GetMainWnd())
			{
				GetMainWnd()->ShowWindow(SW_HIDE);
				GetMainWnd()->PostMessage(WM_CLOSE, 0, 0);
			}

			return TRUE;
		}

	}//namespace FRAME

}//namespace NMBASE


