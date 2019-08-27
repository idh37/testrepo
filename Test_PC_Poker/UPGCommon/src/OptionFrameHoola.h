#pragma once
#include "OptionFrameBase.h"

class COptionFrameHoola : public COptionFrameBase
{
public:
	COptionFrameHoola(COptionDlg* dlg);
	virtual ~COptionFrameHoola();

	virtual void Init();
	virtual void Show();
	virtual void Save();
};

