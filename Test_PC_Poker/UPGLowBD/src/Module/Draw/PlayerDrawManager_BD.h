#pragma once

class CPlayerDrawManager_BD : public CPlayerDrawManager
{
public:
	CPlayerDrawManager_BD( int nMaxPlayer );
	virtual ~CPlayerDrawManager_BD( void );

	int m_nLowBDWinner[MAX_PLAYER];		//�ٵ��� ���� ���� ���� (�ٵ��̴� 1���� �������ϼ� �ִ�.)

public:
	void Draw_CardAll( CDC* pDC );

	void AddLowBDWinnerPNum(int nPNum);// �������� ������ WinnerPlayerNumber�� �Ѱ� ��� �մϴ�.
	bool IsLowBDWinnerPNum(int nPNum);
	void ResetLowBDWinnerPNum();


	virtual void Reset(void);
	virtual void Reset(int nPNum);
};
