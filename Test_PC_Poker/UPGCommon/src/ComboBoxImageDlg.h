#pragma once

#include "UPGDlgBase.h"

#define CRM_SEND_SEEDMONEY   (WM_USER+9500)
#define CRM_SEND_MONEYOPTION (WM_USER+9501)	
#define CRM_SEND_GAMEOPTION  (WM_USER+9502)	
#define CRM_SEND_USERNUM	 (WM_USER+9499)			
#define CRM_SEND_AUTORULE	 (WM_USER+9498)	
#define CRM_SEND_OBSERVER	 (WM_USER+9499)	

enum DRAW_TYPE{
	enum_drawtype_TEXT = 0,		//�ؽ�Ʈ�� �� �޺��ڽ�
	enum_drawtype_IMG,			//�̹����θ� �� �޺� �ڽ�
	enum_drawtype_TEXT_IMG,		//�ؽ�Ʈ�� �̹��� «��
	enum_drawtype_IMG_INDEX,	//�̹��� 0~9 ���� �ε����� ������ �ִ� ���� ��
};

typedef struct  {
	int		Index;				//����Ʈ �ε���
	char	strContent[32];		//����(��Ʈ������ �Ǿ� ������)
	int     sprindex;			//��������Ʈ�� �̿��ҽ� �ε���
	int     MouseOversprindex;			//��������Ʈ�� �̿��ҽ� �ε���
	INT64   nContentData;		//���� ������ ��� ���� �ݾ��� �ɼ� �ְ� �ε����� �ɼ� �ִ�.	
	CRect	Rect;
	void Clear(){
		Index = -1;				//����Ʈ �ε���
		ZeroMemory(strContent,sizeof(strContent));		//����(��Ʈ������ �Ǿ� ������)
		sprindex = -1;			//��������Ʈ�� �̿��ҽ� �ε���
		MouseOversprindex = -1;			//��������Ʈ�� �̿��ҽ� �ε���
		nContentData = -1;		//���� ������ ��� ���� �ݾ��� �ɼ� �ְ� �ε����� �ɼ� �ִ�.
		Rect = 0;
	}
}COMBOBOX_LIST;

typedef struct  {
	DRAW_TYPE	d_type;
	NMBASE::GRAPHICGDI::xSprite		*pSpr;
	POINT		BasePT;
	WPARAM		BtnCmd;
	//�޺� �ڽ� spr ���� ���� ��ġ�� Ʋ�� �־� �ش�.
	int			nOutLineSprIndex;		//�޺� �ڽ� ���� �׵θ� �ε���
	int			nUnderLineSprIndex;		//�޺� �ڽ� ����Ʈ ���� �����ϱ� ���� �ε���
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
	int						m_nSelectContent;				//deque�� �ε����� �˰� �ִ´�.
	COMBOBOX_LIST			*GetSelectComboBoxContent();	//������ �����͸� �����Ѵ�.

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