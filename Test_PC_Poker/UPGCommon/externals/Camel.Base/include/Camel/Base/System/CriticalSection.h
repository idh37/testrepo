#ifndef __Camel_Base_System_Threading_CriticalSection__
#define __Camel_Base_System_Threading_CriticalSection__

#include "SystemBase.h"

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace Threading
{
	class CAMEL_BASE_SYSTEM_API CriticalSection
	{
	private:
		struct Impl;
		Impl *impl_;

	public:
		explicit CriticalSection(unsigned int spinCount = 4000);
		~CriticalSection(void);

		void Enter(void);
		bool TryEnter(void);
		void Leave(void);

		void Lock(void);
		void Unlock(void);
		
		unsigned long SetSpinCount(unsigned long spinCount);

	private:
		CriticalSection(const CriticalSection &);
		const CriticalSection &operator=(const CriticalSection &);
	};
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_Threading_CriticalSection__