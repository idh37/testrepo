#pragma once

#include "skingdidefine.h"
#include "MyBitmap.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CSliderCtrlEx : public CSliderCtrl
		{
			DECLARE_DYNAMIC(CSliderCtrlEx)

		public:
			CSliderCtrlEx();
			virtual ~CSliderCtrlEx();

		protected:
			DECLARE_MESSAGE_MAP()
			BOOL OnSliderDrawThumb( CDC* pDC,  CRect& rect, UINT nState);
			BOOL OnSliderDrawChannel( CDC* pDC,  CRect& rect, UINT nState);
			BOOL OnSliderDrawTics( CDC* pDC,  CRect& rect, UINT nState){ return TRUE;};

		private:
			CMyBitmap m_bmpDrag;
			CMyBitmap m_bmpBg;
			CRect     m_Rect;

		public:
			void Init( CRect& rtPos, CString sBackBmpFileName, CString sThumbBmpFileName );

		public:
			afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

		protected:
			virtual void PreSubclassWindow();

		public:
			afx_msg BOOL OnEraseBkgnd(CDC* pDC);
			LRESULT OnNotifyTransparent( WPARAM wParam, LPARAM lParam);
			virtual BOOL PreTranslateMessage(MSG* pMsg);
		};

	} //namespace SKINGDI

}//namespace NMBASE