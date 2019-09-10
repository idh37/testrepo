#pragma once
#include <afxhtml.h>


#ifdef _WIN32_WCE
#error "Windows CE������ CHtmlView�� �������� �ʽ��ϴ�."
#endif 

#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{
		// CCustomHtml HTML ���Դϴ�.

		class NMSKINGDI_CLASS CCustomHtml : public CHtmlView
		{
			DECLARE_DYNCREATE(CCustomHtml)

		public:
			CCustomHtml();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

			DECLARE_MESSAGE_MAP()
		};

	} //namespace SKINGDI

}//namespace NMBASE
