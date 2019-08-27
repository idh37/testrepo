#pragma once

class CPlayerDrawManager_BD : public CPlayerDrawManager
{
public:
	CPlayerDrawManager_BD( int nMaxPlayer );
	virtual ~CPlayerDrawManager_BD( void );

	int m_nLowBDWinner[MAX_PLAYER];		//바둑이 승자 정보 저장 (바둑이는 1등이 여러명일수 있다.)

public:
	void Draw_CardAll( CDC* pDC );

	void AddLowBDWinnerPNum(int nPNum);// 서버에서 보내준 WinnerPlayerNumber를 넘겨 줘야 합니다.
	bool IsLowBDWinnerPNum(int nPNum);
	void ResetLowBDWinnerPNum();


	virtual void Reset(void);
	virtual void Reset(int nPNum);
};
