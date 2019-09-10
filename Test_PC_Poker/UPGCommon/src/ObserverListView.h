// ObserverListView.h: interface for the CObserverListView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBSERVERLISTVIEW_H__ED34003B_DC14_4B62_8949_ADEF94EE19B6__INCLUDED_)
#define AFX_OBSERVERLISTVIEW_H__ED34003B_DC14_4B62_8949_ADEF94EE19B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "userstruct.h"

#define  MAX_OBSERVER_BUFFER (110)  //������ ���� �˳��� 110������ ����
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
	CRect			m_ScrollBaseRect;	//	��ũ�ѹ� ����

	int				m_NowIndex;			//	���� �ε���
	int				m_MouseOverIndex;	//	������ �ε���
	int				m_SelectIndex;		//	���õ� �ε��� - ���ȣ, �Ǵ� ������ȣ

	CPoint			m_LButtonDownThumbPos;	
//	============= ����Ʈ ���� ========================
	//OBSERVER_DATA	**m_ppUserInfoList;		//	��������Ʈ
	OBSERVER_DATA	*m_pUserInfoList[ MAX_OBSERVER_BUFFER ];		//	��������Ʈ


	std::deque< OBSERVER_DATA > m_deque_ObUser;

	NMBASE::GRAPHICGDI::xSprite *m_pLevelSpr, *m_pEtcSpr;

	BOOL		m_bEnableBanish;
	


	//CPtrList	m_List;
	bool			InsertList( OBSERVER_DATA *pList );	
	void			SetList( int unum, char *id, int seq );
	void			DelList( int DelIndex, char *pID = NULL );		//	���϶��� pID ��� �ȴ�
	void			DelAllList( void );
	void			SetIndex( int Index );					//	�ε��� �����ϱ�	
	int				GetSelectIndex( void );					//	���õ� �ε��� ã�ƿ���	
	OBSERVER_DATA	*GetSelectIndex_strID();					//  ������ �� ���̵� 
	void ResetReservation();	

	void		RenewalShortList( void );		//	���� ����
//	================ �����ϱ� ========================
	void		ResetControlls( void );
//	==================================================

	void		Draw( CDC *pDC );
	void		DrawRoom( CDC *pDC );		//	���϶�
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
