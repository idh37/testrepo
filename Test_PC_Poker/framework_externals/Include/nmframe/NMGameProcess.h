#pragma once

#include "FrameDefine.h"
#include "./Delegate/IComponent.h"

namespace NMBASE
{
	namespace FRAME
	{

		class CNMGameView;

		class NMFRAME_CLASS CNMGameProcess : public IComponent
		{
		public:
			CNMGameProcess();
			CNMGameProcess( CNMGameView* pGameView);
			virtual ~CNMGameProcess();

			virtual BOOL OnAttached();
			virtual BOOL OnDetached();

			virtual LRESULT	 OnClickExitGameBtn(WPARAM& wParam, LPARAM& lParam);	///< 게임 종료 버튼

		protected:
			CNMGameView*	m_pNMGameView;	
		};

	}//namespace FRAME

}//namespace NMBASE
