
#pragma once

namespace NMBASE
{
	namespace GRAPHICGDI
	{
		void setup_dither_mask(DWORD Red,DWORD Green,DWORD Blue,int BytesPerPixel);
		void dither_rgb24(BYTE *Source,int Xsize,int Ysize,BYTE **pTarget);

	}//namespace GRAPHICGDI	

}//namespace NMBASE

