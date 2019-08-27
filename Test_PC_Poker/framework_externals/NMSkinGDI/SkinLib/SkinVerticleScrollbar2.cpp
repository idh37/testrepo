// SkinVerticleScrollbar2.cpp: implementation of the CSkinVerticleScrollbar2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkinVerticleScrollbar2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace SKINGDI
	{

		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////

		CSkinVerticleScrollbar2::CSkinVerticleScrollbar2()
		{

		}

		CSkinVerticleScrollbar2::~CSkinVerticleScrollbar2()
		{

		}

		void CSkinVerticleScrollbar2::LoadBtnImage2()
		{
			m_ScrUPBtn.LoadBitmapFile(".\\common\\image\\vip\\vip_UpArrow.bmp");
			m_ScrDownBtn.LoadBitmapFile(".\\common\\image\\vip\\vip_DownArrow.bmp");
			m_ScrThumbBtn.LoadBitmapFile(".\\common\\image\\vip\\vip_Thumb.bmp");
			m_BkImage.LoadBitmapFile(".\\common\\image\\vip\\vip_Span.bmp");
		}


	} //namespace SKINGDI

}//namespace NMBASE