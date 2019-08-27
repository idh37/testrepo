#ifndef __Camel_Common_IOHandler__
#define __Camel_Common_IOHandler__

#include <Camel/Base/System/SmallObject.h>

namespace Camel
{
    namespace Common
    {
        template<typename LinkT>
        class IIOHandler : public Camel::Base::System::TPL::SmallObject<IIOHandler<LinkT> >
        {
        public:
            virtual ~IIOHandler(void) {}

        public:
            virtual void OnOpened(LinkT link) = 0;
            virtual void OnOpened(LinkT link, void *context) = 0;
            virtual void OnClosed(LinkT link) = 0;
            virtual void OnReceived(LinkT link) = 0;
        };
    }
}

#endif
