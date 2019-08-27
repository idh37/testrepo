#include "StdAfx.h"
#include "./NMWidget/NMTextButton.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		CNMTextButton::CNMTextButton(void)
		{

			m_colorOver=0;
			m_colorDown=0;
			m_colorNormal=0;
			m_colorDisable=0;
			m_nCharacterExtra=0;	//자간
			m_nTextOffsetX =0;
			m_nTextOffsetY =0;

			m_pFont=NULL;
		}

		CNMTextButton::~CNMTextButton(void)
		{
		}

		void CNMTextButton::SetTextOffset(int nTextOffsetX, int nTextOffsetY)
		{
			m_nTextOffsetX = nTextOffsetX;
			m_nTextOffsetY = nTextOffsetY;

		}

		void CNMTextButton::SetText( const char *pText )
		{
			m_strText = pText;
		}

		void CNMTextButton::SetTextCharacterExtra(int extra)
		{
			m_nCharacterExtra = extra;
		}

		const char* CNMTextButton::GetText()
		{
			return m_strText.operator LPCTSTR();
		}

		void CNMTextButton::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
		{
			if( m_bShow == FALSE ) return;

			CNMButton::DrawWidget(Page, DC);

			//DC.Rectangle(m_rectPosition);

			CFont *pOldFont = NULL;
			if (m_pFont)
				pOldFont=DC.SelectObject(m_pFont);

			COLORREF oldcolor = DC.GetTextColor();
			//자간설정
			int oldextra = ::SetTextCharacterExtra(DC.m_hDC, m_nCharacterExtra);


			CRect rectText = m_rectPosition;
			rectText.OffsetRect(m_nTextOffsetX, m_nTextOffsetY);


			switch(m_nCurSprID- m_nSprID)
			{
			case 0:	//정상
				DC.SetTextColor(m_colorNormal);
				DC.DrawText(m_strText, rectText, DT_CENTER| DT_VCENTER|DT_SINGLELINE );
				break;
			case 1:	//오버
				DC.SetTextColor(m_colorOver);
				DC.DrawText(m_strText, rectText, DT_CENTER| DT_VCENTER|DT_SINGLELINE );
				break;
			case 2: //다운
				{			
					rectText.OffsetRect(1,1);
					DC.SetTextColor(m_colorDown);
					DC.DrawText(m_strText, rectText, DT_CENTER| DT_VCENTER|DT_SINGLELINE );
				}
				break;
			case 3: //비활성

				DC.SetTextColor(m_colorDisable);
				DC.DrawText(m_strText, m_rectPosition, DT_CENTER| DT_VCENTER|DT_SINGLELINE );
				break;
			}

			//	Page.DrawEmptyBox(m_rectPosition.left, m_rectPosition.top, m_rectPosition.right, m_rectPosition.bottom, NMBASE::GRAPHICGDI::xSprite::RGB555(240,0,0));

			::SetTextCharacterExtra(DC.m_hDC, oldextra);
			DC.SetTextColor(oldcolor);


			if (pOldFont)
				DC.SelectObject(pOldFont);

		}


		void CNMTextButton::SetTextColor(COLORREF colorNormal, COLORREF colorOver, COLORREF colorDown, COLORREF colorDisable)
		{
			m_colorOver=colorOver;
			m_colorDown=colorDown;
			m_colorDisable=colorDisable;
			m_colorNormal=colorNormal;
		}

		void CNMTextButton::SetFont(CFont *pFont)
		{
			m_pFont = pFont;
		}

		BOOL CNMTextButton::Initialize( int nControlID,
			CWnd* pParent,
			int nPosX, int nPosY,
			int nWidth, int nHeight )
		{
			m_nSprCount = 4;
			return CNMInvisibleButton::Initialize( nControlID, pParent, nPosX, nPosY, nWidth, nHeight );
		}

		BOOL CNMTextButton::Initialize( int nControlID,
			CWnd* pParent,
			int nPosX, int nPosY,
			char* szSprKey,
			int nSprID,
			int nSprCout )
		{

			return CNMButton::Initialize( nControlID, pParent, nPosX, nPosY, szSprKey, nSprID , nSprCout);


		}


	} //namespace SKINGDI

}//namespace NMBASE