#pragma once
#include <afxhtml.h>


#ifdef _WIN32_WCE
#error "Windows CE에서는 CHtmlView가 지원되지 않습니다."
#endif 

#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{
		// CCustomHtml HTML 뷰입니다.

		class NMSKINGDI_CLASS CCustomHtml : public CHtmlView
		{
			DECLARE_DYNCREATE(CCustomHtml)

		public:
			CCustomHtml();           // 동적 만들기에 사용되는 protected 생성자입니다.
			virtual ~CCustomHtml();

		public:
#ifdef _DEBUG
			virtual void AssertValid() const;
			virtual void Dump(CDumpContext& dc) const;
#endif

			void SetStyle(const char *szCSS);
			void InsertHtml(const char *szHtml);
			void SetNoBorder();

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

			DECLARE_MESSAGE_MAP()
		};

	} //namespace SKINGDI

}//namespace NMBASE
