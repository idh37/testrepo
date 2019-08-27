#pragma once
#include <assert.h>

namespace LSY
{
#define RESULT_FAIL 0	// 실패
#define RESULT_OK   1	// 성공

class CMessage;

// 펑터 타입
enum EFUNCTORTYPE
{
	EMFUNCTOR = 0,	// 멤버 펑터
	ESFUNCTOR = 1,	// 비멤버 펑터
};
/**
 * This struct includes functor information.
 * @see IFunctor::Info()
 */
struct FINFO
{
	void*	pObj;	/**< An object's pointer	*/
	void*	pFunc;	/**< An function's pointer	*/
};

/**
 * This interface represents a functor.
 */
struct IFunctor
{
	EFUNCTORTYPE m_eFunctorType;	// 0: CMFunctor 1: CSFunctor(사용 안함)

	virtual LRESULT	Call(CMessage *pMsg) = 0;
	virtual void	Release()			= 0;
	virtual FINFO	Info()				= 0;
	virtual bool	operator == (IFunctor& pFunctor) = 0;
};


/** None 멤버 펑션 포인터  */
typedef LRESULT (*P_SFUNC)(CMessage *pMsg);


/** 멤버 펑션 포인터 */
template<typename T>
class TYPE
{
public:
	typedef LRESULT (T::*P_MFUNC)(CMessage *pMsg);
};

/** A utility function for converting a member-function's pointer */
template <typename Dest, typename Src>
Dest super_cast(Src pFunc)
{
	union Convert
	{
		Dest	target;
		Src		src;
	} convert;
	
	convert.src = pFunc;
	return convert.target;
}

//**********************************************************//
//						멤버 Functor						//
//**********************************************************//


// For member functions
template<typename T>
class CMFunctor : public IFunctor
{
public:
	CMFunctor(T* pObj, typename TYPE<T>::P_MFUNC pFunc)
	{
		assert( pObj && pFunc);

		m_pObj	= pObj;
		m_pFunc	= pFunc;
		m_eFunctorType	= EMFUNCTOR;
	}

	virtual LRESULT Call(CMessage *pMsg)
	{
		return (m_pObj->*m_pFunc)(pMsg);
	}

	virtual void Release()
	{
		// This makes the class use the new and delete operator of the same CRT.
		delete this;
	}

	virtual FINFO Info()
	{
		FINFO finfo;
		finfo.pFunc = super_cast<void*>( m_pFunc );
		finfo.pObj	= reinterpret_cast<void*>( m_pObj );
		return finfo;
	}
	
	virtual bool operator == (IFunctor& pFunctor)
	{
		CMFunctor* pMFunctor = dynamic_cast<CMFunctor*>(&pFunctor);

		if( pMFunctor && (m_pObj == pMFunctor->GetMemberObjet() && m_pFunc == pMFunctor->GetFunctor()) )
			return true;

		return false;
	}

	T* GetMemberObjet(){return m_pObj;}

	typename TYPE<T>::P_MFUNC GetFunctor(){return m_pFunc;}


protected:
	T*							m_pObj;
	typename TYPE<T>::P_MFUNC	m_pFunc;

private:
	/* create instance only on heap memory*/
	~CMFunctor()
	{
	}
};

// 멤버 펑션 포인터
template<typename T>
inline IFunctor* Fnt(T* pObj, typename TYPE<T>::P_MFUNC	pFunc)
{
// Using MFC Code
#ifdef _DEBUG	
	return DEBUG_NEW CMFunctor<T>(pObj, pFunc);
#else
	return new CMFunctor<T>(pObj, pFunc);
#endif
}



//**********************************************************//
//						비 멤버 Functor						//
//**********************************************************//


// For non-member functions
class CSFunctor : public IFunctor
{
public:
	CSFunctor(P_SFUNC pFunc)
	{
		assert( pFunc);

		m_pFunc = pFunc;
		m_eFunctorType	= ESFUNCTOR;
	}

	virtual LRESULT Call(CMessage *pMsg)
	{
		return (*m_pFunc)(pMsg);
	}

	virtual void Release()
	{
		// This makes the class use the new and delete operator of the same CRT.
		delete this;
	}

	virtual FINFO Info()
	{
		FINFO finfo;
		finfo.pFunc = reinterpret_cast<void*>( m_pFunc);
		finfo.pObj	= NULL;
		return finfo;
	}

	virtual bool operator == (IFunctor& pFunctor)
	{
		CSFunctor* pSFunctor = dynamic_cast<CSFunctor*>(&pFunctor);

		if(pSFunctor && m_pFunc == pSFunctor->GetFunctor())
			return true;

		return false;
	}
	
	P_SFUNC GetFunctor(){return m_pFunc;}


protected:
	P_SFUNC	m_pFunc;

private:
	/* create instance only on heap memory*/
	~CSFunctor()
	{
	}
};


// For non-member functions
UPG_LSYLIB_EXPORT IFunctor* Fnt(P_SFUNC pFunc);



//**********************************************************//
//					Functor 유틸 함수						//
//**********************************************************//

class UPG_LSYLIB_EXPORT CFunctor
{
protected:
	IFunctor*	m_pFnt;
public:
	CFunctor();
	~CFunctor();

	void		operator=(IFunctor* fnt);
	LRESULT		Call( CMessage *pMsg);
	IFunctor*	Get() const;
};
} //namespace LSY