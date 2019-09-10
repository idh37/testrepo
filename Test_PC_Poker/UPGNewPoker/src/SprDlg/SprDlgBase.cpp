// SprDlgBase.cpp: implementation of the CSprDlgBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SprDlgBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprDlgBase::CSprDlgBase()
{
	m_pPreNode = m_pNextNode = NULL;

	m_nSprDlgKind = SDLG_NONE;	//���̾�α� ����
	m_nDrawStep = 0;		//�׸��� ����

	m_nDXp = 0;		//�׸��� ������
	m_nDYp = 0;

	m_nMXp = 0;		//���콺 ��ǥ
	m_nMYp = 0;		//���콺 ��ǥ
	m_nResult = 0;

}

CSprDlgBase::~CSprDlgBase()
{
	m_nDrawStep = 0;
}
