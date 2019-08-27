#pragma once

class CSelectCardHighLow : public CSelectCard
{
public:
	CSelectCardHighLow();
	virtual ~CSelectCardHighLow();

	virtual void OnEndSelectCard(BOOL bautoselect);
	static CSelectCardHighLow &GetInstance();
};

CSelectCardHighLow &g_SelectCard();