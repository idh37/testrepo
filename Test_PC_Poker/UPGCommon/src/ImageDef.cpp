#include "stdafx.h"
#include "ImageDef.h"


NMBASE::GRAPHICGDI::xSprite EtcSpr;			// ¥����

extern void RegistCommonImageFiles()
{

	AddLoadImageList(&EtcSpr,".\\common\\data\\Etc_icons.spr");
}