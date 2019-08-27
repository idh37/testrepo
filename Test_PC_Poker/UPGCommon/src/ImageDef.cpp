#include "stdafx.h"
#include "ImageDef.h"


NMBASE::GRAPHICGDI::xSprite EtcSpr;			// Â¥Åõ¸®

extern void RegistCommonImageFiles()
{

	AddLoadImageList(&EtcSpr,".\\common\\data\\Etc_icons.spr");
}