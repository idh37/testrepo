#pragma once


class CPlayerDrawManager_HL :
	public CPlayerDrawManager
{
	int							m_nLowWinnerPNum;				// 하이 승자(서버Pnum)
	int							m_nSwingWinnerPNum;				// 스윙 승자(서버Pnum)

public:
	CPlayerDrawManager_HL( int nMaxPlayer );
	~CPlayerDrawManager_HL(void);


	virtual void SetHighWinnerPNum(int nPNum){ SetWinnerPNum(nPNum); }	// 서버에서 보내준 WinnerPlayerNumber를 넘겨 줘야 합니다.
	virtual int  GetHighWinnerPNum(void){ return GetWinnerPNum(); }

	virtual void SetLowWinnerPNum(int nPNum){m_nLowWinnerPNum = nPNum;}	// 서버에서 보내준 WinnerPlayerNumber를 넘겨 줘야 합니다.
	virtual int  GetLowWinnerPNum(void){return m_nLowWinnerPNum;}

	virtual void SetSwingWinnerPNum(int nPNum){m_nSwingWinnerPNum = nPNum;}	// 서버에서 보내준 WinnerPlayerNumber를 넘겨 줘야 합니다.
	virtual int  GetSwingWinnerPNum(void){return m_nSwingWinnerPNum;}

	virtual void Reset();
	virtual void Reset(int nPNum);
	virtual void CheckResultValue(void);


	virtual void SetSelectedHLS(int nPNum, bool bSelectHLS);			//유저가 하이/로우/스윙을 선택했다고 알려줌

};
