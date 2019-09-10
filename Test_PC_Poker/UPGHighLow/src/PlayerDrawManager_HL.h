#pragma once


class CPlayerDrawManager_HL :
	public CPlayerDrawManager
{
	int							m_nLowWinnerPNum;				// ���� ����(����Pnum)
	int							m_nSwingWinnerPNum;				// ���� ����(����Pnum)

public:
	CPlayerDrawManager_HL( int nMaxPlayer );
	~CPlayerDrawManager_HL(void);


	virtual void SetHighWinnerPNum(int nPNum){ SetWinnerPNum(nPNum); }	// �������� ������ WinnerPlayerNumber�� �Ѱ� ��� �մϴ�.
	virtual int  GetHighWinnerPNum(void){ return GetWinnerPNum(); }

	virtual void SetLowWinnerPNum(int nPNum){m_nLowWinnerPNum = nPNum;}	// �������� ������ WinnerPlayerNumber�� �Ѱ� ��� �մϴ�.
	virtual int  GetLowWinnerPNum(void){return m_nLowWinnerPNum;}

	virtual void SetSwingWinnerPNum(int nPNum){m_nSwingWinnerPNum = nPNum;}	// �������� ������ WinnerPlayerNumber�� �Ѱ� ��� �մϴ�.
	virtual int  GetSwingWinnerPNum(void){return m_nSwingWinnerPNum;}

	virtual void Reset();
	virtual void Reset(int nPNum);
	virtual void CheckResultValue(void);


	virtual void SetSelectedHLS(int nPNum, bool bSelectHLS);			//������ ����/�ο�/������ �����ߴٰ� �˷���

};
