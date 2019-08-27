#pragma once

class CSelectCard7Poker : public CSelectCard
{
public:
	CSelectCard7Poker();
	virtual ~CSelectCard7Poker();

	virtual void OnEndSelectCard(BOOL bautoselect);
	static CSelectCard7Poker &GetInstance();
};

CSelectCard7Poker &g_SelectCard();