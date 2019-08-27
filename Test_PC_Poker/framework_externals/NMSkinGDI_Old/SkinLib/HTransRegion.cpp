// HTransRegion.cpp: implementation of the CHTransRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HTransRegion.h"

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

		CHTransRegion::CHTransRegion()
		{
			m_hBitmap = NULL;
		}

		CHTransRegion::~CHTransRegion()
		{

		}

		BOOL CHTransRegion::SetRgn(int width,int height,CDialog *pDlg)
		{
			CRgn crRgn;
			crRgn.CreateRoundRectRgn(0,0,width+1, height+1,16,16);
			pDlg->SetWindowRgn( crRgn, TRUE );
			crRgn.DeleteObject();
			return TRUE;
		}


		BOOL CHTransRegion::SetRgn( HBITMAP hBitmap, CDialog* pDlg, COLORREF transcolor, CBitmap* pBmp )
		{
			if( pBmp == NULL ) return FALSE;	

			BITMAP bmpinfo;
			memset( &bmpinfo, 0, sizeof(bmpinfo) );

			if( pBmp->GetBitmap( &bmpinfo ) == 0 ) return FALSE;
			/*
			CRgn crRgn;
			crRgn.CreateRoundRectRgn(0,0,bmpinfo.bmWidth+1, bmpinfo.bmHeight+1,16,16);
			pDlg->SetWindowRgn( crRgn, TRUE );
			crRgn.DeleteObject();
			return TRUE;
			/////
			*/
			return SetRgn( hBitmap, pDlg, transcolor, bmpinfo.bmHeight, bmpinfo.bmWidth - 1 );
		}

		BOOL CHTransRegion::SetRgn( HBITMAP hBitmap, CDialog* pDlg, COLORREF transcolor, int bmp_height, int bmp_width  )
		{
			if( pDlg == NULL ) return FALSE;
			if( bmp_height <= 0 || bmp_width <= 0 ) return FALSE;

			ASSERT( pDlg && hBitmap && bmp_height > 0 && bmp_width > 0 );

			m_hBitmap = hBitmap;
			if( m_hBitmap == NULL ) return FALSE;

			//Get information about the bitmap..
			//	GetObject(m_hBitmap, sizeof(m_Bitmap), &m_Bitmap);	// Get info about the bitmap 

			// Put the bitmap into a memory device context
			CPaintDC dc(pDlg);

			//get a memory dc object
			CDC dcMem;

			//create a compatible dc
			dcMem.CreateCompatibleDC(&dc);	// Select the bitmap into the in-memory DC
			//Select the bitmap into the dc
			CBitmap* pOldBitmap = dcMem.SelectObject(CBitmap::FromHandle(m_hBitmap));
			//Create a couple of region objects.
			CRgn crRgn, crRgnTmp;

			//create an empty region
			crRgn.CreateRectRgn(0, 0, 0, 0);

			//Create a region from a bitmap with transparency colour of Purple
			COLORREF crTransparent = transcolor;	

			int iX = 0;
			for (int iY = 0; iY < bmp_height; iY++)
			{
				do
				{
					//skip over transparent pixels at start of lines.
					while (iX <= bmp_width && dcMem.GetPixel(iX, iY) == crTransparent)
						iX++;
					//remember this pixel
					int iLeftX = iX;
					//now find first non transparent pixel
					while (iX <= bmp_width && dcMem.GetPixel(iX, iY) != crTransparent)
						++iX;
					//create a temp region on this info
					crRgnTmp.CreateRectRgn(iLeftX, iY, iX, iY+1);
					//combine into main region.
					crRgn.CombineRgn(&crRgn, &crRgnTmp, RGN_OR);
					//delete the temp region for next pass (otherwise you'll get an ASSERT)
					crRgnTmp.DeleteObject();
				}while(iX < bmp_width );
				iX = 0;
			}
			//Centre it on current desktop
			pDlg->SetWindowRgn( crRgn, TRUE );

			/*
			iX = (GetSystemMetrics(SM_CXSCREEN)) / 2 - (m_Bitmap.bmWidth / 2);
			iY = (GetSystemMetrics(SM_CYSCREEN)) / 2 - (m_Bitmap.bmHeight / 2);
			SetWindowPos(&wndTopMost, iX, iY, m_Bitmap.bmWidth, m_Bitmap.bmHeight, NULL); 
			*/
			// Free resources.
			dcMem.SelectObject(pOldBitmap);	// Put the original bitmap back (prevents memory leaks)
			dcMem.DeleteDC();
			crRgn.DeleteObject();

			return TRUE;
		}

	}//namespace SKINGDI

}//namespace NMBASE
