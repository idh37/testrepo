#if !defined(AFX_LoadingDlg_H__070F5001_1C5C_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_LoadingDlg_H__070F5001_1C5C_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoadingDlg.h : header file
//

#include <mmsystem.h>
#include "UPGDlgBase.h"

#define MSGBOX_WAIT_TIMER 334
#define MSGBOX_ANI_TIMER	335

#define UM_START_LODING (WM_USER+5000)
#define UM_END_LODING	(WM_USER+5001)
#define UM_CHANGE_LODING (WM_USER+5002)

/////////////////////////////////////////////////////////////////////////////
// CLoadingDlg dialog
//#include "MyButton.h"
//#include "MyBitmap.h"

class CLoadingDlg;

class CLoadManager
{
	BOOL m_bComplete;
	
	enum RES_TYPE{ RES_TYPE_SPR, RES_TYPE_SND };
	struct stRESOURCE
	{
		RES_TYPE ResType;
		void *pResource;
		string szPath;
	};

	vector<stRESOURCE>		m_vecResList;
public :

	char *PathFindFileName(char *Path)
	{
		int len=strlen(Path);
		for(int i=len-1; i>=0; i--)
		{
			if (Path[i]=='\\') return &Path[i+1];
		}
		return Path;
	}

	int m_Prog;
	CLoadManager() { m_Prog =0; m_bComplete=FALSE;}
	void	AddSprite(NMBASE::GRAPHICGDI::xSprite *pAddSpr, char *pFileName)
	{
		stRESOURCE res;
		res.ResType = RES_TYPE_SPR;
		res.pResource = (void*)pAddSpr;
		res.szPath = pFileName;
		m_vecResList.push_back(res);
	}
	void	LoadAllRes(CLoadingDlg *pDlg);
	int		GetResSize() { return m_vecResList.size(); }
	void	Clear(){m_vecResList.clear(); m_bComplete= false;}

};

class UPG_COMMON_EXPORT CLoadingDlg : public CUPGDlgBase
{
// Construction
public:
	//int hTimerWait;	
	DWORD			m_dwCreationTime;	// 창 생성 시간 
	DWORD			m_dwMinShowTime;	// 최소 창이 열려있어야하는 시간(ms)

	BOOL			m_bBasedOnTime;		//로딩바 시간기준이냐 스프라이트 로딩 기준이냐?

	BOOL			m_bReserveClose;	//창 닫기 예약 
	BOOL			m_bReserveDestroy;
	int				m_nDestroyCount;
	NMBASE::SKINGDI::CMyBitmap		m_imgBackBit, m_imgArrow, m_imgPage;

	HANDLE			m_hCreate;
	HANDLE			m_hDelete;
	HANDLE			m_hThread;
	DWORD			m_dwThread;
	bool			m_bContinue;
	bool			m_bRedarw;

	bool			m_bFirstLoading;
	CLoadManager	m_loadman;

	CLoadingDlg(CWnd* pParent = NULL);   // standard constructor

	void Draw(CDC *pDC);

	void LoseTopMost()
	{
		SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
// Dialog Data
	//{{AFX_DATA(CLoadingDlg)
	enum { IDD = IDD_DIALOG_LOADING };	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoadingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	static DWORD WINAPI stLoadThreadProc( LPVOID pArg );

	// Generated message map functions
	//{{AFX_MSG(CLoadingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	bool IsCheckClose(void);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnNotifyLoading(WPARAM, LPARAM);
	afx_msg LRESULT OnStartLoading(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndLoading(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChangeLoading(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	void LoadAllRes(void);

	LRESULT OnClearLoadImageList(WPARAM &, LPARAM&);
	LRESULT OnAddLoadImageList(WPARAM &, LPARAM&);
	LRESULT OnLoadingImage(WPARAM &, LPARAM &);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LoadingDlg_H__070F5001_1C5C_11D4_97A5_0050BF0FBE67__INCLUDED_)
