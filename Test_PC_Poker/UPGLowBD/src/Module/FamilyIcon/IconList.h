// IconList.h: interface for the CIconList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICONLIST_H__F74D7E05_AC79_4C17_8C44_2D8958F96524__INCLUDED_)
#define AFX_ICONLIST_H__F74D7E05_AC79_4C17_8C44_2D8958F96524__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



enum IC_STATE
{
	ICS_NULL = 0,		// 미보유
	ICS_OFF,
	ICS_ON,
	ICS_DISABLE,
	ICS_HIDE, 
	MAX_ICON_STATE,
};

class CIconObject 
{
public:

	CIconObject();
	virtual ~CIconObject();

	void Init( xSprite* pSpr, CPoint BasePos, int Cmd, int SprIndex = 0, int TooltipIndex = -1, IC_STATE state = ICS_NULL );
	void Draw(CDC& DC,  NMBASE::GRAPHICGDI::CPage& Page, CPoint& mousept );

	void SetToolTip( int Index, CPoint ToolTipPos );
	void SetToolTip( int Index );
	BOOL PtInRect( CPoint pt );

	void SetState( IC_STATE st );
	IC_STATE GetState(){ return m_State; }
	void SendCommand( HWND hwnd );
	BOOL IsValid(){
		if( m_State == ICS_DISABLE )return FALSE;
		return TRUE;
	}

	void RePostion(int x, int y);

	 //자리 지킴이 
	void SetToolTip(int index, CPoint ToolTipPos, CRect &Rect, const char *pText, COLORREF color); //툴팁에 시스템 폰트를 찍을 필요가 있을때

protected:	

	DWORD			m_Cmd;

	xSprite*		m_pSpr;
	CPoint			m_Pt;

	IC_STATE		m_State;
	int				m_SprIndex;
	
	int				m_ToolTipIndex;
	CPoint			m_ToolTipPos;

	CPoint			m_ToolTip_Gab;

	int				m_DataVal;

	CString 		m_strText;
	CRect			m_TextRect;
	COLORREF		m_TextColor;

};

class CIconList
{
public:
	CIconList();
	virtual ~CIconList();

	void Init( HWND ParentWnd );
	
	int AddIcon( xSprite* pSpr, CPoint BasePos, int Cmd, int SprIndex = 0, int TooltipIndex = -1, IC_STATE state = ICS_NULL );	
	CIconObject* GetIcon( int index );

	void Draw( CDC& DC, NMBASE::GRAPHICGDI::CPage& Page, CPoint mousept );

	void SetIconState( int Index, IC_STATE state);

	BOOL OnMouseMove( int x, int y );
	BOOL OnLButtonDown( int x, int y );
	BOOL OnLButtonUp( int x, int y );

	

protected:

	HWND		m_hParentWnd;
	HCURSOR		m_hCursor[2];
	
	CPoint		m_MousePt;
	int			m_Clicked_Index;
	CPoint		m_BasePt;
	vector< CIconObject* > m_Icons;//[MAX_ICON_TYPE];

};
#endif // !defined(AFX_ICONLIST_H__F74D7E05_AC79_4C17_8C44_2D8958F96524__INCLUDED_)
