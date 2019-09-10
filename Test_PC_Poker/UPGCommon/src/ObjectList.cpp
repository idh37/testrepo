// ObjectList.cpp: implementation of the CObjectList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ObjectList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CObjectList::CObjectList()
{
	TotNum = 0;
	pFirst = pLast = NULL;
}

CObjectList::~CObjectList()
{
	Destroy();
}

void CObjectList::Destroy()
{
	TotNum = 0;
	pLast = NULL;
	if(pFirst == NULL) return;

	CBaseObject* pNext = pFirst;
	while(pNext)
	{
		CBaseObject* pNow = pNext;
		pNext = pNow->pNextNode;
		delete pNow;
	}

	pFirst = NULL;
	return;
}

BOOL CObjectList::AddObject(CBaseObject* pObj)
{
	if (!pObj)
		return FALSE;
	
	TotNum++;
	
	if (pFirst == NULL)
	{
		pFirst = pLast = pObj;

		return TRUE;
	}
	
	pLast->pNextNode = pObj;
	pObj->pPreNode = pLast;
	pLast = pObj;
	
	return TRUE;
}

BOOL CObjectList::DelObject(CBaseObject* pObj)
{
	if(!pObj) return FALSE;
	if(TotNum <= 0) return FALSE;

	TotNum--;

	CBaseObject* pPre = pObj->pPreNode;
	CBaseObject* pNext = pObj->pNextNode;
	
	if(pPre) pPre->pNextNode = pNext;
	else pFirst = pNext;

	if(pNext) pNext->pPreNode = pPre;
	else pLast = pPre;

	if(!pPre && !pNext) pFirst = NULL;

	delete pObj;
	pObj = NULL;

	return TRUE;
}

BOOL CObjectList::DelObject(int skind , int ekind)
{
	CBaseObject *pNow = NULL;

	CBaseObject* pNext = pFirst;	
	while(pNext)
	{
		pNow = pNext;
		pNext = pNow->pNextNode;
		if(pNow->m_nObjID >= skind && pNow->m_nObjID <= ekind )
		{
			DelObject(pNow);
		//	return TRUE;
		}		
	}	
	return FALSE;
}

BOOL CObjectList::DelObject(int kind)
{
	CBaseObject *pNow = NULL;

	CBaseObject* pNext = pFirst;	
	while(pNext)
	{
		pNow = pNext;
		pNext = pNow->pNextNode;
		if(pNow->m_nObjID == kind)
		{
			DelObject(pNow);
			return TRUE;
		}		
	}	
	return FALSE;
}

void CObjectList::ProcessAll()
{
	CBaseObject* pNext = pFirst;
	while(pNext)
	{
		CBaseObject* pNow = pNext;
		pNext = pNow->pNextNode;

		if(pNow->ProcessObject() == PROCESS_KILLME) {		

			DelObject(pNow);
		//	break;
		}		
	}
}

void CObjectList::DrawAll(CDC *pDC)
{
	CBaseObject* pNext = pFirst;
	while(pNext)
	{
		CBaseObject* pNow = pNext;
		pNext = pNow->pNextNode;
		pNow->DrawObject(pDC);
	}
}

void CObjectList::TopDrawAll(CDC *pDC)
{
	CBaseObject* pNext = pFirst;
	while(pNext)
	{
		CBaseObject* pNow = pNext;
		pNext = pNow->pNextNode;
		if(pNow->DrawFaceKind == DRAW_BOTTOM || pNow->DrawFaceKind == DRAW_MIDDLE )continue;	
		pNow->DrawObject(pDC);		
	}
}

void CObjectList::BottomDrawAll(CDC *pDC)
{
	CBaseObject* pNext = pFirst;	
	while(pNext)
	{
		CBaseObject* pNow = pNext;
		pNext = pNow->pNextNode;
		if(pNow->DrawFaceKind == DRAW_TOP || pNow->DrawFaceKind == DRAW_MIDDLE)continue;		
		pNow->DrawObject(pDC);		
	}
}

