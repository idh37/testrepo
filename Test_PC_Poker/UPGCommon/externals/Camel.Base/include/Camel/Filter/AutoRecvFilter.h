#ifndef __Camel_Filter_AutoRecvFilter__
#define __Camel_Filter_AutoRecvFilter__

#include <Camel/Common/Filter.h>
//#include <Camel/Common/Exceptions.h>

namespace Camel
{
    namespace Filter
    {
        namespace Detail
        {
            namespace AutoRecvFilter
            {
                const char * const FILTER_NAME = "Camel::Filter::AutoRecvFilter";
                const char * const CONTEXT_NAME = "Camel::Filter::AutoRecvFilter::Context";
            }
        }

        class AutoRecvFilter : public Common::IFilter
        {
        private:
            size_t recvBufferSize_;
            size_t recvPendingCount_;

        public:
            AutoRecvFilter(void)
                : Common::IFilter(Detail::AutoRecvFilter::FILTER_NAME),
                recvBufferSize_(1024), recvPendingCount_(1)
            {}
            AutoRecvFilter(size_t recvBufferSize, size_t recvPendingCount)
                : Common::IFilter(Detail::AutoRecvFilter::FILTER_NAME),
                recvBufferSize_(recvBufferSize), recvPendingCount_(recvPendingCount)
            {
                if (recvBufferSize_ == 0)
                {
                    //Common::throw_exception(
                    //    Common::InvalidArgumentException(
                    //        "[AutoRecvFilter::AutoRecvFilter()] recvBufferSize_ == 0"));
                    return;
                }
                if (recvPendingCount_ == 0)
                {
                    //Common::throw_exception(
                    //    Common::InvalidArgumentException(
                    //        "[AutoRecvFilter::AutoRecvFilter()] recvPendingCount_ == 0"));
                    return;
                }
            }
            virtual ~AutoRecvFilter(void)
            {}

        protected:
            virtual void OnOpened(Camel::Base::Network::Filter::Handle next,
                Camel::Base::Network::Link::Handle link)
            {
                for (size_t i = 0; i != recvPendingCount_; ++i)
                {
                    Camel::Base::Network::Link::Recv(link, recvBufferSize_);
                }

                Camel::Base::Network::Filter::DoOnLinkOpened(next, link);
            }
            virtual void OnReceived(Camel::Base::Network::Filter::Handle next,
                Camel::Base::Network::Link::Handle link,
                Camel::Base::System::SPHeap::Block buffer, size_t size)
            {
                Camel::Base::Network::Link::Recv(link, recvBufferSize_);

                Camel::Base::Network::Filter::DoOnReceived(next, link, buffer, size);
            }
        };
    }
}

#endif
