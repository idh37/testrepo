// GraphButtonEx.cpp: implementation of the CGraphButtonEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphButtonEx.h"

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

		CGraphButtonEx::CGraphButtonEx()
		{

		}

		CGraphButtonEx::~CGraphButtonEx()
		{

		}

		void CGraphButtonEx::SetSprite(NMBASE::GRAPHICGDI::xSprite *pspr, int iButtonNumber , int iButtoncnt)
		{
			m_iButtonNumber = iButtonNumber;
			m_pSpr = pspr;
			if(m_pSpr->spr != NULL)
			{
				m_Width = m_pSpr->spr[ m_iButtonNumber * iButtoncnt ].xl;
				m_Height = m_pSpr->spr[ m_iButtonNumber * iButtoncnt ].yl;
			}
		}

	} //namespace SKINGDI

}//namespace NMBASE