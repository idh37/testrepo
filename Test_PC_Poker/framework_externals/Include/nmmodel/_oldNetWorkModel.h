
#pragma once

#include "IComponent.h"
#include "ModelDefine.h"

class NMMODEL_CLASS CNetWorkModel : public IComponent
{
public:

	CNetWorkModel();
	virtual ~CNetWorkModel();

public:
	virtual bool OnAttached();
	virtual bool OnDetached();

protected:

private:

};

NMMODEL_API CNetWorkModel& gNetwork();