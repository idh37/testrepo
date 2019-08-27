#ifndef __Camel_Base_System_TPL_Atomic__
#define __Camel_Base_System_TPL_Atomic__

#include "SystemBase.h"
#include "Interlocked.h"
#include <stdexcept>

_DECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
namespace TPL
{
	template <typename T>
	class Atomic 
	{
		public:
			Atomic() : v_()
			{
			}
			explicit Atomic( const T& v )
			{
				System::Threading::Interlocked::Increment((volatile int*)&v_, v);
			}
			~Atomic()
			{
			}
			// Copy construction 
			Atomic( const Atomic<T>& rhs )
			{
				T v = (T)rhs;
				System::Threading::Interlocked::Increment((volatile int*)&v_, v);
			}
			// Atomic add
			//const Atomic& Add( const T& v )
			//{
			//	::InterlockedExchangeAdd( (volatile long*)&v_, v );
			//	return *this;
			//}

			 // Atomic increment by 1
			const Atomic& Increment()
			{
				System::Threading::Interlocked::Increment((volatile int*)&v_);
				return *this;
			}

			// Atomic decrement
			const Atomic& Dec()
			{
				System::Threading::Interlocked::Decrement((volatile int*)&v_);
				return *this;
			}

			 // Assignment with a raw value 
			const Atomic& operator=( const T& v )
			{
				System::Threading::Interlocked::Exchange((volatile int*)&v_, v);
				return *this;
			}

			// Type conversion to T. Because of Locking, this cannot be const
			operator T () const
			{
				return (T)System::Threading::Interlocked::CompareExchange((volatile int*)&v_, v_, v_);
			}	

			// Comparison of raw values
			bool operator==( const T& v)
			{
				T ev = System::Threading::Interlocked::CompareExchange((volatile int*)&v_, v_, v_);
				return ( ev == v );
			}
		private:
			volatile T v_;
    };

	template <>
	class Atomic<bool> 
	{
	public:
		Atomic() : v_( false )
		{
		}
		explicit Atomic( const bool& v )
		{
			Threading::Interlocked::Exchange((volatile int*)&v_, v);
		}
		~Atomic()
		{
		}

		 // Copy construction 
		Atomic( const Atomic<bool>& rhs )
		{
			bool v = (bool)rhs;
			Threading::Interlocked::Exchange((volatile int*)&v_, (long)v);
		}

		 // Assignment with a raw value 
		const Atomic& operator=( const bool& v )
		{
			Threading::Interlocked::Exchange((volatile int*)&v_, (long)v);
			return *this;
		}

		operator bool () const
		{
			return ( Threading::Interlocked::CompareExchange((volatile int*)&v_, v_, v_ ) != 0);
		}

		// Comparison of raw values
		bool operator==( const bool& v)
		{
			bool ev = ( Threading::Interlocked::CompareExchange((volatile int*)&v_, v_, v_ ) != 0);
			return ( ev == v );
		}

	private:
		volatile bool v_;	
	};

}
_UNDECLARE_NAMESPACE_CAMEL_BASE_SYSTEM_
#endif // __Camel_Base_System_TPL_Atomic__