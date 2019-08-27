#ifndef __Camel_Base_System_TPL_ThreadSafeObject__
#define __Camel_Base_System_TPL_ThreadSafeObject__

#include "SyncObject.h"
#include "NonCopyable.h"

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace TPL
{
	namespace Private
	{
		template<typename T>
		struct AddPointer
		{
			typedef T* PointerType;
			typedef T PointeeType;
		};
		template<typename T>
		struct AddPointer<T *>
		{
			typedef T* PointerType;
			typedef T PointeeType;
		};
	}

	template<typename T>
	class ThreadSafeObject : private NonCopyable
	{
	private:
		typename Private::AddPointer<T>::PointeeType obj_;
		Threading::CriticalSection lock_;

	public:
		ThreadSafeObject(void)
		{
		}
		~ThreadSafeObject(void)
		{
		}

		typename Private::AddPointer<T>::PointerType Lock(void)
		{
			lock_.Enter();

			return &obj_;
		}
		void Unlock(void)
		{
			lock_.Leave();
		}
	};

	template<typename T>
	class RWThreadSafeObject : private NonCopyable
	{
	private:
		typename Private::AddPointer<T>::PointeeType obj_;
		Threading::RWCriticalSection lock_;

	public:
		RWThreadSafeObject(void)
		{
		}
		~RWThreadSafeObject(void)
		{
		}

		typename Private::AddPointer<T>::PointerType ReadLock(void)
		{
			lock_.ReadLock();

			return &obj_;
		}
		void ReadUnlock(void)
		{
			lock_.ReadUnlock();
		}
		typename Private::AddPointer<T>::PointerType WriteLock(void)
		{
			lock_.WriteLock();

			return &obj_;
		}
		void WriteUnlock(void)
		{
			lock_.WriteUnlock();
		}
	};
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_TPL_ThreadSafeObject__
