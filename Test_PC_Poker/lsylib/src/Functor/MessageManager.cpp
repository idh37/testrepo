#include "StdAfx.h"
#include "MessageManager.h"
#include "Message.h"
#include <algorithm>
#include "../ThreadLock/ThreadLock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace LSY
{
CMessageManager::CMessageManager(void)
{
	InitializeCriticalSection(&m_cLock);
}

CMessageManager::~CMessageManager(void)
{
	//destroyed = true;

	Clear();
	DeleteCriticalSection(&m_cLock);	
}

bool CMessageManager::AddMsgH(int nCommand, IFunctor* pFnt)
{
	CThreadLock lock(&m_cLock);

	CDelegate* pDel = FindDelegate( nCommand );

	if(!pDel)
	{
		// create a new delegate
		CDelegate del;
		del.Default( pFnt );

		m_map[ nCommand ] = del;
		return true;
	}

	if( false == pDel->Default( pFnt ) )
	{		
		return false;
	}

	return true;
}


bool CMessageManager::AppendMsgH(int nCommand, IFunctor* pFunctor )
{
	CThreadLock lock(&m_cLock);

	CDelegate* pDel = FindDelegate( nCommand );

	if(!pDel )
	{
		// create a new delegate
		CDelegate del;
		del.Append( pFunctor );

		m_map[ nCommand ] = del;
		return true;
	}

	if( false == pDel->Append( pFunctor ) )
	{		
		return false;
	}

	return true;
}

bool CMessageManager::OverrideMsgH(int nCommand, IFunctor* pFnt)
{
	CThreadLock lock(&m_cLock);

	CDelegate* pDel = FindDelegate( nCommand );
	if(!pDel )
	{
		// create a new delegate
		CDelegate del;
		del.Override( pFnt );

		m_map[ nCommand ] = del;
		return true;
	}

	// Override
	if(false ==  pDel->Override( pFnt) )
	{		
		return false;
	}

	return true;
}

LRESULT CMessageManager::Call(CMessage *pMsg)
{
	ASSERT(pMsg != NULL);
	CThreadLock lock(&m_cLock);
	// Find
	IT it = m_map.find( pMsg->GetID() );
	if(m_map.end() == it)
	{		
		return FALSE;
	}

	// Call
	LRESULT nRet = it->second.Fire( pMsg );
	
	if(nRet < 0)
	{		
		return FALSE;
	}

	return nRet;
}


LRESULT	CMessageManager::CallDefault(CMessage *pMsg)
{
	CThreadLock lock(&m_cLock);
	// Find
	IT it = m_map.find( pMsg->GetID() );
	if(m_map.end() == it)
	{		
		return FALSE;
	}

	// Call
	LRESULT nRet = it->second.FireDefault(pMsg);

	if(nRet < 0)
	{		
		return FALSE;
	}

	return nRet;
}


bool CMessageManager::RemoveMsgH(int nCommand)
{
	CThreadLock lock(&m_cLock);
	// Find
	IT it = m_map.find( nCommand );
	if( m_map.end() == it)
	{		
		return false;
	}

	// Remove
	it->second.Release();
	m_map.erase(it);
	return true;
}

bool CMessageManager::RemoveAppendMsgH(int nCommand, IFunctor* pFunctor)
{
	CThreadLock lock(&m_cLock);
	// Find
	IT it = m_map.find( nCommand );
	if( m_map.end() == it)
	{		
		return false;
	}

	// AppendRemove
	it->second.ReleaseAppend(pFunctor);
	pFunctor->Release();

	return true;
}

bool CMessageManager::RemoveOverrideMsgH(int nCommand, IFunctor* pFunctor)
{
	CThreadLock lock(&m_cLock);
	// Find
	IT it = m_map.find( nCommand );
	if( m_map.end() == it)
	{		
		return false;
	}

	// Remove
	it->second.ReleaseOverride(pFunctor);

	return true;
}


SFINFOS CMessageManager::Info(int nCommand)
{
	/* WARN : Do NOT use assert, VERIFY, etc in this function */
	/* Because this fuction will be called in the watch window */

	// To return
	CThreadLock lock(&m_cLock);
	SFINFOS finfos = {NULL, NULL, NULL, NULL};

	/* Find */
	IT it = m_map.find( nCommand );
	if( m_map.end() == it)
	{
		return finfos;
	}
	
	return it->second.Info();
}

void CMessageManager::Clear()
{
	// Clear map
	CThreadLock lock(&m_cLock);
	IT it;
	for(it = m_map.begin(); it != m_map.end(); ++it)
	{
		it->second.Release();
	}
	m_map.clear();
}


CDelegate* CMessageManager::FindDelegate( int nCommand )
{
	CThreadLock lock(&m_cLock);
	IT it = m_map.find( nCommand );
	if( m_map.end() == it )
	{
		return NULL;
	}

	return &it->second;
}

void CMessageManager::DumpAllHandlers(bool bSort)
{
	CThreadLock lock(&m_cLock);
	#ifdef _DEBUG
	printf("------- Dump handlers (begin)------\n");
	#endif

	if(!bSort) 
	{
		for(IT it = m_map.begin(); it != m_map.end(); ++it) 
		{
			#ifdef _DEBUG
			printf("nCommand : %d", it->first);
			printf("\n");
			#endif
		}
	}
	else 
	{
		// Copy the map to a vector
		typedef std::vector<std::string> TClones;
		TClones clone( m_map.size() );

		IT it1 = m_map.begin();
		TClones::iterator it2 = clone.begin();
		for(;it1 != m_map.end(); ++it1, ++it2)		
		{
			*it2 = it1->first;
		}

		// Sort
		std::sort( clone.begin(), clone.end() );

		for( TClones::iterator it = clone.begin(); it != clone.end(); ++it ) 
		{
			#ifdef _DEBUG
			printf(it->c_str());
			printf("\n");
			#endif
		}
	}

	#ifdef _DEBUG
	printf("------- Dump handlers (end)------\n");
	#endif
}
}//namespace LSY