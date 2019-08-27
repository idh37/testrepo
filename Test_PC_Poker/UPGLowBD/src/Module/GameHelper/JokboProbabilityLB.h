#pragma once

class CJokboProbabilityLB : public CJokboProbability
{
public:
	CJokboProbabilityLB();
	virtual ~CJokboProbabilityLB();

	virtual void SetAllUserCardInfo();
	virtual void CalculationProbability();
	virtual int GetMyCurJokboIndex();
	virtual float GetProbability(int nJokbo);
};