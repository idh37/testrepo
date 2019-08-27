#include "stdafx.h"
#include <mmsystem.h>
#include "FrameSkip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
FrameSkip::FrameSkip()
:	m_Timer(0.0f),
	m_SecPerFrame(0.0f),
	m_bTimerInitialized(FALSE),
	m_bUsingQPF(FALSE),
	m_bTimerStopped(FALSE),
	m_llQPFTicksPerSec(0),
	m_llStopTime(0),
	m_llLastElapsedTime(0),
	m_llBaseTime(0),
	m_fLastElapsedTime(0.0f),
	m_fBaseTime(0.0f),
	m_fStopTime(0.0f)
{
	Clear();
}

FLOAT FrameSkip::stTimer( FrameSkip::TIMER_COMMAND command )
{
	if( FALSE == m_bTimerInitialized )
	{
		m_bTimerInitialized = TRUE;

		LARGE_INTEGER qwTicksPerSec;
		m_bUsingQPF = QueryPerformanceFrequency( &qwTicksPerSec );
		if( m_bUsingQPF )
			m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
	}

	if( m_bUsingQPF )
	{
		double fTime;
		double fElapsedTime;
		LARGE_INTEGER qwTime;

		if( m_llStopTime && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
			qwTime.QuadPart = m_llStopTime;
		else
			QueryPerformanceCounter( &qwTime );

		if( command == TIMER_GETELAPSEDTIME )
		{
			fElapsedTime = (double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec;
			m_llLastElapsedTime = qwTime.QuadPart;
			return (FLOAT) fElapsedTime;
		}

		// Return the current time
		if( command == TIMER_GETAPPTIME )
		{
			double fAppTime = (double) ( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;
			return (FLOAT) fAppTime;
		}

		// Reset the timer
		if( command == TIMER_RESET )
		{
			m_llBaseTime        = qwTime.QuadPart;
			m_llLastElapsedTime = qwTime.QuadPart;
			m_llStopTime        = 0;
			m_bTimerStopped     = FALSE;
			return 0.0f;
		}

		// Start the timer
		if( command == TIMER_START )
		{
			if( m_bTimerStopped )
				m_llBaseTime += qwTime.QuadPart - m_llStopTime;
			m_llStopTime = 0;
			m_llLastElapsedTime = qwTime.QuadPart;
			m_bTimerStopped = FALSE;
			return 0.0f;
		}

		// Stop the timer
		if( command == TIMER_STOP )
		{
			m_llStopTime = qwTime.QuadPart;
			m_llLastElapsedTime = qwTime.QuadPart;
			m_bTimerStopped = TRUE;
			return 0.0f;
		}

		// Advance the timer by 1/10th second
		if( command == TIMER_ADVANCE )
		{
			m_llStopTime += m_llQPFTicksPerSec/10;
			return 0.0f;
		}

		if( command == TIMER_GETABSOLUTETIME )
		{
			fTime = qwTime.QuadPart / (double) m_llQPFTicksPerSec;
			return (FLOAT) fTime;
		}

		return -1.0f; // Invalid command specified
	}
	else
	{
		// Get the time using timeGetTime()
		double fTime;
		double fElapsedTime;

		// Get either the current time or the stop time, depending
		// on whether we're stopped and what command was sent
		if( m_fStopTime != 0.0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
			fTime = m_fStopTime;
		else
			fTime = timeGetTime() * 0.001;

		// Return the elapsed time
		if( command == TIMER_GETELAPSEDTIME )
		{   
			fElapsedTime = (double) (fTime - m_fLastElapsedTime);
			m_fLastElapsedTime = fTime;
			return (FLOAT) fElapsedTime;
		}

		// Return the current time
		if( command == TIMER_GETAPPTIME )
		{
			return (FLOAT) (fTime - m_fBaseTime);
		}

		// Reset the timer
		if( command == TIMER_RESET )
		{
			m_fBaseTime         = fTime;
			m_fLastElapsedTime  = fTime;
			m_fStopTime         = 0;
			m_bTimerStopped     = FALSE;
			return 0.0f;
		}

		// Start the timer
		if( command == TIMER_START )
		{
			if( m_bTimerStopped )
				m_fBaseTime += fTime - m_fStopTime;
			m_fStopTime = 0.0f;
			m_fLastElapsedTime  = fTime;
			m_bTimerStopped = FALSE;
			return 0.0f;
		}

		// Stop the timer
		if( command == TIMER_STOP )
		{
			m_fStopTime = fTime;
			m_fLastElapsedTime  = fTime;
			m_bTimerStopped = TRUE;
			return 0.0f;
		}

		// Advance the timer by 1/10th second
		if( command == TIMER_ADVANCE )
		{
			m_fStopTime += 0.1f;
			return 0.0f;
		}

		if( command == TIMER_GETABSOLUTETIME )
		{
			return (FLOAT) fTime;
		}

		return -1.0f; // Invalid command specified
	}
}
} //namespace LSY