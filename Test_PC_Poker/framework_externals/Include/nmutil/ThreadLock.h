#pragma once

#include "UtilDefine.h"

namespace NMBASE
{
	namespace UTIL
	{

		class NMUTIL_CLASS CThreadLock
		{
		protected:
			CRITICAL_SECTION	*m_pSection;

		public:
			CThreadLock(LPCRITICAL_SECTION pSection);
			virtual ~CThreadLock();
		};

	}//namespace UTIL

}//namespace NMBASE

