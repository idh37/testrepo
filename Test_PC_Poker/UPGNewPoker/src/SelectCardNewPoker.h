#pragma once

class CSelectCardNewPoker : public CSelectCard
{
public:
	CSelectCardNewPoker();
	virtual ~CSelectCardNewPoker();

	virtual void OnEndSelectCard(BOOL bautoselect);
};