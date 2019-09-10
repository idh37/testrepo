#pragma once

class CMovementEx  
{
public:
	enum MOVEMENT_TYPE
	{
		MT_NORMAL = 0,		//�Ϲ� �̵�
		MT_MSPRITE,			//��Ƽ�� ��������Ʈ ���
		MT_SSPRITE,			//�̱۾� ��������Ʈ ���
		MT_IMAGE,			//�̹��� ����Ʈ�� ����� ���
	};
protected:
	CPoint	m_ptStart;
	CPoint	m_ptEnd;
	CPoint	m_ptCurrent;
	CPoint	m_ptGab;

	DWORD			m_dwStartTime;
	DWORD			m_dwEndTime;
	DWORD			m_dwMoveTime;
	DWORD			m_dwCurTime;
	DWORD			m_dwGab;

	bool			m_bStart;		//�̵����� �÷���
	bool			m_bEnd;
	bool			m_bMoveStart;
	MOVEMENT_TYPE	m_nType;
	int				m_nIndex;
	NMBASE::GRAPHICGDI::xSprite			*m_pSprite;
	int				m_nOpt;

	bool			m_bPlaySound;
	std::string		m_strSoundID;
	float			m_fPlayProportion;
public:
	CMovementEx();
	virtual ~CMovementEx();

	void	ReSet();
	void	StartMove(CPoint ptStart, CPoint ptEnd, DWORD dwDelay, DWORD dwMove, float	fPlayProportion = 1.0f);
	void	StartMoveMSprite(NMBASE::GRAPHICGDI::xSprite *pSprite, CPoint ptStart, CPoint ptEnd, int nIndex, DWORD dwDelay, int opt = 0, float	fPlayProportion = 1.0f);
	void	StartMoveSSprite(NMBASE::GRAPHICGDI::xSprite *pSprite, CPoint ptStart, CPoint ptEnd, int nIndex, DWORD dwDelay, int opt = 0, float	fPlayProportion = 1.0f);
	void	StartMoveImage(NMBASE::GRAPHICGDI::xSprite *pSprite, CPoint ptStart, CPoint ptEnd, DWORD dwGab, DWORD dwDelay, int opt = 0, float	fPlayProportion = 1.0f);
	void	OnTimer();

	void	SetSoundID(std::string strSoundID){m_strSoundID = strSoundID;}

	bool	IsMoveEnd(){ return m_bEnd;}
	void    SetMoveEnd(bool bFlag){m_bEnd = bFlag;}	// ���ӵ��� ������ ��� �ٸ� ������ �̹� ���� ī��� ī���� Endó���� ����� ȭ�鿡 ���δ�.
	bool	IsMoveStart(){return m_bMoveStart;}
	CMovementEx::MOVEMENT_TYPE	GetMoveType(){return m_nType;}
	void	DrawSprite(NMBASE::GRAPHICGDI::CPage *pPage);

	void	SetCurPos(CPoint pt){m_ptCurrent = pt;}
	CPoint	GetCurPos(){return m_ptCurrent;}
};