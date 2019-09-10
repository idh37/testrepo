#pragma once


#include "PromotionBase.h"
#include <EventCommon.h>

class UPG_GAMEBASE_EXPORT CPromotionUPP : public CPromotionBase
{
enum eExit_Type
{
	eExit_None = 0,
	eExit_Timer = 1,	//�ð� �˸��̿� ���� ����
	eExit_Force = 2,	//�Ǽ� ������ ���� ����
};

public:
	CPromotionUPP( CPromotionManager *pManager, int nType );
	virtual ~CPromotionUPP(void);

protected:
	CRect m_rtRemainTime;
	CRect m_rtRemainPan;

	int m_nLevel ; // UPP ���� 
	int m_bAgree ; // UPP ��å�� agree �ߴ��� ���� 
	int m_nWarning; // ��� â ���� ( �ʴ��� ) 
	int m_nExit   ; // ���� ���� ( �ʴ��� )

	int m_nRemainTimeSec;	//�ܿ��ð�
	int m_nRemainRound;		//�ܿ��Ǽ�

	eExit_Type m_eExitType;

	BOOL m_bForceExitSubscript;

	NMBASE::GRAPHICGDI::xSprite	m_sprBack;					//���
public:
	//�α��ν� ������ UPP�����͸� ����
	void SetUPPData(CSV_UPPINFO* msg);
	BOOL CheckUPPAgreement();
	void ShowUPPInfoDlg();

	virtual const bool IsShowTab(void) { return false; }

	virtual void SetActive(const bool &bActive);
	virtual void DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	
	virtual void Accept_OutRoom();
	virtual void Accept_CreateRoom();
	virtual void Accept_EnterRoom();

	virtual BOOL OnLButtonDownMiddle(int &x , int &y);
	virtual BOOL OnLButtonUpMiddle(int &x , int &y);
	virtual BOOL OnMouseMoveMiddle(int &x , int &y);

	virtual void DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart);
	virtual BOOL OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData);
	
	LRESULT  OnShowEndingPopup(WPARAM &wParam, LPARAM &lParam);

	LRESULT OnIsActive(WPARAM &wParam, LPARAM &lParam);
	virtual BOOL OnCommand(WPARAM &wParam, LPARAM &lParam);

	void Test();
};
