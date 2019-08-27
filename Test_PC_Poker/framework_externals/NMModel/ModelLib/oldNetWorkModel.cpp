#include "stdafx.h"
#include "NetWorkModel.h"


CNetWorkModel& Network()
{
	static	CNetWorkModel sNetwork;
	return	sNetwork;
}



CNetWorkModel::CNetWorkModel()
{

}


CNetWorkModel::~CNetWorkModel()
{

}


bool CNetWorkModel::OnAttached()
{

	return true;
}

bool CNetWorkModel::OnDetached()
{

	return true;
}