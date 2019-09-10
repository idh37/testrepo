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

// ������Ʈ ����
enum ENUM_SDLGKIND
{
	SDLG_NONE = 0,
	SDLG_PROFILE,			//������ 
	SDLG_DSLEVEL,		//�������� 	 
	SDLG_TIP,
};

class CSprDlgBase;

class CSprDlgBase  
{
public:

	CSprDlgBase		*m_pPreNode;
	CSprDlgBase		*m_pNextNode;

	ENUM_SDLGKIND	m_nSprDlgKind;	//���̾�α� ����
	int				m_nDrawStep;	//�׸��� ����
	int				m_nResult;		// ���

	int				m_nDXp;			//�׸��� ������
	int				m_nDYp;

	int				m_nMXp;			//���콺 ��ǥ
	int				m_nMYp;			//���콺 ��ǥ

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
