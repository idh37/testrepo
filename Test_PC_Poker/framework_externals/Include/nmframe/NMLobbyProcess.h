#pragma once

#include "FrameDefine.h"
#include "./Delegate/IComponent.h"

namespace NMBASE
{
	namespace FRAME
	{

		class CNMLobbyView;

		class NMFRAME_CLASS CNMLobbyProcess : public IComponent
		{
		public:
			CNMLobbyProcess();
			CNMLobbyProcess( CNMLobbyView* pLobbyView);
			virtual ~CNMLobbyProcess();

			virtual BOOL OnAttached();
			virtual BOOL OnDetached();

			virtual LRESULT	 OnClickRefreshBtn(WPARAM& wParam, LPARAM& lParam);
			virtual LRESULT	 OnClickJoinBtn(WPARAM& wParam, LPARAM& lParam);
			virtual LRESULT	 OnClickExitGameBtn(WPARAM& wParam, LPARAM& lParam);


			virtual LRESULT	 OnGameServerAcceptLogInOk(WPARAM& wParam, LPARAM& lParam);

		protected:
			CNMLobbyView*	m_pNMLobbyView;	
		};

	}//namespace FRAME

}//namespace NMBASE
