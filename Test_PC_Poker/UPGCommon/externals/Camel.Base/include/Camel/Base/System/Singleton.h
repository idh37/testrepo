#ifndef __Camel_Base_System_TPL_Singleton__
#define __Camel_Base_System_TPL_Singleton__

#include "SystemBase.h"
#include "SyncObject.h"
#include <stdexcept>

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace TPL
{
	template<typename T>
	class Singleton
	{
	private:
		static volatile T *instance_;
		static bool destroyed_;
		static Threading::CriticalSection lock_;

	public:
		inline static T &Instance(void)
		{
			if (instance_ == NULL)
			{
				MakeInstance();
			}

			return const_cast<T &>(*instance_);
		}

	private:
		Singleton(void)
		{
			InitializeCriticalSection(&csObject_);
		}

		static void MakeInstance(void)
		{
			Threading::ScopedLock<Threading::CriticalSection> lock(lock_);

			if (instance_ == NULL)
			{
				if (destroyed_ == true)
				{
					throw std::logic_error("[Camel::Base::System::TPL::Singleton] Dead reference detected.");
				}

				instance_ = new T;
				if (NULL == instance_)
				{
					throw std::bad_alloc();
				}
                
				atexit(DestroySingleton);
			}
		}
		static void DestroySingleton(void)
		{
			delete instance_;
			instance_ = NULL;

			destroyed_ = true;
		}
	};

	template<typename T> volatile T *Singleton<T>::instance_ = NULL;
	template<typename T> bool Singleton<T>::destroyed_ = false;
	template<typename T> Threading::CriticalSection Singleton<T>::lock_;
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif
