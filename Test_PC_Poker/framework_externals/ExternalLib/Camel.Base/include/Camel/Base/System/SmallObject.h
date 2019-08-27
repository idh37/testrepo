#ifndef __Camel_Base_System_TPL_SmallObject__
#define __Camel_Base_System_TPL_SmallObject__

#include "SystemBase.h"
#include "MPHeap.h"
#include <Windows.h>

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace TPL
{
	template<typename T>
	class SmallObject : private NonCopyable
	{
	public:
		static void *operator new(size_t size)
		{
			if (size == 0)
			{
				size = 1;
			}

			void *result = NULL;

#pragma warning(disable:4127)
			while (true)
#pragma warning(default:4127)
			{
				result = MPHeap::Alloc(NULL, size);
				if (result != NULL)
				{
					break;
				}

				std::new_handler newHandler = std::set_new_handler(0);
				std::set_new_handler(newHandler);

				if (newHandler != NULL)
				{
					(*newHandler)();
				}
				else
				{
					//throw_exception(std::bad_alloc());
					throw std::bad_alloc(); 
				}
			}

			return result;
		}
		static void operator delete(void *p) throw()
		{
			if (p == NULL)
			{
				return;
			}

			MPHeap::Free(NULL, p);
			p = NULL;
		}
	};
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_TPL_SmallObject__