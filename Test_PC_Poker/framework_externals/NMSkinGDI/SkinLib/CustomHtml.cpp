// CustomHtml.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CustomHtml.h"

#include <atlbase.h>
#pragma warning(push)
#pragma warning(disable: 4099)
#pragma warning(disable: 4146)
#pragma warning(disable: 4192)
#import <mshtml.tlb> rename("TranslateAccelerator", "TranslateAcceleratorA")
#pragma warning(default: 4192)
#pragma warning(default: 4049)
#pragma warning(default: 4146)
#pragma warning(pop)



namespace NMBASE
{
	namespace SKINGDI
	{
		// CCustomHtml

		IMPLEMENT_DYNCREATE(CCustomHtml, CHtmlView)

			CCustomHtml::CCustomHtml()
		{

		}

		CCustomHtml::~CCustomHtml()
		{
		}

		void CCustomHtml::DoDataExchange(CDataExchange* pDX)
		{
			CHtmlView::DoDataExchange(pDX);
		}

		BEGIN_MESSAGE_MAP(CCustomHtml, CHtmlView)
		END_MESSAGE_MAP()


		// CCustomHtml 진단입니다.

#ifdef _DEBUG
		void CCustomHtml::AssertValid() const
		{
			CHtmlView::AssertValid();
		}

		void CCustomHtml::Dump(CDumpContext& dc) const
		{
			CHtmlView::Dump(dc);
		}
#endif //_DEBUG


		// CCustomHtml 메시지 처리기입니다.


		void CCustomHtml::SetNoBorder()
		{
			MSHTML::IHTMLDocument2Ptr doc = (MSHTML::IHTMLDocument2Ptr)GetHtmlDocument();
			if(doc!=NULL) {
				MSHTML::IHTMLElementPtr body = (MSHTML::IHTMLElementPtr)doc->body;      
				if(body!=NULL)
				{
					body->style->border = _T("none");
				}
			}

		}
		void CCustomHtml::InsertHtml(const char *szHtml)
		{

			MSHTML::IHTMLDocument2Ptr doc = (MSHTML::IHTMLDocument2Ptr)GetHtmlDocument();
			if(doc == NULL) return;
			MSHTML::IHTMLElementPtr body = (MSHTML::IHTMLElementPtr)doc->body;      
			if(body == NULL) return;

			body->insertAdjacentHTML("BeforeEnd", (LPCTSTR)szHtml);

		}


		void CCustomHtml::SetStyle(const char *szCSS)
		{
			/*	MSHTML::IHTMLDocument2Ptr doc = (MSHTML::IHTMLDocument2Ptr)GetHtmlDocument();
			if(doc == NULL) return;
			MSHTML::IHTMLElementPtr body = (MSHTML::IHTMLElementPtr)doc->body;      
			if(body == NULL) return;

			CString strCSS = szCSS;

			body->style->put_cssText(strCSS.AllocSysString());
			*/
			//	CComPtr<IHTMLStyleSheet> spStyle;
			//CComBSTR bstrValue(_T(""));
			CString str;
			BSTR bstrValue=str.AllocSysString();
			MSHTML::IHTMLDocument2Ptr spDoc = (MSHTML::IHTMLDocument2Ptr)GetHtmlDocument();
			MSHTML::IHTMLStyleSheetPtr spStyle = (MSHTML::IHTMLStyleSheetPtr) spDoc->createStyleSheet(bstrValue, 0);
			if(spStyle != NULL)
			{
				str = szCSS;
				spStyle->put_cssText(str.AllocSysString());
				spStyle = NULL;
			}


		}

	} //namespace SKINGDI

}//namespace NMBASE