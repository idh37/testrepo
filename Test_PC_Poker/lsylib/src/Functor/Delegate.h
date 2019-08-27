#pragma once

#include "Functor.h"
#include <list>

namespace LSY
{
typedef struct _tagFINFOS
{
	void*	pDefObj;	/**< An object's pointer of a default functor		*/
	void*	pDefFunc;	/**< An function's pointer of a default functor		*/
	void*	pOvrObj;	/**< An object's pointer of a overriding functor	*/
	void*	pOvrFunc;	/**< An function's pointer of a overriding functor	*/
}SFINFOS;


class UPG_LSYLIB_EXPORT CDelegate
{
public:
	CDelegate(bool bAutoRelease = false);
	~CDelegate(void);
	
	LRESULT	Fire(CMessage *pMsg);
	LRESULT	FireDefault(CMessage *pMsg);
	bool	Default ( IFunctor* pFunctor);
	bool	Override( IFunctor* pFunctor);
	bool	Append  ( IFunctor* pFunctor);
	SFINFOS	Info() const;
	void	Release();

	void    ReleaseOverride(IFunctor* pFunctor);
	void    ReleaseAppend(IFunctor* pFunctor);

protected:
	typedef std::list<IFunctor*>	TFunctorList;
	TFunctorList*	m_pAppendes;
	IFunctor*		m_pDefault;
	IFunctor*		m_pOverride;

	bool			m_bAutoRelease; 
};
}//namespace LSY