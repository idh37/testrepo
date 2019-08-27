#ifndef __Camel_Common_Filter__
#define __Camel_Common_Filter__

//#include <Camel/Common/Exceptions.h>
#include <Camel/Base/System/SmallObject.h>
#include <Camel/Base/System/System.h>
#include <Camel/Base/Network/Network.h>

namespace Camel
{
    namespace Common
    {
        class IFilter : public Camel::Base::System::TPL::SmallObject<IFilter>
        {
        private:
            Camel::Base::Network::Filter::Handle filter_;
            Camel::Base::System::TPL::string name_;

        public:
            explicit IFilter(const char *name)
                : filter_(Camel::Base::Network::Filter::CreateHandle()), name_(name)
            {
                if (filter_ == NULL)
                {
                    //throw_exception(NullPointerException("[IFilter::IFilter()] filter_ == NULL"));
                    return;
                }

                Camel::Base::Network::Filter::SetOnLinkOpened(filter_, &OnLinkOpened, this);
                Camel::Base::Network::Filter::SetOnLinkClosed(filter_, &OnLinkClosed, this);
                Camel::Base::Network::Filter::SetOnSend(filter_, &OnLinkSend, this);
                Camel::Base::Network::Filter::SetOnReceived(filter_, &OnLinkReceived, this);
            }
            virtual ~IFilter(void)
            {
                Camel::Base::Network::Filter::DestroyHandle(filter_);
                filter_ = NULL;
            }

            Camel::Base::Network::Filter::Handle GetHandle(void) const
            {
                return filter_;
            }

            const char *GetName(void) const
            {
                return name_.c_str();
            }

        protected:
            virtual void OnOpened(Camel::Base::Network::Filter::Handle next, Camel::Base::Network::Link::Handle link)
            {
                Camel::Base::Network::Filter::DoOnLinkOpened(next, link);
            }
            virtual void OnClosed(Camel::Base::Network::Filter::Handle next,
                Camel::Base::Network::Link::Handle link)
            {
                Camel::Base::Network::Filter::DoOnLinkClosed(next, link);
            }
            virtual void OnSend(Camel::Base::Network::Filter::Handle next,
                Camel::Base::Network::Link::Handle link,
                Camel::Base::System::SPHeap::Block buffer, size_t size)
            {
                Camel::Base::Network::Filter::DoOnSend(next, link, buffer, size);
            }
            virtual void OnReceived(Camel::Base::Network::Filter::Handle next,
                Camel::Base::Network::Link::Handle link,
                Camel::Base::System::SPHeap::Block buffer, size_t size)
            {
                Camel::Base::Network::Filter::DoOnReceived(next, link, buffer, size);
            }

        private:
            static void __stdcall OnLinkOpened(Camel::Base::Network::Filter::Handle next,
                Camel::Base::Network::Link::Handle link, void *data)
            {
                IFilter *filter = static_cast<IFilter *>(data);
                filter->OnOpened(next, link);
            }
            static void __stdcall OnLinkClosed(Camel::Base::Network::Filter::Handle next,
                Camel::Base::Network::Link::Handle link, void *data)
            {
                IFilter *filter = static_cast<IFilter *>(data);
                filter->OnClosed(next, link);
            }
            static void __stdcall OnLinkSend(Camel::Base::Network::Filter::Handle next,
                Camel::Base::Network::Link::Handle link,
                Camel::Base::System::SPHeap::Block buffer, size_t size, void *data)
            {
                IFilter *filter = static_cast<IFilter *>(data);
                filter->OnSend(next, link, buffer, size);
            }
            static void __stdcall OnLinkReceived(Camel::Base::Network::Filter::Handle next,
                Camel::Base::Network::Link::Handle link,
                Camel::Base::System::SPHeap::Block buffer, size_t size, void *data)
            {
                IFilter *filter = static_cast<IFilter *>(data);
                filter->OnReceived(next, link, buffer, size);
            }
        };
    }
}

#endif // __Camel_Common_Filter__
