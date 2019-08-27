#ifndef __Camel_Base_System_Threading_ScopedRWLock__
#define __Camel_Base_System_Threading_ScopedRWLock__

#include "SystemBase.h"
#include "RWCriticalSection.h"
#include "NonCopyable.h"

#include <cassert>

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace Threading
{
	// ScopedRWLock
	namespace ScopedRWLockState
	{
		enum TRWLockState
		{
			Unlocked = 0,
			ReadLocked = 1,
			WriteLocked = 2
		};
	}

	template<typename T>
	class ScopedRWLock : private TPL::NonCopyable
	{
	private:
		T &mutex_;
		ScopedRWLockState::TRWLockState state_;

	public:
		ScopedRWLock(T *mutex, ScopedRWLockState::TRWLockState initialState) : mutex_(*mutex), state_(ScopedRWLockState::Unlocked)
		{
			if (initialState == ScopedRWLockState::ReadLocked)
			{
				ReadLock();
			}
			else if (initialState == ScopedRWLockState::WriteLocked)
			{
				WriteLock();
			}
		}
		ScopedRWLock(T &mutex, ScopedRWLockState::TRWLockState initialState) : mutex_(mutex), state_(ScopedRWLockState::Unlocked)
		{
			if (initialState == ScopedRWLockState::ReadLocked)
			{
				ReadLock();
			}
			else if (initialState == ScopedRWLockState::WriteLocked)
			{
				WriteLock();
			}
		}
		~ScopedRWLock(void)
		{
			if (state_ != ScopedRWLockState::Unlocked)
			{
				Unlock();
			}
		}

		void ReadLock(void)
		{
			if (state_ != ScopedRWLockState::Unlocked)
			{
				assert(!"[ScopedRWLock::ReadLock()] state_ must be ScopedRWLockState::Unlocked.");
				throw "[ScopedRWLock::ReadLock()] state_ must be ScopedRWLockState::Unlocked.";
			}

			mutex_.ReadLock();
			state_ = ScopedRWLockState::ReadLocked;
		}
		void WriteLock(void)
		{
			if (state_ != ScopedRWLockState::Unlocked)
			{
				assert(!"[ScopedRWLock::WriteLock()] state_ must be ScopedRWLockState::Unlocked.");
				throw "[ScopedRWLock::WriteLock()] state_ must be ScopedRWLockState::Unlocked.";
			}

			mutex_.WriteLock();
			state_ = ScopedRWLockState::WriteLocked;
		}
		void Unlock(void)
		{
			if (state_ == ScopedRWLockState::Unlocked)
			{
				assert(!"[ScopedRWLock::Unlock()] state_ must not be ScopedRWLockState::Unlocked.");
				throw "[ScopedRWLock::Unlock()] state_ must not be ScopedRWLockState::Unlocked.";
			}

			if (state_ == ScopedRWLockState::ReadLocked)
			{
				mutex_.ReadUnlock();
			}
			else if (state_ == ScopedRWLockState::WriteLocked)
			{
				mutex_.WriteUnlock();
			}
		}

		bool IsLocked(void) const
		{
			return state_ != ScopedRWLockState::Unlocked;
		}
		bool IsReadLocked(void) const
		{
			return state_ == ScopedRWLockState::ReadLocked;
		}
		bool IsWriteLocked(void) const
		{
			return state_ == ScopedRWLockState::WriteLocked;
		}

		ScopedRWLockState::TRWLockState GetState(void) const
		{
			return state_;
		}
	};

	typedef ScopedRWLock<RWCriticalSection> RWCSGuardLock;
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif //__Camel_Base_System_Threading_ScopedRWLock__