#include "stdafx.h"
#include "IObject.h"
#include "../../Define.h"
#include "../../Functor/Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
IObject::IObject()
: m_dwLogicStartTime(0)
{
}

IObject::~IObject()
{
}

bool IObject::AddHandler(int nCommond, IFunctor *pFunctor)
{
	return m_cMessage.AddMsgH(nCommond, pFunctor);
}

bool IObject::AppendHandler(int nCommond, IFunctor *pFunctor)
{
	return m_cMessage.AppendMsgH(nCommond, pFunctor);
}

bool IObject::OverrideHandler(int nCommond, IFunctor *pFunctor)
{
	return m_cMessage.OverrideMsgH(nCommond, pFunctor);
}

bool IObject::RemoveHandler(int nCommond)
{
	return m_cMessage.RemoveMsgH(nCommond);
}

bool IObject::RemoveAppendHandler(int nCommond, IFunctor *pFunctor)
{
	return m_cMessage.RemoveAppendMsgH(nCommond, pFunctor);
}

bool IObject::RemoveOverrideHandler(int nCommond, IFunctor *pFunctor)
{
	return m_cMessage.RemoveOverrideMsgH(nCommond, pFunctor);
}

} //namespace LSY