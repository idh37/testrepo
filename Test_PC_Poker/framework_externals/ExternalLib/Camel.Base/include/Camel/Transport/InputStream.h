#ifndef __Camel_Transport_InputStream__
#define __Camel_Transport_InputStream__

#include <Camel/Transport/LinkTraits.h>
#include <Camel/Base/System/SmallObject.h>
#include <Camel/Base/System/System.h>

namespace Camel
{
    namespace Transport
    {
        template<typename LinkT>
        class InputStream : public Camel::Base::System::TPL::SmallObject<InputStream<LinkT> >
        {
        private:
            typename LinkTraits<LinkT>::PointerType link_;
            Camel::Base::System::Stream::Handle stream_;

        public:
            explicit InputStream(typename LinkTraits<LinkT>::ParamType link)
                : link_(link), stream_(link->LockReadStream())
            {}
            ~InputStream(void)
            {
                link_->UnlockReadStream();
            }

            void Attach(Camel::Base::System::SPHeap::Block p)
            {
                Camel::Base::System::Stream::Attach(stream_, p);
            }

            Camel::Base::System::SPHeap::Block Detach(void)
            {
                return Camel::Base::System::Stream::Detach(stream_);
            }

            Camel::Base::System::SPHeap::Block GetRawPtr(void)
            {
                return Camel::Base::System::Stream::GetRawPtr(stream_);
            }

            size_t GetLength(void) const
            {
                return Camel::Base::System::Stream::GetLength(stream_);
            }

            void Clear(void)
            {
                Camel::Base::System::Stream::Clear(stream_);
            }

            size_t Read(void *p, size_t size)
            {
                return Camel::Base::System::Stream::Read(stream_, p, size);
            }

            size_t Peek(void *p, size_t size)
            {
                return Camel::Base::System::Stream::Peek(stream_, p, size);
            }

            size_t RemoveLeft(size_t size)
            {
                return Camel::Base::System::Stream::RemoveLeft(stream_, size);
            }

            size_t RemoveRight(size_t size)
            {
                return Camel::Base::System::Stream::RemoveRight(stream_, size);
            }
        };
    }
}

#endif
