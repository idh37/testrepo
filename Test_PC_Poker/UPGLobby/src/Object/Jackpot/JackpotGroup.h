#pragma once

class CJackpotMoney;
class CHalfJackpotMoney;
class CJackpotCurrentInfoDlg;
class UPG_LOBBY_EXPORT CJackpotGroup : public LSY::CGroup
{
	enum JackpotTimeState
	{
		State_NoJPTime = 0,
		State_JPTimeReady_Start, // 잭팟 타임 준비.
		State_JPTimeStart,  // 잭팟 타임 시작 후.
		State_JPTimeBeforeEnd,  // 잭팟 타임 종료 10초전.
		State_JPTimeEnd,  // 잭팟 타임 종료 됬지만, 이전 방 미 종료.
		State_JPRoomChoice, // 방선정 대기 중.
		State_JPRoomChoiceEnd, // 방 선정됨. 상태에 따라 잭팟방 인지 아닌지 노출.
		State_JPRoom_Start, // 선정된 잭팟판 시작.
		State_JPRoom_End, // 잭팟판 종료. 당첨자 금액 노출.
		State_JPTimt_ALLEND, // 잭팟타임 모두 종료 후 30초 딜레이.

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

	// 하프잭팟 
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

	// 하프잭팟 관련 함수
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

	// 하프잭팟 관련
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