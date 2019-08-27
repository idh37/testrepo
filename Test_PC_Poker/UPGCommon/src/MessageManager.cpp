#include "StdAfx.h"
#include "MessageManager.h"
#include <algorithm>
#include "ThreadLock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//bool CMessageManager::destroyed       = false;
//CMessageManager *CMessageManager::pInst = 0;


/**
* Get a CMessageManager instance. 
*/
CMessageManager& MM()
{
	return CMessageManager::Get();
}


CMessageManager::CMessageManager(void)
{
	InitializeCriticalSection(&m_cLock);
}

CMessageManager::~CMessageManager(void)
{
	//destroyed = true;

	DeleteCriticalSection(&m_cLock);

	Clear();	
}

CMessageManager& CMessageManager::Get()
{
	static CMessageManager MessageManager;
	return MessageManager;
}


bool CMessageManager::AddMsgH(std::string stCommand, IFunctor* pFnt)
{
	CThreadLock lock(&m_cLock);

	CDelegate* pDel = FindDelegate( stCommand );

	if(!pDel)
	{
		// create a new delegate
		CDelegate del;
		del.Default( pFnt );

		m_map[ stCommand ] = del;
		return true;
	}

	if( false == pDel->Default( pFnt ) )
	{		
		//OutputDebugString("err");
		return false;
	}

	return true;
}


bool CMessageManager::AppendMsgH(std::string stCommand, IFunctor* pFunctor )
{
	CThreadLock lock(&m_cLock);

	CDelegate* pDel = FindDelegate( stCommand );

	if(!pDel )
	{
		// create a new delegate
		CDelegate del;
		del.Append( pFunctor );

		m_map[ stCommand ] = del;
		return true;
	}

	if( false == pDel->Append( pFunctor ) )
	{		
		//OutputDebugString("err");
		return false;
	}

	return true;
}

bool CMessageManager::OverrideMsgH(std::string stCommand, IFunctor* pFnt)
{
	CThreadLock lock(&m_cLock);

	CDelegate* pDel = FindDelegate( stCommand );
	if(!pDel )
	{
		// create a new delegate
		CDelegate del;
		del.Override( pFnt );

		m_map[ stCommand ] = del;
		return true;
	}

	// Override
	if(false ==  pDel->Override( pFnt) )
	{		
		//OutputDebugString("err");
		return false;
	}

	return true;
}

LRESULT CMessageManager::Call(std::string stCommand, WPARAM wParam, LPARAM lParam)
{
	CThreadLock lock(&m_cLock);
	// Find
	IT it = m_map.find( stCommand );
	if(m_map.end() == it)
	{		
		//OutputDebugString("err");
		return false;
	}

	// Call
	LRESULT nRet = it->second.Fire( wParam, lParam);
	
	if(nRet < 0)
	{		
		//OutputDebugString("err");
		return false;
	}

	return nRet;
}


LRESULT	CMessageManager::CallDefault(std::string stCommand, WPARAM wParam, LPARAM lParam)
{
	CThreadLock lock(&m_cLock);
	// Find
	IT it = m_map.find( stCommand );
	if(m_map.end() == it)
	{		
		//OutputDebugString("err");	
		return false;
	}

	// Call
	LRESULT nRet = it->second.FireDefault( wParam, lParam);

	if(nRet < 0)
	{		
		//OutputDebugString("err");
		return false;
	}

	return nRet;
}


bool CMessageManager::RemoveMsgH(std::string stCommand)
{
	CThreadLock lock(&m_cLock);
	// Find
	IT it = m_map.find( stCommand );
	if( m_map.end() == it)
	{		
		//OutputDebugString("err");

		return false;
	}

	// Remove
	it->second.Release();
	m_map.erase(it);
	return true;
}

bool CMessageManager::RemoveAppendMsgH(std::string stCommand, IFunctor* pFunctor)
{
	CThreadLock lock(&m_cLock);
	// Find
	IT it = m_map.find( stCommand );
	if( m_map.end() == it)
	{		
		//OutputDebugString("err");
		return false;
	}

	// AppendRemove
	it->second.ReleaseAppend(pFunctor);
	pFunctor->Release();

	return true;
}

bool CMessageManager::RemoveOverrideMsgH(std::string stCommand, IFunctor* pFunctor)
{
	CThreadLock lock(&m_cLock);
	// Find
	IT it = m_map.find( stCommand );
	if( m_map.end() == it)
	{		
		//OutputDebugString("err");
		return false;
	}

	// Remove
	it->second.ReleaseOverride(pFunctor);
	pFunctor->Release();

	return true;
}


SFINFOS CMessageManager::Info(std::string stCommand)
{
	/* WARN : Do NOT use assert, VERIFY, etc in this function */
	/* Because this fuction will be called in the watch window */

	// To return
	CThreadLock lock(&m_cLock);
	SFINFOS finfos = {NULL, NULL, NULL, NULL};

	/* Find */
	IT it = m_map.find( stCommand );
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


CDelegate* CMessageManager::FindDelegate( std::string stCommand )
{
	CThreadLock lock(&m_cLock);
	IT it = m_map.find( stCommand );
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
	//::OutputDebugString( "------- Dump handlers (begin)------\n");	
	#endif

	if(!bSort) 
	{
		for(IT it = m_map.begin(); it != m_map.end(); ++it) 
		{
			#ifdef _DEBUG
			printf(it->first.c_str());
			printf("\n");
			//::OutputDebugString( it->first.c_str() );			
			//::OutputDebugString( "\n" );
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
			//::OutputDebugString( it->c_str() );
			//::OutputDebugString( "\n" );
			#endif
		}
	}

	#ifdef _DEBUG
	printf("------- Dump handlers (end)------\n");
	//::OutputDebugString( "------- Dump handlers (end) ------\n");
	#endif
}
