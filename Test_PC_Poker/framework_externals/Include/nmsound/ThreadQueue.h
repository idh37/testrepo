#pragma once

#include <queue>
#include <vector>
using namespace std;

namespace NMBASE
{
	namespace SOUND
	{
		template<class Type> 
		class CThreadQueue
		{

		public:
			CThreadQueue()
			{
				m_nCount = 0;
				::InitializeCriticalSection( &m_cs );
			} 

			virtual ~CThreadQueue()
			{
				if(m_Vector.size())
				{
					m_Vector.clear();
				}

				::DeleteCriticalSection( &m_cs );
			} 

			// Additional data from the place.
			void AddToBack( Type type ) 
			{ 
				::EnterCriticalSection( &m_cs );

				//m_Queue.push(type);
				m_Vector.push_back(type);

				::LeaveCriticalSection( &m_cs );
			} 

			// Added data to pull it out one by one from the Queue
// 			bool GetQueueFront( Type &inType )
// 			{ 
// 				bool result = true;
// 
// 				::EnterCriticalSection( &m_cs );
// 				try
// 				{
// 					if( m_Queue.size() )
// 					{
// 						inType = m_Queue.front();						
// 					}
// 					else 
// 					{ 
// 						result = false; 
// 					}
// 				}
// 				catch(...)
// 				{ 
// 				}
// 
// 				::LeaveCriticalSection( &m_cs );
// 
// 				return result;
// 			} 

			bool GetQueueFront( Type &inType )
			{ 
				bool result = true;

				::EnterCriticalSection( &m_cs );

				if(m_Vector.size() <= 0)
				{
					::LeaveCriticalSection( &m_cs );
					return false;
				}

				// Count 개산...
				if(m_nCount >= m_Vector.size())
				{
					m_nCount = 0;
				}				

				try
				{
					if( m_Vector.size() )
					{
						inType = m_Vector[m_nCount];
					}
					else 
					{ 
						result = false; 
					}
				}
				catch(...)
				{ 
				}

				++m_nCount;

				::LeaveCriticalSection( &m_cs );

				return result;
			}


			// Added data to pull it out one by one from the Queue
// 			bool RemoveFront()
// 			{ 
// 				bool result = true;
// 
// 				::EnterCriticalSection( &m_cs );
// 				try
// 				{
// 					if( m_Queue.size() )
// 					{
// 						m_Queue.pop();
// 					}
// 					else 
// 					{ 
// 						result = false; 
// 					}
// 				}
// 				catch(...)
// 				{ 
// 				}
// 
// 				::LeaveCriticalSection( &m_cs );
// 
// 				return result;
// 			} 

			bool RemoveFront(Type& inType)
			{ 
				bool result = true;

				::EnterCriticalSection( &m_cs );
				try
				{
					if( m_Vector.size() )
					{

						vector<Type>::iterator it = m_Vector.begin();
						for(; it!= m_Vector.end(); it++)
						{
							if(*it == inType)
							{
								m_Vector.erase(it);								
								break;
							}
						}					
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
// 			bool RemoveFromFront( Type &inType )
// 			{ 
// 				bool result = true;
// 
// 				::EnterCriticalSection( &m_cs );
// 				try
// 				{
// 					if( m_Queue.size() )
// 					{
// 						inType = m_Queue.front();
// 						m_Queue.pop();
// 					}
// 					else 
// 					{ 
// 						result = false; 
// 					}
// 				}
// 				catch(...)
// 				{ 
// 				}
// 
// 				::LeaveCriticalSection( &m_cs );
// 
// 				return result;
// 			} 

// 			bool RemoveData( IN Type* pPoint, OUT Type &inType )
// 			{ 
// 				bool result = true;
// 
// 				::EnterCriticalSection( &m_cs );
// 				try
// 				{
// 					if( m_Queue.size() )
// 					{
// 						inType = m_Queue.front();
// 						m_Queue.pop();
// 					}
// 					else 
// 					{ 
// 						result = false; 
// 					}
// 				}
// 				catch(...)
// 				{ 
// 				}
// 
// 				::LeaveCriticalSection( &m_cs );
// 
// 				return result;
// 			}

			int GetQueueCount()
			{
				int nCount = 0;

				::EnterCriticalSection( &m_cs );

				//nCount = static_cast<int>( m_Queue.size() );
				nCount = static_cast<int>( m_Vector.size() );

				::LeaveCriticalSection( &m_cs );

				return nCount; 
			}

		protected:

			CRITICAL_SECTION m_cs;

			//queue<Type>		 m_Queue;
			vector<Type>	m_Vector;

			int m_nCount;
		};


	} //namespace SOUND

}	//namespace NMBASE