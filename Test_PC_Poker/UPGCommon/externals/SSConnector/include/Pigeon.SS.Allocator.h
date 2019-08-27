#pragma once

#include "Pigeon.SS.ConnectorAPI.h"

namespace Pigeon
{
	namespace SS
	{
		class PIGEON_SS_CONNECTOR_API Allocator
		{
		public:		
			static Camel::Dar2::IDarMsg* Alloc(unsigned int msg);
			static void Free( Camel::Dar2::IDarMsg* );
		};

		template<typename _Ty>
		class ScopedAllocator
		{
		public:
			ScopedAllocator() 
			{
				m_pObj = static_cast<_Ty* >(Allocator::Alloc(_Ty::msgid));
				if (NULL == m_pObj)
				{
					assert("Allocation failed");
				}
			}

			~ScopedAllocator() throw()
			{
				if (NULL != m_pObj)
				{
					Allocator::Free(m_pObj);
					m_pObj = NULL;
				}
			}

			_Ty** operator&()
			{
				return &m_pObj;
			}

			_Ty* operator->()
			{
				return m_pObj;
			}

		private:
			_Ty* m_pObj;
		};
	}
}