#ifndef __Camel_Base_System_TPL_NonCopyable__
#define __Camel_Base_System_TPL_NonCopyable__

#include "SystemBase.h"

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace TPL
{
	class NonCopyable
	{
	protected:
		NonCopyable(void)
		{
		}
		~NonCopyable(void)
		{
		}

	private:
		NonCopyable(const NonCopyable &);
		const NonCopyable &operator=(const NonCopyable &);
	};
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif
