#ifndef __Camel_Common_ThreadPool__
#define __Camel_Common_ThreadPool__

//#include <Camel/Base/System/Exceptions.h>
#include <Camel/Base/System/SmallObject.h>
#include <Camel/Base/System/System.h>

namespace Camel
{
    namespace Common
    {
        class ThreadPool : public Camel::Base::System::TPL::SmallObject<ThreadPool>
        {
        private:
            Camel::Base::System::ThreadPool::Handle handle_;

        public:
            explicit ThreadPool(size_t size)
                : handle_(Camel::Base::System::ThreadPool::CreateHandle(size))
            {
                if (handle_ == NULL)
                {
                    //throw_exception(NullPointerException("[ThreadPool::ThreadPool()] handle_ == NULL"));
                    return;
                }
            }
            ~ThreadPool(void)
            {
                Camel::Base::System::ThreadPool::DestroyHandle(handle_);
                handle_ = NULL;
            }

            size_t GetSize(void) const
            {
                return Camel::Base::System::ThreadPool::GetPoolSize(handle_);
            }

#if defined(_WIN32) || defined(_WIN64)
            bool BindCompletionCallback(HANDLE handle,
                Camel::Base::System::ThreadPool::OverlappedCompletionEvent completionCallback)
            {
                return Camel::Base::System::ThreadPool::BindCompletionCallback(
                    handle_, handle, completionCallback);
            }
#endif

            bool QueueWorkItem(Camel::Base::System::ThreadPool::ThreadEvent callback, void *context)
            {
                return Camel::Base::System::ThreadPool::QueueWorkItem(handle_, callback, context);
            }
            bool QueueWorkItem(Camel::Base::System::ThreadPool::OverlappedCompletionEvent callback,
                unsigned int errorCode, unsigned int numberOfBytesTransferred, void *context)
            {
                return Camel::Base::System::ThreadPool::QueueWorkItem(
                    handle_, callback, errorCode, numberOfBytesTransferred, context);
            }

            Camel::Base::System::ThreadPool::Handle GetHandle(void) const
            {
                return handle_;
            }
        };
    }
}

#endif
