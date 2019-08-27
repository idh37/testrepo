// MyBitmap.h: interface for the CMyBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBITMAP_H__0A94EE65_7FF3_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_MYBITMAP_H__0A94EE65_7FF3_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{
		class NMSKINGDI_CLASS CMyBitmap : public CBitmap
		{

		public:

			CMyBitmap();
			virtual ~CMyBitmap();
			BOOL LoadBitmapFile(LPCTSTR szFilename);
			BOOL TransDraw(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, int nXSrc, int nYSrc, COLORREF colorTransparent);
			BOOL TransDraw(HDC hdcDest, int nXDest, int nYDest, COLORREF colorTransparent = RGB(255, 0, 255));
			BOOL TransStretchDraw(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, int nXSrc, int nYSrc, COLORREF colorTransparent);
			BOOL TransDraw_andTextDraw(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, int nXSrc, int nYSrc, COLORREF colorTransparent,CString str,CRect rt,CFont *pFont = NULL);

			void ResizeWnd( CWnd* pWnd, int offset_x, int offset_y, BOOL bCenter, CWnd* pParaent );

			int GetWidth(){ return m_width; }
			int GetHeight(){ return m_height; }

			void BitBlt( CPaintDC& dc, int x = 0, int y = 0 );
			void BitBlt( CDC& dc, int x = 0, int y = 0 );

			void DrawGauge( INT64 Now, INT64 Max, CDC& TempDC, CDC& DestDC, int Xp, int Yp );
			void DrawNumber( int Num, CDC& tempdc, CDC& destdc, int Xp, int Yp, BOOL bTransDraw = FALSE );

		protected:

			int m_width;
			int m_height;

			BOOL m_bValidDC;
			CDC  m_tempdc;
		};

	}//namespace SKINGDI
}//namespace NMBASE


#endif // !defined(AFX_MYBITMAP_H__0A94EE65_7FF3_11D4_97A4_0050BF0FBE67__INCLUDED_)

