#pragma once

#include <queue>
using namespace std;


namespace NMBASE
{
	namespace UTIL
	{
		template<class Type> 
		class CThreadQueue
		{

		public:
			CThreadQueue()
			{
				::InitializeCriticalSection( &m_cs );
			} 

			virtual ~CThreadQueue()
			{
				::DeleteCriticalSection( &m_cs );
			} 

			// Additional data from the place.
			void AddToBack( Type type ) 
			{ 
				::EnterCriticalSection( &m_cs );

				m_Queue.push(type);

				::LeaveCriticalSection( &m_cs );
			} 

			// Added data to pull it out one by one from the Queue
			bool GetQueueFront( Type &inType )
			{ 
				bool result = true;

				::EnterCriticalSection( &m_cs );
				try
				{
					if( m_Queue.size() )
					{
						inType = m_Queue.front();						
					}
					else 
					{ 
						result = false; 
					}
				}
				catch(...)
				{ 
				}

				::LeaveCriticalSection( &m_cs );

				return result;
			} 


			// Added data to pull it out one by one from the Queue
			bool RemoveFront()
			{ 
				bool result = true;

				::EnterCriticalSection( &m_cs );
				try
				{
					if( m_Queue.size() )
					{
						m_Queue.pop();
					}
					else 
					{ 
						result = false; 
					}
				}
				catch(...)
				{ 
				}

				::LeaveCriticalSection( &m_cs );

				return result;
			} 


			// Added data to pull it out one by one from the Queue
			bool RemoveFromFront( Type &inType )
			{ 
				bool result = true;

				::EnterCriticalSection( &m_cs );
				try
				{
					if( m_Queue.size() )
					{
						inType = m_Queue.front();
						m_Queue.pop();
					}
					else 
					{ 
						result = false; 
					}
				}
				catch(...)
				{ 
				}

				::LeaveCriticalSection( &m_cs );

				return result;
			} 

			int GetQueueCount()
			{
				int nCount = 0;

				::EnterCriticalSection( &m_cs );

				nCount = static_cast<int>( m_Queue.size() );

				::LeaveCriticalSection( &m_cs );

				return nCount; 
			}

		protected:

			CRITICAL_SECTION m_cs;

			queue<Type>		 m_Queue;
		};

	}//namespace UTIL

}//namespace NMBASE