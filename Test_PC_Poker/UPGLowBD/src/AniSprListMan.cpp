// AniSprListMan.cpp: implementation of the CAniSprListMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "AniSprListMan.h"

#include "./Effect/BetCmdEffect.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAniSprListMan::CAniSprListMan()
{
	Destroy();
}

CAniSprListMan::~CAniSprListMan()
{
	Destroy();
}
void CAniSprListMan::Destroy(int kind)
{	
	m_AniSprMan.Destroy();		
}

void CAniSprListMan::ADDObject(CBaseObject *pObj)
{
	if( pObj == NULL )return;
	m_AniSprMan.AddObject(pObj);
}

void CAniSprListMan::DrawObject(CDC *pDC)
{
	if( pDC == NULL )return;

	m_AniSprMan.DrawAll(pDC);
}

void CAniSprListMan::DrawTop(CDC *pDC)
{
	if( pDC == NULL )return;

	m_AniSprMan.TopDrawAll(pDC);
}
void CAniSprListMan::DrawBottom(CDC *pDC)
{
	if( pDC == NULL )return;

	m_AniSprMan.BottomDrawAll(pDC);	
}

void CAniSprListMan::DrawMiddle(CDC *pDC)
{
	if( pDC == NULL )return;
	m_AniSprMan.MiddleDrawAll(pDC);	
}

void CAniSprListMan::AniSprProcess()
{
	m_AniSprMan.ProcessAll();	
}

void CAniSprListMan::OnLButtonDown(int x, int y)
{
	m_AniSprMan.OnLButtonDown(x, y);	
}
void CAniSprListMan::OnLButtonUp(int x, int y)
{
	m_AniSprMan.OnLButtonUp(x, y);	
}
void CAniSprListMan::OnMouseMove(int x, int y)
{
	m_AniSprMan.OnMouseMove(x, y);	
}

void CAniSprListMan::Del_AniListOBj(OBJECT_TYPE skind , OBJECT_TYPE ekind)
{
	m_AniSprMan.DelObject(skind,ekind);	
}

void CAniSprListMan::Del_AniListOBj(OBJECT_TYPE skind)
{
	m_AniSprMan.DelObject(skind);
}

void CAniSprListMan::Del_AniListOBj(CBaseObject* pObj)
{
	m_AniSprMan.DelObject(pObj);
}

CBaseObject* CAniSprListMan::GetAniListOBj(OBJECT_TYPE kind)
{
	CBaseObject *pObj = NULL ; 
	pObj = m_AniSprMan.GetObject(kind);	
	return pObj;
}


//�� anilist �� Ưȭ�� �Լ�
//BET_CMD
void CAniSprListMan::BetCmd_ChangeCmd(int pnum)
{
	if (m_AniSprMan.pFirst != NULL )
	{				
		CBaseObject* pNext = m_AniSprMan.pFirst;	
		
		while( pNext )
		{
			CBaseObject* pNow = pNext;
			
			if ( pNow->m_nObjID == ANF_BET_CMD )
			{
				effect::CBetCmdEffect* pObj =static_cast< effect::CBetCmdEffect* >( pNow );	
				
				if ( pObj->GetPlayerNumber()  == pnum ) //���� ���� �Ҷ��� �����Ŵ� ����� �ٽ� ��´�.
				{
					pObj->Destroy();
				}				
				else  // ���� Ŀ���� ������
				{
					pObj->SetSmallCmd();
				}
			}
			pNext = pNow->pNextNode;
		}			
	}
}

void CAniSprListMan::BetCmd_ClearCmd(int pnum, int nDelayTime )
{
	if(m_AniSprMan.pFirst != NULL)
	{				
		CBaseObject* pNext = m_AniSprMan.pFirst;	
		
		while(pNext)
		{
			CBaseObject* pNow = pNext;
			
			if(pNow->m_nObjID == ANF_BET_CMD)
			{
				effect::CBetCmdEffect* pObj = (effect::CBetCmdEffect*)pNow;	
				//���� ���� �Ҷ��� �����Ŵ� ����� �ٽ� ��´�.
				if (pObj->GetPlayerNumber()  == pnum)
				{
					if ( nDelayTime != 0 )
					{
						pObj->SetDelTime( nDelayTime );
					}
					else
					{
						pObj->Destroy();
					}
				}
			}
			pNext = pNow->pNextNode;
		}			
	}
}

//���� Ŀ��带 ������ �� �����.
void CAniSprListMan::BetCmd_DelDelay(int delaytime)
{
	//���� Ŀ��带 ����
	if(m_AniSprMan.pFirst != NULL)
	{			
		CBaseObject* pNext = m_AniSprMan.pFirst;	
		while(pNext)
		{
			CBaseObject* pNow = pNext;				
			
			if(pNow->m_nObjID == ANF_BET_CMD)
			{
				effect::CBetCmdEffect* pObj = (effect::CBetCmdEffect*)pNow;	
				pObj->SetDelTime(delaytime);
			}				
			pNext = pNow->pNextNode;
		}			
	}
}
