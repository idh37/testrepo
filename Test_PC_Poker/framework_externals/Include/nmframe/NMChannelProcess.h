#pragma once

#include "FrameDefine.h"
#include "./Delegate/IComponent.h"

namespace NMBASE
{
	namespace FRAME
	{

		class CNMChannelView;

		class NMFRAME_CLASS CNMChannelProcess : public IComponent
		{
		public:
			CNMChannelProcess();
			CNMChannelProcess( CNMChannelView* pChannelView);
			virtual ~CNMChannelProcess();

			virtual BOOL OnAttached();
			virtual BOOL OnDetached();

			virtual LRESULT	 OnClickRefreshBtn(WPARAM& wParam, LPARAM& lParam);
			virtual LRESULT	 OnClickJoinBtn(WPARAM& wParam, LPARAM& lParam);

			virtual LRESULT	 OnAllChannelInfo(WPARAM& wParam, LPARAM& lParam);		// 채널 정보

		protected:
			CNMChannelView*		m_pNMChannelView;	
		};


	}//namespace FRAME

}//namespace NMBASE
