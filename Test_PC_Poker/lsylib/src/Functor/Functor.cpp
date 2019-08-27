#include "StdAfx.h"
#include "Functor.h"
#include "Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace LSY
{
// For non-member functions
IFunctor* Fnt(P_SFUNC pFunc)
{
	return new CSFunctor(pFunc);
}


CFunctor::CFunctor()
{
	m_pFnt = NULL;
}

CFunctor::~CFunctor()
{
	if( m_pFnt )
	{
		m_pFnt->Release();
	}
}

void CFunctor::operator=(IFunctor* fnt)
{
	if( m_pFnt )
	{
		m_pFnt->Release();
	}

	m_pFnt = fnt;
}

LRESULT	CFunctor::Call( CMessage *pMsg )
{
	if( m_pFnt )
	{
		return m_pFnt->Call( pMsg );
	}

	return RESULT_OK;
}

IFunctor* CFunctor::Get() const
{
	return m_pFnt;
}
}//namespace LSY

