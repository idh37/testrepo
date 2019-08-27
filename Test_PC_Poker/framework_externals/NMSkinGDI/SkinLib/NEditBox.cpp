// NEditBox.cpp: implementation of the CNEditBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NEditBox.h"

// 임시
//#include "Mainwindow.h"

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

		CNEditBox::CNEditBox()
		{	
			m_pParentWnd = NULL;	
			pSendEdit = NULL;
			m_pPage = NULL;
			m_nFontIndex = NMBASE::UTIL::FONT_12_NORMAL;
		}


		CNEditBox::~CNEditBox()
		{
			if(pSendEdit != NULL) {

				pSendEdit->DestroyWindow();

				delete pSendEdit;
				pSendEdit = NULL;
			}

		}

		void CNEditBox::Init(int x, int y ,int w, int h,CWnd *pParentWnd , NMBASE::GRAPHICGDI::CPage *pPage)
		{
			m_Xp = x;
			m_Yp = y;
			m_bShowCaret = TRUE;
			m_pPage = pPage;

			m_pParentWnd = pParentWnd;

			m_Width = w;
			m_Height = h;

			pSendEdit = new CSendEdit;

			CRect rc;
			rc.SetRect(m_Xp, m_Yp, m_Xp+m_Width, m_Yp+m_Height);

			if(m_pParentWnd) {
				pSendEdit->Create(ES_WANTRETURN|ES_MULTILINE|ES_AUTOHSCROLL, rc, m_pParentWnd, 123);
				pSendEdit->m_nUse =1;
				pSendEdit->SetFont(&NMBASE::UTIL::GetMyFont(m_nFontIndex));
				pSendEdit->GetClientRect(rc);
				rc.OffsetRect(-1000, 0);
				pSendEdit->MoveWindow(rc);
				pSendEdit->ShowWindow(SW_SHOW);
				pParentWnd->SetFocus();
				SetLimitText(100);
			}
		}

		void CNEditBox::SetFontIndex(NMBASE::UTIL::STD_FONT nFontIndex)
		{
			m_nFontIndex = nFontIndex;
			pSendEdit->SetFont(&NMBASE::UTIL::GetMyFont(m_nFontIndex));
		}

		void CNEditBox::SetLimitText(unsigned int nLimit)
		{
			if(pSendEdit != NULL)
				pSendEdit->SetLimitText(nLimit);
		}

		void CNEditBox::Clear()
		{
			if(pSendEdit != NULL)
				pSendEdit->SetWindowText("");
		}

		void CNEditBox::OnSetFocus()
		{
			if(pSendEdit)
				pSendEdit->SetFocus();
		}

		BOOL CNEditBox::Draw(CDC *pDC)
		{
			if(pSendEdit == NULL)
				return FALSE;

			char buf[256]={0,};
			//SendEdit.GetLine(SendEdit.LineFromChar(-1), buf, 255);	// 현재 캐럿 위치의 문자열을 읽음

			pSendEdit->GetWindowText(buf, 255);

			m_strText.Format("%s", buf);

			CRect rc;

			rc.SetRect(m_Xp, m_Yp, m_Xp+m_Width, m_Yp+m_Height);

			CPoint cpnt = pSendEdit->PosFromChar(0);
			if(m_strText.GetLength()>0 && m_strText.GetAt(0)!=-1)
			{
				pDC->SetBkMode(TRANSPARENT);
				COLORREF colr = pDC->GetTextColor();

				pDC->SetTextColor(RGB(200, 255, 230));

				CFont *pOld = NULL;	
				pOld = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(m_nFontIndex));


				pDC->IntersectClipRect( rc );
				rc.left += cpnt.x;
				pDC->DrawText(m_strText, &rc, DT_LEFT);
				pDC->SelectClipRgn( NULL );//rc사이즈 만큼 클리핑 

				if(pOld)
					pDC->SelectObject(pOld);	

				pDC->SetTextColor(colr);
			}

			if( m_pPage == NULL )return FALSE;
			CPoint pnt = pSendEdit->GetCaretPos();

			pnt.x += m_Xp;
			pnt.y += m_Yp; //m_Yp+m_Height+ypos;

			static int caretCnt = 0;
			caretCnt++;
			if(caretCnt>20) {m_bShowCaret ^= 1; caretCnt = 0; }

			CWnd *pWnd  = pSendEdit->GetFocus();
			if( pWnd != NULL) {	
				if (::IsWindow(pWnd->m_hWnd))
				{			
					if(pWnd->m_hWnd == pSendEdit->m_hWnd)	// 포커스가 있을동안만 그려준다
					{
						//if(m_bShowCaret) DrawBoxXOR( pnt.x, pnt.y , pnt.x+1, pnt.y+12, (WORD*)m_pPage->lpBit);		//%%% 맞는지 체크 
						if(m_bShowCaret) m_pPage->DrawEmptyBox( pnt.x, pnt.y , pnt.x+1, pnt.y+12, 0, TRUE);

					}
				}
			}

			return TRUE;
		}


	} //namespace SKINGDI

}//namespace NMBASE