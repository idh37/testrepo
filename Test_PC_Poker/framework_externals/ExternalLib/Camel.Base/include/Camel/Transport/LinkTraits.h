#ifndef __Camel_Transport_LinkTraits__
#define __Camel_Transport_LinkTraits__

#include <Camel/Base/System/System.h>

namespace Camel
{
    namespace Transport
    {
        template<typename T>
        struct LinkTraits
        {
            typedef T ValueType;
            typedef T* RawPtrType;
            typedef T* PointerType;
            typedef T* ParamType;
        };

        template<typename T>
        struct LinkTraits<Camel::Base::System::TPL::RefCountPtr<T> >
        {
            typedef T ValueType;
            typedef T* RawPtrType;
            typedef Camel::Base::System::TPL::RefCountPtr<T> PointerType;
            typedef const Camel::Base::System::TPL::RefCountPtr<T>& ParamType;
        };

        template<typename T>
        struct LinkTraits<Camel::Base::System::TPL::SharedPtr<T> >
        {
            typedef T ValueType;
            typedef T* RawPtrType;
            typedef Camel::Base::System::TPL::SharedPtr<T> PointerType;
            typedef const Camel::Base::System::TPL::SharedPtr<T>& ParamType;
        };
    }
}

#endif
