// NText.cpp: implementation of the CNText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NText.h"

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

		CFont* CNText::m_pBasicFont = NULL;
		CNText::CNText()
		{
			if(m_pBasicFont == NULL)
				CreateBasefont();
		}

		CNText::CNText(CRect rt, char* szText, int nID, NMBASE::GRAPHICGDI::CPage* pPage , CDC *pDC )	
		{
			if( pPage == NULL )return;
			m_pPage = pPage;

			m_cDC.CreateCompatibleDC(pDC);

			//디폴트가 가지고 있던 텍스트랑 같더라도 다시 그리 못한다.
			m_bPermitSameText = FALSE;

			m_strText = szText;
			m_uAlign = DT_TOP | DT_LEFT | DT_SINGLELINE;
			m_uColor = RGB(0,0,0);

			if(m_pBasicFont == NULL)
				CreateBasefont();

			m_pFont = m_pBasicFont;
			m_bOutLine	= FALSE;
			m_uOutColor = 0;
			m_bAntiAlias = FALSE;
			m_bVisible = TRUE;

			m_Rt = rt;

			_m_bDebugText = FALSE;


			if(m_Rt.Width() %2 == 1) 
			{
				m_Rt.right ++;
				m_Rt.left --;
			}

			if(m_Rt.Height()%2 == 1) {
				m_Rt.top --;
				m_Rt.bottom ++;
			}


			m_pFontPage.Init(m_Rt.Width(), m_Rt.Height(), 16);
			m_cDC.SelectObject(m_pFontPage.hBmp);
			MakeFontPage();
		}

		CNText::~CNText()
		{
			if(m_cDC.m_hDC) m_cDC.DeleteDC();

			if(m_pBasicFont != NULL) {
				m_pBasicFont->DeleteObject();

				delete m_pBasicFont;
				m_pBasicFont = NULL;
			}

			m_pFontPage.Destroy();
		}

		void CNText::CreateBasefont()
		{

			m_pBasicFont = new CFont();

			m_pBasicFont->CreateFont(12,0,0,0,
				FW_NORMAL,
				//FW_BOLD,
				FALSE,FALSE,
				0,
				//ANSI_CHARSET,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS,
				"굴림체");
		}

		void CNText::SetAlign(DWORD align)
		{
			if(align == m_uAlign)
				return;

			if(m_uAlign & DT_VCENTER) {
				m_uAlign = align | DT_SINGLELINE;
			}
			else {
				m_uAlign = align;
			}

			MakeFontPage();	
		}

		void CNText::SetColor(DWORD color)
		{
			if(m_uColor == color)
				return;

			m_uColor = color;
			MakeFontPage();
		}

		void CNText::SetText(CString strText)
		{
			if( m_bPermitSameText == FALSE ){
				if(strText == m_strText)
					return;
			}

			m_strText = strText;

			MakeFontPage();
		}

		void CNText::SetText(char* szText)
		{
			if(szText) 
			{		
				if( m_bPermitSameText == FALSE )				
				{
					if(strcmp(m_strText.operator LPCTSTR(), szText) == 0)
						return;
				}

				m_strText = szText;

				MakeFontPage();	
			}
		}

		void CNText::SetText(const char *fmt, ...)
		{
			char buf[512];

			va_list va;
			va_start(va, fmt);
			_vsnprintf(buf, 512, fmt, va);
			va_end(va);
			buf[511]=0;

			if( m_bPermitSameText == FALSE )
			{
				if(strcmp(m_strText.operator LPCTSTR(), buf) == 0)
					return;
			}	

			m_strText = buf;

			MakeFontPage();	
		}

		char* CNText::GetText()
		{
			return (char*)m_strText.operator LPCTSTR();
		}

		void CNText::SetFont(CFont* pFont)
		{
			m_pFont = pFont;
		}

		void CNText::SetOutLine(DWORD outColor)
		{
			m_bOutLine = TRUE;
			m_uOutColor = outColor;
		}

		void CNText::SetAntiAlias()
		{
			m_bAntiAlias = TRUE;
		}

		void CNText::SetVisible(BOOL bshow){
			m_bVisible = bshow;
		}

		void CNText::MakeFontPage()
		{
			//	if(Game.GameCnt % 2 == 0) 
			//		return;

			if(_m_bDebugText == TRUE)	// 디버그창 텍스트 이면 리턴
				return;

			if(m_cDC.m_hDC == NULL) 
				return;

			m_bVisible = TRUE;
			// FontPage
			m_cDC.SetBkMode(TRANSPARENT);

			m_pFontPage.DrawFillBox(0, 0, m_Rt.Width(), m_Rt.Height(), COLOR_555(RGB(255, 0, 255)));

			////////////////////////////////////////////////////////
			m_cDC.SetTextColor(m_uColor);
			CGdiObject* pOld;
			if(m_pFont)
				pOld = m_cDC.SelectObject(m_pFont);

			CRect rect;
			rect.SetRect(0, 1, m_Rt.Width(), m_Rt.Height());

			if(m_bOutLine) 
			{
				CRect rcTmp;
				m_cDC.SetTextColor(m_uOutColor);
				rcTmp = rect; rcTmp.OffsetRect(-1, 0);
				m_cDC.DrawText(m_strText, m_strText.GetLength(), &rcTmp, m_uAlign);
				rcTmp = rect; rcTmp.OffsetRect(0, -1);
				m_cDC.DrawText(m_strText, m_strText.GetLength(), &rcTmp, m_uAlign);
				rcTmp = rect; rcTmp.OffsetRect(1, 0);
				m_cDC.DrawText(m_strText, m_strText.GetLength(), &rcTmp, m_uAlign);
				rcTmp = rect; rcTmp.OffsetRect(0, 1);
				m_cDC.DrawText(m_strText, m_strText.GetLength(), &rcTmp, m_uAlign);
				m_cDC.SetTextColor(m_uColor);
			}

			m_cDC.DrawText(m_strText, m_strText.GetLength(), &rect, m_uAlign);
		}

		void CNText::DrawText(CDC *pDC)
		{	
			if( m_bVisible == FALSE )
				return;

			m_pPage->PutFromPageWithColorKey(m_Rt.left,m_Rt.top, &m_pFontPage, COLOR_555(RGB(255, 0, 255)));

			//	g_pGameView->Page.DrawFillBoxGB(m_Rt.left,m_Rt.top,m_Rt.right,m_Rt.bottom,RGBmix( 0, 2, 5 ),3);
		}



		void CNText::ReSetPostion(CRect rt)
		{	
			m_Rt = rt;
			if(m_Rt.Width() %2 == 1)
			{
				m_Rt.right ++;
				m_Rt.left --;
			}

			if(m_Rt.Height()%2 == 1) 
			{
				m_Rt.top --;
				m_Rt.bottom ++;
			}

			m_pFontPage.Destroy();
			m_pFontPage.Init(m_Rt.Width(), m_Rt.Height(), 16);
			m_cDC.SelectObject(m_pFontPage.hBmp);
			MakeFontPage();
		}

	} //namespace SKINGDI

}//namespace NMBASE