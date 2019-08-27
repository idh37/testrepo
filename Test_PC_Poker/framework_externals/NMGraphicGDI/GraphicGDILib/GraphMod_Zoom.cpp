
#include "stdafx.h"
#include "GraphMod.h"


namespace NMBASE
{
	namespace GRAPHICGDI
	{

		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////                             /////////////////////////////////
		////////////////////////////    스프라이트 줌인/아웃     /////////////////////////////////
		////////////////////////////                             /////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		void _ZoomSprUD(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				//		tar += ( yp+( tyl - t - 1 ) ) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) sx=x*xLen/txl;
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 
				sor += st * xLen + sx;
				sorXL = xLen -sx - sh;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;         // Y축으로 찍을 소스의 도트수 계산

			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				//	tar += yp * _LPITCH + xp; 
			}
			tar += ( yp+( tyl - t - 1 ) ) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			yErt=0;
			yCnt = sorYL;
			for(; yCnt > 0 ; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
								*dest = *src;
							++dest;
						}
						++src;
					}
					tar -= _LPITCH;
				}
				sor += xLen;
			}
		}
		void _ZoomSprUD_GB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				//		tar += ( yp+( tyl - t - 1 ) ) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) sx=x*xLen/txl;
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 
				sor += st * xLen + sx;
				sorXL = xLen -sx - sh;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;         // Y축으로 찍을 소스의 도트수 계산

			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				//	tar += yp * _LPITCH + xp; 
			}
			tar += ( yp+( tyl - t - 1 ) ) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산
			int alpha = grade;
			int dest_alpha = 31 - alpha;

			yErt=0;
			yCnt = sorYL;
			for(; yCnt > 0 ; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
							{
								*dest = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
							}
							//	*dest = *src;
							++dest;
						}
						++src;
					}
					tar -= _LPITCH;
				}
				sor += xLen;
			}
		}
		void _ZoomSprUD_LE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				//		tar += ( yp+( tyl - t - 1 ) ) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) sx=x*xLen/txl;
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 
				sor += st * xLen + sx;
				sorXL = xLen -sx - sh;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;         // Y축으로 찍을 소스의 도트수 계산

			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				//	tar += yp * _LPITCH + xp; 
			}
			tar += ( yp+( tyl - t - 1 ) ) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			yErt=0;
			yCnt = sorYL;
			for(; yCnt > 0 ; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
							{
								*dest = LEMIXW_555(*dest, *src, grade);
							}
							//	*dest = *src;
							++dest;
						}
						++src;
					}
					tar -= _LPITCH;
				}
				sor += xLen;
			}
		}
		void _ZoomSprUD_DE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				//		tar += ( yp+( tyl - t - 1 ) ) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) sx=x*xLen/txl;
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 
				sor += st * xLen + sx;
				sorXL = xLen -sx - sh;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;         // Y축으로 찍을 소스의 도트수 계산

			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				//	tar += yp * _LPITCH + xp; 
			}
			tar += ( yp+( tyl - t - 1 ) ) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			yErt=0;
			yCnt = sorYL;
			for(; yCnt > 0 ; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
							{
								*dest = DEMIXW_555(*dest, *src, grade);
							}
							//*dest = *src;
							++dest;
						}
						++src;
					}
					tar -= _LPITCH;
				}
				sor += xLen;
			}
		}
		void _ZoomSprUD_AB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, BYTE* alpha_table, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			BYTE* now_alpha = alpha_table;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				//		tar += ( yp+( tyl - t - 1 ) ) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) sx=x*xLen/txl;
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 
				sor += st * xLen + sx;
				now_alpha += st * xLen + sx;
				sorXL = xLen -sx - sh;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;         // Y축으로 찍을 소스의 도트수 계산

			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				//	tar += yp * _LPITCH + xp; 
			}
			tar += ( yp+( tyl - t - 1 ) ) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			yErt=0;
			yCnt = sorYL;
			for(; yCnt > 0 ; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;
					BYTE* palpha = now_alpha;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
							{
								int alpha = ABGRADE[grade][(*palpha)];
								int dest_alpha = 32-alpha;
								if(alpha > 0) 
									*dest = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
								//*dest = *src;
							}
							++dest;
						}
						++palpha;
						++src;
					}
					tar -= _LPITCH;
				}
				sor += xLen;
				now_alpha += xLen;
			}
		}
		void _ZoomSprUD_HB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				//		tar += ( yp+( tyl - t - 1 ) ) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) sx=x*xLen/txl;
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 
				sor += st * xLen + sx;
				sorXL = xLen -sx - sh;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;         // Y축으로 찍을 소스의 도트수 계산

			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				//	tar += yp * _LPITCH + xp; 
			}
			tar += ( yp+( tyl - t - 1 ) ) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			yErt=0;
			yCnt = sorYL;
			for(; yCnt > 0 ; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
							{
								*dest = HBMIXW_555(*dest, *src);
							}
							//*dest = *src;
							++dest;
						}
						++src;
					}
					tar -= _LPITCH;
				}
				sor += xLen;
			}
		}

		void _ZoomSprLR(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x) + ( tarXL-1 );  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp + ( tarXL-1 ); 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt > 0 ; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
								*dest = *src;
							--dest;
						}
						++src;
					}
					tar += _LPITCH;
				}
				sor += xLen;
			}
		}

		void _ZoomSprLR_GB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x) + ( tarXL-1 );  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp + ( tarXL-1 ); 
			}
			int alpha = grade;
			int dest_alpha = 31 - alpha;
			yErt=0;
			yCnt = sorYL;
			for(; yCnt > 0 ; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
							{
								*dest = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
							}
							//	*dest = *src;
							--dest;
						}
						++src;
					}
					tar += _LPITCH;
				}
				sor += xLen;
			}
		}
		void _ZoomSprLR_LE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x) + ( tarXL-1 );  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp + ( tarXL-1 ); 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt > 0 ; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
							{
								*dest = LEMIXW_555(*dest, *src, grade);
							}
							//	*dest = *src;
							--dest;
						}
						++src;
					}
					tar += _LPITCH;
				}
				sor += xLen;
			}
		}
		void _ZoomSprLR_DE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x) + ( tarXL-1 );  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp + ( tarXL-1 ); 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt > 0 ; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
							{
								*dest-- = DEMIXW_555(*dest, *src, grade);
							}
							//	*dest = *src;
						}
						++src;
					}
					tar += _LPITCH;
				}
				sor += xLen;
			}
		}
		void _ZoomSprLR_AB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, BYTE* alpha_table, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;
			BYTE* now_alpha = alpha_table;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x) + ( tarXL-1 );  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + h;
				now_alpha += y * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp + ( tarXL-1 ); 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt > 0 ; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;
					BYTE* palpha = now_alpha;
					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							//if(*src != colorkey) 
							//	*dest = *src;

							if(*src != colorkey) {
								int alpha = ABGRADE[grade][(*palpha)];
								int dest_alpha = 32-alpha;
								if(alpha > 0) 
									*dest = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
							}
							--dest;
						}
						++palpha;
						++src;
					}
					tar += _LPITCH;
				}
				sor += xLen;
				now_alpha += xLen;
			}
		}
		void _ZoomSprLR_HB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x) + ( tarXL-1 );  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp + ( tarXL-1 ); 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt > 0 ; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
							{
								*dest = HBMIXW_555(*dest, *src);
							}
							//	*dest = *src;
							--dest;
						}
						++src;
					}
					tar += _LPITCH;
				}
				sor += xLen;
			}
		}


		////////////////////////////////////////////////////////////////////////////////////
		//  Sprite Zoom In/Out Function
		////////////////////////////////////////////////////////////////////////////////////
		void _ZoomSpr_Mono(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, BYTE* alpha_table, int grade, WORD color)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			BYTE* palpha = NULL;
			for(; yCnt; --yCnt)
			{

				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;
					if( alpha_table )
						palpha = alpha_table + ((sorYL-yCnt-1) * xLen) + x;
					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
							{
								if( alpha_table )
								{
									int alpha = ABGRADE[grade][(*palpha)];
									int dest_alpha = 32-alpha;
									*dest =  ALPHAMIXW_555(*dest , color  , dest_alpha, alpha);
								}
								else
								{
									*dest = color;
								}

							}
							++dest;
						}
						if( alpha_table )
						{
							palpha++;
						}
						++src;
					}
					tar += _LPITCH;
				}
				sor += xLen;
			}
		}
		void _ZoomSpr_Grayscl(int xp, int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, BYTE* alpha_table, BYTE grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			BYTE* palpha = NULL;
			for(; yCnt; --yCnt)
			{

				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;
					if( alpha_table )
						palpha = alpha_table + ((sorYL-yCnt-1) * xLen) + x;
					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;

							if( alpha_table )
							{
								MemCpyGraysclAB(dest, src, 1,palpha,grade);
							}
							else
							{
								MemCpyGrayscl(dest, src, 1);
							}
							++dest;
						}
						if( alpha_table )
						{
							palpha++;
						}
						++src;
					}
					tar += _LPITCH;
				}
				sor += xLen;
			}
		}

		void _ZoomSpr(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
								*dest = *src;
							++dest;
						}
						++src;
					}
					tar += _LPITCH;
				}
				sor += xLen;
			}
		}

		void _ZoomSpr_GB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp; 
			}

			int alpha = grade;
			int dest_alpha = 31 - alpha;

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
								*dest = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
							++dest;
						}
						++src;
					}
					tar += _LPITCH;
				}
				sor += xLen;
			}
		}

		void _ZoomSpr_LE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
								*dest = LEMIXW_555(*dest, *src, grade);
							++dest;
						}
						++src;
					}
					tar += _LPITCH;
				}
				sor += xLen;
			}
		}

		void _ZoomSpr_DE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) 
								*dest++ = DEMIXW_555(*dest, *src, grade);
						}
						++src;
					}
					tar += _LPITCH;
				}
				sor += xLen;
			}
		}

		void _ZoomSpr_AB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, BYTE* alpha_table, int grade)
		{
			int x=0, y=0, h=0, t=0;  // 타겟초과치

			int xErt,yErt=0;
			register int xCnt, yCnt;
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			BYTE* now_alpha = alpha_table;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산
				tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -y - t;         // Y축으로 찍을 소스의 도트수 계산
				sor += y * xLen + x;

				now_alpha += y * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; --yCnt)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt = 0;

					BYTE* palpha = now_alpha;

					for(; xCnt; --xCnt)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt -= sorXL;
							if(*src != colorkey) {
								int alpha = ABGRADE[grade][(*palpha)];
								int dest_alpha = 32-alpha;
								if(alpha > 0) 
									*dest = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
							}
							++dest;
						}
						++palpha;
						++src;
					}
					tar += _LPITCH;
				}
				sor += xLen;
				now_alpha += xLen;
			}
		}


		////////////////////////////////////////////////////////////////////////////////////
		//  Compress Sprite Zoom In/Out Function
		////////////////////////////////////////////////////////////////////////////////////
		void _ZoomCspUD(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += ( yp + ( tyl-t - 1 ) ) * _LPITCH + ( xp + x );  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, st+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sx) {
									if(tbyte-pbyte >= sx) { 
										sbyte = tbyte - sx - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sx);
										pbyte = tbyte - sx; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}

							temp = sorXL+sx;
							if( tbyte >= temp )		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									*dest++ = *src;
									//dest++;
								}
								++src;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				int count = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;
						int linecount = 0;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++linecount;
								++dest;
							}

							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									++linecount;
									xErt -= sorXL;
									*dest++ = *src;
								}
								++src;
							}

						}
						tar -= _LPITCH;
					}
				}
			}
		}
		void _ZoomCspUD_HB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += ( yp + ( tyl-t - 1 ) ) * _LPITCH + ( xp + x );  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, st+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sx) {
									if(tbyte-pbyte >= sx) { 
										sbyte = tbyte - sx - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sx);
										pbyte = tbyte - sx; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}

							temp = sorXL+sx;
							if( tbyte >= temp )		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									//*dest++ = *src;
									*dest++ = HBMIXW_555(*dest, *src);
									//dest++;
								}
								++src;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				int count = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;
						int linecount = 0;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++linecount;
								++dest;
							}

							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									++linecount;
									xErt -= sorXL;
									//*dest++ = *src;
									*dest++ = HBMIXW_555(*dest, *src);
								}
								++src;
							}

						}
						tar -= _LPITCH;
					}
				}
			}
		}
		void _ZoomCspUD_GB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int alpha)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += ( yp + ( tyl-t - 1 ) ) * _LPITCH + ( xp + x );  //실제찍을 좌표를 초과치로 보정하여 주소계산
			int dest_alpha = 32-alpha;
			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, st+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sx) {
									if(tbyte-pbyte >= sx) { 
										sbyte = tbyte - sx - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sx);
										pbyte = tbyte - sx; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}

							temp = sorXL+sx;
							if( tbyte >= temp )		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									//	*dest++ = *src;
									*dest++ = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
									//dest++;
								}
								++src;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				int count = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;
						int linecount = 0;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++linecount;
								++dest;
							}

							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									++linecount;
									xErt -= sorXL;
									//	*dest++ = *src;
									*dest++ = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
								}
								++src;
							}

						}
						tar -= _LPITCH;
					}
				}
			}	
		}
		void _ZoomCspUD_LE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int alpha)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += ( yp + ( tyl-t - 1 ) ) * _LPITCH + ( xp + x );  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, st+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sx) {
									if(tbyte-pbyte >= sx) { 
										sbyte = tbyte - sx - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sx);
										pbyte = tbyte - sx; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}

							temp = sorXL+sx;
							if( tbyte >= temp )		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									//*dest++ = *src;
									*dest++ = LEMIXW_555(*dest, *src, alpha);
									//dest++;
								}
								++src;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				int count = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;
						int linecount = 0;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++linecount;
								++dest;
							}

							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									++linecount;
									xErt -= sorXL;
									//*dest++ = *src;
									*dest++ = LEMIXW_555(*dest, *src, alpha);
								}
								++src;
							}

						}
						tar -= _LPITCH;
					}
				}
			}	
		}
		void _ZoomCspUD_DE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int grade)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += ( yp + ( tyl-t - 1 ) ) * _LPITCH + ( xp + x );  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, st+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sx) {
									if(tbyte-pbyte >= sx) { 
										sbyte = tbyte - sx - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sx);
										pbyte = tbyte - sx; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}

							temp = sorXL+sx;
							if( tbyte >= temp )		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									//*dest++ = *src;
									*dest++ =DEMIXW_555(*dest, *src, grade);
									//dest++;
								}
								++src;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				int count = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;
						int linecount = 0;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++linecount;
								++dest;
							}

							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									++linecount;
									xErt -= sorXL;
									//*dest++ = *src;
									*dest++ =DEMIXW_555(*dest, *src, grade);
								}
								++src;
							}

						}
						tar -= _LPITCH;
					}
				}
			}	
		}
		void _ZoomCspUD_AB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, BYTE* alpha_table, int grade)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += ( yp + ( tyl-t - 1 ) ) * _LPITCH + ( xp + x );  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if(x||y||t||h)       // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, st+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;
						BYTE* palpha = alpha_table + ((xLen * (st+i))) + sx;
						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sx) {
									if(tbyte-pbyte >= sx) { 
										sbyte = tbyte - sx - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sx);
										pbyte = tbyte - sx; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}
							if(!first) 
								palpha += sbyte;
							temp = sorXL+sx;
							if( tbyte >= temp )		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									int alpha = ABGRADE[grade][(*palpha)];
									int dest_alpha = 32-alpha;
									if(alpha > 0) {
										*dest = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);	
									}
									*dest++;
								}
								++palpha;
								++src;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				int count = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,st+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;
						int linecount = 0;
						BYTE* palpha = alpha_table + (xLen * (sy+i));
						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++linecount;
								++dest;	
							}
							palpha+=sbyte;
							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									++linecount;
									xErt -= sorXL;
									//*dest++ = *src;
									int alpha = ABGRADE[grade][(*palpha)];
									int dest_alpha = 32-alpha;
									if(alpha > 0) {
										*dest = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
									}
									*dest++;
								}
								++palpha;
								++src;
							}

						}
						tar -= _LPITCH;
					}
				}
			}
		}

		void _ZoomCspLR(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x) + ( tarXL-1 );  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sh) {
									if(tbyte-pbyte >= sh) { 
										sbyte = tbyte - sh - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sh);
										pbyte = tbyte - sh; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								--dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									*dest-- = *src;
									//dest++;
								}
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								--dest;
							}

							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									*dest-- = *src;
								}
								++src;
							}
						}
						tar += _LPITCH;
					}
				}
			}
		}
		void _ZoomCspLR_HB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x) + ( tarXL-1 );  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sh) {
									if(tbyte-pbyte >= sh) { 
										sbyte = tbyte - sh - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sh);
										pbyte = tbyte - sh; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								--dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									//*dest-- = *src;
									*dest-- = HBMIXW_555(*dest, *src);
									//dest++;
								}
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								--dest;
							}

							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									//*dest-- = *src;
									*dest-- = HBMIXW_555(*dest, *src);
								}
								++src;
							}
						}
						tar += _LPITCH;
					}
				}
			}	
		}
		void _ZoomCspLR_GB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int alpha)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x) + ( tarXL-1 );  //실제찍을 좌표를 초과치로 보정하여 주소계산
			int dest_alpha = 32-alpha;
			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sh) {
									if(tbyte-pbyte >= sh) { 
										sbyte = tbyte - sh - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sh);
										pbyte = tbyte - sh; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								--dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									//*dest-- = *src;
									*dest-- = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
									//dest++;
								}
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								--dest;
							}

							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									//*dest-- = *src;
									*dest-- = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
								}
								++src;
							}
						}
						tar += _LPITCH;
					}
				}
			}	
		}
		void _ZoomCspLR_LE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int alpha)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x) + ( tarXL-1 );  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sh) {
									if(tbyte-pbyte >= sh) { 
										sbyte = tbyte - sh - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sh);
										pbyte = tbyte - sh; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								--dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									//*dest-- = *src;
									*dest-- =LEMIXW_555(*dest, *src, alpha);
									//dest++;
								}
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								--dest;
							}

							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									//*dest-- = *src;
									*dest-- =LEMIXW_555(*dest, *src, alpha);
								}
								++src;
							}
						}
						tar += _LPITCH;
					}
				}
			}	
		}
		void _ZoomCspLR_DE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int grade)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x) + ( tarXL-1 );  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sh) {
									if(tbyte-pbyte >= sh) { 
										sbyte = tbyte - sh - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sh);
										pbyte = tbyte - sh; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								--dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									//*dest-- = *src;
									*dest-- = DEMIXW_555(*dest, *src, grade);
									//dest++;
								}
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								--dest;
							}

							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									*dest-- = DEMIXW_555(*dest, *src, grade);
									//*dest-- = *src;
								}
								++src;
							}
						}
						tar += _LPITCH;
					}
				}
			}	
		}
		void _ZoomCspLR_AB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, BYTE* alpha_table, int grade)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x) + ( tarXL-1 );  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;
						BYTE* palpha = alpha_table + (xLen * (sy+i)) + sh;
						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sh) {
									if(tbyte-pbyte >= sh) { 
										sbyte = tbyte - sh - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sh);
										pbyte = tbyte - sh; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}
							if(!first) 
								palpha += sbyte;
							temp = sorXL+sh;
							if(tbyte>=temp)		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								--dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;

									int alpha = ABGRADE[grade][(*palpha)];
									int dest_alpha = 32-alpha;
									if(alpha > 0) {
										*dest = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
									}
									--dest;
									//*dest-- = *src;
								}
								++palpha;
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;
						BYTE* palpha = alpha_table + (xLen * (sy+i));
						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								--dest;
							}

							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									int alpha = ABGRADE[grade][(*palpha)];
									int dest_alpha = 32-alpha;
									if(alpha > 0) {
										*dest = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
									}
									--dest;
									//*dest-- = *src;
								}
								++palpha;
								++src;
							}
						}
						tar += _LPITCH;
					}
				}
			}
		}

		void _ZoomCsp(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sx) {
									if(tbyte-pbyte >= sx) { 
										sbyte = tbyte - sx - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sx);
										pbyte = tbyte - sx; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									*dest++ = *src;
									//dest++;
								}
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++dest;
							}

							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									*dest++ = *src;
								}
								++src;
							}
						}
						tar += _LPITCH;
					}
				}
			}
		}

		void _ZoomCsp_HB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sx) 
								{
									if(tbyte-pbyte >= sx) { sbyte=tbyte-sx-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sx);
										pbyte=tbyte-sx; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = HBMIXW_555(*dest, *src);
								}
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기


							xErt += tarXL*sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = HBMIXW_555(*dest, *src);
								}
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}

		void _ZoomCsp_GB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int alpha)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			int dest_alpha = 32-alpha;

			tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sx) 
								{
									if(tbyte-pbyte >= sx) { sbyte=tbyte-sx-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sx);
										pbyte=tbyte-sx; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
								}
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기


							xErt += tarXL*sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
								}
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}

		//void _ZoomCsp_LE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor)
		void _ZoomCsp_LE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int alpha)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sx) 
								{
									if(tbyte-pbyte >= sx) { sbyte=tbyte-sx-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sx);
										pbyte=tbyte-sx; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = LEMIXW_555(*dest, *src, alpha);
								}
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기


							xErt += tarXL*sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest = LEMIXW_555(*dest, *src, alpha);
									++dest;
								}
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}

		void _ZoomCsp_DE(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, int grade)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte+=sbyte+pbyte;

							if(first==TRUE)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sx) 
								{
									if(tbyte-pbyte >= sx) { sbyte=tbyte-sx-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sx);
										pbyte=tbyte-sx; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = DEMIXW_555(*dest, *src, grade);
								}
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기


							xErt += tarXL*sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = DEMIXW_555(*dest, *src, grade);
								}
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}

		void _ZoomCsp_AB(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, BYTE* alpha_table, int grade)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt,yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx=x*xLen/txl;          // 소스의 초과치
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산

			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i,j,ptnCnt;
				int sbyte,pbyte;
				int tbyte;
				int first,temp;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						first=TRUE;
						tbyte=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						// 알파테이블 포인터를 찍기 시작할 라인의 X좌표로 이동
						BYTE* palpha = alpha_table + (xLen * (sy+i)) + sx;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte+pbyte;


							if(first==TRUE)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte>=sx) 
								{
									if(tbyte-pbyte >= sx) { 
										sbyte = tbyte-sx-pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte-(tbyte-sx);
										pbyte = tbyte-sx; 
									}
									first=FALSE;
								}
								else { 
									src += pbyte;
									continue;
								}
							}

							if(!first) 
								palpha += sbyte;


							temp = sorXL+sx;
							if(tbyte>=temp)		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									int alpha = ABGRADE[grade][(*palpha)];
									int dest_alpha = 32-alpha;
									if(alpha > 0) {
										*dest = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
									}
									++dest;
								}
								++palpha;
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt=0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt=0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						// 알파테이블 포인터를 찍기 시작할 라인으로 이동
						BYTE* palpha = alpha_table + (xLen * (sy+i));

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							palpha += sbyte;

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt-=sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									int alpha = ABGRADE[grade][(*palpha)];
									int dest_alpha = 32-alpha;
									if(alpha > 0) {
										*dest = ALPHAMIXW_555(*dest, *src, dest_alpha, alpha);
									}
									++dest;
								}
								++palpha;
								++src;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}
		void _ZoomCsp_Mono(int xp,int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, WORD colorkey, BYTE* alpha_table, int grade, WORD color)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산
			BYTE* palpha = NULL;
			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;

				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;
					if( alpha_table )
					{
						palpha = alpha_table + i * xLen + x;
					}
					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sx) {
									if(tbyte-pbyte >= sx) { 
										sbyte = tbyte - sx - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sx);
										pbyte = tbyte - sx; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								if( alpha_table )
								{
									palpha+= sbyte;
								}
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									if( alpha_table )
									{

										int alpha = ABGRADE[grade][(*palpha)];
										int dest_alpha = 32-alpha;
										*dest =  ALPHAMIXW_555(*dest , color  , dest_alpha, alpha);
									}
									else
									{
										*dest = color;
									}
									*dest++;
								}
								++src;
								if( alpha_table )
								{
									++palpha;
								}
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;
						if( alpha_table )
						{
							palpha = alpha_table + (i * xLen) + x;
						}
						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++dest;
							}

							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									if( alpha_table )
									{
										int alpha = *palpha;
										int dest_alpha = 32-alpha;
										*dest =  ALPHAMIXW_555(*dest , color  , dest_alpha, alpha);
									}
									else
									{
										*dest = color;
									}
									*dest++;
								}
								++src;
								if( alpha_table )
								{
									++palpha;
								}
							}
						}
						tar += _LPITCH;
					}
				}
			}
		}
		void _ZoomCsp_Grayscl(int xp, int yp, int xLen, int yLen, WORD *tar, int txl, int tyl, WORD *sor, BYTE* alpha_table,BYTE grade)
		{
			int x=0, y=0, h=0, t=0;         // 타겟초과치
			int sx=0, sy=0, sh=0, st=0;     // 소스초과치

			int xErt, yErt=0;                // X,Y 에러텀
			WORD *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // 소스와 타겟의 출력될 X,Y길이

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X축으로 찍을 타겟의 도트수 계산
				tarYL = tyl -y - t;           // Y축으로 찍을 타겟의 도트수 계산

				if(x) sx = x*xLen/txl;        // 소스의 초과치
				if(y) sy = y*yLen/tyl; 
				if(h) sh = h*xLen/txl;
				if(t) st = t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X축으로 찍을 소스의 도트수 계산
				sorYL = yLen -sy - st;        // Y축으로 찍을 소스의 도트수 계산
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //실제찍을 좌표를 초과치로 보정하여 주소계산
			BYTE* palpha = NULL;
			if( x || h )         // 좌우 클리핑이 발생되면
			{
				int i, j, ptnCnt;
				int sbyte, pbyte;
				int tbyte;
				int first, temp;

				yErt = 0;

				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;
					if( alpha_table )
					{
						palpha = alpha_table + i * xLen + x;
					}
					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						first = TRUE;
						tbyte = 0;

						src = GET_CSPLINEPTR(sor, sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;

						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							tbyte += sbyte + pbyte;

							if(first)				// 출력이 시작되는 곳을 계산
							{
								if(tbyte >= sx) {
									if(tbyte-pbyte >= sx) { 
										sbyte = tbyte - sx - pbyte;
									}
									else { 
										sbyte = 0; 
										src += pbyte - (tbyte - sx);
										pbyte = tbyte - sx; 
									}
									first = FALSE;
								}
								else { 
									src += pbyte; 
									continue;
								}
							}

							temp = sorXL+sx;
							if(tbyte>=temp)		// 출력의 마지막을 계산
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} // 찍을 필요가 없으므로
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0;		// 더이상 루프를 돌지 못하게 한다
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++dest;
							}

							for(j=0; j<pbyte; j++) // 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								if( alpha_table )
								{
									palpha+= sbyte;
								}
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									if( alpha_table )
									{	
										MemCpyGraysclAB(dest, src, 1,palpha,grade);
									}
									else
									{
										MemCpyGrayscl(dest, src, 1);
									}
									*dest++;
								}
								++src;
								if( alpha_table )
								{
									++palpha;
								}
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // 클리핑이 발생되지 않았거나 상하 클리핑만 발생했을 경우
			{

				int i,j,ptnCnt;
				int sbyte,pbyte;

				yErt = 0;
				for(i=0; i<sorYL; i++)
				{
					yErt += tarYL;

					while(yErt >= sorYL)
					{
						yErt -= sorYL;
						dest = tar;
						xErt = 0;

						src = GET_CSPLINEPTR(sor,sy+i); // 찍을 소스의 라인으로 이동
						ptnCnt = *src++;
						if( alpha_table )
						{
							palpha = alpha_table + (i * xLen) + x;
						}
						while(ptnCnt--)					// 패턴수 만큼 반복
						{
							sbyte = *src++;				// 0번색의 개수 읽기
							pbyte = *src++;				// 데이타의 개수 읽기

							xErt += tarXL * sbyte;

							while(xErt >= sorXL)		// 0번색 스킵(확대/축소하면서)
							{
								xErt -= sorXL;
								++dest;
							}

							for(j=0; j<pbyte; ++j)		// 데이타 찍기(확대/축소하면서)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt -= sorXL;
									if( alpha_table )
									{
										MemCpyGraysclAB(dest, src, 1,palpha,grade);
									}
									else
									{
										MemCpyGrayscl(dest, src, 1);
									}
									*dest++;
								}
								++src;
								if( alpha_table )
								{
									++palpha;
								}
							}
						}
						tar += _LPITCH;
					}
				}
			}
		}

	}//namespace GRAPHICGDI

}//namespace NMBASE

