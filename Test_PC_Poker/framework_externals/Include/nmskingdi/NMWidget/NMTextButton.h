#pragma once
#include "nmbutton.h"
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CNMTextButton : public CNMButton
		{
		protected:
			CString m_strText;

			COLORREF m_colorOver;
			COLORREF m_colorDown;
			COLORREF m_colorDisable;
			COLORREF m_colorNormal;

			CFont *m_pFont;
			int		m_nCharacterExtra;		//자간조정

			int		m_nTextOffsetX;			//텍스트 오프셋 X
			int		m_nTextOffsetY;			//텍스트 오프셋 Y

		public:

			CNMTextButton(void);
			virtual ~CNMTextButton(void);

			virtual BOOL Initialize( int nControlID,
				CWnd* pParent,
				int nPosX, int nPosY,
				int nWidth, int nHeight );

			virtual BOOL Initialize( int nControlID,
				CWnd* pParent,
				int nPosX, int nPosY,
				char* szSprKey,
				int nSprID,
				int nSprCount );

			virtual void SetFont(CFont *pFont);
			virtual void SetText( const char *pText );
			virtual void SetTextOffset(int nTextOffsetX, int nTextOffsetY);
			virtual void SetTextCharacterExtra(int extra);
			virtual const char* GetText();
			virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );	


			void SetTextColor(COLORREF colorNormal, COLORREF colorOver=0, COLORREF colorDown=0, COLORREF colorDisable=0);


		};


	} //namespace SKINGDI

}//namespace NMBASE