// NMovement.h: interface for the CNMovement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMOVEMENT_H__E2C2640B_866D_4142_B1F7_0B13B53043FD__INCLUDED_)
#define AFX_NMOVEMENT_H__E2C2640B_866D_4142_B1F7_0B13B53043FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _CARD_ANI_MOVE_


class NPOINT
{

public:
	int x;
	int y;

public:

	NPOINT(){ x = y = 0; }
	virtual ~NPOINT(){};


	NPOINT& operator = ( POINT a )
	{
		x = a.x;
		y = a.y;

		return (*this);
	}

	NPOINT& operator = ( NPOINT a )
	{
		x = a.x;
		y = a.y;

		return (*this);
	}

	BOOL operator >= ( NPOINT a )
	{	
		BOOL bok = FALSE;
		if( x >= a.x && y >= a.y )
			bok = TRUE;		

		return bok;
	}


	BOOL operator > ( NPOINT a )
	{
		BOOL bok = FALSE;
		if( x > a.x && y > a.y )
			bok = TRUE;		

		return bok;
	}

	BOOL operator <= ( NPOINT a )
	{
		BOOL bok = FALSE;
		if( x <= a.x && y <= a.y )
			bok = TRUE;		

		return bok;
	}

	BOOL operator < ( NPOINT a )
	{
		BOOL bok = FALSE;
		if( x < a.x && y < a.y )
			bok = TRUE;		

		return bok;
	}

	BOOL operator == ( NPOINT a )
	{
		BOOL bok = FALSE;
		if( x == a.x && y == a.y )
			bok = TRUE;		

		return bok;
	}

	POINT GetPOINT()
	{
		POINT ret;
		ret.x = x;
		ret.y = y;

		return ret;
	}
};

class CNMovement  
{
public:
	CNMovement();
	virtual ~CNMovement();

	void ReSet();	

	void StraightMoveTo( POINT curpt, POINT destpt, float fSpeed , int delay = 0 ,BOOL bChange = FALSE);


	void Stop()
	{ 	
		NPOINT pt =  m_dest = m_cur;
		ReSet();
		m_dest = m_cur = pt;
	}

	inline bool IsMoving() const 
	{
		return !m_bStop; 
	}
	inline bool IsStopped() const 
	{
		return m_bStop; 
	}

	void SetCurPos( POINT pt)
	{
		m_cur = pt; 
	}
	void SetDestPos( POINT pt )
	{
		m_dest = pt; 
	}	

	POINT GetCurPos()
	{ 
		return m_cur.GetPOINT();
	}

	POINT GetDest()
	{
		POINT temp;
		temp.x = (long)m_dest.x;
		temp.y = (long)m_dest.y;
		return temp;
	}

	void OnTimer( );

	void SetDelay( int delay )
	{ 
		if( delay > 0 ) 
			m_delay = delay; 
	}	

	int GetDelay() 
	{ 
		return m_delay;
	}

	void SetPosIndex( int nIndex )
	{
		m_nDestIndex = nIndex;  
	}

	int  GetPosIndex( )
	{
		return m_nDestIndex;
	}

	void SetUseScene( bool bScene )
	{
		m_bUseScene = bScene;
	}

	bool IsUseScene()
	{
		return m_bUseScene;
	}


#ifdef _CARD_ANI_MOVE_
	void AniDraw( NMBASE::GRAPHICGDI::CPage *pPage );
#endif

protected:

	bool m_bChange;			// 카드 바꾸기 상태인가.
	bool m_bStop;			// 현재 정지상태인가..
	int  m_delay;			// 얼마나 기다린 후에 운동을 시작할 것인가.

	NPOINT m_start;
	NPOINT m_cur;			// 현재 위치값.	
	NPOINT m_dest;			// 끝점	
	float  m_nmovinglen;	// 이동한 거리
	
	float  m_SlowDownPos;	// 감속 하는 부분의 위치
	double m_temp;

	DWORD  m_dwStartTime;
	DWORD  m_dwCurTime;
	int	   m_nDestIndex;

	DWORD  m_dwDeltaTime;
	DWORD  m_dwPlayTime;

	NMBASE::GRAPHICGDI::xSprite* m_pSpr;

	float m_fPlayProportion; // 카드 이동속도 애니와 일반이동 같이 사용한다.


	bool m_bUseScene;
};

#endif // !defined(AFX_NMOVEMENT_H__E2C2640B_866D_4142_B1F7_0B13B53043FD__INCLUDED_)
