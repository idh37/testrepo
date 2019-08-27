// SprDlgList.h: interface for the CSprDlgList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRDLGLIST_H__865CE7FD_BBD1_4872_8D56_089536E66C08__INCLUDED_)
#define AFX_SPRDLGLIST_H__865CE7FD_BBD1_4872_8D56_089536E66C08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SprDlgBase.h"

class CSprDlgList  : public CSprDlgBase
{
public:
	int				m_nTotNum;
	CSprDlgBase		*m_pFirst;
	CSprDlgBase		*m_pLast;

	CSprDlgList();
	virtual ~CSprDlgList();
	
	void Destroy();

	BOOL AddObject(CSprDlgBase* pObj);
	BOOL DelObject(CSprDlgBase* pObj);
	BOOL DelObject();
	CSprDlgBase* GetObject(ENUM_SDLGKIND kind);

	void SprDlgLButtonDown(int x, int y);
	void SprDlgLButtonUp(int x, int y);
	void SprDlgMouseMove(int x, int y);
	void DrawAll(CDC *pDC);

	void ProcessAll();
};

#endif // !defined(AFX_SPRDLGLIST_H__865CE7FD_BBD1_4872_8D56_089536E66C08__INCLUDED_)
