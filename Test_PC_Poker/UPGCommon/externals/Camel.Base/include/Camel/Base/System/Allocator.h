#ifndef __Camel_Base_System_TPL_Allocator__
#define __Camel_Base_System_TPL_Allocator__

#include "System.h"

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace TPL
{
	template<typename T>
	class Allocator
	{   // generic Allocator for objects of class T
	public:
		typedef typename T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef const value_type* const_pointer;
		typedef const value_type& const_reference;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		template<class _Other>
		struct rebind
		{   // convert an Allocator<T> to an Allocator <_Other>
			typedef Allocator<_Other> other;
		};

		pointer address(reference _Val) const
		{   // return address of mutable _Val
			return (&_Val);
		}

		const_pointer address(const_reference _Val) const
		{   // return address of nonmutable _Val
			return (&_Val);
		}

		Allocator()
		{   // construct default Allocator (do nothing)
		}

		Allocator(const Allocator<T>&)
		{   // construct by copying (do nothing)
		}

		template<class _Other>
		Allocator(const Allocator<_Other>&)
		{   // construct from a related Allocator (do nothing)
		}

		template<class _Other>
		Allocator<T>& operator=(const Allocator<_Other>&)
		{   // assign from a related Allocator (do nothing)
			return (*this);
		}

		void deallocate(pointer _Ptr, size_type)
		{   // deallocate object at _Ptr, ignore size
			//::operator delete(_Ptr);
			Camel::Base::System::MPHeap::Free(NULL, _Ptr);
		}

		pointer allocate(size_type _Count)
		{   // allocate array of _Count elements
			return static_cast<pointer>(Camel::Base::System::MPHeap::Alloc(NULL, sizeof(value_type) * _Count));
		}

		pointer allocate(size_type _Count, const void *)
		{   // allocate array of _Count elements, ignore hint
			return (allocate(_Count));
		}

		void construct(pointer _Ptr, T&& _Val)
		{   // construct object at _Ptr with value _Val
			//new(_Ptr) value_type(_Val);
#if defined( _MSC_VER )
	#if ( _MSC_VER < 1910) // 1910 = VS2017
			std::_Construct(_Ptr, _Val);
	#else
			std::_Construct_in_place(*_Ptr, _Val);
	#endif
#endif
			// std::_Construct(_Ptr, _Val); // VS2017로 전환하면서 변경된 함수
			// std::_Construct_in_place(*_Ptr, _Val);
		}

		void destroy(pointer _Ptr)
		{   // destroy object at _Ptr

#if defined( _MSC_VER )
	#if ( _MSC_VER < 1910) // 1910 = VS2017
			std::_Destroy(_Ptr);
	#else
			std::_Destroy_in_place(_Ptr);
	#endif
#endif
			// std::_Destroy(_Ptr); // VS2017로 전환하면서 변경된 함수
			// std::_Destroy_in_place(_Ptr);
		}

		size_t max_size() const
		{   // estimate maximum array size
			size_t _Count = (size_t)(-1) / sizeof (T);
			return (0 < _Count ? _Count : 1);
		}
	};

	// Allocator TEMPLATE OPERATORS
	template<typename T, typename _Other>
	inline bool operator==(const Allocator<T>&, const Allocator<_Other>&)
	{   // test for Allocator equality (always true)
		return true;
	}

	template<typename T, typename _Other>
	inline bool operator!=(const Allocator<T>&, const Allocator<_Other>&)
	{   // test for Allocator inequality (always false)
		return false;
	}
}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif
