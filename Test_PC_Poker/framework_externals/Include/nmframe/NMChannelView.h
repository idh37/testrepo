#pragma once

#include "FrameDefine.h"
#include "NMViewBase.h"

namespace NMBASE
{
	namespace FRAME
	{

		class NMFRAME_CLASS CNMChannelView : public CNMViewBase
		{
		public:
			CNMChannelView(void);
			~CNMChannelView(void);

		public:

			virtual void		OnCreate();
			virtual void		OnDestroy();
			virtual void		OnDraw(){}
			virtual void		OnUpdate(){} 	
			virtual void		OnActiveOn(){}				///< 
			virtual void		OnActiveOff(){}				///< 

		public:
			UTIL::CFunctor			m_cOnClickRefreshBtn;		// 갱신버튼
			UTIL::CFunctor			m_cOnClickJoinBtn;			// 입장
		};

	}//namespace FRAME

}//namespace NMBASE
