// ObserverListView.h: interface for the CObserverListView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBSERVERLISTVIEW_H__ED34003B_DC14_4B62_8949_ADEF94EE19B6__INCLUDED_)
#define AFX_OBSERVERLISTVIEW_H__ED34003B_DC14_4B62_8949_ADEF94EE19B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "userstruct.h"

#define  MAX_OBSERVER_BUFFER (110)  //관전자 버퍼 넉넉히 110명으로 잡음
#if MAX_OBSERVER_BUFFER < MAX_OBSERVER
#error MAX_OBSERVER_BUFFER should be greater than MAX_OBSERVER
#endif

typedef struct  {
	char	strID[20];
	char	strNickName[20];
	INT64	PMoney;	
	int		Sex;
	int		Seq;
	BOOL	bSelected;
	int		UNum;
	UINT64  llUserKey;

	void	Clear(){
		ZeroMemory(strID,sizeof(strID));
		ZeroMemory(strNickName,sizeof(strNickName));
		
		PMoney = 0;	
		Sex  = -1;
		Seq = 0;
		bSelected = 0;
		UNum = -1;
		llUserKey = -1;
	}
} OBSERVER_DATA ;

#define MAX_OBSERVERCHECK 10

class UPG_COMMON_EXPORT CObserverListView : public CWnd  
{
public:
	CWnd*           m_pParentWnd;
	NMBASE::GRAPHICGDI::CPage			m_Page;
	NMBASE::GRAPHICGDI::xSprite			*m_pBackSpr;
	NMBASE::GRAPHICGDI::xSprite			*m_pButtonSpr;

	NMBASE::SKINGDI::CGraphButton	m_UpBtn;
	NMBASE::SKINGDI::CGraphButton	m_DownBtn;
	NMBASE::SKINGDI::CGraphButton	m_ThumbBtn;	
	
	UINT			m_hTimer;	
	int				m_TimeCnt;

	CRect			m_UserCheckRect_Max[ MAX_OBSERVERCHECK ];
	CRect			m_ScrollBaseRect;	//	스크롤바 영역

	int				m_NowIndex;			//	현재 인덱스
	int				m_MouseOverIndex;	//	오버된 인덱스
	int				m_SelectIndex;		//	선택된 인덱스 - 방번호, 또는 유저번호

	CPoint			m_LButtonDownThumbPos;	
//	============= 리스트 관련 ========================
	//OBSERVER_DATA	**m_ppUserInfoList;		//	유저리스트
	OBSERVER_DATA	*m_pUserInfoList[ MAX_OBSERVER_BUFFER ];		//	유저리스트


	std::deque< OBSERVER_DATA > m_deque_ObUser;

	NMBASE::GRAPHICGDI::xSprite *m_pLevelSpr, *m_pEtcSpr;

	BOOL		m_bEnableBanish;
	


	//CPtrList	m_List;
	bool			InsertList( OBSERVER_DATA *pList );	
	void			SetList( int unum, char *id, int seq );
	void			DelList( int DelIndex, char *pID = NULL );		//	방일때는 pID 없어도 된다
	void			DelAllList( void );
	void			SetIndex( int Index );					//	인덱스 설정하기	
	int				GetSelectIndex( void );					//	선택된 인덱스 찾아오기	
	OBSERVER_DATA	*GetSelectIndex_strID();					//  선택한 넘 아이디 
	void ResetReservation();	

	void		RenewalShortList( void );		//	정보 갱신
//	================ 정렬하기 ========================
	void		ResetControlls( void );
//	==================================================

	void		Draw( CDC *pDC );
	void		DrawRoom( CDC *pDC );		//	룸일때
	void		DrawButton( CDC *pDC );	
	
	void	OnButtonViewprofile();
	void	SetResizeObserverList();
	int GetSelectedUserCount();

// Operations
public:
	void		Init( NMBASE::GRAPHICGDI::xSprite *pBackSpr, NMBASE::GRAPHICGDI::xSprite *pButtonSpr , NMBASE::GRAPHICGDI::xSprite *pLevelSpr, NMBASE::GRAPHICGDI::xSprite *pEtcSpr);
	virtual void Create( CWnd* pParentWnd, const RECT& rect, UINT nID, int UseMode );

	void SetEnableBanish(BOOL bBanish);
	void KickOutSelectedUser();
	void SetCheckAll(BOOL bCheck);
public:
	CObserverListView();
	virtual ~CObserverListView();

protected:
	DECLARE_DYNCREATE(CObserverListView)


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObserverListView)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CObserverListView)
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_OBSERVERLISTVIEW_H__ED34003B_DC14_4B62_8949_ADEF94EE19B6__INCLUDED_)
