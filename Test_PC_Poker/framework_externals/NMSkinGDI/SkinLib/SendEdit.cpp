// SendEdit.cpp : implementation file
//

#include "stdafx.h"
#include "SendEdit.h"



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
		// CSendEdit

		CSendEdit::CSendEdit()
		{
			m_nUse=0;
			m_nNowHPos = 0;
			m_fnProcessChat=NULL;
		}

		CSendEdit::~CSendEdit()
		{
		}


		BEGIN_MESSAGE_MAP(CSendEdit, CEdit)
			//{{AFX_MSG_MAP(CSendEdit)
			ON_WM_KEYDOWN()
			ON_WM_CHAR()
			//}}AFX_MSG_MAP
		END_MESSAGE_MAP()

		/////////////////////////////////////////////////////////////////////////////
		// CSendEdit message handlers

		BOOL CSendEdit::PreCreateWindow(CREATESTRUCT& cs) 
		{
			// TODO: Add your specialized code here and/or call the base class
			if(m_nUse == 1)
			{
				//cs.style |= WS_CHILD | WS_BORDER;
				//cs.dwExStyle |= WS_EX_CLIENTEDGE;
			}

			return CEdit::PreCreateWindow(cs);
		}

		void CSendEdit::SetProcSendChatFn(FN_CALLBACK_PROCESSSENDCHAT fnProcessChat)
		{
			m_fnProcessChat = fnProcessChat;
		}

		void CSendEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
		{
			// TODO: Add your message handler code here and/or call default
			switch(nChar)
			{
			case VK_RETURN:
				{			
					if(nFlags != 28) return;

					char str[256]={0,};

					GetWindowText(str, 128);
					SetWindowText("");						// ����Ʈ ��Ʈ���� ����

					m_nNowHPos = 0;

					// �˻�
					if(strlen(str)==0) return;
					if(strlen(str)==1 && str[0]==-1) return;

					// ���� ���� �ٴ� ���� ���� �ڵ带 ������
					if(str[strlen(str)-1]==1) str[strlen(str)-1]=0;

					// ������ ���� Ư�����ڸ� ����Ͽ��ٸ� ���鹮�ڷ� �ٲ�
					int slen = strlen(str);
					for(int s=0; s<slen-1; s++) {
						// 8:backspace, 9:tab, 10:linefeed, 13:carriage return
						if(str[s] == 8 || str[s] == 9 || str[s] == 10 || str[s] == 13) {
							str[s] = ' ';
						}
					}

					// �����丮 ����
					for(int i=9; i>1; i--)
					{
						m_listHistory[i] = m_listHistory[i-1];
					}
					m_listHistory[1].Format("%s",str);
					m_listHistory[0].Format("");

					if (m_fnProcessChat) m_fnProcessChat(str);
				}
				break;
			case VK_UP:
				if(m_nNowHPos < 9)
				{
					m_nNowHPos++;
					SetWindowText(m_listHistory[m_nNowHPos]);
				}
				break;
			case VK_DOWN:
				if(m_nNowHPos > 0)
				{
					m_nNowHPos--;
					SetWindowText(m_listHistory[m_nNowHPos]);
				}
				break;
			}

			CEdit::OnKeyDown(nChar, nRepCnt, nFlags);

		}


		void CSendEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
		{
			// TODO: Add your message handler code here and/or call default
			// ���Ͱ� ���ö� ����� ��~ �Ҹ��� ������
			if(nChar == 13 && nFlags == 28) {
				// ���๮�� ��� ȨŰ �����°����� ��ü
				SendMessage(WM_KEYDOWN, VK_HOME, NULL);
				return;
			}
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}

	} //namespace SKINGDI

}//namespace NMBASE