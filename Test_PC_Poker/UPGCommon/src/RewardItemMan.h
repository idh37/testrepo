// RewardItemMan.h: interface for the CRewardItemMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REWARDITEMMAN_H__685B1862_0805_4A4A_9C71_FC0C5E067804__INCLUDED_)
#define AFX_REWARDITEMMAN_H__685B1862_0805_4A4A_9C71_FC0C5E067804__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"
#include <vector>
using namespace std;

// #define ITEM_SCODE_REWARD_FH				51		//풀하우스
// #define ITEM_SCODE_REWARD_FCARD				52		//포카드
// #define ITEM_SCODE_REWARD_SF				53		//스티플
// #define ITEM_SCODE_REWARD_RSF				54		//노티플
// 
// #define ITEM_SCODE_REWARD_MADE				58		// 메이드

// #define ITEM_SCODE_REWARD_WIN10				102		//초심자 찬스 10
// #define ITEM_SCODE_REWARD_WIN100_300		103		//판수 찬스 100~300
// #define ITEM_SCODE_REWARD_WIN500_700		104		//판수 찬스 500,700
// #define ITEM_SCODE_REWARD_WIN1000_100000	105		//1000~1000000
// #define ITEM_SCODE_REWARD_DSLV1050			106		//도신렙10~50
// #define ITEM_SCODE_REWARD_DSLV6070			107		//도신렙60,70
// #define ITEM_SCODE_REWARD_DSLV80100			108		//도신렙80~100
// #define ITEM_SCODE_REWARD_LUCKAVATA			109		//[행운아바타] 2006.07.24
// 
// #define ITEM_SCODE_REWARD_LOWGRADE			123		//승부사 입문 [입문아이템]
 #define ITEM_SCODE_REWARD_HIGHGRADE			124		//승부사 등극
// #define ITEM_SCODE_REWARD_ALLINKING			125		//올인왕찬스	
// 
// #define ITEM_SCODE_REWARD_SPECIALLUCKITEM	132		//[스행운]2007.10.17
// #define ITEM_SCODE_REWARD_ALLINCHANCE		134		//[올인찬스아바타] 2008.07

typedef struct  {
	int itemcode[10];
	int gamecode[10];
	SYSTEMTIME rctime[10];
	SYSTEMTIME expiretime[10];

	BOOL bValid;
	int TotalCnt;
}REWARDITEM_DATA;

class UPG_COMMON_EXPORT CRewardItemMan  
{
public:
	enum REWARD_TYPE
	{
		REWARD_TYPE_NONE = -1,
		REWARD_TYPE_MADE = 0,
		REWARD_TYPE_REGEN,
		REWARD_TYPE_MAX,
	};

	CRewardItemMan();
	virtual ~CRewardItemMan();

	//구배창 닫기
	void CloseBuyDlg();
	// 구매창 보여주기
	void ShowBuyDlg( char* cookidata, int itemcode, int gamecode, SYSTEMTIME& recvtime,int buyitemcnt = 0 );
	// 구매여부 결과
	void CheckRewardItemResult( int PurchaseResult, int ItemCode, SYSTEMTIME&RecvTime, INT64 money );
	// 아이템 구매 가능
	void AllowBuyItem( int itemcode, int gamecode, SYSTEMTIME rctime, SYSTEMTIME expiretime, int viewItemCode ) ;
	
	// 현재 살 수 있는 게임용 특수 아이템을 보여준다.
	void ShowRewardItemState(int nGamecode);			
	// 아이템 우선순위를 돌려준다.
	static int GetRewardItemPreciousLevel( int itemcode );	
	// 특정 아이템을 삭제한다.
	void ClearItem( int itemcode, SYSTEMTIME recvtime ); 
	// 특정 아이템 리스트를 삭제한다.
	void ClearItemList( int itemcode, REWARD_TYPE eRewardType );

	// 아이템 이름을 가져온다.
	static CString GetRewardItemName( int itemcode );	     	
	// 특정 아이템 param을 요청
	void RequestFirstPriorityItem( int gamecode, REWARD_TYPE eRewardType );		     

	//만료된아이템 삭제
	void RemoveExpiredItem();

	// 아이템 획득
	void AddRewardItem( int itemcode, int gamecode, SYSTEMTIME rctime, SYSTEMTIME expiretime ); 
	
	int	get_Item_GameCode(int item );

	
	CBuyLuckAvataDlg	m_LuckAvataItemDlg;	//찬스 아바타 구매창	//UPG작업 ClinetDlg 멤버함수를 여기로 옮김

	// [5광찬스 아이템 작업] [2005-11-27]
	vector<REWARD_GAMEITEM_INFO> m_RewardItem;				//	게임용 특수 아이템 저장소

	// -> 아래 한줄 추가
	BOOL m_bUpdateNavi; // 아이템 유료화 영역을 새로 읽는다.


	REWARD_GAMEITEM_INFO *GetFirstPriorityItem(int gamecode, REWARD_TYPE eRewardType);	//대기실에 노출될 최고우선순위 찬스아이템 가져오기
	REWARD_GAMEITEM_INFO *GetFirstPriorityItem(int gamecode);
	REWARD_TYPE GetItemType(int itemcode);

//protected:
	int							 m_Persuasion_ItemIndex;	// 권유 아이템 인덱스
public:
	static CRewardItemMan& GetInstance();
	REWARD_TYPE GetITemType(int itemcode);
};

UPG_COMMON_EXPORT CRewardItemMan& RewardItemMan();



#endif // !defined(AFX_REWARDITEMMAN_H__685B1862_0805_4A4A_9C71_FC0C5E067804__INCLUDED_)