void CObjectList::MiddleDrawAll(CDC *pDC)
{
	CBaseObject* pNext = pFirst;
	while(pNext)	
	{
		CBaseObject* pNow = pNext;
		pNext = pNow->pNextNode;
		if(pNow->DrawFaceKind == DRAW_TOP || pNow->DrawFaceKind == DRAW_BOTTOM)continue;		
		pNow->DrawObject(pDC);		
	}
}

CBaseObject* CObjectList::GetObject(int kind)
{
	CBaseObject *pNow = NULL;

	CBaseObject* pNext = pFirst;	
	while(pNext)
	{
		pNow = pNext;
		pNext = pNow->pNextNode;
		if(pNow->m_nObjID == kind)
		{
			return pNow;
		}		
	}
	return NULL;
}

BOOL CObjectList::CheckObject(int kind)
{
	CBaseObject *pNow = NULL;
	
	CBaseObject* pNext = pFirst;	
	while(pNext)
	{
		pNow = pNext;
		pNext = pNow->pNextNode;
		if(pNow->m_nObjID == kind)
		{
			return TRUE;
		}		
	}
	return FALSE;
}

void CObjectList::OnLButtonDown(int x, int y)
{
	//���߿� ������ ���� �˻� ( �׷����� �� ���� ���� ����
	CBaseObject* pPreNode = pLast;
	while(pPreNode)
	{
		CBaseObject* pNow = pPreNode;
		pPreNode = pNow->pPreNode;
		
		if( pNow->ClickCheck(x,y) && pNow->bMove == TRUE ){

			//���� ���� �ø���.(��Ʈ�� ��)
			if( pNow != pLast ){
				CBaseObject* pTemp = NULL;
				//���� ���� pFirst���� �˻�
				if( pNow == pFirst )
				{
					//���� ó���̸� ���� ������ ó���� �ְ�					
					pTemp = pNow->pNextNode;
					if( pTemp == pLast ){
						pTemp->pPreNode = NULL;
						pTemp->pNextNode = pNow;
						
						pNow->pPreNode = pLast;
						pNow->pNextNode = NULL;
						pLast = pNow;
						pFirst = pTemp;
					}
					//�� ������ ó������ �ٲٰ� �� ���������� �ٲ۴�.
					else{
						pTemp->pPreNode = NULL;						
						pNow->pNextNode = NULL;						
						pLast->pNextNode = pNow;
						pNow->pPreNode = pLast;
						pLast = pNow;
						pFirst = pTemp;
					}					
				}
				//�� �յڷ� ��尡 �ִ�.
				else{
					pTemp = pNow->pNextNode;
					pTemp->pPreNode = pNow->pPreNode;
					pPreNode->pNextNode = pTemp;
					
					pNow->pNextNode = NULL;

					pLast->pNextNode = pNow;
					pNow->pPreNode = pLast;
					pLast = pNow;				
				}
			}			
			
			if(pNow->ProcessObject() == PROCESS_KILLME) {
				DelObject(pNow);
				break;
			}
			pNow->OnLButtonDown(x,y);		
			break;
		}		
	}	
}

void CObjectList::OnLButtonUp(int x, int y)
{
		//���߿� ������ ���� �˻� ( �׷����� �� ���� ���� ����
	CBaseObject* pPreNode = pLast;
	while(pPreNode)
	{
		CBaseObject* pNow = pPreNode;
		pPreNode = pNow->pPreNode;
	
		if( pNow->ClickCheck(x,y) && pNow->bMove == TRUE ){			
			if(pNow->ProcessObject() == PROCESS_KILLME) {
				DelObject(pNow);
				break;
			}
			pNow->OnLButtonUp(x,y);		
			break;
		}		
	}
}

void CObjectList::OnMouseMove(int x, int y)
{
			//���߿� ������ ���� �˻� ( �׷����� �� ���� ���� ����
	CBaseObject* pPreNode = pLast;
	while(pPreNode)
	{
		CBaseObject* pNow = pPreNode;
		pPreNode = pNow->pPreNode;

		if( pNow->ClickCheck(x,y) && pNow->bMove == TRUE ){
			if(pNow->ProcessObject() == PROCESS_KILLME) {
				DelObject(pNow);
				break;
			}
			pNow->OnMouseMove(x,y);		
			break;
		}		
	}	
}