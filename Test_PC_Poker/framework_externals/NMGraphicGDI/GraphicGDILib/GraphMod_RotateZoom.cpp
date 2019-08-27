
#include "stdafx.h"
#include "GraphMod.h"
#include <math.h>


namespace NMBASE
{
	namespace GRAPHICGDI
	{

#define RADIAN (0.0174532888f)

#define FLOATROUNDINT(x) ( (int)((x)+0.5f) )
#define FLOATROUND2(x) ( floor(10.0*((x)+0.05))/10.0 )
#define FLOATROUND3(x) ( floor(100.0*((x)+0.005))/100.0 )
#define FLOATROUND4(x) ( floor(1000.0*((x)+0.0005))/1000.0 )
#define FLOATROUND5(x) ( floor(10000.0*((x)+0.00005))/10000.0 )

#define SWAPi(a,b) {(a)^=(b); (b)^=(a); (a)^=(b);}

		//////////////////////////////////////////////////////////////////////////////////

		float _GetAngle(int x1, int y1, int x2, int y2)
		{
			// 픽셀 거리
			int w = x2-x1;
			int h = y1-y2;

			if(w==0 && h==0) return 0;
			else if(w==0) {
				if(h>0) return 90.0f;
				else return 270.0f;
			}
			else if(h==0) {
				if(w>0) return 0.0f;
				else return 180.0f;
			}

			// 기울기 구하기
			float al = (float)((float)abs(h)/(float)abs(w));
			float angle = (float)(atan(al) / RADIAN);

			if(w==0 && h==0) return 0;
			else if(w>=0 && h>=0) {		// 1사분면
			}
			else if(w<0 && h>=0) {	// 2사분면
				angle = 180.0f - angle;
			}
			else if(w<0 && h<0) {	// 3사분면
				angle += 180.0f;
			}	
			else if(w>=0 && h<0) {	// 4사분면
				angle = 360.0f - angle;
			}

			return angle;
		}

		POINT _GetMovePoint(int sx, int sy, float angle, float distance)
		{
			POINT pnt;
			pnt.x = sx;
			pnt.y = sy;
			if(distance == 0) return pnt;
			double rad = (double)angle * RADIAN;
			int x = FLOATROUNDINT(cos(rad) * (double)distance);
			int y = -FLOATROUNDINT(sin(rad) * (double)distance);
			pnt.x += x;
			pnt.y += y;
			return pnt;
		}


		void _RotatePoint(POINT* pPoint, float angle)
		{
			if(angle != 0) {
				double rad = angle * RADIAN;

				// 정점 회전(3D에서의 올바른 표현)
				//int x = (int)(pPoint->x * cos(rad) - pPoint->y * sin(rad));
				//int y = (int)(pPoint->x * sin(rad) + pPoint->y * cos(rad));

				// 정점 회전(Y축이 뒤집힌 2D 스크린 픽셀 모드에서의 회전)
				int x = FLOATROUNDINT(pPoint->x * cos(rad) + pPoint->y * sin(rad));
				int y = FLOATROUNDINT(pPoint->y * cos(rad) - pPoint->x * sin(rad));

				pPoint->x = x;
				pPoint->y = y;
			}
		}

		void _RotatePoint(float* pxp, float* pyp, float angle)
		{
			if(angle != 0) {
				double rad = angle * RADIAN;

				// 정점 회전(3D에서의 올바른 표현)
				//int x = (int)(pPoint->x * cos(rad) - pPoint->y * sin(rad));
				//int y = (int)(pPoint->x * sin(rad) + pPoint->y * cos(rad));

				// 정점 회전(Y축이 뒤집힌 2D 스크린 픽셀 모드에서의 회전)
				float x = (float)((*pxp) * cos(rad) + (*pyp) * sin(rad));
				float y = (float)((*pyp) * cos(rad) - (*pxp) * sin(rad));

				*pxp = x;
				*pyp = y;
			}
		}

		void _AxisRotatePoint(POINT* pPoint, float angle, POINT axis)
		{
			if(angle != 0) {

				POINT pt = *pPoint;
				pt.x -= axis.x;
				pt.y -= axis.y;

				_RotatePoint(&pt, angle);

				pPoint->x = pt.x + axis.x;
				pPoint->y = pt.y + axis.y;
			}
		}

