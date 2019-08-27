#ifndef __Camel_Base_System_SystemHandles__
#define __Camel_Base_System_SystemHandles__

namespace Camel
{
namespace Base
{
	namespace System
	{
		namespace Heap
		{
			typedef struct _Block
			{
			} *Block;
		}
		namespace LFHeap
		{
			typedef struct _Block
			{
			} *Block;
		}
		namespace SPHeap
		{
			typedef struct _Block
			{
			} *Block;
		}
		namespace MPHeap
		{
			typedef struct _Handle
			{
			} *Handle;
		}
		namespace Stream
		{
			typedef struct _Handle
			{
			} *Handle;

		}
		namespace ThreadPool
		{
			typedef struct _Handle
			{
			} *Handle;
		}
		namespace Timers
		{
			namespace TimerQueue
			{
				typedef struct _Handle
				{
				} *Handle;
			}
			namespace Timer
			{
				typedef struct _Handle
				{
				} *Handle;
			}
		}
	}
}
}
#endif // __Camel_Base_System_SystemHandles__