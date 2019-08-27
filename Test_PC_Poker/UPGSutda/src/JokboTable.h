#pragma once


#define MAX_JOKBO_TABLE  (12)	//안내창에 나오는 족보의 종류

typedef struct
{
	BOOL jum;
	RECT JokboRect;
//	int xpos;
//	int ypos;
} _jokbo;


class CJokboTable  
{
public:
	CJokboTable();
	virtual ~CJokboTable();

	CRect m_JokboRect[MAX_JOKBO_TABLE];

	BOOL m_bMouseron;
	BOOL m_TableIndex[MAX_JOKBO_TABLE];
	
	int m_Xp[MAX_JOKBO_TABLE];
	int m_Yp[MAX_JOKBO_TABLE];

	//족보팁 
	int m_JokboTip_Num;
	DWORD m_dwJokboTipOn;
	NMBASE::GRAPHICGDI::xSprite *m_pJokboTableSpr;

	
	void Init(NMBASE::GRAPHICGDI::xSprite *pJokboTableSpr);
	void DrawBottom( NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC );
	void DrawTop( NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC );
	void OnMouseMove( int nX, int nY );
	void SetJokboTableIndex(int nIndex);
	void ReSetJokboTable();
	
	

	//_jokbo JOKBO[MAX_JOKBO];	//족보표시위치정보
};
