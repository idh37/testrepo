// MyRichEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ChatViewEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




namespace NMBASE
{
	namespace SKINGDI
	{
		/////////////////////////////////////////////////////////////////////////////
		// CChatViewEdit

		CChatViewEdit::CChatViewEdit()
		{
			oldline = 0;
		}

		CChatViewEdit::~CChatViewEdit()
		{
		}


		BEGIN_MESSAGE_MAP(CChatViewEdit, CRichEditCtrl)
			//{{AFX_MSG_MAP(CChatViewEdit)
			ON_WM_SETFOCUS()
			//}}AFX_MSG_MAP
		END_MESSAGE_MAP()

		/////////////////////////////////////////////////////////////////////////////
		// CChatViewEdit message handlers


		BOOL CChatViewEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
		{
			if (!AfxInitRichEdit2())
				return FALSE;

			CWnd* pWnd = this;
			return pWnd->Create(_T("RICHEDIT20A"), NULL, dwStyle, rect, pParentWnd, nID);
		}

		void CChatViewEdit::SetTextFormat(int kind)
		{
			/*
			CHARFORMAT cf;
			cf.cbSize = sizeof(cf);

			SetWordCharFormat(cf);


			switch(kind)
			{
			case 0:
			GetDefaultCharFormat(cf);
			cf.dwMask = CFM_BOLD | CFM_CHARSET | CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_OFFSET | CFM_PROTECTED | CFM_SIZE | CFM_STRIKEOUT | CFM_UNDERLINE;
			cf.dwEffects &= !CFE_UNDERLINE & !CFE_AUTOCOLOR;
			break;
			case 1:
			GetDefaultCharFormat(cf);
			cf.dwMask |= CFM_BOLD;
			cf.dwEffects |= CFE_BOLD;
			break;
			}

			SetWordCharFormat(cf);
			*/
		}

		void CChatViewEdit::AddText(CString *str, COLORREF crNewColor, BOOL bScroll,BOOL bchangestate)
		{
			AddText( (char*)str->operator LPCTSTR(), crNewColor, bScroll , TRUE);
		}

		void CChatViewEdit::AddText(char *str /* str2 */,  COLORREF crNewColor, BOOL bScroll,BOOL bchangestate) {
			// 색상을 바꿀 택스트의 시작위치를 구함
			// GetLength()로 구하지 않는 이유는 Windows2000에서는 값이 더 크게 나오기때문
			// 스크롤 박스가 제일 밑에 있는지 확인
			BOOL bEndPos = TRUE;
			int nPos = GetScrollPos(SB_VERT);
			int nLimit = GetScrollLimit(SB_VERT);
			if (nPos < nLimit - 2)
			{
				bEndPos = FALSE;
			}

			int iStartPos = LineIndex(max(GetLineCount() - 1, 0));
			int iTotalTextLength = GetWindowTextLength();
			SetSel(-1, -1);

			// 	//메세지 색깔 통일 2009.01.12
			CHARFORMAT cf;
			cf.cbSize		= sizeof(CHARFORMAT);
			cf.dwMask		= CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;

			// 만약 bold체를 사용하려면 파라미터값으로 bBold값을 받아오고 이부분의 주석을 푼다.
			cf.dwEffects =(unsigned long) ~( CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
			cf.crTextColor	= crNewColor;//RGB(0, 0, 0);

			//bchangestate 텍스트의 상태가 변경될수도 있는것들 ( 대화내용 지우기 기능 추가로 사용한다)
			// TRUE면 상태가 변할수 있는것들이다. 값들을 가지고 오려면 .NET에서만 가능하다

			int iEndPos = iStartPos + strlen(str);
			SetSel(iTotalTextLength, iTotalTextLength);
			SetSelectionCharFormat(cf);
			ReplaceSel(str);
			HideSelection(TRUE, FALSE);	
			SetSel(-1, -1);

			const int MAX_RICHCHARSIZE = 1024;

			// Over Flow 됐을 때 추가될 양 만큼 윗줄에서 삭제한다
			if (iTotalTextLength > MAX_RICHCHARSIZE)
			{
				SetSel(0, iEndPos - iStartPos);
				ReplaceSel("");
			}

			int srMin, srMax;
			GetScrollRange(SB_VERT, &srMin, &srMax);
			if (srMax > 0)
			{
				// 포커스가 잡혀있고 스코롤 박스가 제일 밑에 있지 않으면
				// 다시 보고있는 위치로 스크롤한다
				if (GetFocus() == this && !bEndPos)
				{
					UINT wParam = MAKELONG((WORD)SB_THUMBPOSITION, nPos);
					SendMessage(WM_VSCROLL, wParam, NULL);
					ShowCaret();
				}
				// 그 이외에는 전부 제일 밑으로 스크롤한다
				else
				{
					UINT wParam = SB_BOTTOM;
					SendMessage(WM_VSCROLL, wParam, NULL);
				}
			}
		}


		void CChatViewEdit::OnSetFocus(CWnd* pOldWnd) 
		{
			CRichEditCtrl::OnSetFocus(pOldWnd);
			::SetFocus(pOldWnd->m_hWnd);

			// TODO: Add your message handler code here
		}

		LRESULT CChatViewEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
		{
			// TODO: Add your specialized code here and/or call the base class

			return CRichEditCtrl::WindowProc(message, wParam, lParam);
		}

		// 링크가 걸리는 텍스트를 추가한다.
		void CChatViewEdit::AddLinkText( CString str, DWORD nParam )
		{
			CHARFORMAT2 cf;
			cf.cbSize = sizeof(CHARFORMAT2);
			cf.dwMask = CFM_LINK | CFM_COLOR |CFM_BOLD;
			cf.dwEffects = CFE_LINK |CFM_BOLD;
			cf.crTextColor = nParam;	// TextColor에 nParam값을 저장한다

			int iTotalTextLength = GetWindowTextLength();

			SetSel(iTotalTextLength, iTotalTextLength);
			SetSelectionCharFormat(cf);
			ReplaceSel(str);
			SetSel(-1, -1);
		}

	} //namespace SKINGDI

}//namespace NMBASE