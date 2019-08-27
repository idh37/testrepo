#ifndef __Camel_Transport_DefaultLinkFactory__
#define __Camel_Transport_DefaultLinkFactory__

#include <Camel/Transport/LinkTraits.h>
//#include <Camel/Base/System/Exceptions.h>
#include <Camel/Base/System/System.h>
#include <Camel/Base/Network/Network.h>

namespace Camel
{
    namespace Transport
    {
        template<typename LinkT>
        struct DefaultLinkFactory : private Camel::Base::System::TPL::NonCopyable
        {
            typedef typename LinkTraits<LinkT>::ValueType LinkType;
            typedef typename LinkTraits<LinkT>::PointerType LinkPtr;
            typedef typename LinkTraits<LinkT>::RawPtrType LinkRawPtr;

            static LinkPtr Create(Camel::Base::Network::Link::Handle link)
            {
                if (link == NULL)
                {
                    //Common::throw_exception(
                    //    Common::NullPointerException("[DefaultLinkFactory::Create()] link == NULL"));
                    return LinkPtr();
                }

                LinkRawPtr linkPtr(new LinkType(link));
                Camel::Base::Network::Link::SetData(link, linkPtr, &OnLinkDestroy, NULL);

                return LinkPtr(linkPtr);
            }
            static void Destroy(LinkRawPtr linkPtr)
            {
                delete linkPtr;
                linkPtr = NULL;
            }
            static LinkPtr ToLinkInstance(Camel::Base::Network::Link::Handle link)
            {
                if (link == NULL)
                {
                    //Common::throw_exception(
                    //    Common::NullPointerException("[DefaultLinkFactory::ToLinkInstance()] link == NULL"));
                    return LinkPtr();
                }

                return LinkPtr(static_cast<LinkRawPtr>(Camel::Base::Network::Link::GetData(link)));
            }

        private:
            static void __stdcall OnLinkDestroy(Camel::Base::Network::Link::Handle link, void *)
            {
                LinkRawPtr linkPtr(static_cast<LinkRawPtr>(Camel::Base::Network::Link::GetData(link)));
                Destroy(linkPtr);
                linkPtr = NULL;
            }
        };
    }
}

#endif
