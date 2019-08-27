#pragma once

#include "Pigeon.GS.ConnectorAPI.h"

namespace Pigeon
{
	namespace GS
	{
		class GSCONNECTOR_API Allocator
		{
		public:		
			static const Camel::Dar2::IDarMsg* Alloc(unsigned int msg);
			static void Free(const Camel::Dar2::IDarMsg* );
		};

		template<typename _Ty>
		class ScopedAllocator
		{
		public:
			ScopedAllocator() 
			{
				m_pObj = static_cast<const _Ty* >(Allocator::Alloc(_Ty::msgid));
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

			const _Ty** operator&()
			{
				return &m_pObj;
			}

			const _Ty* operator->()
			{
				return m_pObj;
			}

			const _Ty* GetRawPtr()
			{
				return m_pObj;
			}

		private:
			const _Ty* m_pObj;
		};
	}
}