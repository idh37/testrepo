#pragma once

#include "FrameDefine.h"
#include "./Delegate/IComponent.h"

namespace NMBASE
{
	namespace FRAME
	{

		class CNMMainFrmView;

		class NMFRAME_CLASS CNMMainFrmProcess : public IComponent
		{
		public:
			CNMMainFrmProcess(void);	
			~CNMMainFrmProcess(void);

		public:

			virtual void OnCreate();
			virtual void OnDestroy();	

		public:
			virtual BOOL OnAttached();
			virtual BOOL OnDetached();

			virtual LRESULT	 OnClickExitBtn(WPARAM& wParam, LPARAM& lParam);		///< 종료버튼	
			virtual LRESULT	 OnClickMiniBtn(WPARAM& wParam, LPARAM& lParam);		///< MiniMize버튼
			virtual LRESULT	 OnClickRestoreBtn(WPARAM& wParam, LPARAM& lParam);		///< Restore버튼
			virtual LRESULT	 OnOnClickOptionBtn(WPARAM& wParam, LPARAM& lParam);	///< 옵션버튼
			virtual LRESULT	 OnOnClickAltF4KeyDown(WPARAM& wParam, LPARAM& lParam);	///< Alt + F4 KeyDown

			virtual LRESULT	 OnWindowsDestroy(WPARAM& wParam, LPARAM& lParam);		///< Windows Destroy

		protected:
			CNMMainFrmView*	GetMainWnd() const;

		protected:
			CNMMainFrmView*	m_pNMainFrmView;
		};

	}//namespace FRAME

}//namespace NMBASE
