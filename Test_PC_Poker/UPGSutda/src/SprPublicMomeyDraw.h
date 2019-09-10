// SprPublicMomeyDraw.h: interface for the CSprPublicMomeyDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRPUBLICMOMEYDRAW_H__5D36742D_8C8B_4831_99FB_30B910C64333__INCLUDED_)
#define AFX_SPRPUBLICMOMEYDRAW_H__5D36742D_8C8B_4831_99FB_30B910C64333__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSprPublicMomeyDraw  
{
public:
	CSprPublicMomeyDraw();
	virtual ~CSprPublicMomeyDraw();

	NMBASE::GRAPHICGDI::CPage *pPage;					// ������	
	NMBASE::GRAPHICGDI::xSprite *pNumberSpr;			// ��������Ʈ 

	INT64	nMoney;					// �Ӵ�
	int		nMoneyDeck[30];			// ��������Ʈ �ε���
	int		nTMoneyDeck[4];

	int		Xp,Yp;					// ���� ��ǥ
	int		nTotalMoneyIndex;		// �Ӵ� �ε���

	int		nDrawMode;	

	BOOL	bShow;	
	BOOL	bMoneyMark;				// FALSE =  ���ڷθ� , TRUE = �ѱ� 	

	int		PNum;
	int		SNumberToParse(INT64 money, int &Num1, int &Num2, int &Num3, int &Num4);

	void Init(NMBASE::GRAPHICGDI::CPage *page, NMBASE::GRAPHICGDI::xSprite *spr,int xp, int yp , BOOL bbox = FALSE,int pnum=0);

	void ReSet();
	void Clear();

	void SetMoney(INT64 money,BOOL bmoneymark = FALSE);
	void Draw(int gap = 0);



};

#endif // !defined(AFX_SPRPUBLICMOMEYDRAW_H__5D36742D_8C8B_4831_99FB_30B910C64333__INCLUDED_)
