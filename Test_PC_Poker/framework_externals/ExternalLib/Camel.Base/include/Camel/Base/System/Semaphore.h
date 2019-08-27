#ifndef __Camel_Base_System_Threading_Semaphore__
#define __Camel_Base_System_Threading_Semaphore__

#include "SystemBase.h"
#include <Windows.h>

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace Threading
{
	class CAMEL_BASE_SYSTEM_API Semaphore
	{
	private:
		struct Impl;
		Impl *impl_;

	public:
		Semaphore(int initialCount, int maximumCount, const char *name = NULL, LPSECURITY_ATTRIBUTES sa = NULL);
		Semaphore(const char *nameToOpen, unsigned int desiredAccess = SEMAPHORE_ALL_ACCESS, bool inheritHandle = false);
		~Semaphore(void);

		bool Release(int releaseCount, int *previousCount = NULL);

		bool Wait(unsigned int milliseconds = INFINITE);
		bool WaitEx(bool alertable, unsigned int milliseconds = INFINITE);

		bool Lock(unsigned int milliseconds = INFINITE);
		bool Unlock(void);

	private:
		Semaphore(const Semaphore &);
		const Semaphore &operator=(const Semaphore &);
	};
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_Threading_Semaphore__