// GrpChatEdit2.h: interface for the CGrpChatEdit2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRPCHATEDIT2_H__B5BFE73C_D077_4D94_BC0E_93BD80213975__INCLUDED_)
#define AFX_GRPCHATEDIT2_H__B5BFE73C_D077_4D94_BC0E_93BD80213975__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "skingdidefine.h"
#include "ChatViewEdit.h"
#include "SkinVerticleScrollbar2.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CGrpChatEdit2 : public CChatViewEdit  
		{
		public:
			CGrpChatEdit2();
			virtual ~CGrpChatEdit2();


		public:
			CSkinVerticleScrollbar2 m_SkinVerticleScrollbar;


		public:
			virtual void Init(BOOL bViewScroll=TRUE);			// 초기화 끝나고 불러준다.

		public:
			// Overrides
			// ClassWizard generated virtual function overrides
			//{{AFX_VIRTUAL(CSkinListCtrl)
		protected:
			virtual void PreSubclassWindow();
			//}}AFX_VIRTUAL

			// Generated message map functions
		protected:
			//{{AFX_MSG(CSkinListCtrl)
			afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
			afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
			afx_msg void OnPaint();
			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg void OnFontChange();
			//}}AFX_MSG
			DECLARE_MESSAGE_MAP()

		protected:
			void UpdateScrollSize(void);
		};

	}//namespace SKINGDI

}//namespace NMBASE

#endif // !defined(AFX_GRPCHATEDIT2_H__B5BFE73C_D077_4D94_BC0E_93BD80213975__INCLUDED_)
