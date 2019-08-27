#ifndef __Camel_Base_System_Threading_ScopedTimedLock__
#define __Camel_Base_System_Threading_ScopedTimedLock__

#include "SystemBase.h"
#include "Semaphore.h"

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace Threading
{
	// ScopedTimedLock
	template<typename T>
	class ScopedTimedLock : private TPL::NonCopyable
	{
	private:
		T &mutex_;
		bool locked_;

	public:
		ScopedTimedLock(T *mutex, unsigned int milliseconds) : mutex_(*mutex), locked_(false)
		{
			TimedLock(milliseconds);
		}
		explicit ScopedTimedLock(T *mutex, bool initiallyLocked = true) : mutex_(*mutex), locked_(false)
		{
			if (initiallyLocked == true)
			{
				Lock();
			}
		}
		ScopedTimedLock(T &mutex, unsigned int milliseconds) : mutex_(mutex), locked_(false)
		{
			TimedLock(milliseconds);
		}
		explicit ScopedTimedLock(T &mutex, bool initiallyLocked = true) : mutex_(mutex), locked_(false)
		{
			if (initiallyLocked == true)
			{
				Lock();
			}
		}
		~ScopedTimedLock(void)
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
				assert(!"[ScopedTimedLock::Lock()] locked_ must be false.");
				throw "[ScopedTimedLock::Lock()] locked_ must be false.";
			}

			mutex_.Lock(INFINITE);
			locked_ = true;
		}
		void TimedLock(unsigned int milliseconds)
		{
			if (locked_ == true)
			{
				assert(!"[ScopedTimedLock::TimedLock()] locked_ must be false.");
				throw "[ScopedTimedLock::TimedLock()] locked_ must be false.";
			}

			mutex_.Lock(milliseconds);
			locked_ = true;
		}
		void Unlock(void)
		{
			if (locked_ == false)
			{
				assert(!"[ScopedTimedLock::Unlock()] locked_ must be true.");
				throw "[ScopedTimedLock::Unlock()] locked_ must be true.";
			}

			locked_ = false;
			mutex_.Unlock();
		}

		bool IsLocked(void) const
		{
			return locked_;
		}
	};
	
	typedef ScopedTimedLock<Semaphore> SMTimedGuardLock;
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif //__Camel_Base_System_Threading_ScopedTimedLock__
