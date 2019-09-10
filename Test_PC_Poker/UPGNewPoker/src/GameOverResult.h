// GameOverResult.h: interface for the CGameOverResult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEOVERRESULT_H__1A7405AE_F0A1_4839_B372_8B3C6D58E807__INCLUDED_)
#define AFX_GAMEOVERRESULT_H__1A7405AE_F0A1_4839_B372_8B3C6D58E807__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"

class CGameOverResult  
{
public:
	CGameOverResult();
	virtual ~CGameOverResult();

	NMBASE::GRAPHICGDI::CPage				*m_pPage;	
	CCharView			m_cWinnerAV;		//�¸��� 
	
	GAMEOVERRESULT		m_cGOR;				//��� ����Ÿ
	USERINFO			m_cUI;
	
	BOOL				m_bItemBuyUse;		//��ȣõ��
	INT64				m_i64LostMoney;		//���� �Ӵ�

	BOOL				m_bShow;			//�׷���
	BOOL				m_bShowAngel;		//���𰡵� �����ֱ�

	int					m_nXp,m_nYp;		//�׷��� ��ǥ
	int					m_nShowCnt;			//������ �ð�
	int					m_nCardNo[5];
	int					m_nWinCase;			//1 �̸� �������� �¸� 0 �̸� ��ǽ�
	int					m_nWinnerKind;		//0 �¸���,1�г�,2���������	
	CString				m_strResultCardName;

	DWORD				m_dwSetStartTime;
	DWORD				m_dwDrawTime;
	int					m_nPNum;
	int					m_nBackIndex;
	int					m_nDelay;

	void Init(NMBASE::GRAPHICGDI::CPage *page);										// �ʱ�
	void Clear();
	void ReSet();
	void OnTimer();
	
	void AngelDraw(CDC *pdc, NMBASE::GRAPHICGDI::CPage *page);
	void PutAngelResult(GAMEOVERRESULT *pgor,USERINFO *ui,int pnum,int wincase = 0 , int delay = 0,BOOL bShow = TRUE);

	INT64 Get_angel_defmoney(const INT64 &lmoney);
	INT64 Get_angel_defmoneyEx(const INT64 &lmoney);
};

#endif // !defined(AFX_GAMEOVERRESULT_H__1A7405AE_F0A1_4839_B372_8B3C6D58E807__INCLUDED_)
