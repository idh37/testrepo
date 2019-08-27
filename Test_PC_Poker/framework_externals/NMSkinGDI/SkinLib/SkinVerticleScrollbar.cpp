// SkinVerticleScrollbar.cpp : implementation file
//

#include "stdafx.h"
#include "SkinVerticleScrollbar.h"
#include "memdc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


namespace NMBASE
{
	namespace SKINGDI
	{
		/////////////////////////////////////////////////////////////////////////////
		// CSkinVerticleScrollbar

		CSkinVerticleScrollbar::CSkinVerticleScrollbar()
		{
			bMouseDown = false;
			bMouseDownArrowUp = false;
			bMouseDownArrowDown = false;
			bDragging = false;


			dbThumbInterval = 0.000000;


			m_ScrollUP.x = 12;
			m_ScrollUP.y = 26;

			m_ScrollDown.x = 12;
			m_ScrollDown.y = 26;

			m_Thumb.x = 12;
			m_Thumb.y = 26;
			m_BkSize.x = 12;
			m_BkSize.y = 1;


			nThumbTop = m_ScrollUP.y;

		}

		CSkinVerticleScrollbar::~CSkinVerticleScrollbar()
		{
		}


		BEGIN_MESSAGE_MAP(CSkinVerticleScrollbar, CStatic)
			//{{AFX_MSG_MAP(CSkinVerticleScrollbar)
			ON_WM_ERASEBKGND()
			ON_WM_LBUTTONDOWN()
			ON_WM_LBUTTONUP()
			ON_WM_MOUSEMOVE()
			ON_WM_PAINT()
			ON_WM_TIMER()
			//}}AFX_MSG_MAP
		END_MESSAGE_MAP()

		/////////////////////////////////////////////////////////////////////////////
		// CSkinVerticleScrollbar message handlers

		BOOL CSkinVerticleScrollbar::OnEraseBkgnd(CDC* pDC) 
		{
			return CStatic::OnEraseBkgnd(pDC);
		}

		void CSkinVerticleScrollbar::OnLButtonDown(UINT nFlags, CPoint point) 
		{
			SetCapture();
			CRect clientRect;
			GetClientRect(&clientRect);


			//int nHeight = clientRect.Height() - 37;
			int nHeight = clientRect.Height() - m_ScrollUP.y;

			//CRect rectUpArrow(0,11,12,37);
			//CRect rectDownArrow(0,nHeight,12,nHeight+26);
			//CRect rectThumb(0,nThumbTop,12,nThumbTop+26);
			CRect rectUpArrow(0,0,m_ScrollUP.x,m_ScrollUP.y);
			CRect rectDownArrow(0,nHeight,m_ScrollDown.x,m_ScrollDown.y+nHeight);
			CRect rectThumb(0,nThumbTop,m_Thumb.x,nThumbTop+m_Thumb.y);

			if(rectThumb.PtInRect(point))
			{
				bMouseDown = true;
			}

			if(rectDownArrow.PtInRect(point))
			{
				bMouseDownArrowDown = true;
				SetTimer(2,250,NULL);
			}

			if(rectUpArrow.PtInRect(point))
			{
				bMouseDownArrowUp = true;
				SetTimer(2,250,NULL);
			}

			if(bMouseDownArrowUp || bMouseDownArrowDown || bMouseDown) {
				this->InvalidateRect(NULL,FALSE);
			}	
			CStatic::OnLButtonDown(nFlags, point);
		}

		void CSkinVerticleScrollbar::OnLButtonUp(UINT nFlags, CPoint point) 
		{
			UpdateThumbPosition();
			KillTimer(1);
			ReleaseCapture();

			bool bInChannel = true;

			CRect clientRect;
			GetClientRect(&clientRect);
			//int nHeight = clientRect.Height() - 37;
			//CRect rectUpArrow(0,11,12,37);
			//CRect rectDownArrow(0,nHeight,12,nHeight+26);
			//CRect rectThumb(0,nThumbTop,12,nThumbTop+26);
			int nHeight = clientRect.Height() - m_ScrollUP.y;
			CRect rectUpArrow(0,0,m_ScrollUP.x,m_ScrollUP.y);
			CRect rectDownArrow(0,nHeight,m_ScrollDown.x,m_ScrollDown.y+nHeight);
			CRect rectThumb(0,nThumbTop,m_Thumb.x,nThumbTop+m_Thumb.y);

			if(bMouseDownArrowUp || bMouseDownArrowDown || bMouseDown) {
				this->InvalidateRect(NULL,FALSE);
			}


			if(rectUpArrow.PtInRect(point) && bMouseDownArrowUp)
			{
				ScrollUp();	
				bInChannel = false;
			}

			if(rectDownArrow.PtInRect(point) && bMouseDownArrowDown)
			{
				ScrollDown();
				bInChannel = false;
			}

			if(rectThumb.PtInRect(point))
			{
				bInChannel = false;
			}

			if(bInChannel == true)
			{
				if(point.y > nThumbTop)
				{
					PageDown();
				}
				else
				{
					PageUp();
				}
			}


			bMouseDown = false;
			bDragging = false;
			bMouseDownArrowUp = false;
			bMouseDownArrowDown = false;

			CStatic::OnLButtonUp(nFlags, point);
		}

