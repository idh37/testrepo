#pragma once

class CJackpotMoney;
class CHalfJackpotMoney;
class CJackpotCurrentInfoDlg;
class UPG_LOBBY_EXPORT CJackpotGroup : public LSY::CGroup
{
	enum JackpotTimeState
	{
		State_NoJPTime = 0,
		State_JPTimeReady_Start, // ���� Ÿ�� �غ�.
		State_JPTimeStart,  // ���� Ÿ�� ���� ��.
		State_JPTimeBeforeEnd,  // ���� Ÿ�� ���� 10����.
		State_JPTimeEnd,  // ���� Ÿ�� ���� ������, ���� �� �� ����.
		State_JPRoomChoice, // �漱�� ��� ��.
		State_JPRoomChoiceEnd, // �� ������. ���¿� ���� ���̹� ���� �ƴ��� ����.
		State_JPRoom_Start, // ������ ������ ����.
		State_JPRoom_End, // ������ ����. ��÷�� �ݾ� ����.
		State_JPTimt_ALLEND, // ����Ÿ�� ��� ���� �� 30�� ������.

		State_Max,
	};

	struct HALFJPENDNOTIFY 
	{
		bool isInData ;
		std::string str ;
	
	} ;

protected:
	CJackpotMoney			*m_pJackpotMoney;
	LSY::CImage				*m_pJackpotRelease;	
	LSY::CButton			*m_pBtnJackpotDetail;
	CJackpotCurrentInfoDlg	*m_pJackpotDetailDlg;
	LSY::CSprite			*m_pSprJackpotRelease;

	// �������� 
	LSY::CSprite			*m_pHalfJackpotBack;
	LSY::CText				*m_pHalfJackpotNotiText;
	LSY::CText				*m_pHalfJackpotChannelText;
	CHalfJackpotMoney		*m_pHalfJackpotJackpotMoney;
	
public:
	CJackpotGroup();
	virtual ~CJackpotGroup();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);

	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnReleaseMouseOver(LSY::CMessage *msg);
	LRESULT OnReleaseMouseLeave(LSY::CMessage *msg);
	LRESULT OnDetailDlgLocalChildNotify(LSY::CMessage *msg);
	void SetJackpotRelease(void);

	// �������� ���� �Լ�
	void SetHalfJackpotInit();
	void ResetHalfJackpot() ;

	void SetHalfJackpotReady(UINT32 nTime);
	void SetHalfJackpotTimeStart(UINT32 nTime);
	void SetHalfJackpotTimeBeforeEnd();
	void SetHalfJackpotTimeEnd();
	void SetHalfJackpotAlarm(promotion::_HALFJACKPOT_ALARM* pAlarm );
	void SetHalfJackpotMoney(promotion::HALFJACKPOT_MONEY* pMoney) ;
	void SetHalfJackpotRoom(promotion::HALFJACKPOT_ROOM* pRoom);
	void SetHalfJackpotRoomEnd(promotion::HALFJACKPOT_PRIZE* pPrize);

	void SetHalfJackpotReadyNotiText(int nTime);
	void SetHalfJackpotStartNotiText(int nTime) ;

	void SetHalfJackpotMoney(int nTime) ;

	LRESULT OnTimer(LSY::CMessage *msg) ;

protected:
	void SetJackpotRelease(bool bRelease);

	// �������� ����
	bool m_bIsShowJackpot ;
	bool m_bIsShowJackpotRoomNum ;
	bool m_bIsArriveAllNoti ;

	int  m_nJackpotState ;

	DWORD m_dwNowTime ;
	DWORD m_dwPreTime ;
	DWORD m_dwTime ;

	char	m_szJackpotMoney[128] ;

	int  m_nPlayTime ;
	int  m_nPreTime ;

	int m_nJPEndNotiCount ;
	int m_nJPEndNotiPlayCount ;
	int m_nJPWinnerNotiCount ;
	int m_nJPWinnerNotiPlayCount ;

	int m_nJPRoomNotiCount ;
	int m_nJPChoiceRoomCount ;
	int m_nJPWinnerCount ;

	int m_nWinnerNotiLoopCount ;

	HALFJPENDNOTIFY m_stJPEndNoti[16] ;
	HALFJPENDNOTIFY m_stJPWinnerNoti[46] ;

	int m_nArriveRoomNum[16] ;

	void convertJPMoneytoStr(INT64 jackpotmoney);

	void SetChangeRoomNoti();
	void SetChangeWinnerNoti();


	promotion::HALFJACKPOT_MONEY m_pHalfjackpotMoney ;
};