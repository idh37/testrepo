// MyBitmap.cpp: implementation of the CMyBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyBitmap.h"

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

		CMyBitmap::CMyBitmap()
		{
			m_width = m_height = 0;
			m_bValidDC = FALSE;
		}

		CMyBitmap::~CMyBitmap()
		{
			if( m_hObject )	DeleteObject();
		}

		BOOL CMyBitmap::LoadBitmapFile(LPCTSTR szFilename) 
		{ 
			if(szFilename==NULL) return FALSE;
			if(strlen((const char *)szFilename)==0) return FALSE;

			DeleteObject();

			HBITMAP hBitmap = NULL; 
			hBitmap = (HBITMAP)LoadImage(NULL, szFilename, IMAGE_BITMAP, 0, 0, 
				LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 

			BOOL bRtn = Attach(hBitmap);
			if( m_hObject && bRtn )
			{
				BITMAP bmpstr;
				memset( &bmpstr, 0, sizeof(bmpstr) );
				if(	GetBitmap( &bmpstr ) == 0 ) return FALSE;

				m_width = bmpstr.bmWidth;
				m_height = bmpstr.bmHeight;
			}

			m_bValidDC = FALSE;

			return bRtn;
		}

		BOOL CMyBitmap::TransDraw(HDC hdcDest, int nXDest, int nYDest, COLORREF colorTransparent)
		{
			return CMyBitmap::TransDraw( hdcDest, nXDest, nYDest, GetWidth(), GetHeight(), 0, 0, colorTransparent );
		}

		BOOL CMyBitmap::TransDraw(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, int nXSrc, int nYSrc, COLORREF colorTransparent)
		{
			if(!m_hObject) return FALSE;

			if(nWidth == 0 || nHeight == 0) {
				// 비트맵 사이즈를 얻어옴
				if(m_hObject) {
					BITMAP bm;
					::GetObject(m_hObject, sizeof(bm), &bm); // BMP의 가로 세로 사이즈를 얻는데만 씀
					nWidth = bm.bmWidth;
					nHeight = bm.bmHeight;
				}
			}

			CDC dc, memDC, maskDC, tempDC;
			dc.Attach( hdcDest );
			maskDC.CreateCompatibleDC(&dc);
			CBitmap maskBitmap;

			// 이전 오브젝트 저장
			CBitmap* pOldMemBmp = NULL;
			CBitmap* pOldMaskBmp = NULL;
			HBITMAP hOldTempBmp = NULL;

			memDC.CreateCompatibleDC(&dc);
			tempDC.CreateCompatibleDC(&dc);
			CBitmap bmpImage;
			bmpImage.CreateCompatibleBitmap( &dc, nWidth, nHeight );
			pOldMemBmp = memDC.SelectObject( &bmpImage );

			hOldTempBmp = (HBITMAP) ::SelectObject( tempDC.m_hDC, m_hObject );

			memDC.BitBlt( 0,0,nWidth, nHeight, &tempDC, nXSrc, nYSrc, SRCCOPY );

			// 마스크를 위한 흑백 비트맵 생성
			maskBitmap.CreateBitmap( nWidth, nHeight, 1, 1, NULL );
			pOldMaskBmp = maskDC.SelectObject( &maskBitmap );
			memDC.SetBkColor( colorTransparent );

			// 메모리 DC로부터 마스크 생성
			maskDC.BitBlt( 0, 0, nWidth, nHeight, &memDC, 0, 0, SRCCOPY );

			// memDC의 백그라운드를 검은색으로 세팅한다.
			// 검은색으로 SRCPAINT를 사용하여 다른색은 나타나도록 만들어 검은색이 투명색이된다.
			memDC.SetBkColor(RGB(0,0,0));
			memDC.SetTextColor(RGB(255,255,255));
			memDC.BitBlt(0, 0, nWidth, nHeight, &maskDC, 0, 0, SRCAND);

			// 포그라운드를 검은색으로 세팅한다
			dc.SetBkColor(RGB(255,255,255));
			dc.SetTextColor(RGB(0,0,0));
			dc.BitBlt(nXDest, nYDest, nWidth, nHeight, &maskDC, 0, 0, SRCAND);

			// 포그라운드로 백그라운드를 융합
			dc.BitBlt(nXDest, nYDest, nWidth, nHeight, &memDC, 0, 0, SRCPAINT);

			if(hOldTempBmp) ::SelectObject( tempDC.m_hDC, hOldTempBmp);
			if(pOldMaskBmp) maskDC.SelectObject( pOldMaskBmp );
			if(pOldMemBmp) memDC.SelectObject( pOldMemBmp );

			// 사용이 끝난 DC 및 비트맵 해제
			maskDC.DeleteDC();
			memDC.DeleteDC();
			bmpImage.DeleteObject();

			dc.Detach();

			return TRUE;
		}

		void CMyBitmap::ResizeWnd( CWnd* pWnd, int offset_x, int offset_y, BOOL bCenter, CWnd* pParent )
		{
			if( pWnd == NULL ) return;

			if( m_hObject == NULL ) return;

			CRect rc;
			pWnd->GetClientRect( rc );

			//	BITMAP bmpstr;
			//	memset( &bmpstr, 0, sizeof(bmpstr) );
			//	if(	GetBitmap( &bmpstr ) == 0 ) return;

			//	int w = bmpstr.bmWidth;
			//	int h = bmpstr.bmHeight;

			rc.right = rc.left + m_width;
			rc.bottom = rc.top + m_height;

			rc.OffsetRect( offset_x, offset_y );

			if( pParent )
			{
				CRect wndrc;
				pParent->GetWindowRect( wndrc );
				rc.OffsetRect( wndrc.left, wndrc.top );
			}

			pWnd->MoveWindow( rc, TRUE );

			if( bCenter )
			{
				pWnd->CenterWindow(pParent);
			}
		}

		void CMyBitmap::BitBlt( CPaintDC& dc, int x, int y )
		{
			if( dc.m_hDC == NULL ) return;

			if( m_tempdc.m_hDC == NULL )
				m_tempdc.CreateCompatibleDC( &dc );

			m_tempdc.SelectObject(this);
			dc.BitBlt(x, y, GetWidth(), GetHeight(), &m_tempdc, 0, 0, SRCCOPY);
		}

		void CMyBitmap::BitBlt( CDC& dc, int x, int y )
		{
			if( dc.m_hDC == NULL ) return;

			/*
			CDC  m_tempdc;
			m_tempdc.CreateCompatibleDC( &dc );
			if( m_tempdc.m_hDC == NULL ) return;
			*/
			if( m_tempdc.m_hDC == NULL )
				m_tempdc.CreateCompatibleDC( &dc );

			m_tempdc.SelectObject(this);
			dc.BitBlt(x, y, GetWidth(), GetHeight(), &m_tempdc, 0, 0, SRCCOPY);
		}

		void CMyBitmap::DrawGauge( INT64 Now, INT64 Max, CDC& TempDC, CDC& DestDC, int Xp, int Yp )
		{
			INT64 NowPcnt = Now * 100 / Max;
			float pcnt = ((float)NowPcnt / 100.0f);
			if( pcnt > 1.0f ) pcnt = 1.0f;
			int graph_len = (int) (GetWidth() * pcnt);

			TempDC.SelectObject( *this );
			DestDC.BitBlt( Xp, Yp, graph_len, GetHeight(), &TempDC, 0, 0, SRCCOPY );
		}

		void CMyBitmap::DrawNumber( int Num, CDC& tempdc, CDC& destdc, int Xp, int Yp, BOOL bTransDraw )
		{
			int numwidth = GetWidth() / 10; // 0~9

			Num %= 10;
			if( bTransDraw ) 
			{
				TransDraw( destdc.m_hDC, Xp, Yp, numwidth, GetHeight(), numwidth * Num, 0, RGB(255, 0, 255) );
			}
			else 
			{
				tempdc.SelectObject( *this );
				destdc.BitBlt( Xp, Yp, numwidth, GetHeight(), &tempdc, numwidth * Num, 0, SRCCOPY );
			}
		}

		BOOL CMyBitmap::TransDraw_andTextDraw(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, int nXSrc, int nYSrc, COLORREF colorTransparent,CString str,CRect rt,CFont *pFont)
		{
			if(!m_hObject) return FALSE;

			if(nWidth == 0 || nHeight == 0) {
				// 비트맵 사이즈를 얻어옴
				if(m_hObject) {
					BITMAP bm;
					::GetObject(m_hObject, sizeof(bm), &bm); // BMP의 가로 세로 사이즈를 얻는데만 씀
					nWidth = bm.bmWidth;
					nHeight = bm.bmHeight;
				}
			}

			CDC dc, memDC, maskDC, tempDC;
			dc.Attach( hdcDest );
			maskDC.CreateCompatibleDC(&dc);
			CBitmap maskBitmap;

			// 이전 오브젝트 저장
			CBitmap* pOldMemBmp = NULL;
			CBitmap* pOldMaskBmp = NULL;
			HBITMAP hOldTempBmp = NULL;

			memDC.CreateCompatibleDC(&dc);
			tempDC.CreateCompatibleDC(&dc);
			CBitmap bmpImage;
			bmpImage.CreateCompatibleBitmap( &dc, nWidth, nHeight );
			pOldMemBmp = memDC.SelectObject( &bmpImage );

			hOldTempBmp = (HBITMAP) ::SelectObject( tempDC.m_hDC, m_hObject );

			memDC.BitBlt( 0,0,nWidth, nHeight, &tempDC, nXSrc, nYSrc, SRCCOPY );

			// 마스크를 위한 흑백 비트맵 생성
			maskBitmap.CreateBitmap( nWidth, nHeight, 1, 1, NULL );
			pOldMaskBmp = maskDC.SelectObject( &maskBitmap );
			memDC.SetBkColor( colorTransparent );

			// 메모리 DC로부터 마스크 생성
			maskDC.BitBlt( 0, 0, nWidth, nHeight, &memDC, 0, 0, SRCCOPY );

			// memDC의 백그라운드를 검은색으로 세팅한다.
			// 검은색으로 SRCPAINT를 사용하여 다른색은 나타나도록 만들어 검은색이 투명색이된다.
			memDC.SetBkColor(RGB(0,0,0));
			memDC.SetTextColor(RGB(255,255,255));
			memDC.BitBlt(0, 0, nWidth, nHeight, &maskDC, 0, 0, SRCAND);

			// 포그라운드를 검은색으로 세팅한다
			dc.SetBkColor(RGB(255,255,255));
			dc.SetTextColor(RGB(0,0,0));
			dc.BitBlt(nXDest, nYDest, nWidth, nHeight, &maskDC, 0, 0, SRCAND);

			if(str.GetLength() > 0){		

				if( pFont != NULL ){
					memDC.SelectObject(pFont);
				}
				memDC.SetTextColor(RGB(0,0,0));
				memDC.SetBkMode(TRANSPARENT);
				memDC.DrawText(str,&rt, DT_CENTER | DT_WORDBREAK);
			}

			// 포그라운드로 백그라운드를 융합
			dc.BitBlt(nXDest, nYDest, nWidth, nHeight, &memDC, 0, 0, SRCPAINT);


			if(hOldTempBmp) ::SelectObject( tempDC.m_hDC, hOldTempBmp);
			if(pOldMaskBmp) maskDC.SelectObject( pOldMaskBmp );
			if(pOldMemBmp) memDC.SelectObject( pOldMemBmp );

			// 사용이 끝난 DC 및 비트맵 해제
			maskDC.DeleteDC();
			memDC.DeleteDC();
			tempDC.DeleteDC();
			bmpImage.DeleteObject();
			maskBitmap.DeleteObject();

			dc.Detach();

			return TRUE;
		}

	}//namespace SKINGDI

}//namespace NMBASE
