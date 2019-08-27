#pragma once
#include <assert.h>

namespace LSY
{
/**
 * This interface represents a functor.
 */
struct ILogicFunctor
{
	virtual void	Logic(const DWORD &dwTime) = 0;
	virtual void	Release()			= 0;
	virtual bool	operator == (IFunctor& pFunctor) = 0;
};

/** None 멤버 펑션 포인터  */
typedef void (*P_LOGICFUNC)(const DWORD &dwTime);


/** 멤버 펑션 포인터 */
template<typename T>
class LOGIGTYPE
{
public:
	typedef void (T::*P_MFUNC)(const DWORD &dwTime);
};

// For member functions
template<typename T>
class CLogicFunctor : public ILogicFunctor
{
public:
	CLogicFunctor(T* pObj, typename LOGIGTYPE<T>::P_MFUNC pFunc)
	{
		assert( pObj && pFunc);

		m_pObj	= pObj;
		m_pFunc	= pFunc;
	}

	virtual Logic(const DWORD &dwTime)
	{
		(m_pObj->*m_pFunc)(dwTime);
	}

	virtual void Release()
	{
		// This makes the class use the new and delete operator of the same CRT.
		delete this;
	}
	
	virtual bool operator == (IFunctor& pFunctor)
	{
		CLogicFunctor* pMFunctor = dynamic_cast<CLogicFunctor*>(&pFunctor);

		if( pMFunctor && (m_pObj == pMFunctor->GetMemberObjet() && m_pFunc == pMFunctor->GetFunctor()) )
			return true;

		return false;
	}

	T* GetMemberObjet(){return m_pObj;}

	typename LOGIGTYPE<T>::P_MFUNC GetFunctor(){return m_pFunc;}


protected:
	T*								m_pObj;
	typename LOGIGTYPE<T>::P_MFUNC	m_pFunc;

private:
	/* create instance only on heap memory*/
	~CMFunctor()
	{
	}
};

// 멤버 펑션 포인터
template<typename T>
inline ILogicFunctor* FntLogic(T* pObj, typename LOGIGTYPE<T>::P_MFUNC	pFunc)
{
// Using MFC Code
#ifdef _DEBUG	
	return DEBUG_NEW CLogicFunctor<T>(pObj, pFunc);
#else
	return new CLogicFunctor<T>(pObj, pFunc);
#endif
}
} //namespace LSY