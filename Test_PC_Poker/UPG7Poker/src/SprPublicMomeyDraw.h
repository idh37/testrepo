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

	NMBASE::GRAPHICGDI::CPage *pPage;					// 페이지	
	NMBASE::GRAPHICGDI::xSprite *pNumberSpr;			// 스프라이트 

	INT64	nMoney;					// 머니
	int		nMoneyDeck[30];			// 스프라이트 인덱스
	int		nTMoneyDeck[4];

	int		Xp,Yp;					// 시작 좌표
	int		nTotalMoneyIndex;		// 머니 인덱스

	int		nDrawMode;	

	BOOL	bShow;	
	BOOL	bMoneyMark;				// FALSE =  숫자로만 , TRUE = 한글 	

	int		PNum;
	int		SNumberToParse(INT64 money, int &Num1, int &Num2, int &Num3, int &Num4);

	void Init(NMBASE::GRAPHICGDI::CPage *page, NMBASE::GRAPHICGDI::xSprite *spr,int xp, int yp , BOOL bbox = FALSE,int pnum=0);

	void ReSet();
	void Clear();

	void SetMoney(INT64 money,BOOL bmoneymark = FALSE);
	void Draw(int gap = 0);



};

#endif // !defined(AFX_SPRPUBLICMOMEYDRAW_H__5D36742D_8C8B_4831_99FB_30B910C64333__INCLUDED_)
