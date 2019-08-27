#pragma once

struct IComponent
{
public:
	IComponent(){}

	virtual BOOL OnAttached(){return TRUE;}
	virtual BOOL OnDetached(){return TRUE;}
};