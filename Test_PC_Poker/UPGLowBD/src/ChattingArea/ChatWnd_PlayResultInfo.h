// ChatWnd_PlayResultInfo.h: interface for the CChatWnd_PlayResultInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATWND_PLAYRESULTINFO_H__ECF7DBF8_9780_4CDC_94BF_5EDC9E296235__INCLUDED_)
#define AFX_CHATWND_PLAYRESULTINFO_H__ECF7DBF8_9780_4CDC_94BF_5EDC9E296235__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLAYRESULTINFO_UP_BTN 420
#define PLAYRESULTINFO_DOWN_BTN 421

typedef struct
{		
	int 			nPNum;		    //내 인덱스	
	int				nServPNum;		//서버인덱스
	int				nWinType;		//나의 승패여부
	std::string		strID;	
	std::string		strValueName;   //나의 족보
	int				nValueKind;		//족보 종류
	INT64			llResultMoney;	//나의 결과 머니	
	int				arrCard[ TOTAL_CARD_BD ];	//카드정보

	int				nWinCase;	    //하이 로우
	float			fGoryRatio;		//고리비율
	int				PremMenuNo;		//회원 등급
	INT64			nGoryMoney;
	bool			bAllIn;			//올인유무
	
	void Clear()
	{
		nPNum = -1;		    // 내 인덱스	
		nServPNum = -1;
		nWinType = -1;		// 나의 승패여부
		strID.clear();
		strValueName.clear();	
		nValueKind = 0;
		memset( arrCard, 0, sizeof( arrCard ));

		llResultMoney = 0;	//나의 결과 머니	
		nWinCase = 0;
		fGoryRatio = 0.000f;
		PremMenuNo = 0;
		nGoryMoney = 0;
		bAllIn = false;
	}

}PLAYRESULTINFO;

typedef struct  
{
	int		nPageIndex;		//내 인덱스
	PLAYRESULTINFO Data[5];

	int     nDrawIndex[5];	//드로우 순서

	void Clear()
	{
		for ( int i = 0 ; i < 5 ; i ++ )
		{
			Data[ i ].Clear();
			nDrawIndex[ i ] = -1;
		}
	}
}PLAYRESULTINFO_DATA;

class CChatWnd_PlayResultInfo  
{
public:
	CChatWnd_PlayResultInfo();
	virtual ~CChatWnd_PlayResultInfo();

	deque<PLAYRESULTINFO_DATA> m_deque_PlayResultInfo;

	BOOL	m_bShow;

	CPoint			m_OffSetpt;				//실제 히스토리가 위치하는 좌표값		
	NMBASE::SKINGDI::CGraphButtonEx	m_ScrollUpBtn;			//스크롤 업 버튼
	NMBASE::SKINGDI::CGraphButtonEx	m_ScrollDownBtn;		//스크롤 다운 버튼
	
	int nCurViewPageNum;		// 현재 페이지 인덱스
	int nCurTotalPageNum;

	void Init(int x, int y);
	void Clear();
	void InitButton();
	void InPut(PLAYRESULTINFO_DATA data);	
	void Draw(CDC *pdc ,NMBASE::GRAPHICGDI::CPage *page);	
	PLAYRESULTINFO_DATA * GetHistoryData(int index);
	
	void OnLButtonDown(int x, int y);	
	void OnLButtonUp(int x, int y);
	void OnMouseMove(int x, int y);
	void OnScrollButtonUpDown(BOOL bUpDown ); //bUpDown == 0 up bUpDown == 1 down	
	void ReSetWindowPos(int x, int y);

	void GetDrawIndex(int drawindex[] );

	BOOL IsShow(){
		return m_bShow;
	}
	void SetShow(BOOL bshow){
		m_bShow = bshow == TRUE ? 1 : 0;		
	}	

	void EnterRoom();
};

#endif // !defined(AFX_CHATWND_PLAYRESULTINFO_H__ECF7DBF8_9780_4CDC_94BF_5EDC9E296235__INCLUDED_)
