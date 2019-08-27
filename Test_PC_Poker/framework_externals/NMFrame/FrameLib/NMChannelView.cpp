#include "StdAfx.h"
#include "NMChannelView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace FRAME
	{

		CNMChannelView::CNMChannelView(void)
		{
			m_strViewName = "channel";
			m_eViewType =  EVIEW_NOMAL;
		}


		CNMChannelView::~CNMChannelView(void)
		{
		}

		void CNMChannelView::OnCreate(  )
		{

		}


		void CNMChannelView::OnDestroy( )
		{

		}

	}//namespace FRAME

}//namespace NMBASE
