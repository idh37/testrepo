#include "StdAfx.h"
#include "NMLobbyView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace FRAME
	{

		CNMLobbyView::CNMLobbyView(void)
		{
			m_strViewName = "lobby";
			m_eViewType   =  EVIEW_NOMAL;
		}

		CNMLobbyView::~CNMLobbyView(void)
		{

		}

	}//namespace FRAME

}//namespace NMBASE
