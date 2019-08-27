// NTextScroll.cpp: implementation of the CNTextScroll class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NTextScroll.h"

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

		CNTextScroll::CNTextScroll() : CNText()
		{
			m_pViewDC = NULL;	
		}


		CNTextScroll::CNTextScroll(CRect rt, char* szText, int nID, NMBASE::GRAPHICGDI::CPage* pPage,CDC *pDC)
			: CNText(rt, szText, nID, pPage, pDC)
		{

			m_pViewDC = pDC;
			SetText(szText);

			m_bScroll = FALSE;
			m_nScrollNum = 0;
			m_nCurScrollNum = 0;
			nReFlashTime = 0;
			m_bRollingScrool = FALSE;
			m_bContinueScrool = FALSE;
		}

		CNTextScroll::~CNTextScroll()
		{
		}

		void CNTextScroll::Init(CRect rt)
		{
			m_bScroll = FALSE;
			m_nScrollNum = 0;
			m_nCurScrollNum = 0;
		}

		void CNTextScroll::Clear()
		{
			m_strText= "";

			int i;
			for(i=0; i<static_cast<int>(m_lstScrollInfo.size()); i++) {
				m_lstScrollInfo[i].pBitmap->DeleteObject();
				delete m_lstScrollInfo[i].pBitmap;
			}

			m_lstScrollInfo.clear();
			nReFlashStartTime = 0;
		}

		void CNTextScroll::set_TextInfo( CFont *pFont , COLORREF color)
		{
			m_pFont = pFont;
			m_uColor = (DWORD)color;
		}

		void CNTextScroll::SetText(char* szText)
		{
			int nCut = 256;

			nReFlashStartTime = timeGetTime();

			CNText::SetText(szText);

			m_strText = szText;

			//	m_strText = "abcdefghijklmnopqrstuvwxyasdjflajfo2foijfosijlkfjslkdjfslkjfslkdjflakdjfalsdjfaldjfalkdjfasdkfja;slfdkjadlkfja;lsdjasldkjfaldjfalsdkjfz12345678asdkfjalsdjfaslkfdjasldkjfasdfkjas;dkfjas;lkfjas;dlkfjas;ldkjasldkfjasldj90ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz1234567890";

			if(szText == NULL)
				return;
			//	set_TextInfo 에서 받은 폰트를 사용함
			// 	if(CNText::m_pBasicFont == NULL)
			// 		return;

			int i;
			for(i=0; i<static_cast<int>(m_lstScrollInfo.size()); i++) {
				m_lstScrollInfo[i].pBitmap->DeleteObject();
				delete m_lstScrollInfo[i].pBitmap;
			}

			m_lstScrollInfo.clear();

			// 100바이트 단위로 나눔
			int nTotal = m_strText.GetLength()/nCut;

			if(m_strText.GetLength()%nCut != 0)
				nTotal++;

			//m_uColor = RGB(233,233,233);

			CDC* cdc = m_pViewDC;	
			if( cdc == NULL ) return;

			for(i=0; i<nTotal; i++) {
				int start;
				int end;

				start = i*nCut;
				end = nCut;

				if(i == nTotal-1 && m_strText.GetLength()%nCut != 0)
					end = m_strText.GetLength()%nCut;

				CString strTmp = m_strText.Mid(start, end);		
				CDC hdcMem;
				hdcMem.CreateCompatibleDC(cdc);

				// 문자열 넓이, 높이 구함
				CSize size;
				RECT rc;
				CFont *oldFont = hdcMem.SelectObject(CNText::m_pFont);
				size = hdcMem.GetTextExtent(strTmp);


				SCROLL_INFO stScroll;

				ZeroMemory(&stScroll, sizeof(SCROLL_INFO));

				int height = m_Rt.bottom-m_Rt.top;
				int width = m_Rt.right-m_Rt.left;

				// 비트맵 생성해서 비트맵에 글씨씀 
				stScroll.pBitmap = new CMyBitmap;
				stScroll.pBitmap->CreateCompatibleBitmap(cdc, size.cx, height);
				CMyBitmap* oldBitmap = (CMyBitmap*)hdcMem.SelectObject(stScroll.pBitmap);
				SetRect(&rc, 0, 0, size.cx, height);	
				hdcMem.SetBkColor(RGB(0,0,0));
				hdcMem.SetTextColor(m_uColor);		
				hdcMem.DrawText(strTmp, strTmp.GetLength(), &rc, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

				hdcMem.SelectObject(oldBitmap);
				hdcMem.SelectObject(oldFont);
				hdcMem.DeleteDC();

				stScroll.width = size.cx;
				stScroll.height = height;

				for(int j=0; j<static_cast<int>(m_lstScrollInfo.size()); j++) {
					stScroll.offsetX += m_lstScrollInfo[j].width;
				}
				stScroll.offsetX += width;

				m_lstScrollInfo.push_back(stScroll);
			}

		}

		void CNTextScroll::ADDSetText(char* szText)
		{
			if( strlen(szText) == 0 )return;
			StopScroll();
			m_strText +="   ";
			m_strText+=szText;
			SetText((char*)m_strText.operator LPCTSTR());
		}

		void CNTextScroll::StartScroll(int nNum , BOOL bRollingScrool/*TRUE 무한 반복*/)
		{
			m_nScrollNum = nNum;
			m_nCurScrollNum = 0;
			m_bScroll = TRUE;
			m_bRollingScrool = bRollingScrool;
			SetText((char*)m_strText.operator LPCTSTR());
			m_bContinueScrool = TRUE;
		}

		void CNTextScroll::ResumeScroll()
		{
			nReFlashStartTime = timeGetTime();
			m_bScroll = TRUE;	
		}

		void CNTextScroll::StopScroll()
		{
			nReFlashStartTime = 0;
			m_bScroll = FALSE;
		}

		BOOL CNTextScroll::DrawObject(CDC* pDC)
		{
			int i;	
			for(i=0; i<static_cast<int>(m_lstScrollInfo.size()); i++) {

				SCROLL_INFO stScroll = m_lstScrollInfo[i];

				stScroll.offsetX += 0.5f;

				if(stScroll.offsetX <= m_Rt.right-m_Rt.left) {


					if(stScroll.offsetX < 0 && abs((int)stScroll.offsetX) >= stScroll.width)
						continue;

					int posX;
					int posY;

					posX = m_Rt.left;
					posY = m_Rt.top;

					int x = (int) ( posX+ ((stScroll.offsetX > 0)? stScroll.offsetX : 0) );
					int y = posY;
					int width;
					int height;

					if(posX+stScroll.offsetX+stScroll.width > posX+(m_Rt.right-m_Rt.left))	// 넘어가는경우
						width= (posX+(m_Rt.right-m_Rt.left))-x;
					else
						width = (int)(stScroll.width + ((stScroll.offsetX < 0)? stScroll.offsetX : 0));

					height	= stScroll.height;

					int sx = (int) ( (stScroll.offsetX > 0)? 0 : -stScroll.offsetX );

					stScroll.pBitmap->TransDraw(pDC->m_hDC,x,y,width,m_Rt.Height(),sx,0,RGB(0,0,0));
				}		
			}

			return TRUE;
		}

		BOOL CNTextScroll::ProcessObject(DWORD time)
		{

			if(m_bVisible == FALSE)
				return FALSE;

			if(m_bScroll == FALSE){
				if( m_strText.GetLength() > 0 && static_cast<int>(m_lstScrollInfo.size()) > 0 && nReFlashTime >= 0 && nReFlashStartTime > 0){

					DWORD nCurtime = timeGetTime() - nReFlashStartTime;
					if( nCurtime >= nReFlashTime ){				
						m_bScroll = TRUE;
					}
					else {
						return TRUE;
					}
				}
				else{
					return TRUE;
				}		
			}

			int i;
			int total = 0;

			SCROLL_INFO *pStScroll;

			if(static_cast<int>(m_lstScrollInfo.size()) == 0)
				return TRUE;

			for(i=0; i<static_cast<int>(m_lstScrollInfo.size()); i++) {
				pStScroll = &m_lstScrollInfo[i];
				total += pStScroll->width;
				pStScroll->offsetX -= (float)time / 20.0f;
			}

			pStScroll = &m_lstScrollInfo[0];

			if (total < abs((int)pStScroll->offsetX) && pStScroll->offsetX < 0) {
				SetText((char*)m_strText.operator LPCTSTR());

				m_nCurScrollNum++;

				if(m_nCurScrollNum >= m_nScrollNum){
					if( m_bRollingScrool == TRUE ){
						StopScroll();
						nReFlashStartTime = timeGetTime();
						m_bContinueScrool = TRUE;//돌고 있는 상태다
					}
					else{				
						StopScroll();
						m_bContinueScrool = FALSE;// 멈춘 상태다
						return FALSE;
					}		
				}
			}


			return TRUE;
		}

	} //namespace SKINGDI

}//namespace NMBASE