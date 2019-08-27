// GrpChatEdit.h: interface for the CGrpChatEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRPCHATEDIT_H__54741C69_5699_4A53_9371_E8419DB371DF__INCLUDED_)
#define AFX_GRPCHATEDIT_H__54741C69_5699_4A53_9371_E8419DB371DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChatViewEdit.h"
#include "SkinVerticleScrollbar.h"
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CGrpChatEdit : public CChatViewEdit  
		{
		public:
			CGrpChatEdit();
			virtual ~CGrpChatEdit();


		public:
			//	CSkinVerticleScrollbar m_SkinVerticleScrollbar;


		public:
			void Init(void);			// 초기화 끝나고 불러준다.

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

	} //namespace SKINGDI

}//namespace NMBASE

#endif // !defined(AFX_GRPCHATEDIT_H__54741C69_5699_4A53_9371_E8419DB371DF__INCLUDED_)

