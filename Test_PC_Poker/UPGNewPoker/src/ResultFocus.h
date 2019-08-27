// ResultFocus.h: interface for the CResultFocus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESULTFOCUS_H__A243E225_A48D_4A42_B51E_3EA7F874755F__INCLUDED_)
#define AFX_RESULTFOCUS_H__A243E225_A48D_4A42_B51E_3EA7F874755F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Afxtempl.h>

class CResultFocus  
{
public:
	CResultFocus();
	virtual ~CResultFocus();

	NMBASE::GRAPHICGDI::CPage		*m_pPage;	

	BOOL		m_bShow;
	BOOL		m_bMyHighRule;							// 포카드 이상 이팩트 판단 

	int			m_nStarCnt;

	int			m_nMyhighrule;
	int			m_nWinPNum;
	DWORD		m_dwWinDrawTime;

	DWORD		m_dwChanceDrawTime;

	void Init(NMBASE::GRAPHICGDI::CPage *page);
	void Clear();
	void ReSet();
	void OnTimer();
	void SetRFocus(int wpnum,int myhighrule);
	void Draw(CDC *pDC);
};

#endif // !defined(AFX_RESULTFOCUS_H__A243E225_A48D_4A42_B51E_3EA7F874755F__INCLUDED_)
