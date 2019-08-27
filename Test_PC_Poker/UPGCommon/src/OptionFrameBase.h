#pragma once


class COptionDlg;

class COptionFrameBase
{
protected:
	COptionDlg* m_OptionDlg;

public:
	COptionFrameBase(COptionDlg* dlg);
	virtual ~COptionFrameBase();

	virtual void Init();
	virtual void Show();
	virtual void Save();
};

