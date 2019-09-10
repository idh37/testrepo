// Player_Base.h: interface for the CPlayer_BD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER_BASE_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_)
#define AFX_PLAYER_BASE_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"
#include "NMyCard.h"
#include "AnimationDefine.h"	//UPG�۾� �߰�
#include "BadugiRule.h"

#define NCARD_TOTAL 7

// ĳ���Ϳ� ǥ���Ǵ� Ÿ��
enum ECHARTYPE
{
	EHOST_TYPE  = 0,	// �Ϲݹ���
	ESHOST_TYPE = 1,	// ���۹���
	EDEALER_TYPE= 2,	// ����
	EBOSS_TYPE  = 3,	// ����
};

typedef struct  
{
	// 0 ����(����,�ٵ�����) 1 �ο� 3 ����
	int		nValueKind;
	// 0 ���� ���� �䱸�� �ƴ� 1 ���� ������ �䱸 
	BOOL	bValueEqual;
	BOOL	bValueEqual_low;
	// HI ������ 1 ���� 2 �ο� 3 ���� ���̷ο쿡���� ����
	int		nWinCase;

	// TRUE �� FALSE ��
	int     bWin;

	void Clear(){
		nValueKind = 0;
		// 0 ���� ���� �䱸�� �ƴ� 1 ���� ������ �䱸 
		bValueEqual = FALSE;
		// 0 ���� 1 ���� 
		nWinCase = FALSE;

		bValueEqual_low = FALSE;

		bWin = FALSE;
	}
}VALUEDATA;

typedef struct  
{
	CPoint		PT_Avatar;				//�ƹ�Ÿ 
	CPoint		PT_Card;				//ī�� ���� ��ġ

} PLAYERPOISTIONDATA;


typedef struct  
{
	int		m_arrCutCount[ 3 ];
	bool	m_arrbShow[ 3 ];
	
	void Clear()
	{
		for( int i = 0 ; i < 3 ; i ++ ) 
		{
			m_arrCutCount[ i ] = -1;
			m_arrbShow[ i ] = false;
		}		
	}	

} CUTCOUNT_INFO;



class CPlayer_BD  : public CPlayer //UGPCommon �۾� �߰�
{
public:
	CPlayer_BD();
	virtual ~CPlayer_BD();

	PLAYERPOISTIONDATA	m_sUIPos;//��ǥ ����
	
	// ���� ����
	int					m_nCardTotal;
	int					m_nOpenTotal;
	INT64				m_llResultMoney;

	/////////////////ī�� ����
	CNMyCard			m_cMyCard[ NCARD_TOTAL ];		   // ī������ Ŭ���� // ###[8��Ŀ]###			
	int					m_nTotalCardNum;
	bool				m_bCardOPenMove;			// ������� ī�� �̵� �÷��� 
	int					m_nSDelay;				// ���� ������	
	bool				m_bShowValue;			//������ ������
	BOOL				m_bSelectCard;			//zī�� ���� �Ͽ����� (ó�� 3 or 4 �� �ް�)
	bool				m_bDontSelectCard;		//���̻� ī�� ������ ���Ѵ�.

	CMyBetChip			m_cMyChip;				// �� ����Ĩ ������ �ִ�
		
	bool	IsValid();

	void OnChangeSelect(CPoint pt = NULL, int index = -1);		//�ٲܶ� ���콺 �̺�Ʈ
	void ReSetCardPostion();
	void ReSetCardPostionOnly();
	//������ ī�带 ���� ���� �ߴ���. �̰ɷ� �ٲٱ� ��ư,�н� ��ư Ȱ��ȭ ������ ��´�.
	int IsCutCardSelectedNum();

	void DrawEft_CompleteMadeValue(int oldpoint = 0, bool bForce = false );
	void FoldUser_CardClose();			//���� ���� ī�� ���� (���� ����)
	void FoldUser_CardOpen();			

	void	set_CardArray();					//ī�� �迭 ���� �⺻��,������


	virtual void Destroy_Text();
	virtual void Clear();
	virtual	void SetPlayerNum(int pnum);
	virtual void SetNewPlayer(USERINFO *pUI);
	virtual	void Reset();
	virtual void OnTimer();
	virtual void InitRoomData();
	// ���� ���� 
	virtual void OnSetChip(INT64 money);
	virtual void OnSetBetDrawMoney(INT64 money,BOOL bPlus = FALSE,int betflg = -1);

	
	// ī�� ����
	virtual void SetResultFace(int index, bool bresult = FALSE);
	
	virtual int  GetCardNo(int index);
	virtual void SetCardNo(int index, int nCardNo );
	virtual bool GetFace(int index);
	virtual void SetFaceUp(int index);
	virtual void SetFaceDown(int index);
	virtual void ShowCard(int index , bool bshow = true);

	virtual void SetGameResultCardOpen(int delay);	// ī�� ���� (�ִ� ó�� ����) �����
	virtual void SetChioceCardMove(BOOL bdirect , int face = 0); // ���̽��϶� face 0 : �Ϲ� 1�� ���ε� �޸� 2�� �� �ε� �ո� 

	virtual bool MouseOver_PtInCardRect(int xp,int yp);// ī�� ������ Ŭ�� �ߴ��� 
	virtual int  Select_PtInCardRect(CPoint pt);// ī�� ������ �ִ���

	virtual void OnLButtonDown(int xp, int yp);
	virtual void OnLButtonUp(int xp, int yp);
	virtual bool OnMouseMove(int xp, int yp);	

	//����Ʈ ȿ�� �����
	virtual void ClearObjectEffect( OBJECT_TYPE kind, bool ballclear = false );
	

//////////////////////////////////////////////////////////////////////////
//  �� �Լ� ȣ��
	virtual void SetNewCard(int cardNum );
	virtual void SetCardResult();	//ī�带 �����Ͽ� �� ��� �Ѵ�	
	
	virtual const char*   GetValueString(VALUEDATA *data = NULL);
	virtual void    Draw_DarkCard();

	CBadugiRule* GetMyLowBDRule()
	{
		TPASSERT( NULL != m_pMyLowBDRule );
		return m_pMyLowBDRule;
	}

	inline bool IsVaild_CardIndex( int nIndex )
	{
		if ( nIndex < 0 || nIndex >= m_nTotalCardNum )
		{
			return false;
		}

		return true;
	}
	
	CObjectList* GetEffectObjectList() const
	{
		TPASSERT( m_pEffectObjectList != NULL );
		return m_pEffectObjectList;
	}

	void IncTurnLimitCnt()
	{
		++m_nTurnLimitCnt;
	}

	int GetTurnLimitCnt()
	{
		return m_nTurnLimitCnt;
	}

	void ResetTurnLimitCnt()
	{
		m_nTurnLimitCnt = 0;
	}

	CUTCOUNT_INFO& GetCutInfo()
	{
		return m_stCutInfo;
	}

	//max�濡�� ����ݾ� ������Ʈ�� ���� �Լ� �߰�
	INT64 GetNowMoney();
	void SetNowMoney( INT64 nMoney );

protected:
	bool				m_bFirst;
	std::string			m_strValueName;			//����

	CBadugiRule*		m_pMyLowBDRule;			//���ο� �ٵ��̷��̴�
	CObjectList*		m_pEffectObjectList;	//������ ����Ʈ����
	int					m_nTurnLimitCnt;		//���� Ŭ���� �ð��������� ��� �޴��� 3�� �̻��̸� ���� �Ѵ�.
	CUTCOUNT_INFO		m_stCutInfo;			//Ŀ�� Ƚ�� ����
};

#endif // !defined(AFX_PLAYER_BASE_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_)


