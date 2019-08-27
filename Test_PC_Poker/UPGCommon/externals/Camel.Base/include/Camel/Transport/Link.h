#ifndef __Camel_Transport_Link__
#define __Camel_Transport_Link__

//#include <Camel/Base/System/Exceptions.h>
#include <Camel/Base/System/SmallObject.h>
#include <Camel/Base/System/System.h>
#include <Camel/Base/Network/Network.h>

namespace Camel
{
    namespace Transport
    {
        class Link : public Camel::Base::System::TPL::SmallObject<Link>
        {
        private:
            Camel::Base::Network::Link::Handle link_;

        public:
            Link(void)
                : link_(NULL)
            {}
            explicit Link(Camel::Base::Network::Link::Handle link)
                : link_(link)
            {
                if (link_ == NULL)
                {
                    //Common::throw_exception(
                    //    Common::NullPointerException("[Link::Link()] link_ == NULL"));
                    return;
                }
            }
            virtual ~Link(void)
            {}

            int AddRef(void)
            {
                return Camel::Base::Network::Link::AddRef(link_);
            }
            int Release(void)
            {
                return Camel::Base::Network::Link::Release(link_);
            }

            bool Close(void)
            {
                return Camel::Base::Network::Link::Close(link_);
            }

            Camel::Base::Network::Address::Handle GetLocalAddress(void)
            {
                return Camel::Base::Network::Link::GetLocalAddress(link_);
            }
            Camel::Base::Network::Address::Handle GetRemoteAddress(void)
            {
                return Camel::Base::Network::Link::GetRemoteAddress(link_);
            }

            bool Send(const void *data, size_t len)
            {
                return Camel::Base::Network::Link::Send(link_, data, len);
            }
            bool Send(const Camel::Base::System::SPHeap::Block data)
            {
                return Camel::Base::Network::Link::Send(link_, data);
            }
            bool Send(const Camel::Base::System::SPHeap::Block data, size_t len)
            {
                return Camel::Base::Network::Link::Send(link_, data, len);
            }
            bool Recv(size_t len)
            {
                return Camel::Base::Network::Link::Recv(link_, len);
            }

            bool AddAttribute(const char *name, void *data)
            {
                return Camel::Base::Network::Link::AddAttribute(link_, name, data);
            }
            bool RemoveAttribute(const char *name)
            {
                return Camel::Base::Network::Link::RemoveAttribute(link_, name);
            }
            void *GetAttribute(const char *name)
            {
                return Camel::Base::Network::Link::GetAttribute(link_, name);
            }

            Camel::Base::System::Stream::Handle LockReadStream(void)
            {
                return Camel::Base::Network::Link::LockReadStream(link_);
            }
            void UnlockReadStream(void)
            {
                Camel::Base::Network::Link::UnlockReadStream(link_);
            }

            Camel::Base::Network::Link::Handle GetHandle(void) const
            {
                return link_;
            }
        };

        typedef Camel::Base::System::TPL::RefCountPtr<Link> LinkPtr;
    }
}

#endif
