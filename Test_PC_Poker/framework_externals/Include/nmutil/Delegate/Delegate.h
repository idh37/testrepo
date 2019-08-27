/**    
@file    Delegate.h
@date    2011/2/11
@author  PJH(gamking@cj.com)
@brief   Functor 중간 관리자
*/

#pragma once

#include "../UtilDefine.h"
#include "Functor.h"
#include <list>

namespace NMBASE
{
	namespace UTIL
	{

		typedef struct _tagFINFOS
		{
			void*	pDefObj;	/**< An object's pointer of a default functor		*/
			void*	pDefFunc;	/**< An function's pointer of a default functor		*/
			void*	pOvrObj;	/**< An object's pointer of a overriding functor	*/
			void*	pOvrFunc;	/**< An function's pointer of a overriding functor	*/
		}SFINFOS;


		class NMUTIL_CLASS CDelegate
		{
		public:
			CDelegate(BOOL bAutoRelease = FALSE);
			~CDelegate(void);

			LRESULT	Fire(WPARAM& wParam, LPARAM& lParam);
			LRESULT	FireDefault(WPARAM& wParam, LPARAM& lParam);
			BOOL	Default ( IFunctor* pFunctor);
			BOOL	Override( IFunctor* pFunctor);
			BOOL	Append  ( IFunctor* pFunctor);
			SFINFOS	Info() const;
			void	Release();

			void    ReleaseOverride(IFunctor* pFunctor);
			void    ReleaseAppend(IFunctor* pFunctor);

		protected:
			typedef std::list<IFunctor*>	TFunctorList;
			TFunctorList*	m_pAppendes;
			IFunctor*		m_pDefault;
			IFunctor*		m_pOverride;

			BOOL			m_bAutoRelease; 
		};

	}//namespace UTIL

}	//namespace NMBASE