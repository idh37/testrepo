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

	bool m_bChange;			// ī�� �ٲٱ� �����ΰ�.
	bool m_bStop;			// ���� ���������ΰ�..
	int  m_delay;			// �󸶳� ��ٸ� �Ŀ� ��� ������ ���ΰ�.

	NPOINT m_start;
	NPOINT m_cur;			// ���� ��ġ��.	
	NPOINT m_dest;			// ����	
	float  m_nmovinglen;	// �̵��� �Ÿ�
	
	float  m_SlowDownPos;	// ���� �ϴ� �κ��� ��ġ
	double m_temp;

	DWORD  m_dwStartTime;
	DWORD  m_dwCurTime;
	int	   m_nDestIndex;

	DWORD  m_dwDeltaTime;
	DWORD  m_dwPlayTime;

	NMBASE::GRAPHICGDI::xSprite* m_pSpr;

	float m_fPlayProportion; // ī�� �̵��ӵ� �ִϿ� �Ϲ��̵� ���� ����Ѵ�.


	bool m_bUseScene;
};

#endif // !defined(AFX_NMOVEMENT_H__E2C2640B_866D_4142_B1F7_0B13B53043FD__INCLUDED_)