		void _AxisRotatePoint(float* pxp, float* pyp, float angle, float axis_x, float axis_y)
		{
			if(angle != 0) {

				float x = *pxp;
				float y = *pyp;

				x -= axis_x;
				y -= axis_y;

				_RotatePoint(&x, &y, angle);

				*pxp = x + axis_x;
				*pyp = y + axis_y;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////


		/////////////////////////// Rendering Environment

		WORD*	g_pRenderTarget = NULL;
		WORD*	g_pTexture = NULL;
		BYTE*	g_pAlphaTable = NULL;
		int		g_TexWidth = 256;
		int		g_TexHeight = 256;
		WORD	g_TexColorKey = 0;
		int		g_TexAlpha = 31;

		// 함수 포인터
		PFN_HLINETEXTURE g_pfnHLineTexture = HLineTexture;

		///////////////////////////

		void _SetRenderTarget(WORD* bits)
		{
			g_pRenderTarget = bits;
		}

		void _SetTexture(WORD* bits, int width, int height, BYTE* alphatable)
		{
			g_pTexture = bits;
			g_TexWidth = width;
			g_TexHeight = height;
			g_pAlphaTable = alphatable;
		}

		void _SetTextureAlpha(int alpha)
		{
			g_TexAlpha = alpha;
		}

		void _SetTextureColorKey(WORD color)
		{
			g_TexColorKey = color;
		}

		void _SetBlendingFunc_HLineTexture(PFN_HLINETEXTURE pfnNew)
		{
			g_pfnHLineTexture = pfnNew;
		}

		void HLine(int x1, int y, int x2, WORD color)
		{
			int temp;
			register int off;

			if(x1 > x2) 
				SWAPi(x1, x2);

			if(x1 > _CX2 || x2 < _CX1)
				return;

			if(x1 < _CX1) x1 = _CX1;
			if(x2 > _CX2) x2 = _CX2;

			off = (y * _LPITCH) + x1;
			temp = (y * _LPITCH) + x2;

			while ( off <= temp )
			{
				g_pRenderTarget[off] = color;
				++off;
			}
		}

		void FillTri(POINT* pTri, WORD color)
		{
			POINT Top, Middle, Bottom;
			int xTB, xTMB;
			int xdTB, xdTM, xdMB;
			int i, TopIndex, MiddleIndex, BottomIndex;
			int y;

			// Sorting Triangle Vertex
			Top.x = (int)(pTri[0].x);
			Top.y = (int)(pTri[0].y);
			TopIndex = 0;
			for(i=1; i<3; i++) {
				if(Top.y > (int)pTri[i].y) {
					Top.x = (int)pTri[i].x;
					Top.y = (int)pTri[i].y;
					TopIndex = i;
				}
			}

			Bottom.x = (int)(pTri[0].x);
			Bottom.y = (int)(pTri[0].y);
			BottomIndex = 0;
			for(i=1; i<3; i++) {
				if(Bottom.y < (int)pTri[i].y) {
					Bottom.x = (int)pTri[i].x;
					Bottom.y = (int)pTri[i].y;
					BottomIndex = i;
				}
			}

			MiddleIndex = 3 - (TopIndex + BottomIndex);
			Middle.x = (int)(pTri[MiddleIndex].x);
			Middle.y = (int)(pTri[MiddleIndex].y);

			y = Top.y;

			xTB = Top.x<<16;
			xTMB = Top.x<<16;

			if(Bottom.y == Top.y) return;
			xdTB = ((Bottom.x - Top.x)<<16) / (Bottom.y - Top.y);

			if(Middle.y == Top.y) { xTMB = Middle.x<<16; goto MIDDLETOBOTTOM; }
			xdTM = ((Middle.x - Top.x)<<16) / (Middle.y - Top.y);

			if(Middle.y > _CY2) 
				Middle.y = _CY2;

			if(Top.y < _CY1) {
				xTB += (xdTB * (_CY1 - Top.y));
				xTMB += (xdTM * (_CY1 - Top.y));
				y = 0;
			}

			if(Middle. y < _CY1) 
				goto MIDDLETOBOTTOM;

			while(y < Middle.y)
			{
				HLine( (xTB)>>16, y, (xTMB + 32768)>>16, color);	// 32768 == 1<<16 : 최소 1픽셀은 그리기 위하여 더해 줌

				xTB += xdTB;
				xTMB += xdTM;

				++y;
			}

MIDDLETOBOTTOM:	

			if(Bottom.y == Middle.y) return;
			xdMB = ((Bottom.x - Middle.x)<<16) / (Bottom.y - Middle.y);
			if(Bottom.y > _CY2) 
				Bottom.y = _CY2;

			if(Middle.y < _CY1) {
				xTB = Top.x <<16;
				if( Middle.y != Top.y ) xTMB = Top.x <<16;
				else xTMB = Middle.x <<16;
				xTB += (xdTB * (_CY1 - Top.y));
				xTMB += (xdTM * (Middle.y - Top.y));
				xTMB += (xdMB * (_CY1 - Middle.y));
				y = _CY1;
			}

			while(y < Bottom.y)
			{
				HLine( (xTB)>>16, y, (xTMB)>>16, color);

				xTB += xdTB;
				xTMB += xdMB;

				++y;
			}
		}
		void _fastcall CSPHLineTexture(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2)
		{
			int temp;
			register int off;
			TEXCOORDi Tex, dTex;

			if(x1 == x2) return;

			if(x1 > x2) {
				SWAPi(x1, x2);
				SWAPi(tex1.u, tex2.u);
				SWAPi(tex1.v, tex2.v);
			}

			Tex.u = tex1.u;
			Tex.v = tex1.v;

			dTex.u = (tex2.u - tex1.u) / (x2 - x1);
			dTex.v = (tex2.v - tex1.v) / (x2 - x1);

			if(x1 < _CX1) {
				Tex.u += dTex.u * (_CX1 - x1);
				Tex.v += dTex.v * (_CX1 - x1);
				x1 = _CX1;
			}

			if(x2 > _CX2) x2 = _CX2;

			off = (y * _LPITCH) + x1;
			temp = (y * _LPITCH) + x2;

			WORD colorkey = g_TexColorKey;
			WORD texcolor;
			WORD* sor = g_pTexture;
			for( int i = 0 ; i < x2 - x1 ; i++ )
			{
				WORD* src = GET_CSPLINEPTR(sor,(Tex.v>>16)); // 찍을 소스의 라인으로 이동
				int count = 0;
				int xstartpos = int(Tex.u>>16);
				int ptnCnt=(*(short*)src++);
				int bFind = 0;
				int totalbyte = 0;
				while(ptnCnt-- && bFind == FALSE )         // 패턴수 만큼 반복
				{
					int sbyte = (int)(*src++);  // 0번색의 개수 읽기
					int pbyte = (int)(*src++);  // 데이타의 개수 읽기

					totalbyte += (sbyte + pbyte);

					if( totalbyte < xstartpos )
					{
						src+=pbyte;
						count+=(sbyte + pbyte);
					}
					else
					{
						int sub = xstartpos-count;
						if( sbyte + count < xstartpos )
						{
							count+=sbyte;
						}
						else
						{
							count += sub;

							bFind = 1;	
							Tex.u += dTex.u;		
							Tex.v += dTex.v;	
							++off;
							count++;	
							break;
						}


						if( pbyte + count < xstartpos )
						{
							count += pbyte;
							src   += pbyte;
						}
						else
						{
							sub = xstartpos-count;
							count += sub;
							src   += sub;

							bFind = 2;
							texcolor = *((WORD *)(src));
							if(texcolor != colorkey)
								*((WORD*)(g_pRenderTarget + off)) = texcolor;

							Tex.u += dTex.u;		
							Tex.v += dTex.v;		
							++off;
							src++;
							count++;
						}

					}
				}
			}
		}
		void _fastcall CSPHLineTexture_HB(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2)
		{
			int temp;
			register int off;
			TEXCOORDi Tex, dTex;

			if(x1 == x2) return;

			if(x1 > x2) {
				SWAPi(x1, x2);
				SWAPi(tex1.u, tex2.u);
				SWAPi(tex1.v, tex2.v);
			}

			Tex.u = tex1.u;
			Tex.v = tex1.v;

			dTex.u = (tex2.u - tex1.u) / (x2 - x1);
			dTex.v = (tex2.v - tex1.v) / (x2 - x1);

			if(x1 < _CX1) {
				Tex.u += dTex.u * (_CX1 - x1);
				Tex.v += dTex.v * (_CX1 - x1);
				x1 = _CX1;
			}

			if(x2 > _CX2) x2 = _CX2;

			off = (y * _LPITCH) + x1;
			temp = (y * _LPITCH) + x2;

			WORD colorkey = g_TexColorKey;
			WORD texcolor;
			WORD* sor = g_pTexture;
			for( int i = 0 ; i < x2 - x1 ; i++ )
			{
				WORD* src = GET_CSPLINEPTR(sor,(Tex.v>>16)); // 찍을 소스의 라인으로 이동
				int count = 0;
				int xstartpos = int(Tex.u>>16);
				int ptnCnt=(*(short*)src++);
				int bFind = 0;
				int totalbyte = 0;
				while(ptnCnt-- && bFind == FALSE )         // 패턴수 만큼 반복
				{
					int sbyte = (int)(*src++);  // 0번색의 개수 읽기
					int pbyte = (int)(*src++);  // 데이타의 개수 읽기

					totalbyte += (sbyte + pbyte);

					if( totalbyte < xstartpos )
					{
						src+=pbyte;
						count+=(sbyte + pbyte);
					}
					else
					{
						int sub = xstartpos-count;
						if( sbyte + count < xstartpos )
						{
							count+=sbyte;
						}
						else
						{
							count += sub;

							bFind = 1;	
							Tex.u += dTex.u;		
							Tex.v += dTex.v;	
							++off;
							count++;	
							break;
						}


						if( pbyte + count < xstartpos )
						{
							count += pbyte;
							src   += pbyte;
						}
						else
						{
							sub = xstartpos-count;
							count += sub;
							src   += sub;

							bFind = 2;
							texcolor = *((WORD *)(src));
							if(texcolor != colorkey)
							{
								WORD* dest = (WORD*)(g_pRenderTarget + off);
								*dest = HBMIXW_555(*dest, texcolor);
							}

							Tex.u += dTex.u;		
							Tex.v += dTex.v;		
							++off;
							src++;
							count++;
						}

					}
				}
			}
		}
		void _fastcall CSPHLineTexture_GB(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2)
		{
			int temp;
			register int off;
			TEXCOORDi Tex, dTex;

			if(x1 == x2) return;

			if(x1 > x2) {
				SWAPi(x1, x2);
				SWAPi(tex1.u, tex2.u);
				SWAPi(tex1.v, tex2.v);
			}

			Tex.u = tex1.u;
			Tex.v = tex1.v;

			dTex.u = (tex2.u - tex1.u) / (x2 - x1);
			dTex.v = (tex2.v - tex1.v) / (x2 - x1);

			if(x1 < _CX1) {
				Tex.u += dTex.u * (_CX1 - x1);
				Tex.v += dTex.v * (_CX1 - x1);
				x1 = _CX1;
			}

			if(x2 > _CX2) x2 = _CX2;

			off = (y * _LPITCH) + x1;
			temp = (y * _LPITCH) + x2;

			int alpha = g_TexAlpha;
			int dest_alpha = 31 - alpha;

			WORD colorkey = g_TexColorKey;
			WORD texcolor;
			WORD* sor = g_pTexture;
			for( int i = 0 ; i < x2 - x1 ; i++ )
			{
				WORD* src = GET_CSPLINEPTR(sor,(Tex.v>>16)); // 찍을 소스의 라인으로 이동
				int count = 0;
				int xstartpos = int(Tex.u>>16);
				int ptnCnt=(*(short*)src++);
				int bFind = 0;
				int totalbyte = 0;
				while(ptnCnt-- && bFind == FALSE )         // 패턴수 만큼 반복
				{
					int sbyte = (int)(*src++);  // 0번색의 개수 읽기
					int pbyte = (int)(*src++);  // 데이타의 개수 읽기

					totalbyte += (sbyte + pbyte);

					if( totalbyte < xstartpos )
					{
						src+=pbyte;
						count+=(sbyte + pbyte);
					}
					else
					{
						int sub = xstartpos-count;
						if( sbyte + count < xstartpos )
						{	
							count+=sbyte;
						}
						else
						{
							count += sub;

							bFind = 1;	
							Tex.u += dTex.u;		
							Tex.v += dTex.v;	
							++off;
							count++;	
							break;
						}


						if( pbyte + count < xstartpos )
						{
							count += pbyte;
							src   += pbyte;
						}
						else
						{
							sub = xstartpos-count;
							count += sub;
							src   += sub;

							bFind = 2;
							texcolor = *((WORD *)(src));
							if(texcolor != colorkey)
							{
								WORD* dest = (WORD*)(g_pRenderTarget + off);
								*dest = ALPHAMIXW_555(*dest, texcolor, dest_alpha, alpha);
							}
							Tex.u += dTex.u;		
							Tex.v += dTex.v;		
							++off;
							src++;
							count++;
						}

					}
				}
			}
		}
		void _fastcall CSPHLineTexture_LE(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2)
		{
			int temp;
			register int off;
			TEXCOORDi Tex, dTex;

			if(x1 == x2) return;

			if(x1 > x2) {
				SWAPi(x1, x2);
				SWAPi(tex1.u, tex2.u);
				SWAPi(tex1.v, tex2.v);
			}

			Tex.u = tex1.u;
			Tex.v = tex1.v;

			dTex.u = (tex2.u - tex1.u) / (x2 - x1);
			dTex.v = (tex2.v - tex1.v) / (x2 - x1);

			if(x1 < _CX1) {
				Tex.u += dTex.u * (_CX1 - x1);
				Tex.v += dTex.v * (_CX1 - x1);
				x1 = _CX1;
			}

			if(x2 > _CX2) x2 = _CX2;

			off = (y * _LPITCH) + x1;
			temp = (y * _LPITCH) + x2;

			WORD colorkey = g_TexColorKey;
			int alpha = g_TexAlpha;
			int dest_alpha = 31 - alpha;
			WORD texcolor;
			WORD* sor = g_pTexture;
			for( int i = 0 ; i < x2 - x1 ; i++ )
			{
				WORD* src = GET_CSPLINEPTR(sor,(Tex.v>>16)); // 찍을 소스의 라인으로 이동
				int count = 0;
				int xstartpos = int(Tex.u>>16);
				int ptnCnt=(*(short*)src++);
				int bFind = 0;
				int totalbyte = 0;
				while(ptnCnt-- && bFind == FALSE )         // 패턴수 만큼 반복
				{
					int sbyte = (int)(*src++);  // 0번색의 개수 읽기
					int pbyte = (int)(*src++);  // 데이타의 개수 읽기

					totalbyte += (sbyte + pbyte);

					if( totalbyte < xstartpos )
					{
						src+=pbyte;
						count+=(sbyte + pbyte);
					}
					else
					{
						int sub = xstartpos-count;
						if( sbyte + count < xstartpos )
						{
							count+=sbyte;
						}
						else
						{
							count += sub;

							bFind = 1;	
							Tex.u += dTex.u;		
							Tex.v += dTex.v;	
							++off;
							count++;	
							break;
						}


						if( pbyte + count < xstartpos )
						{
							count += pbyte;
							src   += pbyte;
						}
						else
						{
							sub = xstartpos-count;
							count += sub;
							src   += sub;

							bFind = 2;
							texcolor = *((WORD *)(src));
							if(texcolor != colorkey) {
								WORD* dest = (WORD*)(g_pRenderTarget + off);
								*dest = LEMIXW_555(*dest, texcolor, alpha);
							}

							Tex.u += dTex.u;		
							Tex.v += dTex.v;		
							++off;
							src++;
							count++;
						}

					}
				}
			}	
		}
		void _fastcall CSPHLineTexture_DE(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2)
		{
			int temp;
			register int off;
			TEXCOORDi Tex, dTex;

			if(x1 == x2) return;

			if(x1 > x2) {
				SWAPi(x1, x2);
				SWAPi(tex1.u, tex2.u);
				SWAPi(tex1.v, tex2.v);
			}

			Tex.u = tex1.u;
			Tex.v = tex1.v;

			dTex.u = (tex2.u - tex1.u) / (x2 - x1);
			dTex.v = (tex2.v - tex1.v) / (x2 - x1);

			if(x1 < _CX1) {
				Tex.u += dTex.u * (_CX1 - x1);
				Tex.v += dTex.v * (_CX1 - x1);
				x1 = _CX1;
			}

			if(x2 > _CX2) x2 = _CX2;

			off = (y * _LPITCH) + x1;
			temp = (y * _LPITCH) + x2;

			WORD colorkey = g_TexColorKey;
			int alpha = g_TexAlpha;
			int dest_alpha = 31 - alpha;
			WORD texcolor;
			WORD* sor = g_pTexture;
			for( int i = 0 ; i < x2 - x1 ; i++ )
			{
				WORD* src = GET_CSPLINEPTR(sor,(Tex.v>>16)); // 찍을 소스의 라인으로 이동
				int count = 0;
				int xstartpos = int(Tex.u>>16);
				int ptnCnt=(*(short*)src++);
				int bFind = 0;
				int totalbyte = 0;
				while(ptnCnt-- && bFind == FALSE )         // 패턴수 만큼 반복
				{
					int sbyte = (int)(*src++);  // 0번색의 개수 읽기
					int pbyte = (int)(*src++);  // 데이타의 개수 읽기

					totalbyte += (sbyte + pbyte);

					if( totalbyte < xstartpos )
					{
						src+=pbyte;
						count+=(sbyte + pbyte);
					}
					else
					{
						int sub = xstartpos-count;
						if( sbyte + count < xstartpos )
						{
							count+=sbyte;
						}
						else
						{
							count += sub;

							bFind = 1;	
							Tex.u += dTex.u;		
							Tex.v += dTex.v;	
							++off;
							count++;	
							break;
						}


						if( pbyte + count < xstartpos )
						{
							count += pbyte;
							src   += pbyte;
						}
						else
						{
							sub = xstartpos-count;
							count += sub;
							src   += sub;

							bFind = 2;
							texcolor = *((WORD *)(src));
							if(texcolor != colorkey) {
								WORD* dest = (WORD*)(g_pRenderTarget + off);
								*dest = DEMIXW_555(*dest, texcolor, alpha);
							}

							Tex.u += dTex.u;		
							Tex.v += dTex.v;		
							++off;
							src++;
							count++;
						}

					}
				}
			}	
		}
		void _fastcall CSPHLineTexture_AB(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2)
		{
			int temp;
			register int off;
			TEXCOORDi Tex, dTex;
			if(x1 == x2) return;

			if(x1 > x2) {
				SWAPi(x1, x2);
				SWAPi(tex1.u, tex2.u);
				SWAPi(tex1.v, tex2.v);
			}

			Tex.u = tex1.u;
			Tex.v = tex1.v;

			dTex.u = (tex2.u - tex1.u) / (x2 - x1);
			dTex.v = (tex2.v - tex1.v) / (x2 - x1);

			if(x1 < _CX1) {
				Tex.u += dTex.u * (_CX1 - x1);
				Tex.v += dTex.v * (_CX1 - x1);
				x1 = _CX1;
			}

			if(x2 > _CX2) x2 = _CX2;

			off = (y * _LPITCH) + x1;
			temp = (y * _LPITCH) + x2;

			WORD colorkey = g_TexColorKey;
			WORD texcolor;
			WORD* sor = g_pTexture;
			for( int i = 0 ; i < x2 - x1 ; i++ )
			{
				WORD* src = GET_CSPLINEPTR(sor,(Tex.v>>16)); // 찍을 소스의 라인으로 이동
				int count = 0;
				int xstartpos = int(Tex.u>>16);
				int ptnCnt=(*(short*)src++);
				int bFind = 0;
				int totalbyte = 0;
				while(ptnCnt-- && bFind == FALSE )         // 패턴수 만큼 반복
				{
					int sbyte = (int)(*src++);  // 0번색의 개수 읽기
					int pbyte = (int)(*src++);  // 데이타의 개수 읽기

					totalbyte += (sbyte + pbyte);

					if( totalbyte < xstartpos )
					{
						src+=pbyte;
						count+=(sbyte + pbyte);
					}
					else
					{
						int sub = xstartpos-count;
						if( sbyte + count < xstartpos )
						{
							count+=sbyte;
						}
						else
						{
							count += sub;

							bFind = 1;	
							Tex.u += dTex.u;		
							Tex.v += dTex.v;	
							++off;
							count++;	
							break;
						}


						if( pbyte + count < xstartpos )
						{
							count += pbyte;
							src   += pbyte;
						}
						else
						{
							sub = xstartpos-count;
							count += sub;
							src   += sub;

							bFind = 2;

							int toff = ((Tex.v>>16) * g_TexWidth) + (Tex.u>>16);
							texcolor = *((WORD *)(src));
							if(texcolor != colorkey) {
								int alpha = ABGRADE[g_TexAlpha][*(g_pAlphaTable+ toff)];
								int dest_alpha = 31 - alpha;
								if( alpha != 0 )
								{
									WORD* dest = (WORD*)(g_pRenderTarget + off);
									*dest =  ALPHAMIXW_555(*dest, texcolor, dest_alpha, alpha);
								}
							}

							Tex.u += dTex.u;		
							Tex.v += dTex.v;		
							++off;
							src++;
							count++;
						}

					}
				}
			}
		}

		void _fastcall HLineTexture(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2)
		{
			int temp;
			register int off;
			UINT toff;
			TEXCOORDi Tex, dTex;

			if(x1 == x2) return;

			if(x1 > x2) {
				SWAPi(x1, x2);
				SWAPi(tex1.u, tex2.u);
				SWAPi(tex1.v, tex2.v);
			}

			Tex.u = tex1.u;
			Tex.v = tex1.v;

			dTex.u = (tex2.u - tex1.u) / (x2 - x1);
			dTex.v = (tex2.v - tex1.v) / (x2 - x1);

			if(x1 < _CX1) {
				Tex.u += dTex.u * (_CX1 - x1);
				Tex.v += dTex.v * (_CX1 - x1);
				x1 = _CX1;
			}

			if(x2 > _CX2) x2 = _CX2;

			off = (y * _LPITCH) + x1;
			temp = (y * _LPITCH) + x2;

			WORD colorkey = g_TexColorKey;
			WORD texcolor;

			while ( off <= temp )
			{
				// 2의 승수 사이즈의 텍스쳐인 경우 텍스쳐UV의 범위를 0~1로 제한
				//toff = (((Tex.v>>16) & (g_TexHeight-1)) * g_TexWidth) + ((Tex.u>>16) & (g_TexWidth-1));

				// 이 방식은 사이즈 선택은 자유로우나 텍스쳐 UV가 0보다 작거나 1보다 크면 오류가 남
				toff = ((Tex.v>>16) * g_TexWidth) + (Tex.u>>16);

				texcolor = *((WORD *)(g_pTexture + toff));
				if(texcolor != colorkey)
					*((WORD*)(g_pRenderTarget + off)) = texcolor;

				Tex.u += dTex.u;		
				Tex.v += dTex.v;		

				++off;
			}
		}

		void _fastcall HLineTexture_HB(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2)
		{
			int temp;
			register int off;
			UINT toff;
			TEXCOORDi Tex, dTex;

			if(x1 == x2) return;

			if(x1 > x2) {
				SWAPi(x1, x2);
				SWAPi(tex1.u, tex2.u);
				SWAPi(tex1.v, tex2.v);
			}

			Tex.u = tex1.u;
			Tex.v = tex1.v;

			dTex.u = (tex2.u - tex1.u) / (x2 - x1);
			dTex.v = (tex2.v - tex1.v) / (x2 - x1);

			if(x1 < _CX1) {
				Tex.u += dTex.u * (_CX1 - x1);
				Tex.v += dTex.v * (_CX1 - x1);
				x1 = _CX1;
			}

			if(x2 > _CX2) x2 = _CX2;

			off = (y * _LPITCH) + x1;
			temp = (y * _LPITCH) + x2;

			WORD colorkey = g_TexColorKey;
			WORD texcolor;

			while ( off < temp )	// 각 Triangle 접선에서 픽셀 중복을 막기 위하여 마지막 픽셀을 그리지 않음
			{
				toff = ((Tex.v>>16) * g_TexWidth) + (Tex.u>>16);

				texcolor = *((WORD *)(g_pTexture + toff));
				if(texcolor != colorkey) {
					WORD* dest = (WORD*)(g_pRenderTarget + off);
					*dest = HBMIXW_555(*dest, texcolor);
				}

				Tex.u += dTex.u;		
				Tex.v += dTex.v;		

				++off;
			}
		}

		void _fastcall HLineTexture_GB(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2)
		{
			int temp;
			register int off;
			UINT toff;
			TEXCOORDi Tex, dTex;

			if(x1 == x2) return;

			if(x1 > x2) {
				SWAPi(x1, x2);
				SWAPi(tex1.u, tex2.u);
				SWAPi(tex1.v, tex2.v);
			}

			Tex.u = tex1.u;
			Tex.v = tex1.v;

			dTex.u = (tex2.u - tex1.u) / (x2 - x1);
			dTex.v = (tex2.v - tex1.v) / (x2 - x1);

			if(x1 < _CX1) {
				Tex.u += dTex.u * (_CX1 - x1);
				Tex.v += dTex.v * (_CX1 - x1);
				x1 = _CX1;
			}

			if(x2 > _CX2) x2 = _CX2;

			off = (y * _LPITCH) + x1;
			temp = (y * _LPITCH) + x2;

			WORD colorkey = g_TexColorKey;
			int alpha = g_TexAlpha;
			int dest_alpha = 31 - alpha;

			WORD texcolor;

			while ( off < temp )	// 각 Triangle 접선에서 픽셀 중복을 막기 위하여 마지막 픽셀을 그리지 않음
			{
				toff = ((Tex.v>>16) * g_TexWidth) + (Tex.u>>16);

				texcolor = *((WORD *)(g_pTexture + toff));
				if(texcolor != colorkey) {
					WORD* dest = (WORD*)(g_pRenderTarget + off);
					*dest = ALPHAMIXW_555(*dest, texcolor, dest_alpha, alpha);
				}

				Tex.u += dTex.u;		
				Tex.v += dTex.v;		

				++off;
			}
		}

		void _fastcall HLineTexture_LE(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2)
		{
			int temp;
			register int off;
			UINT toff;
			TEXCOORDi Tex, dTex;

			if(x1 == x2) return;

			if(x1 > x2) {
				SWAPi(x1, x2);
				SWAPi(tex1.u, tex2.u);
				SWAPi(tex1.v, tex2.v);
			}

			Tex.u = tex1.u;
			Tex.v = tex1.v;

			dTex.u = (tex2.u - tex1.u) / (x2 - x1);
			dTex.v = (tex2.v - tex1.v) / (x2 - x1);

			if(x1 < _CX1) {
				Tex.u += dTex.u * (_CX1 - x1);
				Tex.v += dTex.v * (_CX1 - x1);
				x1 = _CX1;
			}

			if(x2 > _CX2) x2 = _CX2;

			off = (y * _LPITCH) + x1;
			temp = (y * _LPITCH) + x2;

			WORD colorkey = g_TexColorKey;
			int alpha = g_TexAlpha;
			int dest_alpha = 31 - alpha;

			WORD texcolor;

			while ( off < temp )	// 각 Triangle 접선에서 픽셀 중복을 막기 위하여 마지막 픽셀을 그리지 않음
			{
				toff = ((Tex.v>>16) * g_TexWidth) + (Tex.u>>16);

				texcolor = *((WORD *)(g_pTexture + toff));
				if(texcolor != colorkey) {
					WORD* dest = (WORD*)(g_pRenderTarget + off);
					*dest = LEMIXW_555(*dest, texcolor, alpha);
				}

				Tex.u += dTex.u;		
				Tex.v += dTex.v;		

				++off;
			}
		}

		void _fastcall HLineTexture_DE(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2)
		{
			int temp;
			register int off;
			UINT toff;
			TEXCOORDi Tex, dTex;

			if(x1 == x2) return;

			if(x1 > x2) {
				SWAPi(x1, x2);
				SWAPi(tex1.u, tex2.u);
				SWAPi(tex1.v, tex2.v);
			}

			Tex.u = tex1.u;
			Tex.v = tex1.v;

			dTex.u = (tex2.u - tex1.u) / (x2 - x1);
			dTex.v = (tex2.v - tex1.v) / (x2 - x1);

			if(x1 < _CX1) {
				Tex.u += dTex.u * (_CX1 - x1);
				Tex.v += dTex.v * (_CX1 - x1);
				x1 = _CX1;
			}

			if(x2 > _CX2) x2 = _CX2;

			off = (y * _LPITCH) + x1;
			temp = (y * _LPITCH) + x2;

			WORD colorkey = g_TexColorKey;
			int alpha = g_TexAlpha;
			int dest_alpha = 31 - alpha;

			WORD texcolor;

			while ( off < temp )	// 각 Triangle 접선에서 픽셀 중복을 막기 위하여 마지막 픽셀을 그리지 않음
			{
				toff = ((Tex.v>>16) * g_TexWidth) + (Tex.u>>16);

				texcolor = *((WORD *)(g_pTexture + toff));
				if(texcolor != colorkey) {
					WORD* dest = (WORD*)(g_pRenderTarget + off);
					*dest = DEMIXW_555(*dest, texcolor, alpha);
				}

				Tex.u += dTex.u;		
				Tex.v += dTex.v;		

				++off;
			}
		}

		void _fastcall HLineTexture_AB(int x1, int y, int x2, TEXCOORDi tex1, TEXCOORDi tex2)
		{
			int temp;
			register int off;
			UINT toff;
			TEXCOORDi Tex, dTex;

			if(x1 == x2) return;

			if(x1 > x2) {
				SWAPi(x1, x2);
				SWAPi(tex1.u, tex2.u);
				SWAPi(tex1.v, tex2.v);
			}

			Tex.u = tex1.u;
			Tex.v = tex1.v;

			dTex.u = (tex2.u - tex1.u) / (x2 - x1);
			dTex.v = (tex2.v - tex1.v) / (x2 - x1);

			if(x1 < _CX1) {
				Tex.u += dTex.u * (_CX1 - x1);
				Tex.v += dTex.v * (_CX1 - x1);
				x1 = _CX1;
			}

			if(x2 > _CX2) x2 = _CX2;

			off = (y * _LPITCH) + x1;
			temp = (y * _LPITCH) + x2;

			WORD colorkey = g_TexColorKey;

			while ( off < temp )	// 각 Triangle 접선에서 픽셀 중복을 막기 위하여 마지막 픽셀을 그리지 않음
			{
				toff = ((Tex.v>>16) * g_TexWidth) + (Tex.u>>16);

				WORD texcolor = *((WORD *)(g_pTexture + toff));
				if(texcolor != colorkey) {
					WORD* dest = (WORD*)(g_pRenderTarget + off);

					int alpha = ABGRADE[g_TexAlpha][*(g_pAlphaTable+toff)];
					int dest_alpha = 31 - alpha;
					if(alpha > 0) {
						if(alpha == 31) *dest = texcolor;
						else *dest = ALPHAMIXW_555(*dest, texcolor, dest_alpha, alpha);
					}
				}

				Tex.u += dTex.u;		
				Tex.v += dTex.v;		

				++off;
			}
		}

		void FillTriTexture(POINT* pTri, TEXCOORDf *pTexCoord)
		{
			POINT Top, Middle, Bottom;
			int xTB=0, xTMB=0;
			int xdTB=0, xdTM=0, xdMB=0;
			int i, TopIndex, MiddleIndex, BottomIndex;
			int y;

			int TopU, TopV;
			int MiddleU, MiddleV; 
			int BottomU, BottomV;

			TEXCOORDi tTB={0,}, tTMB={0,};
			TEXCOORDi tdTB={0,}, tdTM={0,}, tdMB={0,};
			

			// Sorting Triangle Vertex
			Top.x = (int)pTri[0].x;
			Top.y = (int)pTri[0].y;
			TopIndex = 0;
			for ( i = 1; i < 3; i++ )
			{
				if( Top.y > (int)pTri[i].y )
				{
					Top.x = (int)pTri[i].x;
					Top.y = (int)pTri[i].y;
					TopIndex = i;
				}
			}

			Bottom.x = (int)pTri[0].x;
			Bottom.y = (int)pTri[0].y;
			BottomIndex = 0;
			for ( i = 1; i < 3; i++ )
			{
				if( Bottom.y < (int)pTri[i].y )
				{
					Bottom.x = (int)pTri[i].x;
					Bottom.y = (int)pTri[i].y;
					BottomIndex = i;
				}
			}

			MiddleIndex = 3 - (TopIndex + BottomIndex);
			Middle.x = (int)pTri[MiddleIndex].x;
			Middle.y = (int)pTri[MiddleIndex].y;

			// Texture coordinate
			TopU = FLOATROUNDINT(pTexCoord[TopIndex].u * (g_TexWidth-1));
			TopV = FLOATROUNDINT(pTexCoord[TopIndex].v * (g_TexHeight-1));

			MiddleU = FLOATROUNDINT(pTexCoord[MiddleIndex].u * (g_TexWidth-1));
			MiddleV = FLOATROUNDINT(pTexCoord[MiddleIndex].v * (g_TexHeight-1));

			BottomU = FLOATROUNDINT(pTexCoord[BottomIndex].u * (g_TexWidth-1));
			BottomV = FLOATROUNDINT(pTexCoord[BottomIndex].v * (g_TexHeight-1));

			y = Top.y;

			xTB = Top.x<<16;
			xTMB = Top.x<<16;

			tTB.u = TopU<<16;
			tTB.v = TopV<<16;
			tTMB.u = TopU<<16;
			tTMB.v = TopV<<16;

			if(Bottom.y == Top.y) return;
			xdTB = ((Bottom.x - Top.x)<<16) / (Bottom.y - Top.y);
			tdTB.u = ((BottomU - TopU)<<16) / (Bottom.y - Top.y);
			tdTB.v = ((BottomV - TopV)<<16) / (Bottom.y - Top.y);

			if(Middle.y == Top.y) 
			{ 
				xTMB = Middle.x<<16;
				tTMB.u = MiddleU<<16; 
				tTMB.v = MiddleV<<16; 
				goto MIDDLETOBOTTOM; 
			}
			xdTM = ((Middle.x - Top.x)<<16) / (Middle.y - Top.y);
			tdTM.u = ((MiddleU - TopU)<<16) / (Middle.y - Top.y);
			tdTM.v = ((MiddleV - TopV)<<16) / (Middle.y - Top.y);

			if( Middle.y > _CY2 ) Middle.y = _CY2;

			if( Top.y < _CY1 ) 
			{
				xTB += (xdTB * (_CY1 - Top.y));
				xTMB += (xdTM * (_CY1 - Top.y));

				tTB.u += (tdTB.u * (_CY1 - Top.y));
				tTB.v += (tdTB.v * (_CY1 - Top.y));
				tTMB.u += (tdTM.u * (_CY1 - Top.y));
				tTMB.v += (tdTM.v * (_CY1 - Top.y));

				y = _CY1;
			}
			if( Middle. y < _CY1 ) goto MIDDLETOBOTTOM;
			while( y < Middle.y )
			{
				g_pfnHLineTexture((xTB)>>16, y, (xTMB)>>16, tTB, tTMB);

				xTB += xdTB;
				xTMB += xdTM;

				tTB.u += tdTB.u;
				tTB.v += tdTB.v;
				tTMB.u += tdTM.u;
				tTMB.v += tdTM.v;

				++y;
			}

MIDDLETOBOTTOM:	

			if(Bottom.y == Middle.y) return;
			xdMB = ((Bottom.x - Middle.x)<<16) / (Bottom.y - Middle.y);
			tdMB.u = ((BottomU - MiddleU)<<16) / (Bottom.y - Middle.y);
			tdMB.v = ((BottomV - MiddleV)<<16) / (Bottom.y - Middle.y);
			if( Bottom.y > _CY2 ) Bottom.y = _CY2;

			if( Middle.y < _CY1 ) 
			{
				xTB = Top.x <<16;
				tTB.u = TopU <<16;
				tTB.v = TopV <<16;
				if( Middle.y != Top.y ) 
				{
					xTMB = Top.x <<16;
					tTMB.u = TopU <<16;
					tTMB.v = TopV <<16;
				}
				else
				{ 
					xTMB = Middle.x <<16;
					tTMB.u = MiddleU <<16;
					tTMB.v = MiddleV <<16;
				}
				xTB += (xdTB * (_CY1 - Top.y));
				xTMB += (xdTM * (Middle.y - Top.y));
				xTMB += (xdMB * (_CY1 - Middle.y));

				tTB.u += (tdTB.u * (_CY1 - Top.y));
				tTB.v += (tdTB.v * (_CY1 - Top.y));
				tTMB.u += (tdTM.u * (Middle.y - Top.y));
				tTMB.v += (tdTM.v * (Middle.y - Top.y));
				tTMB.u += (tdMB.u * (_CY1 - Middle.y));
				tTMB.v += (tdMB.v * (_CY1 - Middle.y));

				y = _CY1;
			}
			while( y < Bottom.y )
			{
				g_pfnHLineTexture((xTB)>>16, y, (xTMB)>>16, tTB, tTMB);

				xTB += xdTB;
				xTMB += xdMB;

				tTB.u += tdTB.u;
				tTB.v += tdTB.v;
				tTMB.u += tdMB.u;
				tTMB.v += tdMB.v;

				++y;
			}
		}

		void _RotateZoomSpr(int xp,int yp, int txl, int tyl, float angle)
		{
			POINT tri[4];
			TEXCOORDf tex[4];

			tri[0].x = 0;		tri[0].y = 0;
			tri[1].x = txl-1;	tri[1].y = 0;
			tri[2].x = 0;		tri[2].y = tyl-1;
			tri[3].x = txl-1;	tri[3].y = tyl-1;

			for(int i=0; i<4; i++) {
				_RotatePoint(tri+i, angle);
				tri[i].x += xp;
				tri[i].y += yp;
			}

			// 클리핑 테스트
			if((tri[0].x < _CX1 && tri[1].x < _CX1 && tri[2].x < _CX1 && tri[3].x < _CX1) ||
				(tri[0].x > _CX2 && tri[1].x > _CX2 && tri[2].x > _CX2 && tri[3].x > _CX2) ||
				(tri[0].y < _CY1 && tri[1].y < _CY1 && tri[2].y < _CY1 && tri[3].y < _CY1) ||
				(tri[0].y > _CY2 && tri[1].y > _CY2 && tri[2].y > _CY2 && tri[3].y > _CY2))
				return;

			tex[0].u = 0;		tex[0].v = 0;
			tex[1].u = 1.0f;	tex[1].v = 0;
			tex[2].u = 0;		tex[2].v = 1.0f;
			tex[3].u = 1.0f;	tex[3].v = 1.0f;

			FillTriTexture(&tri[0], &tex[0]);
			FillTriTexture(&tri[1], &tex[1]);
		}


	}//namespace GRAPHICGDI

}//namespace NMBASE