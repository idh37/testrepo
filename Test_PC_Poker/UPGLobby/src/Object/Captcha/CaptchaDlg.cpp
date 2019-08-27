
#include "stdafx.h"
#include "CaptchaDlg.h"

#define RECT_SSOWEB (CRect(10, 31, 10 + 400, 31 + 610))


CCaptchaDlg::CCaptchaDlg(CWnd* pParent /*=NULL*/)
: CUPGDlgBase(CCaptchaDlg::IDD, pParent),
m_captchaWeb(NULL)
{

}

CCaptchaDlg::~CCaptchaDlg()
{
}

BOOL CCaptchaDlg::SafeClose()
{
	if(!GetSafeHwnd())
	{
		return FALSE;
	}

	PostMessage(WM_COMMAND, IDOK);

	return TRUE;
}

CString CCaptchaDlg::GetAuthentiCode() const
{
	return m_authentiCode;
}

void CCaptchaDlg::SetAuthentiCode(CString authentiCode)
{
	m_authentiCode = authentiCode;
}

bool CCaptchaDlg::GetNeedCaptcha() const
{
	return m_needCaptcha;
}

void CCaptchaDlg::SetNeedCaptcha(bool needCaptcha)
{
	m_needCaptcha = needCaptcha;
}

void CCaptchaDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);

	DDX_Control(pDX, IDCANCEL, m_closeBtn);
}


BEGIN_MESSAGE_MAP(CCaptchaDlg, CUPGDlgBase)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CCaptchaDlg::OnInitDialog()
{
	CUPGDlgBase::OnInitDialog();

	NMBASE::UTIL::CDialogCtrlReposition ctrlRepos;
	ctrlRepos.Reposition(m_hWnd);

	m_backGround.LoadBitmapFile(".\\Lobby\\image\\Captcha\\captchaback.bmp");
	m_closeBtn.Init(386, 5, ".\\common\\image\\commonbtn\\Btn_X.bmp", 4);

	RECT rect;
	GetWindowRect(&rect);
	rect.right = rect.left + m_backGround.GetWidth();
	rect.bottom = rect.top + m_backGround.GetHeight();
	MoveWindow(&rect);

	if(!m_captchaWeb)
	{
		m_captchaWeb = new NMBASE::UTIL::CNMSSOWebBrowser();

		if(!m_captchaWeb->Create(WS_VISIBLE | WS_CHILD,  RECT_SSOWEB, GetSafeHwnd(), 0))
		{
			if(m_captchaWeb)
			{
				delete m_captchaWeb;
				m_captchaWeb = NULL;
			}

			return FALSE;
		}		
	}

	m_captchaWeb->SetCallback_OnBeforeNavigate2(SSO_OnBeforeNavigate2);
	m_captchaWeb->Navigate(getGameURL(IDX_GAMEURL_CAPTCHA));	

	return TRUE;
}

BOOL CCaptchaDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_SYSKEYDOWN)
	{
		if(pMsg->wParam == VK_F4)
		{
			return TRUE;
		}
	}

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CWnd::PreTranslateMessage( pMsg );
}

void CCaptchaDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	if(cdc.m_hDC == NULL) 
	{
		return;
	}

	CBitmap *pOldBmp = cdc.SelectObject(&m_backGround);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	cdc.DeleteDC();		
	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

void CCaptchaDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

void CCaptchaDlg::OnDestroy()
{
	CUPGDlgBase::OnDestroy();

	m_backGround.DeleteObject();
	
	if(m_captchaWeb)
	{
		delete m_captchaWeb;
		m_captchaWeb = NULL;
	}
}