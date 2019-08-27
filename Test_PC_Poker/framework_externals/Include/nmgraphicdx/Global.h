// 글로벌 변수, 클래스, 함수..

#pragma once
//#ifndef __GLOBAL_H__
//#define __GLOBAL_H__

#include "DirectX.h"
#include "DxGraphic.h"
#include "PointRender.h"

extern CDirectX			g_clDirectX;
extern CDxPage			g_clDxGamePage;
extern CPointRender		g_clPointRender;


extern void		AddCustomEffectColor(DWORD dwColor);
extern void		DelCustomEffectColor(DWORD dwColor);
extern void		ResetCustomEffectColor();
extern DWORD	GetCustomEffectColor(DWORD dwColor);
