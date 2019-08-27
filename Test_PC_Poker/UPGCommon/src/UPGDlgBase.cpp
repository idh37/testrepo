// MyVoteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UPGDlgBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CUPGDlgBase, CDialog)

BEGIN_MESSAGE_MAP(CUPGDlgBase, CDialog)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CUPGDlgBase, CDialog)
END_DISPATCH_MAP()

CUPGDlgBase::CUPGDlgBase(UINT nIDTemplate, CWnd* pParentWnd/* = NULL*/)
:	CDialog(nIDTemplate, pParentWnd),
	m_bModal(false)
{

}

CUPGDlgBase::~CUPGDlgBase()
{
}

BOOL CUPGDlgBase::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetTimer(TIMER_UPGDLGBASE_REDRAW, 10, NULL);
	return TRUE;
}

void CUPGDlgBase::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_UPGDLGBASE_REDRAW:
		{
			CWnd *pMainWnd = GM().GetMainWnd();
			pMainWnd->SendMessage(WM_TIMER, TIMER_UPGDLGBASE_REDRAW);
		}
		return;
	}

	CDialog::OnTimer(nIDEvent);
}

void CUPGDlgBase::OnDestroy()
{
	KillTimer(TIMER_UPGDLGBASE_REDRAW);
	CDialog::OnDestroy();
}

INT_PTR CUPGDlgBase::DoModal()
{
	m_bModal = true;
	return CDialog::DoModal();
}