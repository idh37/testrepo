#pragma once

#pragma warning(disable: 4786)

#include "ThreadQueue.h"

namespace NMBASE
{
	namespace SOUND
	{

		class xOggStream;

		class CStreamManager
		{
		public:
			CStreamManager(void);
			virtual ~CStreamManager(void);

		public:
			static CStreamManager& GetInstance();

		public:
			void StreamThread_Entry();								
			
			void AddStream(xOggStream* pStream);					
			void DeleteFront(xOggStream* pStream);

			void AddStreamBGM(xOggStream* pStream);					
			void DeleteFrontBGM(xOggStream* pStream);

			void Destroy();

		private:
			bool BeginManager();									

		protected:

			CThreadQueue<xOggStream*> m_StreamQueue;				// Stream Queue

			CThreadQueue<xOggStream*> m_StreamQueueBGM;				// Stream Queue

			CRITICAL_SECTION m_Tasks_cs;							// Critical Section for Access to task array

			unsigned long	m_StreamThreadID;						// Stream Thread ID

			HANDLE			m_hStreamThread;						// Thread Handle

			HANDLE			m_hTime;								// Time Handle

			BOOL			m_bExit;
		};

		CStreamManager& gStreamMan();

	} //namespace SOUND

}	//namespace NMBASE