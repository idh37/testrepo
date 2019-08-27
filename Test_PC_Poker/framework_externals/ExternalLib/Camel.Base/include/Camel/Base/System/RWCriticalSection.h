#ifndef __Camel_Base_System_Threading_RWCriticalSection__
#define __Camel_Base_System_Threading_RWCriticalSection__

#include "SystemBase.h"

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace Threading
{
	class CAMEL_BASE_SYSTEM_API RWCriticalSection
	{
	private:
		struct Impl;
		Impl *impl_;

	public:
		RWCriticalSection(void);
		~RWCriticalSection(void);

		void ReadLock(void);
		void ReadUnlock(void);
		void WriteLock(void);
		void WriteUnlock(void);

	private:
		void InternalUnlock(void);

	private:
		RWCriticalSection(const RWCriticalSection &);
		const RWCriticalSection &operator=(const RWCriticalSection &);
	};
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_Threading_RWCriticalSection__