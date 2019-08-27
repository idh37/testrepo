#include "StdAfx.h"
#include "Delegate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace LSY
{
///////////////////////////////
// Delgate
/**
 * The contstuctor of CDelegate.
 * @param	bAutoRelease	if true, Release() is called in the dectructor.
 *							If you want to use this class with STL containers,
 *							you should set bAutoReleast to false.
 * @return	 
 */
CDelegate::CDelegate(bool bAutoRelease)
{
	m_pDefault  = NULL;
	m_pOverride = NULL;
	m_pAppendes = NULL;

	m_bAutoRelease = bAutoRelease;
}

CDelegate::~CDelegate(void)
{
	if(m_bAutoRelease)
	{
		Release();
	}
}

/**
* Release resources.
* A client of this class should call this functiona manually.
*/
void CDelegate::Release()
{
	if(m_pDefault)
	{
		m_pDefault->Release();
		m_pDefault = NULL;
	}

	if(m_pOverride)
	{
		m_pOverride->Release();
		m_pOverride = NULL;
	}

	if(m_pAppendes)
	{
		TFunctorList::iterator it;
		for(it = m_pAppendes->begin(); it != m_pAppendes->end(); ++it )
		{
			(*it)->Release();
		}

		delete m_pAppendes;
		m_pAppendes = NULL;
	}	
}


void CDelegate::ReleaseOverride(IFunctor* pFunctor)
{
	if(m_pOverride == pFunctor)
	{
		m_pOverride->Release();
	}
}

void CDelegate::ReleaseAppend(IFunctor* pFunctor)
{
	if(m_pAppendes)
	{
		TFunctorList::iterator it;
		for(it = m_pAppendes->begin(); it != m_pAppendes->end(); ++it )
		{
			if(*(*it) == *pFunctor)
			{
				(*it)->Release();
				m_pAppendes->erase(it);
				break;
			}			
		}
	}
}

LRESULT CDelegate::Fire(CMessage *pMsg)
{
	LRESULT ret = -1;

	bool bOverride = m_pOverride?true:false;
	bool bDefault = m_pDefault?true:false;
	bool bAppendes = m_pAppendes?true:false;

	if(bOverride)
	{
		assert( NULL != m_pDefault);

		ret = m_pOverride->Call(pMsg);
	}
	else if(bDefault)	
	{
		ret = m_pDefault->Call(pMsg);
	}

	if( bAppendes )
	{
		TFunctorList::iterator it;
		for(it = m_pAppendes->begin(); it != m_pAppendes->end(); ++it )
		{
			(*it)->Call(pMsg);
		}

		// If default or override handler does not exist,
		// always returns 0. (ignore the return values of appendees)
		if( ! m_pOverride || ! m_pDefault )
		{
			return RESULT_OK;
		}
	}

	return ret;
}

LRESULT	CDelegate::FireDefault(CMessage *pMsg)
{
	LRESULT ret = -1;

	if(m_pDefault)	
	{
		ret = m_pDefault->Call(pMsg);
	}

	return ret;
}


/**
*	Sets a default handler.
*   @Param			pFunctor	A pointer to the functor returned by Fnt
*	@Example		delegate.Default( Fnt( &client, &CClient::OnCommand) );
*/
bool CDelegate::Default( IFunctor* pFunctor)
{
	assert( NULL == m_pDefault);
	if(NULL != m_pDefault)
	{
		pFunctor->Release();
		return false;
	}

	m_pDefault = pFunctor;
	return true;
}


/**
*	Overrides the default handler.
*   @Param			pFunctor	A pointer to the functor returned by Fnt
*	@Example		delegate.Override( Fnt( &client, &CClient::OnCommand) );
*/
bool CDelegate::Override( IFunctor* pFunctor)
{
	assert( NULL == m_pOverride );

	if(!( NULL == m_pOverride ))
	{
		pFunctor->Release();
		return false;
	}

	m_pOverride = pFunctor;
	return true;
}

/**
 * Append an additional handler.
 * This method could be called multiple times.
 * @param	pFunctor 	A pointer to the functor returned by Fnt
 * @Example delegate.Append( Fnt( &client, &CClient::OnCommand) );
 */
bool CDelegate::Append(	IFunctor* pFunctor)
{
	if(!m_pAppendes)
	{
		/*#ifdef _DEBUG	
				m_pAppendes = DEBUG_NEW TFunctorList();
		#else*/
		m_pAppendes = new TFunctorList();
		//#endif		
	}

	m_pAppendes->push_back( pFunctor );
	return true;
}


SFINFOS	CDelegate::Info() const
{
	SFINFOS finfos = {NULL, NULL, NULL, NULL};

	if(m_pDefault)
	{
		finfos.pDefFunc = m_pDefault->Info().pFunc;
		finfos.pDefObj	= m_pDefault->Info().pObj;
	}

	if(m_pOverride)
	{
		finfos.pOvrFunc = m_pOverride->Info().pFunc;
		finfos.pOvrObj	= m_pOverride->Info().pObj;
	}

	return finfos;
}
} //namespace LSY