// HTransRegion.h: interface for the CHTransRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTRANSREGION_H__0BB34957_EAEA_4865_9860_90954DA475AB__INCLUDED_)
#define AFX_HTRANSREGION_H__0BB34957_EAEA_4865_9860_90954DA475AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{
		class NMSKINGDI_CLASS CHTransRegion  
		{

		public:

			CHTransRegion();
			virtual ~CHTransRegion();

			BOOL SetRgn(int width,int height,CDialog *pDlg);
			BOOL SetRgn( HBITMAP hBitmap, CDialog* pDlg, COLORREF transcolor, int bmp_height, int bmp_width  );
			BOOL SetRgn( HBITMAP hBitmap, CDialog* pDlg, COLORREF transcolor, CBitmap* pBmp );

		protected:

			HBITMAP	m_hBitmap;

		};

	}//namespace SKINGDI

}//namespace NMBASE


#endif // !defined(AFX_HTRANSREGION_H__0BB34957_EAEA_4865_9860_90954DA475AB__INCLUDED_)
