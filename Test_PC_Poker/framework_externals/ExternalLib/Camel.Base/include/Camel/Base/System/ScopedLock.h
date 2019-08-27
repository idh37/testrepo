#ifndef __Camel_Base_System_Threading_ScopedLock__
#define __Camel_Base_System_Threading_ScopedLock__

#include "SystemBase.h"
#include "CriticalSection.h"
#include "NonCopyable.h"

#include <cassert>

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace Threading
{
	// ScopedLock
	template<typename T>
	class ScopedLock : private TPL::NonCopyable
	{
	private:
		T &mutex_;
		bool locked_;

	public:
		explicit ScopedLock(T *mutex, bool initiallyLocked = true) : mutex_(*mutex), locked_(false)
		{
			if (initiallyLocked == true)
			{
				Lock();
			}
		}
		explicit ScopedLock(T &mutex, bool initiallyLocked = true) : mutex_(mutex), locked_(false)
		{
			if (initiallyLocked == true)
			{
				Lock();
			}
		}
		~ScopedLock(void)
		{
			if (locked_ == true)
			{
				Unlock();
			}
		}

		void Lock(void)
		{
			if (locked_ == true)
			{
				assert(!"[ScopedLock::Lock()] locked_ must be false.");
				throw "[ScopedLock::Lock()] locked_ must be false.";
			}

			mutex_.Lock();
			locked_ = true;
		}
		void Unlock(void)
		{
			if (locked_ == false)
			{
				assert(!"[ScopedLock::Unlock()] locked_ must be true.");
				throw "[ScopedLock::Unlock()] locked_ must be true.";
			}

			locked_ = false;
			mutex_.Unlock();
		}

		bool IsLocked(void) const
		{
			return locked_;
		}
	};
	
	typedef ScopedLock<CriticalSection> CSGuardLock;
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_Threading_ScopedLock__