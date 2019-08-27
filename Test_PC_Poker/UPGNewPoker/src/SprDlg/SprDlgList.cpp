// SprDlgList.cpp: implementation of the CSprDlgList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SprDlgList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprDlgList::CSprDlgList()
{
	m_nTotNum = 0;
	m_pFirst = m_pLast = NULL;
}

CSprDlgList::~CSprDlgList()
{
	Destroy();
}

void CSprDlgList::Destroy()
{
	m_nTotNum = 0;
	m_pLast = NULL;
	if(m_pFirst == NULL) return;

	CSprDlgBase* pNext = m_pFirst;
	while(pNext)
	{
		CSprDlgBase* pNow = pNext; 
		pNext = pNow->m_pNextNode;
		delete pNow;
	}

	m_pFirst = NULL;
	return;
}

BOOL CSprDlgList::AddObject(CSprDlgBase* pObj)
{
	if(!pObj) return FALSE;

	m_nTotNum++;

	pObj->m_nDrawStep = m_nTotNum;

	if(m_pFirst == NULL) {
		m_pFirst = m_pLast = pObj;
		return TRUE;
	}
	
	m_pLast->m_pNextNode = pObj;
	pObj->m_pPreNode = m_pLast;
	m_pLast = pObj;

	return TRUE;
}

BOOL CSprDlgList::DelObject(CSprDlgBase* pObj)
{
	if(!pObj) return FALSE;
	if(m_nTotNum <= 0) return FALSE;

	m_nTotNum--;

	CSprDlgBase* pPre = pObj->m_pPreNode;
	CSprDlgBase* pNext = pObj->m_pNextNode;
	
	/*
	if(pPre != NULL){
		if(pPre->m_nDrawStep > 0){
			if(pPre->m_nDrawStep == 2){
				pPre->m_nDrawStep = 1;
			}
		}
	}
	*/
	
	if(pPre) pPre->m_pNextNode = pNext;
	else m_pFirst = pNext;

	if(pNext) pNext->m_pPreNode = pPre;
	else m_pLast = pPre;

	if(!pPre && !pNext) m_pFirst = NULL;

	delete pObj;

	return TRUE;
}

BOOL CSprDlgList::DelObject()
{
	if( m_nTotNum <= 0 )return FALSE;
	if(m_pFirst == NULL) return FALSE;
	CSprDlgBase *pNow = NULL;

	CSprDlgBase* pNext = m_pFirst;	
	while(pNext)
	{
		pNow = pNext;
		pNext = pNow->m_pNextNode;
		if(pNow->m_nDrawStep == m_nTotNum)
		{
			break;
		}		
	}

	if( pNow != NULL )
	{
		DelObject(pNow);
	}

	
	return TRUE;
}


void CSprDlgList::SprDlgLButtonDown(int x, int y)
{
	CSprDlgBase* pNext = m_pFirst;
	while(pNext)
	{
		CSprDlgBase* pNow = pNext;
		pNext = pNow->m_pNextNode;	

		if(pNow->m_nDrawStep == m_nTotNum){
			if( pNow->SDlg_LButtonDown(x,y) ){
			//	if(pNow->SDlg_ProcessObject() == SPRDLG_PROCESS_KILLME) {
			//		DelObject(pNow);
			//	}
				break;
			}			
		}		
	}
}

void CSprDlgList::SprDlgLButtonUp(int x, int y)
{
	CSprDlgBase* pNext = m_pFirst;
	while(pNext)
	{
		CSprDlgBase* pNow = pNext;
		pNext = pNow->m_pNextNode;

	//	if(pNow->SDlg_ProcessObject() == SPRDLG_PROCESS_KILLME) {
	//		DelObject(pNow);
	//	}

		if(pNow->m_nDrawStep == m_nTotNum){
			if( pNow->SDlg_LButtonUp(x,y) ){
				/*
				if(pNow->SDlg_ProcessObject() == SPRDLG_PROCESS_KILLME) {
					DelObject(pNow);
				}
				*/
				break;
			}			
		}
	}
}

void CSprDlgList::SprDlgMouseMove(int x, int y)
{
	CSprDlgBase* pNext = m_pFirst;
	while(pNext)
	{
		CSprDlgBase* pNow = pNext;
		pNext = pNow->m_pNextNode;	
		
	//	if(pNow->SDlg_ProcessObject() == SPRDLG_PROCESS_KILLME) {
	//		DelObject(pNow);
	//	}

		if(pNow->m_nDrawStep == m_nTotNum){
			if( pNow->SDlg_OnMouseMove(x,y) ){
			//	if(pNow->SDlg_ProcessObject() == SPRDLG_PROCESS_KILLME) {
			//		DelObject(pNow);
			//	}
				break;
			}			
		}
	}
}

void CSprDlgList::DrawAll(CDC *pDC)
{
	CSprDlgBase* pNext = m_pFirst;
	while(pNext)
	{
		CSprDlgBase* pNow = pNext;
		pNext = pNow->m_pNextNode;
		pNow->SDlg_Draw(pDC);
	}
}


CSprDlgBase* CSprDlgList::GetObject(ENUM_SDLGKIND kind)
{
	CSprDlgBase *pNow = NULL;

	CSprDlgBase* pNext = m_pFirst;	
	while(pNext)
	{
		CSprDlgBase* pNow = pNext;
		pNext = pNow->m_pNextNode;
		if(pNow->m_nSprDlgKind == kind)
		{
			return pNow;
		}		
	}
	return NULL;
}

void CSprDlgList::ProcessAll()
{
	CSprDlgBase* pNext = m_pFirst;
	while(pNext)
	{
		CSprDlgBase* pNow = pNext;
		pNext = pNow->m_pNextNode;	

		if(pNow->SDlg_ProcessObject() == SPRDLG_PROCESS_KILLME) {		

			DelObject(pNow);			
		}
	}
}