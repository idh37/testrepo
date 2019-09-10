#include "StdAfx.h"
#include "PostBoxDlg.h"

IMPLEMENT_DYNAMIC(CPostBoxDlg, CUPGDlgBase)

#define ID_POSTBOXDLG		50001

CPostBoxDlg::CPostBoxDlg(CWnd* pParent /*=NULL*/)
	: CUPGDlgBase(CPostBoxDlg::IDD, pParent)
{
	m_pBrowser=NULL;
	EnableAutomation();
}

CPostBoxDlg::~CPostBoxDlg()
{
}

void CPostBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_CloseBtn);
}

BEGIN_MESSAGE_MAP(CPostBoxDlg, CUPGDlgBase)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPostBoxDlg, CUPGDlgBase)
	/*
	�ܺ� ActiveX ��� �Ҷ�
	DISP_FUNCTION(Ŭ������, �ܺ��Լ���, �ɹ��Լ���, ����Ÿ��, ���ڰ�1 ���ڰ�2) 
	���� ) http://msdn.microsoft.com/ko-kr/library/cc451431(VS.71).aspx
	*/
	DISP_FUNCTION( CPostBoxDlg, "ClosePopup", ClosePopup, VT_EMPTY, VTS_I2 )
END_DISPATCH_MAP()

// CLobbyEventDlg �޽��� ó�����Դϴ�.

void CPostBoxDlg::DestroyBrowser()
{
	if ( m_pBrowser )
	{
		m_pBrowser->Navigate("about:blank");
		delete m_pBrowser;
		m_pBrowser = NULL;
	}
}

BOOL CPostBoxDlg::CreateBrowser()
{
	DestroyBrowser();

	// CreateWebPage
	CRect rt;
	rt.SetRect( 0, 0, 596, 640 );
	rt.OffsetRect( 2, 40);

	m_pBrowser = new NMBASE::UTIL::CWebCtrl();
	if( !m_pBrowser->Create(NULL, "WebControl", WS_VISIBLE|WS_CHILD, rt, this, ID_POSTBOXDLG) )
	{ 
		if ( m_pBrowser )
		{
			delete m_pBrowser;
			m_pBrowser = NULL;
		}
		return FALSE;
	}
	m_pBrowser->ShowWindow(SW_SHOW);

	char szTemp[256] = {0, };
	char strEn[256] = {0,};

	sprintf_s( szTemp, 256, "%s%s", GM().GetMyInfo()->UI.UniqNo, "test123" );
	Md5(szTemp, strEn);

	CString strRet ="";
	CString strNick = GM().GetMyInfo()->UI.NickName;
	strNick = ANSIToUTF8( strNick.GetString() );
	strNick = URLencode( strNick.GetString() );
	CString strNick1 = m_strFriendName;
	strNick1 = ANSIToUTF8( strNick1.GetString() );
	strNick1 = URLencode( strNick1.GetString() );

	m_mapIDAndNickName.clear();

	if(m_bGroup)
	{
		CCommunityManager::Instance()->GetGroupInIDAndNickName( GM().GetMyInfo()->UI.ID, m_mapIDAndNickName);
	}

	if( strlen( m_strFriendName ) > 0 || m_bGroup == true )
	{
		if( m_bGroup )
		{
			strRet.Format( "token=%s&iNickname=%s&iId=%s&iCn=%s&rNickname=%s&gameType=42&subGameType=%d",strEn,strNick,GM().GetMyInfo()->UI.ID,GM().GetMyInfo()->UI.UniqNo, strNick1, g_GetGameCode(GM().GetCurrentGameType()) );
		}
		else
		{
			strRet.Format( "token=%s&iNickname=%s&iId=%s&iCn=%s&rNickname=%s&gameType=42&subGameType=%d",strEn,strNick,GM().GetMyInfo()->UI.ID,GM().GetMyInfo()->UI.UniqNo, strNick1, g_GetGameCode(GM().GetCurrentGameType()) );
		}
	}
	else
	{
		strRet.Format( "token=%s&iNickname=%s&iId=%s&iCn=%s&gameType=42&subGameType=%d",strEn,strNick,GM().GetMyInfo()->UI.ID,GM().GetMyInfo()->UI.UniqNo, g_GetGameCode(GM().GetCurrentGameType()) );
	}
	m_pBrowser->Navigate(m_strURL,0,NULL,"Content-Type: application/x-www-form-urlencoded\r\n",(LPVOID)strRet.GetString(),0);
	m_pBrowser->SetOption(TRUE);

	m_pBrowser->SetHeight(630);
	m_pBrowser->SetWidth(616);
	return TRUE;
}

BOOL CPostBoxDlg::OnInitDialog()
{
	CUPGDlgBase::OnInitDialog();
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	if (m_BackBit.LoadBitmapFile(".\\Lobby\\Image\\PostBoxDlg\\Back.bmp")==FALSE)
		return FALSE;
	m_CloseBtn.Init( 560, 2, ".\\Lobby\\image\\PostBoxDlg\\btn_close_01.bmp", 4 ); 

	MoveWindow(0,0,m_BackBit.GetWidth() ,m_BackBit.GetHeight());
	CreateBrowser();

	m_BackBit.ResizeWnd(this, 0, 0, TRUE, GM().GetMainWnd());
	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CPostBoxDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	dc.SetBkMode(TRANSPARENT);
	// TODO: Add your message handler code here
	m_BackBit.BitBlt( dc );
	CRect rect;
	CString sCur;

	dc.SelectObject( &NMBASE::UTIL::GetMyFont( NMBASE::UTIL::FONT_12_BOLD ));
	dc.SetTextColor( RGB( 39, 83, 56) );
	rect.SetRect(16, 13, 16+65, 13+12);

	dc.DrawText( "������", rect, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS);
	// Do not call CDialog::OnPaint() for painting messages
}

void CPostBoxDlg::OnDestroy()
{
	MM().Call(UPG_MEMO_CHECK_START);
	CUPGDlgBase::OnDestroy();
	
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	DestroyBrowser();
	
	if (m_BackBit.m_hObject) m_BackBit.DeleteObject();
}

BOOL CPostBoxDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		return TRUE;
	}

	return CUPGDlgBase::PreTranslateMessage(pMsg);
}

void CPostBoxDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

int CPostBoxDlg::DoModal(LPCTSTR szURL)
{
	m_strURL = szURL;
	m_strFriendName = "";
	m_bGroup = false;
	return CUPGDlgBase::DoModal();
}

void CPostBoxDlg::ClosePopup( short i )
{
	CUPGDlgBase::EndDialog(IDOK);
}


int CPostBoxDlg::DoModalSend(LPCTSTR szURL, char* strFirend, bool bGroup )
{
	m_strURL = szURL;
	m_bGroup = bGroup;
	m_strFriendName = strFirend;

	return CUPGDlgBase::DoModal();
}