// SprDlgBase.h: interface for the CSprDlgBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRDLGBASE_H__B8A1FFF4_F984_4B37_8BA2_D6032DB0B21E__INCLUDED_)
#define AFX_SPRDLGBASE_H__B8A1FFF4_F984_4B37_8BA2_D6032DB0B21E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SPRDLG_PROCESS_KILLME	(-1)
#define SPRDLG_PROCESS_OK		(1)

// 오브젝트 종류
enum ENUM_SDLGKIND
{
	SDLG_NONE = 0,
	SDLG_PROFILE,			//프로필 
	SDLG_DSLEVEL,		//도신지수 	 
	SDLG_TIP,
};

class CSprDlgBase;

class CSprDlgBase  
{
public:

	CSprDlgBase		*m_pPreNode;
	CSprDlgBase		*m_pNextNode;

	ENUM_SDLGKIND	m_nSprDlgKind;	//다이얼로그 종류
	int				m_nDrawStep;	//그리기 순서
	int				m_nResult;		// 결과

	int				m_nDXp;			//그리는 시작점
	int				m_nDYp;

	int				m_nMXp;			//마우스 좌표
	int				m_nMYp;			//마우스 좌표

	CSprDlgBase();
	virtual ~CSprDlgBase();

	virtual void SDlg_SetPosition(int x, int y){}
	virtual	BOOL SDlg_LButtonUp(int x, int y){return FALSE;}
	virtual	BOOL SDlg_LButtonDown(int x, int y){return FALSE;}
	virtual	BOOL SDlg_OnMouseMove(int x, int y){return FALSE;}
	virtual void SDlg_Draw(CDC *pDC){}
	virtual void SDlg_Destroy(){}
	virtual int  SDlg_ProcessObject(){return 0;}	
};

#endif // !defined(AFX_SPRDLGBASE_H__B8A1FFF4_F984_4B37_8BA2_D6032DB0B21E__INCLUDED_)
