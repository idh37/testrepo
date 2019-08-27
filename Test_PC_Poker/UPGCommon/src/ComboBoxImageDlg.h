#pragma once

#include "UPGDlgBase.h"

#define CRM_SEND_SEEDMONEY   (WM_USER+9500)
#define CRM_SEND_MONEYOPTION (WM_USER+9501)	
#define CRM_SEND_GAMEOPTION  (WM_USER+9502)	
#define CRM_SEND_USERNUM	 (WM_USER+9499)			
#define CRM_SEND_AUTORULE	 (WM_USER+9498)	
#define CRM_SEND_OBSERVER	 (WM_USER+9499)	

enum DRAW_TYPE{
	enum_drawtype_TEXT = 0,		//텍스트로 된 콤보박스
	enum_drawtype_IMG,			//이미지로만 된 콤보 박스
	enum_drawtype_TEXT_IMG,		//텍스트와 이미지 짬뽕
	enum_drawtype_IMG_INDEX,	//이미지 0~9 가지 인덱스를 가지고 있는 숫자 형
};

typedef struct  {
	int		Index;				//리스트 인덱스
	char	strContent[32];		//내용(스트링으로 되어 있을시)
	int     sprindex;			//스프라이트를 이용할시 인덱스
	int     MouseOversprindex;			//스프라이트를 이용할시 인덱스
	INT64   nContentData;		//실제 내용을 담고 있음 금액이 될수 있고 인덱스가 될수 있다.	
	CRect	Rect;
	void Clear(){
		Index = -1;				//리스트 인덱스
		ZeroMemory(strContent,sizeof(strContent));		//내용(스트링으로 되어 있을시)
		sprindex = -1;			//스프라이트를 이용할시 인덱스
		MouseOversprindex = -1;			//스프라이트를 이용할시 인덱스
		nContentData = -1;		//실제 내용을 담고 있음 금액이 될수 있고 인덱스가 될수 있다.
		Rect = 0;
	}
}COMBOBOX_LIST;

typedef struct  {
	DRAW_TYPE	d_type;
	NMBASE::GRAPHICGDI::xSprite		*pSpr;
	POINT		BasePT;
	WPARAM		BtnCmd;
	//콤보 박스 spr 마다 놓인 위치가 틀려 넣어 준다.
	int			nOutLineSprIndex;		//콤보 박스 겉의 테두리 인덱스
	int			nUnderLineSprIndex;		//콤보 박스 리스트 끼리 구분하기 위한 인덱스
	void Clear(){
		d_type = enum_drawtype_TEXT;
		pSpr = NULL;
		BasePT.x = 0;
		BasePT.y = 0;
		BtnCmd = -1;
		nUnderLineSprIndex = -1;
		nOutLineSprIndex = -1;
	}
}COMBOBOX_INFO;

class UPG_COMMON_EXPORT CComboBoxImageDlg : public CUPGDlgBase
{
public:
	CComboBoxImageDlg(CWnd* pParent = NULL);   // standard constructor
	
	CWnd					*m_pParent;
	COMBOBOX_INFO			m_ComboBoxInfo;
	NMBASE::GRAPHICGDI::CPage					m_Page;
	deque<COMBOBOX_LIST>	m_deque_ComboBoxDeck;	
	int						m_nSelectContent;				//deque에 인덱스를 알고 있는다.
	COMBOBOX_LIST			*GetSelectComboBoxContent();	//선택한 데이터를 리턴한다.

	int						m_CombolistWidth;
	int						m_CombolistHeight;
	int						m_MouseOverIndex;

	void					Draw(CDC *pDC);
	void					Draw_Text(CDC *pDC);
	void					Draw_Image(CDC *pDC);

	void					Draw_Number(CDC *pDC,NMBASE::GRAPHICGDI::CPage *pPage,COMBOBOX_LIST *pData);
	CString					get_Numberstring(INT64 number);
	int						get_MonetaryUnit(char ch);

	BOOL					set_ComboBoxInfo(COMBOBOX_INFO *comboboxinfo);
	void					set_List(COMBOBOX_LIST data);

	BOOL DoModaless(  int nlistNum , COMBOBOX_LIST data[] ,COMBOBOX_INFO *comboboxinfo ,CWnd* pParent);
	BOOL DoModaless( CWnd* pParent );
	BOOL SafeClose();

	enum { IDD = IDD_DIALOG_IMAGECOMBO };

public:
	virtual int DoModal( int nlistNum , COMBOBOX_LIST data[] ,COMBOBOX_INFO *comboboxinfo,CWnd* pParent);
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	virtual void OnCancel();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()
};