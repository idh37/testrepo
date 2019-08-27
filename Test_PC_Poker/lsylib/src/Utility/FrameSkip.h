#pragma once
#include "Point.h"

namespace LSY
{
class UPG_LSYLIB_EXPORT FrameSkip 
{ 
protected: 
	float		m_Timer; 
	float		m_SecPerFrame;

	BOOL		m_bTimerInitialized;
	BOOL		m_bUsingQPF;
	BOOL		m_bTimerStopped;
	LONGLONG	m_llQPFTicksPerSec;

	LONGLONG	m_llStopTime;
	LONGLONG	m_llLastElapsedTime;
	LONGLONG	m_llBaseTime;

	double		m_fLastElapsedTime;
	double		m_fBaseTime;
	double		m_fStopTime;
public:
	// 멤버변수와 생성/소멸자. 
	FrameSkip();
	virtual ~FrameSkip(){}

	void stReset()
	{
		m_Timer = 0.0f;
	}

	inline void Clear() 
	{ 
		SetFramePerSec( 100.0f ); 
		m_Timer = 0.0f; 
	} 

	void SetFramePerSec( float fps ) 
	{ 
		m_SecPerFrame = 1.0f/fps; 
	} 

	// 원하는 프레임보다 너무 빠르면, 
	// false를 반환해서 코드를 동작시키지 않도록 한다. 
	// dt는 '초'단위 (밀리초 아님!!!) 
	inline bool Update( float dt ) 
	{
		m_Timer += dt;   
		if(m_Timer > ( m_SecPerFrame * 4 ) )
			m_Timer = ( m_SecPerFrame * 4 );
		if(m_Timer < 0 ) return false;       
		// 한프레임에 해당하는 시간을 뺀다. 
		m_Timer -= m_SecPerFrame; 
		
		return true; 
	} 

	// Update후에 호출해서 frame skip을 수행해야 하는지 검사한다. 
	// frame skip이 일어나야 한다면 true를 반환한다. 
	inline bool IsFrameSkip() 
	{ 
		return m_Timer >= 0; 
	} 

	enum TIMER_COMMAND 
	{ 
		TIMER_RESET, 
		TIMER_START, 
		TIMER_STOP, 
		TIMER_ADVANCE,
		TIMER_GETABSOLUTETIME,
		TIMER_GETAPPTIME,
		TIMER_GETELAPSEDTIME 
	};

	float stTimer( TIMER_COMMAND );
}; 
} //namespace LSY