		void CSkinVerticleScrollbar::OnMouseMove(UINT nFlags, CPoint point) 
		{
			CRect clientRect;
			GetClientRect(&clientRect);

			if(bMouseDown)
			{

				int nPreviousThumbTop = nThumbTop;
				nThumbTop = point.y-(m_Thumb.y>>1); //-13 so mouse is in middle of thumb
				//nThumbTop = point.y;

				double nMax = pEdit->GetScrollLimit(SB_VERT);
				int nPos= pEdit->GetScrollPos(SB_VERT);

				double nHeight = clientRect.Height()-(m_ScrollUP.y+m_Thumb.y);
				double nVar = nMax;
				dbThumbInterval = nHeight/nVar;

				//figure out how many times to scroll total from top
				//then minus the current position from it

				int nScrollTimes = (int)(((nThumbTop-m_ScrollUP.y)/dbThumbInterval)-nPos) / m_ThumbScrollSize;
				/*		
				CSize size;
				size.cx = 0;
				size.cy = nScrollTimes*13; // is the height of each row at current font
				// I cant figure out how to grab this value
				// dynamically


				//		if(!this->m_bEdit)	pList->Scroll(size);
				*/
				pEdit->LineScroll(nScrollTimes,0);
				LimitThumbPosition();

				Draw();

			}
			CStatic::OnMouseMove(nFlags, point);
		}

		void CSkinVerticleScrollbar::OnPaint() 
		{
			CPaintDC dc(this); 

			Draw();
		}

		void CSkinVerticleScrollbar::OnTimer(UINT nIDEvent) 
		{
			if(nIDEvent == 1)
			{
				if(bMouseDownArrowDown)
				{
					ScrollDown();
				}

				if(bMouseDownArrowUp)
				{
					ScrollUp();
				}
			}
			else if(nIDEvent == 2)
			{
				if(bMouseDownArrowDown)
				{
					KillTimer(2);
					SetTimer(1, 50, NULL);
				}

				if(bMouseDownArrowUp)
				{
					KillTimer(2);
					SetTimer(1, 50, NULL);
				}
			}
			CStatic::OnTimer(nIDEvent);
		}

