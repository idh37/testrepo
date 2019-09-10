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

// #define ITEM_SCODE_REWARD_FH				51		//Ǯ�Ͽ콺
// #define ITEM_SCODE_REWARD_FCARD				52		//��ī��
// #define ITEM_SCODE_REWARD_SF				53		//��Ƽ��
// #define ITEM_SCODE_REWARD_RSF				54		//��Ƽ��
// 
// #define ITEM_SCODE_REWARD_MADE				58		// ���̵�

// #define ITEM_SCODE_REWARD_WIN10				102		//�ʽ��� ���� 10
// #define ITEM_SCODE_REWARD_WIN100_300		103		//�Ǽ� ���� 100~300
// #define ITEM_SCODE_REWARD_WIN500_700		104		//�Ǽ� ���� 500,700
// #define ITEM_SCODE_REWARD_WIN1000_100000	105		//1000~1000000
// #define ITEM_SCODE_REWARD_DSLV1050			106		//���ŷ�10~50
// #define ITEM_SCODE_REWARD_DSLV6070			107		//���ŷ�60,70
// #define ITEM_SCODE_REWARD_DSLV80100			108		//���ŷ�80~100
// #define ITEM_SCODE_REWARD_LUCKAVATA			109		//[���ƹ�Ÿ] 2006.07.24
// 
// #define ITEM_SCODE_REWARD_LOWGRADE			123		//�ºλ� �Թ� [�Թ�������]
 #define ITEM_SCODE_REWARD_HIGHGRADE			124		//�ºλ� ���
// #define ITEM_SCODE_REWARD_ALLINKING			125		//���ο�����	
// 
// #define ITEM_SCODE_REWARD_SPECIALLUCKITEM	132		//[�����]2007.10.17
// #define ITEM_SCODE_REWARD_ALLINCHANCE		134		//[���������ƹ�Ÿ] 2008.07

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

	//����â �ݱ�
	void CloseBuyDlg();
	// ����â �����ֱ�
	void ShowBuyDlg( char* cookidata, int itemcode, int gamecode, SYSTEMTIME& recvtime,int buyitemcnt = 0 );
	// ���ſ��� ���
	void CheckRewardItemResult( int PurchaseResult, int ItemCode, SYSTEMTIME&RecvTime, INT64 money );
	// ������ ���� ����
	void AllowBuyItem( int itemcode, int gamecode, SYSTEMTIME rctime, SYSTEMTIME expiretime, int viewItemCode ) ;
	
	// ���� �� �� �ִ� ���ӿ� Ư�� �������� �����ش�.
	void ShowRewardItemState(int nGamecode);			
	// ������ �켱������ �����ش�.
	static int GetRewardItemPreciousLevel( int itemcode );	
	// Ư�� �������� �����Ѵ�.
	void ClearItem( int itemcode, SYSTEMTIME recvtime ); 
	// Ư�� ������ ����Ʈ�� �����Ѵ�.
	void ClearItemList( int itemcode, REWARD_TYPE eRewardType );

	// ������ �̸��� �����´�.
	static CString GetRewardItemName( int itemcode );	     	
	// Ư�� ������ param�� ��û
	void RequestFirstPriorityItem( int gamecode, REWARD_TYPE eRewardType );		     

	//����Ⱦ����� ����
	void RemoveExpiredItem();

	// ������ ȹ��
	void AddRewardItem( int itemcode, int gamecode, SYSTEMTIME rctime, SYSTEMTIME expiretime ); 
	
	int	get_Item_GameCode(int item );

	
	CBuyLuckAvataDlg	m_LuckAvataItemDlg;	//���� �ƹ�Ÿ ����â	//UPG�۾� ClinetDlg ����Լ��� ����� �ű�

	// [5������ ������ �۾�] [2005-11-27]
	vector<REWARD_GAMEITEM_INFO> m_RewardItem;				//	���ӿ� Ư�� ������ �����

	// -> �Ʒ� ���� �߰�
	BOOL m_bUpdateNavi; // ������ ����ȭ ������ ���� �д´�.


	REWARD_GAMEITEM_INFO *GetFirstPriorityItem(int gamecode, REWARD_TYPE eRewardType);	//���ǿ� ����� �ְ�켱���� ���������� ��������
	REWARD_GAMEITEM_INFO *GetFirstPriorityItem(int gamecode);
	REWARD_TYPE GetItemType(int itemcode);

//protected:
	int							 m_Persuasion_ItemIndex;	// ���� ������ �ε���
public:
	static CRewardItemMan& GetInstance();
	REWARD_TYPE GetITemType(int itemcode);
};

UPG_COMMON_EXPORT CRewardItemMan& RewardItemMan();



#endif // !defined(AFX_REWARDITEMMAN_H__685B1862_0805_4A4A_9C71_FC0C5E067804__INCLUDED_)
