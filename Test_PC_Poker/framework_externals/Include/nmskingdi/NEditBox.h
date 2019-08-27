// NEditBox.h: interface for the CNEditBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEDITBOX_H__B70674F8_1B7F_4D27_92B1_E4254D200AAF__INCLUDED_)
#define AFX_NEDITBOX_H__B70674F8_1B7F_4D27_92B1_E4254D200AAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SendEdit.h"
#include "NText.h"
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		class CNSendEdit;

		class NMSKINGDI_CLASS CNEditBox : public CNText
		{
		protected:
			NMBASE::UTIL::STD_FONT			m_nFontIndex;
		public:
			CNEditBox();	
			virtual ~CNEditBox();

		public:
			CWnd*				m_pParentWnd;
			NMBASE::GRAPHICGDI::CPage				*m_pPage;		
			CSendEdit			*pSendEdit;

			BOOL				m_bShowCaret;
			int					m_Xp;
			int					m_Yp;
			int					m_Width;
			int					m_Height;
			void Init(int x, int y ,int w, int h, CWnd *pParentWnd =NULL , NMBASE::GRAPHICGDI::CPage *pPage = NULL);
			void SetFontIndex(NMBASE::UTIL::STD_FONT nFontIndex);
			void Clear();
			void SetLimitText(unsigned int nLimit);
			void OnSetFocus();	
			BOOL Draw(CDC *pDC);
		};

	} //namespace SKINGDI

}//namespace NMBASE

#endif // !defined(AFX_NEDITBOX_H__B70674F8_1B7F_4D27_92B1_E4254D200AAF__INCLUDED_)
