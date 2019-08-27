#pragma once

#include "DataUserList.h"

//////////////////////////////////////////////////////////////////////////////
// CInviteUserListView
class CInviteUserListView : public CWnd, public NMBASE::SKINGDI::CNMWidgetComposite
{
	DECLARE_DYNAMIC(CInviteUserListView)


public:
	CInviteUserListView();
	virtual ~CInviteUserListView();

	void MessageLooping();

	// Overrides
	//{{AFX_VIRTUAL(CInviteUserListView)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);	

	//virtual BOOL Create(CWnd *pParentWnd);

protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CInviteUserListView)
protected:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnTimer( UINT nIDEvent );
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

protected:
	DECLARE_MESSAGE_MAP()

	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
public:	
	BOOL CreateWidget( CWnd* pParent );
	void EnterWidget();
	void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );

	BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );	
	BOOL OnCommandWidget( int nCommand, WPARAM wParem, LPARAM lParam );
	int GetCurSelIndex();
	

protected:
	NMBASE::GRAPHICGDI::CPage				m_Page;			// Page
	UINT				m_hEventTimer;	// loop timer

public:

public:
	

	// 프레임 동기화 관련 
	DWORD				m_delta;
	DWORD				m_curtime;
	DWORD				m_prevtime;
	void				ResetTime()	{	m_delta = m_curtime = m_prevtime = 0;	}

	
	
	
	
	void ClearDataUserList();									// 유저리스트 정보를 클리어시킨다.
	void SetDataUserList(CDataUserList *pDataUserList);			// 유저리스트 정보를 세팅한다.
	BOOL RefreshUserList();							//유저리스트 갱신
	map<string, UINT64> *GetCheckedUser();
	void SetGameFilter(CDataUserList::KEY_FILTER KeyFilter);	//유저리스트 필터 세팅
	void OnAskInviteUserList();
	

	
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

