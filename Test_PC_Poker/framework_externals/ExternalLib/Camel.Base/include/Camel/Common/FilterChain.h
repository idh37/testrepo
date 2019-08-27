#ifndef __Camel_Common_FilterChain__
#define __Camel_Common_FilterChain__

#include <Camel/Base/System/SmallObject.h>
#include <Camel/Common/Filter.h>
#include <Camel/Base/System/System.h>
#include <Camel/Base/Network/Network.h>

namespace Camel
{
    namespace Common
    {
        class FilterChain : public Camel::Base::System::TPL::SmallObject<FilterChain>
        {
        private:
            typedef Camel::Base::System::TPL::vector<std::pair<Camel::Base::System::TPL::string, IFilter *> > Filters;

        private:
            Camel::Base::Network::FilterChain::Handle handle_;
            Filters filters_;

        public:
            FilterChain(void)
                : handle_(Camel::Base::Network::FilterChain::CreateHandle())
            {}
            ~FilterChain(void)
            {
                Camel::Base::Network::FilterChain::DestroyHandle(handle_);
                handle_ = NULL;
            }

            bool Add(const char *name, IFilter *filter)
            {
                if (Find(name) != NULL)
                {
                    return false;
                }

                filters_.push_back(std::make_pair(name, filter));

                return Camel::Base::Network::FilterChain::Add(GetHandle(),
                    name, filter->GetHandle());
            }
            bool Remove(const char *name)
            {
                for (Filters::iterator it = filters_.begin();
                    it != filters_.end(); ++it)
                {
                    if ((*it).first == name)
                    {
                        filters_.erase(it);
                        break;
                    }
                }

                return Camel::Base::Network::FilterChain::Remove(GetHandle(), name);
            }
            IFilter *Find(const char *name) const
            {
                IFilter *result = NULL;

                for (Filters::const_iterator it = filters_.begin();
                    it != filters_.end(); ++it)
                {
                    if ((*it).first == name)
                    {
                        result = (*it).second;
                        break;
                    }
                }

                return result;
            }

            Camel::Base::Network::FilterChain::Handle GetHandle(void) const
            {
                return handle_;
            }
        };
    }
}

#endif // __Camel_Common_FilterChain__
