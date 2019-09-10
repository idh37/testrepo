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
			// ������ �ٲ� �ý�Ʈ�� ������ġ�� ����
			// GetLength()�� ������ �ʴ� ������ Windows2000������ ���� �� ũ�� �����⶧��
			// ��ũ�� �ڽ��� ���� �ؿ� �ִ��� Ȯ��
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

			// 	//�޼��� ���� ���� 2009.01.12
			CHARFORMAT cf;
			cf.cbSize		= sizeof(CHARFORMAT);
			cf.dwMask		= CFM_COLOR | CFM_UNDERLINE | CFM_BOLD;

			// ���� boldü�� ����Ϸ��� �Ķ���Ͱ����� bBold���� �޾ƿ��� �̺κ��� �ּ��� Ǭ��.
			cf.dwEffects =(unsigned long) ~( CFE_AUTOCOLOR | CFE_UNDERLINE | CFE_BOLD);
			cf.crTextColor	= crNewColor;//RGB(0, 0, 0);

			//bchangestate �ؽ�Ʈ�� ���°� ����ɼ��� �ִ°͵� ( ��ȭ���� ����� ��� �߰��� ����Ѵ�)
			// TRUE�� ���°� ���Ҽ� �ִ°͵��̴�. ������ ������ ������ .NET������ �����ϴ�

			int iEndPos = iStartPos + strlen(str);
			SetSel(iTotalTextLength, iTotalTextLength);
			SetSelectionCharFormat(cf);
			ReplaceSel(str);
			HideSelection(TRUE, FALSE);	
			SetSel(-1, -1);

			const int MAX_RICHCHARSIZE = 1024;

			// Over Flow ���� �� �߰��� �� ��ŭ ���ٿ��� �����Ѵ�
			if (iTotalTextLength > MAX_RICHCHARSIZE)
			{
				SetSel(0, iEndPos - iStartPos);
				ReplaceSel("");
			}

			int srMin, srMax;
			GetScrollRange(SB_VERT, &srMin, &srMax);
			if (srMax > 0)
			{
				// ��Ŀ���� �����ְ� ���ڷ� �ڽ��� ���� �ؿ� ���� ������
				// �ٽ� �����ִ� ��ġ�� ��ũ���Ѵ�
				if (GetFocus() == this && !bEndPos)
				{
					UINT wParam = MAKELONG((WORD)SB_THUMBPOSITION, nPos);
					SendMessage(WM_VSCROLL, wParam, NULL);
					ShowCaret();
				}
				// �� �̿ܿ��� ���� ���� ������ ��ũ���Ѵ�
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

		// ��ũ�� �ɸ��� �ؽ�Ʈ�� �߰��Ѵ�.
		void CChatViewEdit::AddLinkText( CString str, DWORD nParam )
		{
			CHARFORMAT2 cf;
			cf.cbSize = sizeof(CHARFORMAT2);
			cf.dwMask = CFM_LINK | CFM_COLOR |CFM_BOLD;
			cf.dwEffects = CFE_LINK |CFM_BOLD;
			cf.crTextColor = nParam;	// TextColor�� nParam���� �����Ѵ�

			int iTotalTextLength = GetWindowTextLength();

			SetSel(iTotalTextLength, iTotalTextLength);
			SetSelectionCharFormat(cf);
			ReplaceSel(str);
			SetSel(-1, -1);
		}

	} //namespace SKINGDI

}//namespace NMBASE