		void CSkinVerticleScrollbar::PageDown()
		{
			pEdit->SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEDOWN,0),NULL);
			UpdateThumbPosition();
		}

		void CSkinVerticleScrollbar::PageUp()
		{
			pEdit->SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEUP,0),NULL);
			UpdateThumbPosition();
		}

		void CSkinVerticleScrollbar::ScrollUp()
		{
			pEdit->SendMessage(WM_VSCROLL, MAKELONG(SB_LINEUP,0),NULL);
			UpdateThumbPosition();
		}

		void CSkinVerticleScrollbar::ScrollDown()
		{
			pEdit->SendMessage(WM_VSCROLL, MAKELONG(SB_LINEDOWN,0),NULL);
			UpdateThumbPosition();
		}

		void CSkinVerticleScrollbar::UpdateThumbPosition()
		{
			CRect clientRect;
			GetClientRect(&clientRect);

			double nPos = pEdit->GetScrollPos(SB_VERT);
			double nMax = pEdit->GetScrollLimit(SB_VERT);
			double nHeight = (clientRect.Height()-(m_ScrollUP.y+m_ScrollDown.y));//98);
			double nVar = nMax;

			if (nVar==0)
			{
				dbThumbInterval = 0;
			}
			else
			{
				dbThumbInterval = nHeight/nVar;
			}


			double nNewdbValue = (dbThumbInterval * nPos);
			int nNewValue = (int)nNewdbValue;

			nThumbTop = m_ScrollUP.y+nNewValue;

			LimitThumbPosition();

			Draw();
		}



		////////////////////////////////////////////////////////////////////////
		//
		void CSkinVerticleScrollbar::Draw()
		{
			CClientDC dc(this);
			CRect clientRect;
			GetClientRect(&clientRect);
			CMemDC memDC(&dc, &clientRect);
			memDC.FillSolidRect(&clientRect,  RGB(74,82,107));
			CDC bitmapDC;
			bitmapDC.CreateCompatibleDC(&dc);

			CBitmap bitmap;
			CBitmap* pOldBitmap  = NULL;	


			int ButtonDownFlag;	// 눌린진 버튼은 눌러진 그림으로 나오게 한다.


			ButtonDownFlag = bMouseDownArrowUp * m_ScrollUP.x;
			pOldBitmap = bitmapDC.SelectObject(&m_ScrUPBtn);
			memDC.BitBlt(clientRect.left,clientRect.top,m_ScrollUP.x,m_ScrollUP.y,&bitmapDC,ButtonDownFlag,0,SRCCOPY);

			//draw the background (span)
			bitmapDC.SelectObject(&m_BkImage);
			int nHeight = clientRect.Height() - m_ScrollUP.y;

			for(int i=0; i<nHeight; i+=m_BkSize.y)
			{
				memDC.BitBlt(clientRect.left,(clientRect.top+m_ScrollUP.y)+(i),m_BkSize.x,m_BkSize.y,&bitmapDC,0,0,SRCCOPY);
			}

			//draw the down arrow of the scrollbar
			ButtonDownFlag = bMouseDownArrowDown * m_ScrollDown.x;
			bitmapDC.SelectObject(&m_ScrDownBtn);
			memDC.BitBlt(clientRect.left,nHeight,m_ScrollDown.x,m_ScrollDown.y,&bitmapDC,ButtonDownFlag,0,SRCCOPY);

			//draw the thumb control
			ButtonDownFlag = bMouseDown * m_Thumb.x;
			bitmapDC.SelectObject(&m_ScrThumbBtn);
			memDC.BitBlt(clientRect.left,clientRect.top+nThumbTop,m_Thumb.x,m_Thumb.y,&bitmapDC,ButtonDownFlag,0,SRCCOPY);
			bitmapDC.SelectObject(pOldBitmap);
			bitmapDC.DeleteDC();
		}



		////////////////////////////////////////////////////////////////////////
		//
		void CSkinVerticleScrollbar::LimitThumbPosition()
		{
			CRect clientRect;
			GetClientRect(&clientRect);

			if(nThumbTop+m_Thumb.y > (clientRect.Height()-m_ScrollUP.y))
			{
				nThumbTop = clientRect.Height()-(m_ScrollUP.y+m_Thumb.y);//m_ScrollDown.y);
			}

			if(nThumbTop < (clientRect.top+m_ScrollUP.y))
			{
				nThumbTop = clientRect.top+m_ScrollUP.y;
			}
		}



		////////////////////////////////////////////////////////////////////////
		//
		void CSkinVerticleScrollbar::CalcImageSize(void)
		{
			BITMAP bm;
			CBitmap bitmap;

			// up
			GetObject(m_ScrUPBtn, sizeof(bm), &bm);
			m_ScrollUP.x = bm.bmWidth/2;
			m_ScrollUP.y = bm.bmHeight;

			// down
			GetObject(m_ScrDownBtn, sizeof(bm), &bm);
			m_ScrollDown.x = bm.bmWidth/2;
			m_ScrollDown.y = bm.bmHeight;


			// thumb
			GetObject(m_ScrThumbBtn, sizeof(bm), &bm);
			m_Thumb.x = bm.bmWidth/2;
			m_Thumb.y = bm.bmHeight;

			// background
			GetObject(m_BkImage, sizeof(bm), &bm);
			m_BkSize.x = bm.bmWidth;
			m_BkSize.y = bm.bmHeight;

			// 위치 지정 
			nThumbTop = m_ScrollUP.y;
		}



		////////////////////////////////////////////////////////////////////////
		//
		void CSkinVerticleScrollbar::LoadBtnImage()
		{
			m_ScrUPBtn.LoadBitmapFile(".\\common\\image\\sysimage\\VSUpArrow.bmp");
			m_ScrDownBtn.LoadBitmapFile(".\\common\\image\\sysimage\\VSDownArrow.bmp");
			m_ScrThumbBtn.LoadBitmapFile(".\\common\\image\\sysimage\\VSThumb.bmp");
			m_BkImage.LoadBitmapFile(".\\common\\image\\sysimage\\VSSpan.bmp");
		}


	} //namespace SKINGDI

}//namespace NMBASE