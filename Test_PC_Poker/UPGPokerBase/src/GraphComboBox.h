#pragma once

class CGraphComboBox
{

protected:

	struct COMBODATA
	{
		string	str;
		DWORD	ItemData;
		COLORREF	textcolor;
	};

	int				m_nControlID;
	CWnd*			m_pParent;
	CRect			m_rectPosition;
	CRect			m_rectDropDown;
	BOOL			m_bEnable;
	BOOL			m_bPullDown;


	vector<COMBODATA> m_vec_data;

	NMBASE::GRAPHICGDI::CPage *m_pPage;
	NMBASE::SKINGDI::CGraphButton	m_BtnPullDown;

	int				m_nSel;

	WORD			m_colorBack;
	WORD			m_colorOver;
	COLORREF		m_colorText;
	WORD			m_colorBorder;

	CFont			*m_pFont;

	void			*m_pItemData;

	BOOL			m_bShow;

public:


	virtual BOOL Init( CWnd* pParent,
		NMBASE::GRAPHICGDI::CPage* pPage,
		CRect *pRect,
		int nCmdID,
		NMBASE::GRAPHICGDI::xSprite *pSprite,
		int nSprID,
		int nSprCount );


	virtual int GetCurSel()
	{
		return m_nSel;
	}

	virtual BOOL SetCurSel(int nSel);
	virtual void SetBackColor(WORD color)
	{
		m_colorBack = color;
	}
	virtual void SetBorderColor(WORD color)
	{
		m_colorBorder = color;
	}
	virtual void SetMouseOverColor(WORD color)
	{
		m_colorOver = color;
	}
	virtual void SetFont(CFont *pFont)
	{
		m_pFont = pFont;
	}
	virtual void SetTextColor(COLORREF color)
	{
		m_colorText = color;
	}
	virtual void SetTextColor(int nIndex, COLORREF color);

	virtual int GetCount()
	{
		return (int)m_vec_data.size();
	}

	virtual void Show(BOOL bShow);


	virtual void AddString(const char *pStr, DWORD ItemData=0);

	virtual void Draw( CDC* pDC );


	virtual BOOL OnMouseMove( int xp, int yp );
//	virtual BOOL OnMouseLeave( UINT nFlags, CPoint point );
	virtual BOOL OnLButtonDown( int xp, int yp  );
	virtual BOOL OnLButtonUp( int xp, int yp  );


	virtual BOOL SetItemData(int nIndex, DWORD ItemData);
	virtual DWORD GetItemData(int nIndex);
	virtual void ResetContent();





protected:


	virtual int PtInSelRect(POINT pt);
public:
	CGraphComboBox(void);
	virtual ~CGraphComboBox(void);
};
