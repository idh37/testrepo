// NText.h: interface for the CNText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NTEXT_H__B2F709F5_EDB2_4E9C_99EA_12918FFAAE32__INCLUDED_)
#define AFX_NTEXT_H__B2F709F5_EDB2_4E9C_99EA_12918FFAAE32__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		enum
		{
			//보유머니 
			PLAY_MONEY =0,	
			//변동 머니
			PLAY_CHANGEMONEY,
		};

		class NMSKINGDI_CLASS CNText
		{
		public:
			CNText();
			CNText(CRect rt, char* szText,  int nID, NMBASE::GRAPHICGDI::CPage* pPage , CDC *pDC);
			virtual ~CNText();

		public:

			CDC			m_cDC;
			NMBASE::GRAPHICGDI::CPage*		m_pPage;	
			static CFont*	m_pBasicFont;
			DWORD		m_uAlign;
			DWORD		m_uColor;
			CString		m_strText;
			CFont*		m_pFont;
			BOOL		m_bOutLine;
			DWORD		m_uOutColor;
			// GDI+
			BOOL		m_bAntiAlias;
			NMBASE::GRAPHICGDI::CPage		m_pFontPage;
			BOOL		_m_bDebugText;	
			CRect		m_Rt;
			BOOL		m_bVisible;
			BOOL		m_bPermitSameText;

		public:
			void CreateBasefont();

			void SetPermitSameText(BOOL bpermit){m_bPermitSameText = bpermit;};
			void ReSetPostion(CRect rt);
			void SetVisible(BOOL bshow);
			void SetAlign(DWORD align);
			void SetColor(DWORD color);
			virtual void SetText(char* szText);
			void SetText(const char *fmt, ...);
			void SetText(CString strText);
			char* GetText();
			void SetFont(CFont* pFont);

			void SetOutLine(DWORD outColor);
			void SetAntiAlias();
			BOOL DrawTextAntiAlias(CDC *pDC);

			void DrawText(CDC *pDC);

			void MakeFontPage();
			void SetDebugText();
		};

	} //namespace SKINGDI

}//namespace NMBASE

#endif // !defined(AFX_NTEXT_H__B2F709F5_EDB2_4E9C_99EA_12918FFAAE32__INCLUDED_)
