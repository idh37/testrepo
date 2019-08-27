// MyGraphButton.h: interface for the CMyGraphButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYGRAPHBUTTON_H__DBF8EDA6_5A01_466E_8C72_FEE32C51F1D4__INCLUDED_)
#define AFX_MYGRAPHBUTTON_H__DBF8EDA6_5A01_466E_8C72_FEE32C51F1D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CMyGraphButton : public NMBASE::SKINGDI::CGraphButton
{
	DWORD m_dwStartTime;

public:
	void SetStartTime();
	void Draw( CDC *pDC, int opt = 0, int grade = 0 );
	CMyGraphButton();
	virtual ~CMyGraphButton();

	virtual void Show(BOOL bVal); 
	virtual void Enable(BOOL bVal);
	virtual BOOL OnLButtonDown(int x, int y);
	virtual BOOL OnLButtonUp(int x, int y);
//	virtual BOOL OnMouseMove(int x, int y);
	void SetSize( int nWidth, int nHeight );
};

#endif // !defined(AFX_MYGRAPHBUTTON_H__DBF8EDA6_5A01_466E_8C72_FEE32C51F1D4__INCLUDED_)
