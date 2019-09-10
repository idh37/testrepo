#if !defined(AFX_COMBOBOXIMAGEDLG_H__4AD9E801_D639_4EBA_8D96_1839D09CB4E0__INCLUDED_)
#define AFX_COMBOBOXIMAGEDLG_H__4AD9E801_D639_4EBA_8D96_1839D09CB4E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboBoxImageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComboBoxImageDlg dialog




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

/////////////////////////////////////////////////////////////////////////////
// CComboBoxDlg dialog

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
	xSprite		*pSpr;
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


class NMDIALOG_CLASS CComboBoxImageDlg : public CDialog
{
// Construction
public:
	CComboBoxImageDlg(CWnd* pParent = NULL);   // standard constructor
	
	CWnd					*m_pParent;
	COMBOBOX_INFO			m_ComboBoxInfo;	
	UINT					nDrawEvent;
	CPage					m_Page;
	deque<COMBOBOX_LIST>	m_deque_ComboBoxDeck;	
	int						m_nSelectContent;				//deque�� �ε����� �˰� �ִ´�.
	COMBOBOX_LIST			*GetSelectComboBoxContent();	//������ �����͸� �����Ѵ�.

	int						m_CombolistWidth;
	int						m_CombolistHeight;
	int						m_MouseOverIndex;

	void					Draw(CDC *pDC);
	void					Draw_Text(CDC *pDC);
	void					Draw_Image(CDC *pDC);

	void					Draw_Number(CDC *pDC,CPage *pPage,COMBOBOX_LIST *pData);
	CString					get_Numberstring(INT64 number);
	int						get_MonetaryUnit(char ch);

	BOOL					set_ComboBoxInfo(COMBOBOX_INFO *comboboxinfo);
	void					set_List(COMBOBOX_LIST data);

	BOOL DoModaless(  int nlistNum , COMBOBOX_LIST data[] ,COMBOBOX_INFO *comboboxinfo ,CWnd* pParent);
	BOOL DoModaless( CWnd* pParent );
	BOOL SafeClose();
// Dialog Data
	//{{AFX_DATA(CComboBoxImageDlg)
	enum { IDD = IDD_DIALOG_IMAGECOMBO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboBoxImageDlg)
	public:
	virtual int DoModal( int nlistNum , COMBOBOX_LIST data[] ,COMBOBOX_INFO *comboboxinfo,CWnd* pParent);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComboBoxImageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	virtual void OnCancel();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOBOXIMAGEDLG_H__4AD9E801_D639_4EBA_8D96_1839D09CB4E0__INCLUDED_)
