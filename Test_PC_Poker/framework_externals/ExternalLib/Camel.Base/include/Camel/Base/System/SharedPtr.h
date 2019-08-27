#ifndef __Camel_Base_System_TPL_SharedPtr__
#define __Camel_Base_System_TPL_SharedPtr__

#include "SystemBase.h"
#include "MPHeap.h"
#include "Interlocked.h"
#include <algorithm>
#include <cassert>

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace TPL
{
	template<typename T>
	class SharedPtr
	{
	private:
		T *p_;
		unsigned int *count_;

	private:
		template<typename U> friend class SharedPtr;

	public:
		SharedPtr(void)
			: p_(NULL),
			count_(static_cast<unsigned int *>(MPHeap::Alloc(NULL, sizeof(unsigned int))))
		{
			if (count_ == NULL)
			{
				throw std::bad_alloc();
			}
			*count_ = 1;
		}
		explicit SharedPtr(T *p)
			: p_(p),
			count_(static_cast<unsigned int *>(MPHeap::Alloc(NULL, sizeof(unsigned int))))
		{
			if (count_ == NULL)
			{
				throw std::bad_alloc();
			}
			*count_ = 1;
		}
		SharedPtr(const SharedPtr &rhs)
			: p_(rhs.p_),
			count_(rhs.count_)
		{
			Threading::Interlocked::Increment(rhs.count_);
		}
		template<typename U>
		SharedPtr(const SharedPtr<U> &rhs)
			: p_(rhs.p_),
			count_(rhs.count_)
		{
			Threading::Interlocked::Increment(rhs.count_);
		}
		~SharedPtr(void)
		{
			if (Threading::Interlocked::Decrement(count_) == 0)
			{
				MPHeap::Free(NULL, count_);
				count_ = NULL;

				delete p_;
				p_ = NULL;
			}
		}

		void Reset(void)
		{
			SharedPtr().Swap(*this);
		}

		template<typename U>
		void Reset(U *p)
		{
			assert(p == 0 || p != p_); // catch self-reset errors
			SharedPtr(p).Swap(*this);
		}

		void Swap(SharedPtr &rhs)
		{
			std::swap(p_, rhs.p_);
			std::swap(count_, rhs.count_);
		}

		T *GetRawPtr(void) const
		{
			return p_;
		}

		T *operator->() const
		{
			return p_;
		}
		T &operator*() const
		{
			return *p_;
		}

		friend bool operator==(const SharedPtr<T>& rhs, const SharedPtr<T>& lhs)
		{
			return (rhs.GetRawPtr() == lhs.GetRawPtr());
		}

		bool operator!() const // Enables "if (!sp) ..."
		{
			return p_ == NULL;
		}

		SharedPtr &operator=(const SharedPtr &rhs)
		{
			SharedPtr(rhs).Swap(*this);

			return *this;
		}
		template<typename U>
		SharedPtr &operator=(const SharedPtr<U> &rhs)
		{
			SharedPtr(rhs).Swap(*this);

			return *this;
		}
		SharedPtr<T> lock() const
		{
			SharedPtr<T> result;
			if (p_ != NULL)
			{
				int old_count;
				do
				{
					old_count = *count_;
					if (old_count == 0)
					{
						break;
					}
				}
				while (old_count != Threading::Interlocked::CompareExchange(count_, old_count + 1, old_count));
				if (old_count > 0)
				{
					result.p_ = p_;
					result.count_ = count_;
				}
			}
			return result;
		}
	};
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_TPL_SharedPtr__
