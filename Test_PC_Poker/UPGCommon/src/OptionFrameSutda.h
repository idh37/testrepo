#pragma once
#include "OptionFrameBase.h"

class COptionFrameSutda : public COptionFrameBase
{
public:
	COptionFrameSutda(COptionDlg* dlg);
	virtual ~COptionFrameSutda();

	virtual void Init();
	virtual void Show();
	virtual void Save();
};

