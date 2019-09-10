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
	// ��������� ����/�Ҹ���. 
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

	// ���ϴ� �����Ӻ��� �ʹ� ������, 
	// false�� ��ȯ�ؼ� �ڵ带 ���۽�Ű�� �ʵ��� �Ѵ�. 
	// dt�� '��'���� (�и��� �ƴ�!!!) 
	inline bool Update( float dt ) 
	{
		m_Timer += dt;   
		if(m_Timer > ( m_SecPerFrame * 4 ) )
			m_Timer = ( m_SecPerFrame * 4 );
		if(m_Timer < 0 ) return false;       
		// �������ӿ� �ش��ϴ� �ð��� ����. 
		m_Timer -= m_SecPerFrame; 
		
		return true; 
	} 

	// Update�Ŀ� ȣ���ؼ� frame skip�� �����ؾ� �ϴ��� �˻��Ѵ�. 
	// frame skip�� �Ͼ�� �Ѵٸ� true�� ��ȯ�Ѵ�. 
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