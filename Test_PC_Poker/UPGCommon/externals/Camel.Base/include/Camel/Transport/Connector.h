#ifndef __Camel_Transport_Connector__
#define __Camel_Transport_Connector__

#include <Camel/Transport/Link.h>
#include <Camel/Transport/LinkTraits.h>
#include <Camel/Transport/DefaultLinkFactory.h>
//#include <Camel/Common/Exceptions.h>
#include <Camel/Common/IOHandler.h>
#include <Camel/Base/Network/Network.h>
#include <Camel/Base/System/SmallObject.h>

namespace Camel
{
    namespace Common
    {
        class ThreadPool;
    }

    namespace Transport
    {
        template<typename LinkT = Link,
            typename IOHandlerT = Common::IIOHandler<
                typename LinkTraits<LinkT>::ParamType>,
            typename LinkFactoryT = DefaultLinkFactory<LinkT>
        >
        class Connector : public Camel::Base::System::TPL::SmallObject<Connector<LinkT, IOHandlerT, LinkFactoryT>>
        {
        private:
            Camel::Base::Network::Connector::Handle connector_;
            IOHandlerT *handler_;

        public:
            explicit Connector(Common::ThreadPool *threadPool)
                : connector_(Camel::Base::Network::Connector::CreateHandle(threadPool->GetHandle())), handler_(NULL)
            {
                if (connector_ == NULL)
                {
                    //Common::throw_exception(Common::NullPointerException("[Connector::Connector()] connector_ == NULL"));
                    return;
                }

                Camel::Base::Network::Connector::SetOnConnected(connector_, &OnConnected, this);
                Camel::Base::Network::Connector::SetOnConnectedEx(connector_, &OnConnectedEx, this);
                Camel::Base::Network::Connector::SetOnReceived(connector_, &OnReceived, this);
                Camel::Base::Network::Connector::SetOnDisconnected(connector_, &OnDisconnected, this);
            }
			Connector(Common::ThreadPool *threadPool, Common::FilterChain *chain)
				: connector_(Camel::Base::Network::Connector::CreateHandle(threadPool->GetHandle(), chain->GetHandle())), handler_(NULL)
			{
				if (connector_ == NULL)
				{
					//Common::throw_exception(
					//    Common::NullPointerException("[Connector::Connector()] connector_ == NULL"));
					return;
				}

				Camel::Base::Network::Connector::SetOnConnected(connector_, &OnConnected, this);
				Camel::Base::Network::Connector::SetOnConnectedEx(connector_, &OnConnectedEx, this);
				Camel::Base::Network::Connector::SetOnReceived(connector_, &OnReceived, this);
				Camel::Base::Network::Connector::SetOnDisconnected(connector_, &OnDisconnected, this);
			}
            //Connector(Common::ThreadPool *threadPool)
            //    : connector_(Camel::Base::Network::Connector::CreateHandle(threadPool->GetHandle())), handler_(NULL)
            //{
            //    if (connector_ == NULL)
            //    {
            //        //Common::throw_exception(Common::NullPointerException("[Connector::Connector()] connector_ == NULL"));
            //        return;
            //    }

            //    Camel::Base::Network::Connector::SetOnConnected(connector_, &OnConnected, this);
            //    Camel::Base::Network::Connector::SetOnConnectedEx(connector_, &OnConnectedEx, this);
            //    Camel::Base::Network::Connector::SetOnReceived(connector_, &OnReceived, this);
            //    Camel::Base::Network::Connector::SetOnDisconnected(connector_, &OnDisconnected, this);
            //}
            ~Connector(void)
            {
                Camel::Base::Network::Connector::DestroyHandle(connector_);
                connector_ = NULL;
            }

            bool Connect(const char *ipAddress, unsigned short port)
            {
                return Camel::Base::Network::Connector::Connect(GetHandle(),ipAddress, port);
            }
            bool Connect(const char *ipAddress, unsigned short port, int timeout)
            {
                return Camel::Base::Network::Connector::Connect(GetHandle(),
                    ipAddress, port, timeout);
            }

            bool ConnectEx(const char *ipAddress, unsigned short port, void *context)
            {
                return Camel::Base::Network::Connector::ConnectEx(GetHandle(),ipAddress, port, context);
            }
            bool ConnectEx(const char *ipAddress, unsigned short port,
                int timeout, void *context)
            {
                return Camel::Base::Network::Connector::ConnectEx(GetHandle(),ipAddress, port, timeout, context);
            }

            void SetHandler(IOHandlerT *handler)
            {
                handler_ = handler;
            }

            Camel::Base::Network::Connector::Handle GetHandle(void) const
            {
                return connector_;
            }

        private:
            static void __stdcall OnConnected(Camel::Base::Network::Connector::Handle,Camel::Base::Network::Link::Handle link, void *data)
            {
                Connector *connector = static_cast<Connector *>(data);
                if (connector != NULL)
                {
                    if (connector->handler_ != NULL)
                    {
                        connector->handler_->OnOpened(LinkFactoryT::Create(link));
                    }
                }
            }
            static void __stdcall OnConnectedEx(Camel::Base::Network::Connector::Handle,Camel::Base::Network::Link::Handle link, void *data, void *context)
            {
                Connector *connector = static_cast<Connector *>(data);
                if (connector != NULL)
                {
                    if (connector->handler_ != NULL)
                    {
                        connector->handler_->OnOpened(LinkFactoryT::Create(link), context);
                    }
                }
            }
            static void __stdcall OnReceived(Camel::Base::Network::Connector::Handle,Camel::Base::Network::Link::Handle link, void *data)
            {
                Connector *connector = static_cast<Connector *>(data);
                if (connector != NULL)
                {
                    if (connector->handler_ != NULL)
                    {
                        connector->handler_->OnReceived(LinkFactoryT::ToLinkInstance(link));
                    }
                }
            }
            static void __stdcall OnDisconnected(Camel::Base::Network::Connector::Handle,Camel::Base::Network::Link::Handle link, void *data)
            {
                Connector *connector = static_cast<Connector *>(data);
                if (connector != NULL)
                {
                    if (connector->handler_ != NULL)
                    {
                        connector->handler_->OnClosed(LinkFactoryT::ToLinkInstance(link));
                    }
                }
            }
        };
    }
}

#endif
