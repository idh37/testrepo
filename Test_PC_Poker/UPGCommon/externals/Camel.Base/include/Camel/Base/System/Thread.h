#ifndef __Camel_Base_System_Threading_Thread__
#define __Camel_Base_System_Threading_Thread__

#include "SystemBase.h"
#include "NonCopyable.h"

#include <Windows.h>

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace Threading
{
	class CAMEL_BASE_SYSTEM_API Thread
	{
	private:
		struct Impl;
		Impl *impl_;

	public:
		Thread(void);
		virtual ~Thread(void);

		bool Start(void);
		void Stop(void);

		bool Suspend(void);
		bool Resume(void);
		void Terminate(void);
		bool WaitFor(unsigned int timeout = INFINITE);

		void SetFreeOnTerminate(bool value);
		bool GetFreeOnTerminate(void) const;
		void SetFinished(bool value);
		bool GetFinished(void) const;
		bool SetPriority(int value);
		int GetPriority(void);
		void SetSuspended(bool value);
		bool GetSuspended(void) const;

		HANDLE GetHandle(void) const;
		unsigned int GetThreadID(void) const;

	protected:
		bool Terminated(void) const;
		void SetThreadName(const char *name);
		void SetReturnValue(int value);
		int GetReturnValue(void) const;

		virtual void Execute(void) = 0;

	private:
		static unsigned int __stdcall InternalThreadProc(Thread *thread);
	};

	CAMEL_BASE_SYSTEM_API bool __stdcall SetThreadName(unsigned int threadID, const char *name);
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_Threading_Thread__
