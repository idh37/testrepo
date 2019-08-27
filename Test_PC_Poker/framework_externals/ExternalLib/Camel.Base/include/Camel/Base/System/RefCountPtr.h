#ifndef __Camel_Base_System_TPL_RefCountPtr__
#define __Camel_Base_System_TPL_RefCountPtr__

#include "SystemBase.h"

#include <utility>

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace TPL
{
	template<typename T>
	class RefCountPtr
	{
	private:
		typedef T* StoredType;
		typedef T* PointerType;
		typedef T const* ConstPointerType;
		typedef T& ReferenceType;
		typedef T const& ConstReferenceType;

	private:
		StoredType pointee_;

	public:
		RefCountPtr(void) : pointee_(NULL)
		{
		}
		RefCountPtr(const StoredType &p) : pointee_(p)
		{
			if (pointee_ != NULL)
			{
				pointee_->AddRef();
			}
		}
		RefCountPtr(const RefCountPtr &rhs) : pointee_(rhs.pointee_)
		{
			if (pointee_ != NULL)
			{
				pointee_->AddRef();
			}
		}
		template<typename U>
		RefCountPtr(RefCountPtr<U> &rhs) : pointee_(rhs.GetRawPtr())
		{
			if (pointee_ != NULL)
			{
				pointee_->AddRef();
			}
		}
		template<typename U>
		RefCountPtr(const RefCountPtr<U> &rhs) : pointee_(const_cast<T *>(rhs.GetRawPtr()))
		{
			if (pointee_ != NULL)
			{
				pointee_->AddRef();
			}
		}
		~RefCountPtr(void)
		{
			if (pointee_ != NULL)
			{
				pointee_->Release();
			}
		}

		void Swap(RefCountPtr &rhs)
		{
			std::swap(pointee_, rhs.pointee_);
		}

		PointerType GetRawPtr(void) const
		{
			return pointee_;
		}

		PointerType operator->() const
		{
			return GetRawPtr();
		}

		ReferenceType operator*() const
		{
			return *GetRawPtr();
		}

		RefCountPtr &operator=(const RefCountPtr &rhs)
		{
			RefCountPtr temp(rhs);
			temp.Swap(*this);

			return *this;
		}
		template<typename U>
		RefCountPtr &operator=(RefCountPtr<U> &rhs)
		{
			RefCountPtr temp(rhs);
			temp.Swap(*this);

			return *this;
		}
		template<typename U>
		RefCountPtr &operator=(const RefCountPtr<U> &rhs)
		{
			RefCountPtr temp(rhs);
			temp.Swap(*this);

			return *this;
		}
	};

	template<typename T, typename U>
	inline bool operator==(const RefCountPtr<T> &lhs, const RefCountPtr<U> &rhs)
	{
		return lhs.GetRawPtr() == rhs.GetRawPtr();
	}
	template<class T, class U>
	inline bool operator!=(const RefCountPtr<T> &lhs, const RefCountPtr<U> &rhs)
	{
		return lhs.GetRawPtr() != rhs.GetRawPtr();
	}
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif
