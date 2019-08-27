#ifndef __Camel_Transport_OutputStream__
#define __Camel_Transport_OutputStream__

#include <Camel/Transport/LinkTraits.h>
#include <Camel/Base/System/SmallObject.h>
#include <Camel/Base/System/System.h>

namespace Camel
{
    namespace Transport
    {
        template<typename LinkT>
        class OutputStream : public Camel::Base::System::TPL::SmallObject<OutputStream<LinkT> >
        {
        private:
            typename LinkTraits<LinkT>::PointerType link_;
            Camel::Base::System::Stream::Handle stream_;

        public:
            explicit OutputStream(typename LinkTraits<LinkT>::ParamType link)
                : link_(link), stream_(Camel::Base::System::Stream::CreateHandle())
            {}
            ~OutputStream(void)
            {
                Flush();

                Camel::Base::System::Stream::DestroyHandle(stream_);
                stream_ = NULL;
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

            void Flush(void)
            {
                if (GetLength() > 0)
                {
                    link_->Send(GetRawPtr(), GetLength());
                    Clear();
                }
            }

            size_t Write(const void *p, size_t size)
            {
                return Camel::Base::System::Stream::Write(stream_, p, size);
            }
        };
    }
}

#endif
