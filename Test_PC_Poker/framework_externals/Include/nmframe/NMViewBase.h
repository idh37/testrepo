#pragma once

#include "FrameDefine.h"

namespace NMBASE
{
	namespace FRAME
	{

		enum ENUM_VIEW_TYPE
		{
			EVIEW_NOMAL = 0,
			EVIEW_GAME	= 1,		
		};

		class NMFRAME_CLASS CNMViewBase
		{
		public:
			CNMViewBase(void){}
			virtual ~CNMViewBase(void){}

		public:

			virtual void OnActiveOn()	= 0;				///< 
			virtual void OnActiveOff()	= 0;				///< 
			virtual void OnDraw()		= 0; 	
			virtual void OnUpdate()		= 0; 	

			LPCTSTR GetViewName(){return (LPCTSTR)m_strViewName;}
			ENUM_VIEW_TYPE GetViewType(){return m_eViewType;}
		protected:
			CString			m_strViewName;			///< View 이름
			ENUM_VIEW_TYPE	m_eViewType;			///< View Type(Normal, Game)
		};

	}//namespace FRAME

}//namespace NMBASE
