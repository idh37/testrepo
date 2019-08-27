#pragma once

#include "FrameDefine.h"
#include "NMViewBase.h"

namespace NMBASE
{
	namespace FRAME
	{

		class NMFRAME_CLASS CNMGameView : public CNMViewBase
		{
		public:
			CNMGameView(void);
			virtual ~CNMGameView(void);

		public:
			virtual void		OnInit();					///< 
			virtual void		OnDestroy();				///< 

			virtual void		OnDraw(){}					///< 
			virtual void		OnUpdate(){} 				///< 
			virtual void		OnActiveOn(){}				///< 
			virtual void		OnActiveOff(){}				///< 

		public:

			UTIL::CFunctor		m_cOnClickExitGameBtn;				///< 게임 종료 버튼


		};


	}//namespace FRAME

}//namespace NMBASE


