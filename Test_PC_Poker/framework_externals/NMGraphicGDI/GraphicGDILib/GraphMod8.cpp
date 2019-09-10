
//
//    ���� ��������Ʈ ���� �Լ�
//

#include "stdafx.h"
#include "GraphMod8.h"

namespace NMBASE
{
	namespace GRAPHICGDI
	{

#define BEGINPUTSPR 1  // ���� ��������Ʈ Ŭ���ο� ���(������ ��½��ۺ� �����)
#define ENDPUTSPR 2    // ���� ��������Ʈ Ŭ���ο� ���(���� ��º� ó�� ��)

		int	ALPHA_STARTVAL = 0; // ���� ã�Ƴ� ���� ������
		int	ALPHA_ENDVAL = 255;

		// ���� ��������Ʈ�� �� ������ �����͸� ���Ѵ�
#define GET_CSPLINEPTR(pCsp, line)  ((pCsp) + (*(long*)((pCsp)+((line)<<2))))
#define MAX_ALPHAGRADE 7  // �ִ� ���� �ܰ�
#define IsAlphaLimit(val) (((val)>=(ALPHA_STARTVAL)&& (val)<=(ALPHA_ENDVAL)) ?1:0)

		typedef struct 
		{
			char id[16];          // ���� ���̺� ���� �νĹ���
			BYTE grade;           // ���Ĵܰ�
			DWORD size;           // �������̺��� ũ��
			char reserved[3];
		} ALPHAHEADER;

		//static BYTE pAlpha[MAX_ALPHAGRADE][256][256]; // 8��Ʈ ���� ���̺�
		static BYTE* pAlpha = NULL; // 8��Ʈ ���� ���̺� ������

		/*---------------------------------------------------------------------------
		�޸� ���� �Լ�(4����Ʈ ��� ���� ����)

		tar : Ÿ�� ������
		val : ä�� ��(����Ʈ��)
		len : ������ ����
		---------------------------------------------------------------------------*/
		void __fastcall MemSet4b(char *tar, char val, int len)
		{
			_asm
			{
				cld;
				mov   ecx, len;
				mov   edi, tar;

				mov   bl,  val; // EAX�� val�� 4����Ʈ ����
				mov   bh,  val;
				mov   ax,  bx;
				shl   eax, 16;
				mov   ax,  bx;

				shr   ecx, 1;
				jnc   next_c2;
				stosb;
next_c2:
				shr   ecx, 1;
				jnc   next_d2;
				stosw;
next_d2:
				rep   stosd;
			}
		}




		/*---------------------------------------------------------------------------
		������� �޸� ���� �Լ�(4����Ʈ ��� ��������)

		tar, sor : Ÿ�ٰ� �ҽ� ������
		len      : ������ ����
		---------------------------------------------------------------------------*/
		void __fastcall MemCpy4b(char *tar, char *sor, int len)
		{
			_asm
			{
				cld;
				mov   ecx, len;
				mov   esi, sor;
				mov   edi, tar;
				shr   ecx, 1;
				jnc   next_a;
				movsb;
next_a:
				shr   ecx, 1;
				jnc   next_b;
				movsw;
next_b:
				rep   movsd;
			}
		}


		/*---------------------------------------------------------------------------
		��-������� �޸� ���� �Լ�(4����Ʈ ���� ��� ����)

		tar, sor : Ÿ�ٰ� �ҽ�������
		len      : ������ ����
		---------------------------------------------------------------------------*/
		void __fastcall RvrsMemCpy4b(char *tar, char *sor, int len)
		{
			_asm
			{
				cld;
				mov   ecx, len;
				mov   esi, sor;   // �ҽ��� ����
				mov   edi, tar;
				add   edi, len;   // Ÿ���� ����

				shr   ecx, 1;
				jnc   next_a;
				lodsb;
				dec   edi;
				mov   [edi],al;

next_a:
				shr   ecx, 1;
				jnc   next_b;
				lodsw;
				xchg  ah, al;     // ����Ʈ�� �����´�(WORD)
				sub   edi,2;
				mov   [edi],ax;
next_b:
				cmp   ecx, 0;     
				je    next_end;   // �̹��� ������ 0�̸� ����
next_c:
				lodsd;
				bswap eax;        // ����Ʈ�� �����´�(Double WORD)
				sub   edi,4;
				mov   [edi],eax;
				loop  next_c;     // ���� ������ŭ �ݺ�

next_end:
			}
		}


		/*---------------------------------------------------------------------------
		����ó�� �޸� �����Լ�(memset�� ����� �Լ��� ��� ���� ����ȿ���� ó���ȴ�)

		tar    : Ÿ�� ������
		val    : ������ ��
		len    : ó���� ����
		pAlpha : �������̺�(256*256�迭�� ������)
		---------------------------------------------------------------------------*/
		void __fastcall GlassSet(char *tar, char val, int len, BYTE* pAlpha)
		{
			_asm
			{
				cld;
				mov   ecx, len;
				mov   edi, tar;
				cmp   ecx, 0;     
				je    next_b;     // �̹��� ������ 0�̸� ����
next_a:
				xor   eax,eax;    // Clear
				mov   al, [edi];
				shl   eax, 8;     // EAX * 256
				mov   al, val;    // EAX + val
				add   eax, pAlpha;
				mov   bl, [eax];
				mov   [edi],bl;
				inc   edi;
				loop  next_a;    // �̹�������ŭ �ݺ�
next_b:
			}
		}


		/*---------------------------------------------------------------------------
		����ó�� ��-�޸� �����Լ�(���� ��Ʈ)

		tar    : Ÿ�� ������
		val    : ������ ��
		len    : ó���� ����
		pAlpha : �������̺�(256*256�迭�� ������)
		---------------------------------------------------------------------------*/
		void __fastcall RvrsGlassSet(char *tar, char val, int len, BYTE* pAlpha)
		{
			_asm
			{
				cld;
				mov   ecx, len;
				mov   edi, tar;
				add   edi, len;   // Ÿ���� ����

				cmp   ecx, 0;     
				je    next_b;     // �̹��� ������ 0�̸� ����
next_a:
				dec   edi;
				xor   eax,eax;    // Clear
				mov   al, [edi];
				shl   eax, 8;     // EAX * 256
				mov   al, val;  // EAX + ESI
				add   eax, pAlpha;
				mov   bl, [eax];
				mov   [edi],bl;
				loop  next_a;    // �̹�������ŭ �ݺ�
next_b:
			}
		}



		/*---------------------------------------------------------------------------
		����ó�� �޸𸮺����Լ�(memcpy�� ����� �Լ��� ��� ���� ����ȿ���� ó���ȴ�)

		tar, sor : ���� ��������Ʈ ������
		len      : ó���� ����
		pAlpha   : �������̺�(256*256�迭�� ������)
		---------------------------------------------------------------------------*/
		void __fastcall GlassCpy(char *tar, char *sor, int len, BYTE* pAlpha)
		{
			_asm
			{
				cld;
				mov   ecx, len;
				mov   esi, sor;
				mov   edi, tar;
				cmp   ecx, 0;     
				je    next_b;     // �̹��� ������ 0�̸� ����
next_a:
				xor   eax,eax;    // Clear
				mov   al, [edi];
				shl   eax, 8;     // EAX * 256
				mov   al, [esi];  // EAX + ESI
				add   eax, pAlpha;
				mov   bl, [eax];
				mov   [edi],bl;
				inc   edi;
				inc   esi;
				loop  next_a;    // �̹�������ŭ �ݺ�
next_b:
			}
		}

		/*---------------------------------------------------------------------------
		����ó�� ��-�޸𸮺����Լ�(���� ����)

		tar, sor : ���� ��������Ʈ ������
		len      : ó���� ����
		pAlpha   : �������̺�(256*256�迭�� ������)
		---------------------------------------------------------------------------*/
		void __fastcall RvrsGlassCpy(char *tar, char *sor, int len, BYTE* pAlpha)
		{
			_asm
			{
				cld;
				mov   ecx, len;
				mov   esi, sor;   // �ҽ��� ����
				mov   edi, tar;
				add   edi, len;   // Ÿ���� ����

				cmp   ecx, 0;     
				je    next_b;     // �̹��� ������ 0�̸� ����
next_a:
				dec   edi;
				xor   eax,eax;    // Clear
				mov   al, [edi];
				shl   eax, 8;     // EAX * 256
				mov   al, [esi];  // EAX + ESI
				add   eax, pAlpha;
				mov   bl, [eax];
				mov   [edi],bl;
				inc   esi;
				loop  next_a;    // �̹�������ŭ �ݺ�
next_b:
			}
		}


		/*-------------------------------------------------------------
		RGB�� HSV�� �ٲ��ش�(�׽�Ʈ��)
		-------------------------------------------------------------*/
		void Rgb2Hsv(BYTE r, BYTE g, BYTE b, BYTE* h, BYTE* s, BYTE* v)
		{
			int major, minor, diff, round;

			major=max(r,max(g,b)); minor=min(r,min(g,b));
			diff = major - minor;
			*h = *s = 0; *v = major;
			if(diff==0 || major==0) return;

			round = diff/2;
			*s = (diff*100+major/2)/major;
			if(major==r) *h = 16+(((int)g-(int)b)*16+round)/diff;
			if(major==g) *h = 48+(((int)b-(int)r)*16+round)/diff;
			if(major==b) *h = 80+(((int)r-(int)g)*16+round)/diff;
		}

		/*-------------------------------------------------------------
		������ ã��
		-------------------------------------------------------------*/
		BYTE NearColor(int r, int g, int b, RGBQUAD* pal) // ������ ã��
		{
			BYTE find;
			int i;
			int temp, distance = 32767;
			int re,ge,be;      // RGB����
			int maxc, minc;    // RGB������ �ִ�,�ּ�
			//BYTE h,s,v,h2,s2,v2; // ���� HSV��
			//int he,se,ve;      // HSV����	
			int mdif = 1000;   // �ٷ����� �ټҰ�

			//Rgb2Hsv((BYTE)r,(BYTE)g,(BYTE)b, &h, &s, &v);

			for(i=0; i<256; i++)
			{
				re = abs(r - pal[i].rgbRed);
				ge = abs(g - pal[i].rgbGreen);
				be = abs(b - pal[i].rgbBlue);

				/*
				Rgb2Hsv((BYTE)pal[i].rgbRed,
				(BYTE)pal[i].rgbGreen,
				(BYTE)pal[i].rgbBlue,
				&h2, &s2, &v2);

				he = abs(h - h2);
				se = abs(s - s2);
				ve = abs(v - v2);
				*/

				maxc = max(re, max(ge,be));             // ������ �ִ� �ּ� ���ϱ�
				minc = min(re, min(ge,be));
				//temp = re + ge + be;                    // �Ϲݽ�
				temp = re + ge + be + abs(maxc - minc); // ������ ��
				//temp = re + ge + be + he + se + ve;     // HSV���� ����ϴ� ������ ��
				//temp = re + ge + be + ve;     // HSV���� ����ϴ� ������ ��

				if(temp < mdif && IsAlphaLimit(i)) // ���� ã�� ���� ������ �����ϸ�
				{
					find = i;
					mdif = temp;
				}
			}
			return find;
		}

		/*-------------------------------------------------------------
		ȥ�ջ� ã��
		al : ȥ�� ����(al�� Ŭ���� c1>c2 �̴�, �ҽ��� �� ��������)
		-------------------------------------------------------------*/
		BYTE FindBlendColor(BYTE c1, BYTE c2, RGBQUAD* pal, BYTE al) // ȥ�ջ� ã��
		{
			int r1,g1,b1,r2,g2,b2;
			int r,g,b;

			r1 = pal[c1].rgbRed; // RGB �и�
			g1 = pal[c1].rgbGreen;
			b1 = pal[c1].rgbBlue;
			r2 = pal[c2].rgbRed;
			g2 = pal[c2].rgbGreen;
			b2 = pal[c2].rgbBlue;

			r = ((r1*al)+(r2*(8-al)))/8;   // ȥ�յ� ���� RGB
			g = ((g1*al)+(g2*(8-al)))/8;
			b = ((b1*al)+(b2*(8-al)))/8;

			return NearColor(r,g,b,pal); // ���� ������ ���� ã�Ƽ� ����
		}


		/*---------------------------------------------------------------------------
		�������̺� ����
		---------------------------------------------------------------------------*/
		void CreateAlphaTable8()
		{
			if(!pAlpha) pAlpha = new BYTE[MAX_ALPHAGRADE*256*256];
		}

		/*---------------------------------------------------------------------------
		�������̺� ����
		---------------------------------------------------------------------------*/
		void ClearAlphaTable8()
		{
			if(pAlpha) delete pAlpha;
			pAlpha = NULL;
		}

		/*---------------------------------------------------------------------------
		�������̺� �ʱ�ȭ (���� ������ ���� ���� �����θ� ���� �� �ִ�)

		pal   : �ķ�Ʈ ���̺� ������
		start : �ķ�Ʈ �˻� ���� ���۹�ȣ
		eld   : �ķ�Ʈ �˻� ���� ����ȣ
		---------------------------------------------------------------------------*/
		void InitAlphaTable8(RGBQUAD* pal, int start, int end)   // ���� ���̺� �ʱ�ȭ
		{
			ALPHA_STARTVAL = start;
			ALPHA_ENDVAL = end;

			if(!pAlpha) CreateAlphaTable8();

			for(int grade=0; grade<MAX_ALPHAGRADE; grade++) {
				for(int i=0; i<256; i++) {
					for(int j=0; j<256; j++) {
						//pAlpha[grade][i][j] = FindBlendColor(i,j,pal,grade); // ȥ�ջ� ã��
						*(pAlpha+(256*256*grade)+(256*i)+j) = FindBlendColor(i,j,pal,grade); // ȥ�ջ� ã��
					}
					// 0�� ���̺��� �ʿ������ �ӵ������ �����(�ٷ� ���� �������) <-- �޸� �����!
				}
			}
		}

		/*---------------------------------------------------------------------------
		�������̺� ����
		---------------------------------------------------------------------------*/
		int SaveAlphaTable8(char* fname)
		{
			FILE* fp;
			ALPHAHEADER ah;

			if(!pAlpha) return -2;

			fp = fopen( fname, "wb");
			if(fp == NULL ) return -1;   // File open error

			memset(&ah, 0, sizeof(ALPHAHEADER));  // Initializing header
			strncpy(ah.id,"Alpha Table",11);
			ah.grade = MAX_ALPHAGRADE;
			ah.size = 256 * 256 * MAX_ALPHAGRADE;

			fwrite(&ah,sizeof(ALPHAHEADER),1,fp); // Write header
			fwrite(pAlpha,MAX_ALPHAGRADE*256*256,1,fp); // Write pAlpha data
			fclose(fp);
			return TRUE;
		}

		/*---------------------------------------------------------------------------
		�������̺� �ε�
		---------------------------------------------------------------------------*/
		int LoadAlphaTable8(char* fname)
		{
			FILE* fp;
			ALPHAHEADER ah;

			fp = fopen( fname, "rb");
			if(fp == NULL ) return -1;                          // File open error

			fread(&ah,sizeof(ALPHAHEADER),1,fp);                // Read header
			if(strncmp(ah.id,"Alpha Table",11) != 0) return -2; // Not a Alpha table

			if(!pAlpha) CreateAlphaTable8();

			fread(pAlpha,MAX_ALPHAGRADE*256*256,1,fp);           // Read data
			fclose(fp);
			return TRUE;
		}


		/*---------------------------------------------------------------------------
		���������� ��������Ʈ�� ����ϴ� �Լ�
		(grade �� Ŭ���� �� ����������)
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprGlass8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			int x=0, y=0, h=0, t=0;  // Ŭ���ε� �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[?] ������ ã�´�.

			if( x || y || h || t)               // Ŭ������ �߻��Ǹ�(�ʰ�ġ�� ������)
			{
				int xLimit = xLen -x - h;       // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���
				int skip = _LPITCH - xLimit;
				int skip2 = x + h;

				tar += (yp+y) * _LPITCH + (xp+x);  // ���� ������ǥ�� �ʰ�ġ�� �����ؼ� �ּҰ��
				sor += y * xLen + x; 

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0�����̸� ����

					// �������̺��� ��ã�� �������� �Ѹ���
					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					inc   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					add   edi,skip;
					add   esi,skip2;
					loop  next_a;
				}

			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				int skip = _LPITCH - xLen;
				tar += yp * _LPITCH + xp;

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0�����̸� ����

					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					inc   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					add   edi,skip;
					loop  next_a2;
				}
			}
		}

		/*------------------ ���������� ���� -----------------------------*/
		void PutImage90d( int xp, int yp, short xLen, short yLen,\
			char *tar, char *sor);
		void PutImage270d( int xp, int yp, short xLen, short yLen,\
			char *tar, char *sor );

		void PutSpr90d( int xp, int yp, short xLen, short yLen,\
			char *tar, char *sor);
		void PutSpr270d( int xp, int yp, short xLen, short yLen,\
			char *tar, char *sor );

		void PutSprMono_90d( int xp, int yp, short xLen, short yLen,\
			char *tar, char *sor, char color);
		void PutSprMono_270d( int xp, int yp, short xLen, short yLen,\
			char *tar, char *sor, char color);

		/*---------------------------------------------------------------------------
		���� ä�� �ڽ� �׸���(Ŭ����)
		---------------------------------------------------------------------------*/
		void FillBox8( int x1, int y1, int x2, int y2, char *tar, char color )
		{
			register int i;
			register int xl;

			if(x1<_CX1) { x1 = _CX1;}
			if(x2>_CX2) { x2 = _CX2;}
			if(y1<_CY1) { y1 = _CY1;}
			if(y2>_CY2) { y2 = _CY2;}

			xl = x2-(x1-1);

			if(xl<=0) return;

			tar += _LPITCH * y1 + x1;

			for(i=y1; i<=y2; i++)
			{
				memset( tar , color, xl);
				tar += _LPITCH;
			}
		}

		/*---------------------------------------------------------------------------
		��������� ä�� �ڽ� �׸���(Ŭ����)
		---------------------------------------------------------------------------*/
		void FillBoxGlass8( int xp, int yp, int xl, int yl, char *tar, char color, int grade)
		{
			register int i;
			int x1=xp,y1=yp;
			int x2=xp+xl-1;
			int y2=yp+yl-1;
			int xLen, yLen;
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(x1<_CX1) { x1 = _CX1;}
			if(x2>_CX2) { x2 = _CX2;}
			if(y1<_CY1) { y1 = _CY1;}
			if(y2>_CY2) { y2 = _CY2;}

			xLen = x2-x1+1;
			yLen = y2-y1+1;

			if(xLen<1 || yLen<1) return;
			if(grade>=MAX_ALPHAGRADE) return;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[grade] ������ ã�´�.

			tar += _LPITCH * y1 + x1;

			for(i=0; i<yLen; i++)
			{
				GlassSet( tar , color, xLen, pAT);
				tar += _LPITCH;
			}
		}

		/*---------------------------------------------------------------------------
		���� �׸���(Ŭ����)
		---------------------------------------------------------------------------*/
		void DrawHLine8( int x, int y, int len, char *tar, char color )
		{
			if(y < _CY1 || y > _CY2 || x+len-1 < _CX1 || x > _CX2) return;

			if(x<_CX1) x = _CX1;
			if(x + len -1 > _CX2) len = _CX2 - x +1;

			tar += _LPITCH * y + x;
			memset(tar , color, len);
		}

		/*---------------------------------------------------------------------------
		������ �׸���(Ŭ����)
		---------------------------------------------------------------------------*/
		void DrawVLine8( int x, int y, int len, char *tar, char color )
		{
			register int i;
			if(x < _CX1 || x > _CX2 || y+len-1 < _CY1 || y > _CY2) return;

			if(y<_CY1) y = _CY1;
			if(y + len -1 > _CY2) len = _CY2 - y +1;

			tar += _LPITCH * y + x;

			for(i=0; i<len; i++)
			{
				*tar = color;
				tar += _LPITCH;
			}
		}

		/*---------------------------------------------------------------------------
		���� �� �ڽ� �׸���(Ŭ����)
		---------------------------------------------------------------------------*/
		void DrawBox8( int x1, int y1, int x2, int y2, char *tar, char color )
		{
			register int i,yp;
			register int xl;
			int x=0,y=0,h=0,t=0;

			//if(x1>_CX2 || x2<_CX1 || y1>_CY2 || y2<_CY1) return;

			if(x1<_CX1) { x1 = _CX1; x=1;}
			if(x2>_CX2) { x2 = _CX2; y=1;}
			if(y1<_CY1) { y1 = _CY1; h=1;}
			if(y2>_CY2) { y2 = _CY2; t=1;}

			xl = x2-x1+1;

			if(!h) memset( tar + (y1<<8) + (y1<<6) + x1 , color, xl);
			if(!t) memset( tar + (y2<<8) + (y2<<6) + x1 , color, xl);

			yp = (y1<<8) + (y1<<6);

			for(i=y1; i<=y2; i++)
			{
				if(!x) *(tar+yp+x1) = color;
				if(!y) *(tar+yp+x2) = color;
				yp+=_LPITCH;
			}
		}


		/*---------------------------------------------------------------------------
		���ǽ��� ������ ������ ü��
		---------------------------------------------------------------------------*/
		void FillPage8( char *tar, char fillColor )
		{
			for(int i=0; i<_MYL; i++) memset(tar+_LPITCH*i, fillColor, _MXL);
		}


		/*---------------------------------------------------------------------------
		���ǽ��� �� ���
		---------------------------------------------------------------------------*/
		void PutPixel8(int x, int y, char *tar, char color )
		{
			*(tar+_LPITCH * y + x)  = color;
		}

		/*---------------------------------------------------------------------------
		Ư�� ���������� �̹����� ����
		---------------------------------------------------------------------------*/
		void GetImage8(int x, int y, int xl, int yl, char *page, char *tar )
		{
			int i;

			page += _LPITCH * y + x;

			for(i=0; i<yl; i++)
			{
				memcpy( tar , page, xl);
				page += _LPITCH;
				tar += xl;
			}
		}


		/*---------------------------------------------------------------------------
		�̹��� ��� �Լ�
		---------------------------------------------------------------------------*/
		void PutImage8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			register int i;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp; 
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;
				int yLimit = yLen -y - t;

				tar += (yp+y) * _LPITCH + (xp+x) ;
				sor += y * xLen + x;

				for(i=0; i<yLimit; i++)
				{
					memcpy( tar , sor, xLimit);
					tar += _LPITCH;
					sor += xLen;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				tar += yp * _LPITCH + xp; 

				for(i=0; i<yLen; i++)
				{
					memcpy( tar , sor, xLen);
					tar += _LPITCH;
					sor += xLen;
				}
			}
		}


		/*--------------------------------------------------------------------------
		�̹��� ��� �Լ�(�¿� ����) : �׳���� ��ƾ�� ���� �ӵ��� ������
		--------------------------------------------------------------------------*/
		void PutImageLR8( int xp, int yp, short xLen, short yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			int rx, ry;              // ���� ���� ��ǥ
			register int i, j;
			register int xLimit, yLimit, skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; }
			else rx = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; sor += y * xLen; ry=_CY1;}
			else ry = yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				xLimit = xLen -x - h;
				yLimit = yLen -y - t;

				tar += ry*_LPITCH + rx + xLimit-1 ; 
				skip = _LPITCH + xLimit;

				for(i=0; i<yLimit; i++)
				{
					sor += h;

					for(j=0; j<xLimit; j++)
					{
						*tar = *sor;
						sor++;
						tar--;
					}
					tar += skip;
					sor += x;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				tar += yp * _LPITCH + xp + xLen-1;  //(== _LPITCH * yp + xp +xLen-1)
				skip = _LPITCH + xLen;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						*tar = *sor;
						sor++; tar--;
					}
					tar += skip;
				}
			}
		}


		/*--------------------------------------------------------------------------
		�̹��� ��� �Լ�(���� ����)
		--------------------------------------------------------------------------*/
		void PutImageUD8( int xp, int yp, short xLen, short yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			int rx, ry;              // ���� ���� ��ǥ
			register int i;
			register int xLimit, yLimit;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����


			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; }
			else rx = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry=_CY1; }
			else ry = yp;


			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{

				xLimit = xLen -x - h;
				yLimit = yLen -y - t;

				tar += ry * _LPITCH + rx;
				sor += xLen * (yLen -1 -y) + x;

				for(i=0; i<yLimit; i++)
				{
					memcpy( tar , sor, xLimit);
					tar += _LPITCH;
					sor -= xLen;
				}
			}
			else                      // Ŭ������ �߻����� �ʾ��� ���
			{
				tar += ry * _LPITCH + rx;
				sor += xLen * (yLen -1);

				for(i=0; i<yLen; i++)
				{
					memcpy( tar , sor, xLen);
					tar += _LPITCH;
					sor -= xLen;
				}
			}
		}



		/*---------------------------------------------------------------------------
		�̹����� ����ϴ� �Լ�(���� �¿� ����) : �׳���� ��ƾ�� ���� �ӵ��� ������
		---------------------------------------------------------------------------*/
		void PutImageUDLR8( int xp, int yp, short xLen, short yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			int rx, ry;              // ���� ���� ��ǥ
			register int i, j;
			register int xLimit, yLimit, skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����


			if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; }
			else rx = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry=_CY1; }
			else ry = yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;


			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				xLimit = xLen -x - h;
				yLimit = yLen -y - t;

				tar += ry * _LPITCH + rx;
				sor += xLen * (yLen -1 -y) + xLen-1;

				skip = _LPITCH - xLimit;
				//skip2 = x+h;

				for(i=0; i<yLimit; i++)
				{
					sor -= x;

					for(j=0; j<xLimit; j++)
					{
						*tar = *sor;
						sor--;
						tar++;
					}
					tar += skip;
					sor -= h;
				}
			}
			else                      // Ŭ������ �߻����� �ʾ��� ���
			{
				skip = _LPITCH - xLen;
				tar += yp * _LPITCH + xp;
				sor += (xLen * yLen)-1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						*tar = *sor;
						sor--; tar++;
					}
					tar += skip;
				}
			}
		}

		/*---------------------------------------------------------------------------
		���� ���� �̹����� ����ϴ� �Լ�(0,90,180,270��)
		---------------------------------------------------------------------------*/
		void PutImageAngle8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, int ang )
		{
			if(ang==0) PutImage8( xp, yp, xLen, yLen, tar, sor );
			else if(ang==1) PutImage90d8( xp, yp, xLen, yLen, tar, sor );
			else if(ang==2) PutImageUDLR8( xp, yp, xLen, yLen, tar, sor );
			else if(ang==3) PutImage270d8( xp, yp, xLen, yLen, tar, sor );
		}


		/*---------------------------------------------------------------------------
		�̹����� ����ϴ� �Լ�(90�� ȸ��)
		---------------------------------------------------------------------------*/
		void PutImage90d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ(ȸ�� ���� �ʰ�ġ��)
			int rx1, ry1, rx2, ry2;  // ȸ�� �Ŀ� ���� ���� ��ǥ
			register int i, j;
			register int skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			rx2 = xp + yLen-1;
			ry2 = yp + xLen-1;

			if( xp < _CX1 ) { x = _CX1 - xp; sor += x * xLen; rx1 = _CX1; }
			else rx1 = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry1=_CY1;}
			else ry1 = yp;
			if( rx2 > _CX2 ) { h = rx2 - _CX2; rx2 = _CX2;}
			if( ry2 > _CY2 ) { t = ry2 - _CY2; ry2 = _CY2;}

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				skip = (xLen -t -y -1) * _LPITCH +1 +_LPITCH;

				tar += ry2 * _LPITCH + rx1;

				for(i=rx1; i<=rx2; i++)
				{
					sor += t;

					for(j=ry1; j<=ry2; j++)
					{
						*tar = *sor;
						sor++;
						tar-=_LPITCH;
					}
					tar += skip;
					sor += y;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				rx1 = xp;
				ry1 = yp+xLen-1;
				skip = (xLen-1) * _LPITCH +1 +_LPITCH;
				tar += ry1 * _LPITCH + rx1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						*tar = *sor;
						sor++; tar-=_LPITCH;
					}
					tar += skip;
				}
			}
		}


		/*---------------------------------------------------------------------------
		�̹����� ����ϴ� �Լ�(270�� ȸ��)
		---------------------------------------------------------------------------*/
		void PutImage270d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ(ȸ�� ���� �ʰ�ġ��)
			int rx1, ry1, rx2, ry2;  // ȸ�� �Ŀ� ���� ���� ��ǥ
			register int i, j;
			register int skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			rx2 = xp + yLen-1;
			ry2 = yp + xLen-1;

			if( xp < _CX1 ) { x = _CX1 - xp; rx1 = _CX1; }
			else rx1 = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry1= _CY1;}
			else ry1 = yp;
			if( rx2 > _CX2 ) { h = rx2 - _CX2; sor += h * xLen; rx2 = _CX2;}
			if( ry2 > _CY2 ) { t = ry2 - _CY2; ry2 = _CY2;}

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				skip = (xLen -t -y -1) * _LPITCH +1 +_LPITCH;

				tar += ry1 * _LPITCH + rx2;

				for(i=rx1; i<=rx2; i++)
				{
					sor += y;

					for(j=ry1; j<=ry2; j++)
					{
						*tar = *sor;
						sor++;
						tar+=_LPITCH;
					}
					tar -= skip;
					sor += t;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				rx1 = xp+yLen-1;
				ry1 = yp;
				skip = (xLen-1) * _LPITCH +1 +_LPITCH;
				tar += ry1 * _LPITCH + rx1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						*tar = *sor;
						sor++; tar+=_LPITCH;
					}
					tar -= skip;
				}
			}
		}

		/*---------------------------------------------------------------------------
		��������Ʈ�� ����ϴ� �Լ�
		---------------------------------------------------------------------------*/
		void PutSpr8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)               // Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
				int skip = _LPITCH - xLimit;
				int skip2 = x + h;

				tar += (yp+y) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += y * xLen + x;

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					lodsb;          // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c;
					mov   [edi],al;

next_c:
					inc   edi;
					loop  next_b;
					pop   ecx;

					add   edi, skip;
					add   esi, skip2;
					loop  next_a;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				int skip = _LPITCH - xLen;
				tar += yp * _LPITCH + xp;

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx, xLen;

next_b2:
					lodsb;             // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c2;
					mov   [edi],al;

next_c2:
					inc   edi;
					loop  next_b2;
					pop   ecx;

					add   edi, skip;
					loop  next_a2;
				}
			}
		}

		/*---------------------------------------------------------------------------
		��������Ʈ�� ����ϴ� �Լ�(�¿� ����)
		---------------------------------------------------------------------------*/
		void PutSprLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;
				int yLimit = yLen -y - t;
				int skip = _LPITCH + xLimit;
				int skip2 = h + x;

				tar += (yp+y) * _LPITCH + (xp+x) + xLimit-1; 
				sor += y * xLen + h;

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					lodsb;          // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c;
					mov   [edi],al;

next_c:
					dec   edi;
					loop  next_b;
					pop   ecx;

					add   edi, skip;
					add   esi, skip2;
					loop  next_a;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				int skip = _LPITCH + xLen;
				tar += yp * _LPITCH + xp + xLen-1;  //(== _LPITCH * yp + xp +xLen-1)

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx, xLen;

next_b2:
					lodsb;             // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c2;
					mov   [edi],al;

next_c2:
					dec   edi;
					loop  next_b2;
					pop   ecx;

					add   edi, skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		��������Ʈ�� ����ϴ� �Լ�(���� ����)
		---------------------------------------------------------------------------*/
		void PutSprUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����
			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;
				int yLimit = yLen -y - t;
				int skip = _LPITCH + xLimit;
				int skip2 = x + h;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x);
				sor += xLen * t + x;

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx, xLimit;

next_b:
					lodsb;          // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c;
					mov   [edi],al;

next_c:
					inc   edi;
					loop  next_b;
					pop   ecx;

					sub   edi, skip;
					add   esi, skip2;
					loop  next_a;
				}
			}
			else                      // Ŭ������ �߻����� �ʾ��� ���
			{
				int skip = _LPITCH + xLen;
				tar += (yp+yLen-1) * _LPITCH + xp;

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx, xLen;

next_b2:
					lodsb;             // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c2;
					mov   [edi],al;

next_c2:
					inc   edi;
					loop  next_b2;
					pop   ecx;

					sub   edi, skip;
					loop  next_a2;
				}
			}
		}



		/*---------------------------------------------------------------------------
		��������Ʈ�� ����ϴ� �Լ�(���� �¿� ����)
		---------------------------------------------------------------------------*/
		void PutSprUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����
			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;
				int yLimit = yLen -y - t;
				int skip = _LPITCH - xLimit;
				int skip2 = x + h;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x) + xLimit-1;
				sor += xLen * t + h;

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx, xLimit;

next_b:
					lodsb;          // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c;
					mov   [edi],al;

next_c:
					dec   edi;
					loop  next_b;
					pop   ecx;

					sub   edi, skip;
					add   esi, skip2;
					loop  next_a;
				}
			}
			else                      // Ŭ������ �߻����� �ʾ��� ���
			{
				int skip = _LPITCH - xLen;
				tar += (yp+yLen-1) * _LPITCH + xp + xLen-1;

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx, xLen;

next_b2:
					lodsb;             // --> mov al, [esi];  inc esi;
					or    al,  al;
					je    next_c2;
					mov   [edi],al;

next_c2:
					dec   edi;
					loop  next_b2;
					pop   ecx;

					sub   edi, skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		��������Ʈ�� ����ϴ� �Լ�(90�� ȸ��)
		---------------------------------------------------------------------------*/
		void PutSpr90d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ(ȸ�� ���� �ʰ�ġ��)
			int rx1, ry1, rx2, ry2;  // ȸ�� �Ŀ� ���� ���� ��ǥ
			register int i, j;
			register int skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			rx2 = xp + yLen-1;
			ry2 = yp + xLen-1;

			if( xp < _CX1 ) { x = _CX1 - xp; sor += x * xLen; rx1 = _CX1; }
			else rx1 = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry1=_CY1;}
			else ry1 = yp;
			if( rx2 > _CX2 ) { h = rx2 - _CX2; rx2 = _CX2;}
			if( ry2 > _CY2 ) { t = ry2 - _CY2; ry2 = _CY2;}

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				skip = (xLen -t -y -1) * _LPITCH +1 +_LPITCH;

				tar += ry2 * _LPITCH + rx1;

				for(i=rx1; i<=rx2; i++)
				{
					sor += t;

					for(j=ry1; j<=ry2; j++)
					{
						if( *sor != 0) *tar = *sor;
						sor++;
						tar-=_LPITCH;
					}
					tar += skip;
					sor += y;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				rx1 = xp;
				ry1 = yp+xLen-1;
				skip = (xLen-1) * _LPITCH +1 +_LPITCH;
				tar += ry1 * _LPITCH + rx1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if( *sor != 0) *tar = *sor;
						sor++; tar-=_LPITCH;
					}
					tar += skip;
				}
			}
		}


		/*---------------------------------------------------------------------------
		��������Ʈ�� ����ϴ� �Լ�(270�� ȸ��)
		---------------------------------------------------------------------------*/
		void PutSpr270d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ(ȸ�� ���� �ʰ�ġ��)
			int rx1, ry1, rx2, ry2;  // ȸ�� �Ŀ� ���� ���� ��ǥ
			register int i, j;
			register int skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			rx2 = xp + yLen-1;
			ry2 = yp + xLen-1;

			if( xp < _CX1 ) { x = _CX1 - xp; rx1 = _CX1; }
			else rx1 = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry1= _CY1;}
			else ry1 = yp;
			if( rx2 > _CX2 ) { h = rx2 - _CX2; sor += h * xLen; rx2 = _CX2;}
			if( ry2 > _CY2 ) { t = ry2 - _CY2; ry2 = _CY2;}

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				skip = (xLen -t -y -1) * _LPITCH +1 +_LPITCH;

				tar += ry1 * _LPITCH + rx2;

				for(i=rx1; i<=rx2; i++)
				{
					sor += y;

					for(j=ry1; j<=ry2; j++)
					{
						if( *sor != 0) *tar = *sor;
						sor++;
						tar+=_LPITCH;
					}
					tar -= skip;
					sor += t;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				rx1 = xp+yLen-1;
				ry1 = yp;
				skip = (xLen-1) * _LPITCH +1 +_LPITCH;
				tar += ry1 * _LPITCH + rx1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if( *sor != 0) *tar = *sor;
						sor++; tar+=_LPITCH;
					}
					tar -= skip;
				}
			}
		}


		/*---------------------------------------------------------------------------
		���� ���� ��������Ʈ�� ����ϴ� �Լ�(0,90,180,270��)
		---------------------------------------------------------------------------*/
		void PutSprAngle8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, int ang )
		{
			if(ang==0) PutSpr8( xp, yp, xLen, yLen, tar, sor );
			else if(ang==1) PutSpr90d8( xp, yp, xLen, yLen, tar, sor );
			else if(ang==2) PutSprUDLR8( xp, yp, xLen, yLen, tar, sor );
			else if(ang==3) PutSpr270d8( xp, yp, xLen, yLen, tar, sor );
		}



		/*---------------------------------------------------------------------------
		�ܻ� ��������Ʈ�� ����ϴ� �Լ�
		---------------------------------------------------------------------------*/
		void PutSprMono8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			int rx, ry;              // ���� ���� ��ǥ
			register int i, j;
			register int xLimit, yLimit, skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; }
			else rx = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; sor += y * xLen; ry=_CY1;}
			else ry = yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				xLimit = xLen -x - h;
				yLimit = yLen -y - t;

				tar += ry * _LPITCH + rx;
				skip = _LPITCH - xLimit;

				for(i=0; i<yLimit; i++)
				{
					sor += x;

					for(j=0; j<xLimit; j++)
					{
						if( *sor != 0) *tar = color;
						sor++;
						tar++;
					}
					tar += skip;
					sor += h;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				skip = _LPITCH - xLen;
				tar += yp * _LPITCH + xp;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if( *sor != 0) *tar = color;
						sor++; tar++;
					}
					tar += skip;
				}
			}

		}

		/*---------------------------------------------------------------------------
		�ܻ����� ��������Ʈ�� ����ϴ� �Լ�(�¿� ����)
		---------------------------------------------------------------------------*/
		void PutSprMonoLR8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			int rx, ry;              // ���� ���� ��ǥ
			register int i, j;
			register int xLimit, yLimit, skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; }
			else rx = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; sor += y * xLen; ry=_CY1;}
			else ry = yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				xLimit = xLen -x - h;
				yLimit = yLen -y - t;

				tar += ry * _LPITCH + rx + xLimit-1 ;  // _LPITCH * ry + xLimit�� ����.
				skip = _LPITCH + xLimit;

				for(i=0; i<yLimit; i++)
				{
					sor += h;

					for(j=0; j<xLimit; j++)
					{
						if(*sor != 0) *tar = color;
						sor++;
						tar--;
					}
					tar += skip;
					sor += x;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				tar += yp * _LPITCH + xp + xLen-1;  //(== _LPITCH * yp + xp +xLen-1)
				skip = _LPITCH + xLen;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if(*sor != 0) *tar = color;
						sor++; tar--;
					}
					tar += skip;
				}
			}
		}


		/*---------------------------------------------------------------------------
		�ܻ����� ��������Ʈ�� ����ϴ� �Լ�(���� ����)
		---------------------------------------------------------------------------*/
		void PutSprMonoUD8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			int rx, ry;              // ���� ���� ��ǥ
			register int i, j;
			register int xLimit, yLimit, skip, skip2;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; }
			else rx = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry=_CY1; }
			else ry = yp;

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				xLimit = xLen -x - h;
				yLimit = yLen -y - t;

				tar += ry * _LPITCH + rx;
				sor += xLen * (yLen -1 -y);

				skip = _LPITCH - xLimit;
				skip2 = xLimit + xLen + x;

				for(i=0; i<yLimit; i++)
				{
					sor += x;

					for(j=0; j<xLimit; j++)
					{
						if( *sor != 0) *tar = color;
						sor++;
						tar++;
					}
					tar += skip;
					sor -= skip2;
				}
			}
			else                      // Ŭ������ �߻����� �ʾ��� ���
			{
				skip = _LPITCH - xLen;
				skip2 = xLen << 1;    // xLen * 2�� ����.
				tar += yp * _LPITCH + xp;
				sor += xLen * (yLen-1);

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if( *sor != 0) *tar = color;
						sor++; tar++;
					}
					tar += skip;
					sor -= skip2; // xLen * 2��ŭ ������ �̵�
				}
			}
		}

		/*---------------------------------------------------------------------------
		�ܻ����� ��������Ʈ�� ����ϴ� �Լ�(���� �¿� ����)
		---------------------------------------------------------------------------*/
		void PutSprMonoUDLR8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			int rx, ry;              // ���� ���� ��ǥ
			register int i, j;
			register int xLimit, yLimit, skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 ) { x = _CX1 - xp; rx = _CX1; }
			else rx = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry=_CY1; }
			else ry = yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				xLimit = xLen -x - h;
				yLimit = yLen -y - t;

				tar += ry * _LPITCH + rx;
				sor += xLen * (yLen -1 -y) + xLen-1;

				skip = _LPITCH - xLimit;
				//skip2 = x+h;

				for(i=0; i<yLimit; i++)
				{
					sor -= x;

					for(j=0; j<xLimit; j++)
					{
						if( *sor != 0) *tar = color;
						sor--;
						tar++;
					}
					tar += skip;
					sor -= h;
				}
			}
			else                      // Ŭ������ �߻����� �ʾ��� ���
			{
				skip = _LPITCH - xLen;
				tar += yp * _LPITCH + xp;
				sor += (xLen * yLen)-1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if( *sor != 0) *tar = color;
						sor--; tar++;
					}
					tar += skip;
				}
			}
		}



		/*---------------------------------------------------------------------------
		�ܻ����� ��������Ʈ�� ����ϴ� �Լ�(90�� ȸ��)
		---------------------------------------------------------------------------*/
		void PutSprMono90d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ(ȸ�� ���� �ʰ�ġ��)
			int rx1, ry1, rx2, ry2;  // ȸ�� �Ŀ� ���� ���� ��ǥ
			register int i, j;
			register int skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			rx2 = xp + yLen-1;
			ry2 = yp + xLen-1;

			if( xp < _CX1 ) { x = _CX1 - xp; sor += x * xLen; rx1 = _CX1; }
			else rx1 = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry1=_CY1;}
			else ry1 = yp;
			if( rx2 > _CX2 ) { h = rx2 - _CX2; rx2 = _CX2;}
			if( ry2 > _CY2 ) { t = ry2 - _CY2; ry2 = _CY2;}

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				skip = (xLen -t -y -1) * _LPITCH +1 +_LPITCH;

				tar += ry2 * _LPITCH + rx1;

				for(i=rx1; i<=rx2; i++)
				{
					sor += t;

					for(j=ry1; j<=ry2; j++)
					{
						if( *sor != 0) *tar = color;
						sor++;
						tar-=_LPITCH;
					}
					tar += skip;
					sor += y;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				rx1 = xp;
				ry1 = yp+xLen-1;
				skip = (xLen-1) * _LPITCH +1 +_LPITCH;
				tar += ry1 * _LPITCH + rx1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if( *sor != 0) *tar = color;
						sor++; tar-=_LPITCH;
					}
					tar += skip;
				}
			}
		}


		/*---------------------------------------------------------------------------
		�ܻ����� ��������Ʈ�� ����ϴ� �Լ�(270�� ȸ��)
		---------------------------------------------------------------------------*/
		void PutSprMono270d8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ(ȸ�� ���� �ʰ�ġ��)
			int rx1, ry1, rx2, ry2;  // ȸ�� �Ŀ� ���� ���� ��ǥ
			register int i, j;
			register int skip;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			rx2 = xp + yLen-1;
			ry2 = yp + xLen-1;

			if( xp < _CX1 ) { x = _CX1 - xp; rx1 = _CX1; }
			else rx1 = xp;
			if( yp < _CY1 ) { y = _CY1 - yp; ry1= _CY1;}
			else ry1 = yp;
			if( rx2 > _CX2 ) { h = rx2 - _CX2; sor += h * xLen; rx2 = _CX2;}
			if( ry2 > _CY2 ) { t = ry2 - _CY2; ry2 = _CY2;}

			if( x || y || h || t)   // Ŭ������ �߻��Ǹ�
			{
				skip = (xLen -t -y -1) * _LPITCH +1 +_LPITCH;

				tar += ry1 * _LPITCH + rx2;

				for(i=rx1; i<=rx2; i++)
				{
					sor += y;

					for(j=ry1; j<=ry2; j++)
					{
						if( *sor != 0) *tar = color;
						sor++;
						tar+=_LPITCH;
					}
					tar -= skip;
					sor += t;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				rx1 = xp+yLen-1;
				ry1 = yp;
				skip = (xLen-1) * _LPITCH +1 +_LPITCH;
				tar += ry1 * _LPITCH + rx1;

				for(i=0; i<yLen; i++)
				{
					for(j=0; j<xLen; j++)
					{
						if( *sor != 0) *tar = color;
						sor++; tar+=_LPITCH;
					}
					tar -= skip;
				}
			}
		}

		/*---------------------------------------------------------------------------
		���� ���� �ܻ����� ��������Ʈ�� ����ϴ� �Լ�(0,90,180,270��)
		---------------------------------------------------------------------------*/
		void PutSprMonoAngle8( int xp, int yp, short xLen, short yLen, char *tar, char *sor, char color, int ang )
		{
			if(ang==0) PutSprMono8( xp, yp, xLen, yLen, tar, sor, color );
			else if(ang==1) PutSprMono90d8( xp, yp, xLen, yLen, tar, sor, color  );
			else if(ang==2) PutSprMonoUDLR8( xp, yp, xLen, yLen, tar, sor, color  );
			else if(ang==3) PutSprMono270d8( xp, yp, xLen, yLen, tar, sor, color  );
		}

		/*---------------------------------------------------------------------------
		���������� ��������Ʈ�� ����ϴ� �Լ�(�¿����)
		(grade �� Ŭ���� �� ����������)
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprGlassLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			int x=0, y=0, h=0, t=0;  // Ŭ���ε� �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[?] ������ ã�´�.

			if( x || y || h || t)               // Ŭ������ �߻��Ǹ�(�ʰ�ġ�� ������)
			{
				int xLimit = xLen -x - h;       // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���
				int skip = _LPITCH + xLimit;
				int skip2 = x + h;

				tar += (yp+y) * _LPITCH + (xp+x) + (xLimit-1);  // ���� ������ǥ�� �ʰ�ġ�� �����ؼ� �ּҰ��
				sor += y * xLen + h; 

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0�����̸� ����

					// �������̺��� ��ã�� �������� �Ѹ���
					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					dec   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					add   edi,skip;
					add   esi,skip2;
					loop  next_a;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				int skip = _LPITCH + xLen;
				tar += yp * _LPITCH + xp + (xLen-1);

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0�����̸� ����

					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					dec   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					add   edi,skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		���������� ��������Ʈ�� ����ϴ� �Լ�(���Ϲ���)
		(grade �� Ŭ���� �� ����������)
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprGlassUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			int x=0, y=0, h=0, t=0;  // Ŭ���ε� �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[?] ������ ã�´�.

			if( x || y || h || t)               // Ŭ������ �߻��Ǹ�(�ʰ�ġ�� ������)
			{
				int xLimit = xLen -x - h;       // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���
				int skip = _LPITCH + xLimit;
				int skip2 = x + h;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x);  // ���� ������ǥ�� �ʰ�ġ�� �����ؼ� �ּҰ��
				sor += t * xLen + x; 

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0�����̸� ����

					// �������̺��� ��ã�� �������� �Ѹ���
					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					inc   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					sub   edi,skip;
					add   esi,skip2;
					loop  next_a;
				}

			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				int skip = _LPITCH + xLen;
				tar += (yp+yLen-1) * _LPITCH + xp;

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0�����̸� ����

					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					inc   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					sub   edi,skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		���������� ��������Ʈ�� ����ϴ� �Լ�(�����¿� ����)
		(grade �� Ŭ���� �� ����������)
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprGlassUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			int x=0, y=0, h=0, t=0;  // Ŭ���ε� �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[?] ������ ã�´�.

			if( x || y || h || t)               // Ŭ������ �߻��Ǹ�(�ʰ�ġ�� ������)
			{
				int xLimit = xLen -x - h;       // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���
				int skip = _LPITCH - xLimit;
				int skip2 = x + h;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x) + (xLimit-1);  // ���� ������ǥ�� �ʰ�ġ�� �����ؼ� �ּҰ��
				sor += t * xLen + h; 

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0�����̸� ����

					// �������̺��� ��ã�� �������� �Ѹ���
					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					dec   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					sub   edi,skip;
					add   esi,skip2;
					loop  next_a;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				int skip = _LPITCH - xLen;
				tar += (yp+yLen-1) * _LPITCH + xp + (xLen-1);

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0�����̸� ����

					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, [esi];             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					dec   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					sub   edi,skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		�ܻ� ���������� ��������Ʈ�� ����ϴ� �Լ�
		- grade : ������ �ܰ�(Ŭ���� �ҽ��� ����������)
		- color : ������ ��
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprMonoGlass8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			int x=0, y=0, h=0, t=0;  // Ŭ���ε� �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[?] ������ ã�´�.

			if( x || y || h || t)               // Ŭ������ �߻��Ǹ�(�ʰ�ġ�� ������)
			{
				int xLimit = xLen -x - h;       // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���
				int skip = _LPITCH - xLimit;
				int skip2 = x + h;

				tar += (yp+y) * _LPITCH + (xp+x);  // ���� ������ǥ�� �ʰ�ġ�� �����ؼ� �ּҰ��
				sor += y * xLen + x; 

				_asm
				{
					//PUSHAD;
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0�����̸� ����

					// �������̺��� ��ã�� �������� �Ѹ���
					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + color
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					inc   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					add   edi,skip;
					add   esi,skip2;
					loop  next_a;
					//POPAD;
				}

			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				int skip = _LPITCH - xLen;
				tar += yp * _LPITCH + xp;

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0�����̸� ����

					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + color
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					inc   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					add   edi,skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		�ܻ� ���������� ��������Ʈ�� ����ϴ� �Լ�(�¿����)
		(grade �� Ŭ���� �� ����������)
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprMonoGlassLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			int x=0, y=0, h=0, t=0;  // Ŭ���ε� �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[?] ������ ã�´�.

			if( x || y || h || t)               // Ŭ������ �߻��Ǹ�(�ʰ�ġ�� ������)
			{
				int xLimit = xLen -x - h;       // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���
				int skip = _LPITCH + xLimit;
				int skip2 = x + h;

				tar += (yp+y) * _LPITCH + (xp+x) + (xLimit-1);  // ���� ������ǥ�� �ʰ�ġ�� �����ؼ� �ּҰ��
				sor += y * xLen + h; 

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0�����̸� ����

					// �������̺��� ��ã�� �������� �Ѹ���
					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					dec   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					add   edi,skip;
					add   esi,skip2;
					loop  next_a;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				int skip = _LPITCH + xLen;
				tar += yp * _LPITCH + xp + (xLen-1);

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0�����̸� ����

					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					dec   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					add   edi,skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		�ܻ� ���������� ��������Ʈ�� ����ϴ� �Լ�(���Ϲ���)
		(grade �� Ŭ���� �� ����������)
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprMonoGlassUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			int x=0, y=0, h=0, t=0;  // Ŭ���ε� �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[?] ������ ã�´�.

			if( x || y || h || t)               // Ŭ������ �߻��Ǹ�(�ʰ�ġ�� ������)
			{
				int xLimit = xLen -x - h;       // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���
				int skip = _LPITCH + xLimit;
				int skip2 = x + h;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x);  // ���� ������ǥ�� �ʰ�ġ�� �����ؼ� �ּҰ��
				sor += t * xLen + x; 

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0�����̸� ����

					// �������̺��� ��ã�� �������� �Ѹ���
					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					inc   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					sub   edi,skip;
					add   esi,skip2;
					loop  next_a;
				}

			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				int skip = _LPITCH + xLen;
				tar += (yp+yLen-1) * _LPITCH + xp;

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0�����̸� ����

					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					inc   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					sub   edi,skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		�ܻ� ���������� ��������Ʈ�� ����ϴ� �Լ�(�����¿� ����)
		(grade �� Ŭ���� �� ����������)
		(1 <= grade <= 7)
		---------------------------------------------------------------------------*/
		void PutSprMonoGlassUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			int x=0, y=0, h=0, t=0;  // Ŭ���ε� �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[?] ������ ã�´�.

			if( x || y || h || t)               // Ŭ������ �߻��Ǹ�(�ʰ�ġ�� ������)
			{
				int xLimit = xLen -x - h;       // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���
				int skip = _LPITCH - xLimit;
				int skip2 = x + h;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x) + (xLimit-1);  // ���� ������ǥ�� �ʰ�ġ�� �����ؼ� �ּҰ��
				sor += t * xLen + h; 

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;

next_a:
					push  ecx;
					mov   ecx,xLimit;

next_b:
					mov   al,[esi];
					or    al,al;
					je    next_c;                // 0�����̸� ����

					// �������̺��� ��ã�� �������� �Ѹ���
					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c:
					dec   edi;
					inc   esi;
					loop  next_b;
					pop   ecx;

					sub   edi,skip;
					add   esi,skip2;
					loop  next_a;
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				int skip = _LPITCH - xLen;
				tar += (yp+yLen-1) * _LPITCH + xp + (xLen-1);

				_asm
				{
					cld;
					mov   ecx, yLen;
					mov   esi, sor;
					mov   edi, tar;

next_a2:
					push  ecx;
					mov   ecx,xLen;

next_b2:
					mov   al,[esi];
					or    al,al;
					je    next_c2;               // 0�����̸� ����

					xor   eax,eax;               // EAX�� 0���� �ʱ�ȭ
					mov   al, [edi];
					shl   eax, 8;                // EAX * 256
					mov   al, color;             // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;

next_c2:
					dec   edi;
					inc   esi;
					loop  next_b2;
					pop   ecx;

					sub   edi,skip;
					loop  next_a2;
				}
			}
		}


		//////////////////////////////////////////////////////////////////////////////////


		/*---------------------------------------------------------------------------
		���� ��������Ʈ�� ����ϴ� �Լ�(�¿����)
		---------------------------------------------------------------------------*/
		void PutCspLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // ���� ��ǥ ���(�ʰ�ġ ����) - ����
				sor += (*(long*)(sor+(y<<2)));  // ��� ������ �������� �̵� - ����

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;
					//tar+=(xLimit-1);     // ������ ������ ������ �̵�
					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // ���� ��Ʈ��
								RvrsMemCpy4b(tar-sbyte-temp+1, sor, temp);
								doing = ENDPUTSPR;
							}
							else RvrsMemCpy4b(tar-sbyte-pbyte+1, sor, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH + xLen;
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y) * _LPITCH + xp + (xLen);   // ���� ��ǥ���(�ѵ�Ʈ �ʰ�)
				sor += (*(long*)(sor+(y<<2)));  // ��� ������ �������� �̵�

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
					xor   eax, eax;

next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;

next_b2:
					push  ecx;
					xor   eax, eax;
					lodsb;            // 0���� ���� �б�
					sub   edi, eax;
					lodsb;            // �̹��� ���� �б�

					movzx   ecx, al;   
					shr   ecx, 1;
					jnc   next_c2;
					lodsb;
					dec   edi;
					mov   [edi],al;

next_c2:
					shr   ecx, 1;
					jnc   next_d2;
					lodsw;
					xchg  ah, al;     // ����Ʈ ������(WORD)
					sub   edi,2;
					mov   [edi],ax;

next_d2:
					cmp   ecx, 0;     
					je    next_f2;     // �̹��� ������ 0�̸� ����
next_e2:
					lodsd;
					bswap eax;         // ����Ʈ ������(Double WORD)
					sub   edi,4;
					mov   [edi],eax;
					loop  next_e2;     // �̹��� ������ŭ �ݺ�

next_f2:
					pop   ecx;
					loop  next_b2;     // ���ϼ���ŭ �ݺ�

					pop   ecx;
					add   edi, skip;
					loop  next_a2;     // Y���̸�ŭ �ݺ�
				}
			}
		}

		/*---------------------------------------------------------------------------
		���� ��������Ʈ�� ����ϴ� �Լ�(���Ϲ���)
		---------------------------------------------------------------------------*/
		void PutCspUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor)
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(t<<2)));    // ��� ������ �������� �̵�

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								memcpy(tar+sbyte, sor, xLimit-sbyte);
								doing = ENDPUTSPR;
							}
							else memcpy(tar+sbyte, sor, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH + xLen;
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y+yLimit-1) * _LPITCH + xp;   // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(t<<2)));  // ��� ������ �������� �̵�

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
					xor   eax, eax;

next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;

next_b2:
					push  ecx;
					lodsb;            // 0���� ���� �б�
					add   edi, eax;
					lodsb;            // �̹��� ���� �б�

					mov   ecx, eax;   
					shr   ecx, 1;
					jnc   next_c2;
					movsb;
next_c2:
					shr   ecx, 1;
					jnc   next_d2;
					movsw;
next_d2:
					rep   movsd;

					pop   ecx;
					loop  next_b2;

					pop   ecx;
					sub   edi, skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		���� ��������Ʈ�� ����ϴ� �Լ�(�����¿����)
		---------------------------------------------------------------------------*/
		void PutCspUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x) +(xLimit);  // ���� ��ǥ ���(�ѵ�Ʈ�ʰ�) - ����
				sor += (*(long*)(sor+(t<<2)));  // ��� ������ �������� �̵� - ����

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;
					//tar+=(xLimit-1);     // ������ ������ ������ �̵�
					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // ���� ��Ʈ��
								RvrsMemCpy4b(tar-sbyte-temp+1, sor, temp);
								doing = ENDPUTSPR;
							}
							else RvrsMemCpy4b(tar-sbyte-pbyte+1, sor, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH - xLen;
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y+yLimit-1) * _LPITCH + xp + (xLen -1);   // ���� ��ǥ���
				sor += (*(long*)(sor+(t<<2)));  // ��� ������ �������� �̵�

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
					xor   eax, eax;

next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;

next_b2:
					push  ecx;
					xor   eax, eax;
					lodsb;            // 0���� ���� �б�
					sub   edi, eax;
					lodsb;            // �̹��� ���� �б�

					movzx   ecx, al;   
					shr   ecx, 1;
					jnc   next_c2;
					lodsb;
					dec   edi;
					mov   [edi],al;

next_c2:
					shr   ecx, 1;
					jnc   next_d2;
					lodsw;
					xchg  ah, al;     // ����Ʈ ������(WORD)
					sub   edi,2;
					mov   [edi],ax;

next_d2:
					cmp   ecx, 0;     
					je    next_f2;     // �̹��� ������ 0�̸� ����
next_e2:
					lodsd;
					bswap eax;         // ����Ʈ ������(Double WORD)
					sub   edi,4;
					mov   [edi],eax;
					loop  next_e2;     // �̹��� ������ŭ �ݺ�

next_f2:
					pop   ecx;
					loop  next_b2;     // ���ϼ���ŭ �ݺ�

					pop   ecx;
					sub   edi, skip;
					loop  next_a2;     // Y���̸�ŭ �ݺ�
				}
			}
		}



		/*---------------------------------------------------------------------------
		���� ��������Ʈ ���(�׽�Ʈ��)
		---------------------------------------------------------------------------*/
		void PutCsp8( int xp, int yp, int xLen, int yLen, char *tar, char *sor )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )               // Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(y<<2)));    // ��� ������ �������� �̵�

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								memcpy(tar+sbyte, sor, xLimit-sbyte);
								doing = ENDPUTSPR;
							}
							else memcpy(tar+sbyte, sor, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else                    // Ŭ������ �߻����� �ʾ��� ���
			{
				int skip = _LPITCH - xLen;
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���
				tar += (yp+y)* _LPITCH + xp;
				sor += (*(long*)(sor+(y<<2)));    // ��� ������ �������� �̵�

				int i,cnt;
				int sbyte,pbyte;

				for(i=0; i<yLimit; i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);
					sor+=2;

					sbyte=0;
					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte += (BYTE)(*sor++);
						pbyte = (BYTE)(*sor++);

						memcpy(tar+sbyte, sor, pbyte);
						//MemCpy4b(tar+sbyte, sor, pbyte);
						//RvrsMemCpy4b(tar+sbyte, sor, pbyte);

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
		}

		/*---------------------------------------------------------------------------
		�ܻ����� ���� ��������Ʈ�� ����ϴ� �Լ�
		---------------------------------------------------------------------------*/
		void PutCspMono8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(y<<2)));    // ��� ������ �������� �̵�

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								memset(tar+sbyte, color, xLimit-sbyte);
								doing = ENDPUTSPR;
							}
							else memset(tar+sbyte, color, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH - xLen;
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y) * _LPITCH + xp;   // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(y<<2)));  // ��� ������ �������� �̵�

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
					xor   eax, eax;

next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;

next_b2:
					push  ecx;
					xor   eax, eax;
					lodsb;            // 0���� ���� �б�
					add   edi, eax;
					lodsb;            // �̹��� ���� �б�
					mov   ecx, eax;

					mov   bl,  color; // EAX�� color�� 4����Ʈ ����
					mov   bh,  color;
					mov   ax,  bx;
					shl   eax, 16;
					mov   ax,  bx;

					add   esi, ecx;   // �ҽ��� �̸� �̵�

					shr   ecx, 1;
					jnc   next_c2;
					stosb;
next_c2:
					shr   ecx, 1;
					jnc   next_d2;
					stosw;
next_d2:
					rep   stosd;

					pop   ecx;
					loop  next_b2;

					pop   ecx;
					add   edi, skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		�ܻ����� ���� ��������Ʈ�� ����ϴ� �Լ�(�¿����)
		---------------------------------------------------------------------------*/
		void PutCspMonoLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color )
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // ���� ��ǥ ���(�ʰ�ġ ����) - ����
				sor += (*(long*)(sor+(y<<2)));  // ��� ������ �������� �̵� - ����

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;
					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // ���� ��Ʈ��
								MemSet4b(tar-sbyte-temp+1, color, temp);
								doing = ENDPUTSPR;
							}
							else MemSet4b(tar-sbyte-pbyte+1, color, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH + xLen;
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y) * _LPITCH + xp + (xLen);   // ���� ��ǥ���(�ѵ�Ʈ �ʰ�)
				sor += (*(long*)(sor+(y<<2)));  // ��� ������ �������� �̵�

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
					xor   eax, eax;

next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;

next_b2:
					cld;
					push  ecx;
					xor   eax, eax;
					lodsb;            // 0���� ���� �б�
					sub   edi, eax;
					lodsb;            // �̹��� ���� �б�

					movzx ecx, al;   

					mov   bl,  color; // EAX�� color�� 4����Ʈ ����
					mov   bh,  color;
					mov   ax,  bx;
					shl   eax, 16;
					mov   ax,  bx;

					//std;
					add   esi,  ecx;   // �ҽ��� �̸� �̵�
					shr   ecx,  1;
					jnc   next_c2;
					dec   edi;
					mov   [edi],al;
					//

next_c2:
					shr   ecx,  1;
					jnc   next_d2;
					sub   edi,  2;
					mov   [edi],ax;

next_d2:

					cmp   ecx,  0;
					je    next_f2;
next_e2:
					sub   edi,4
						mov   [edi], eax;
					loop  next_e2;

next_f2:
					pop   ecx;
					loop  next_b2;     // ���ϼ���ŭ �ݺ�

					add   edi, skip;
					pop   ecx;
					loop  next_a2;     // Y���̸�ŭ �ݺ�
				}
			}
		}

		/*---------------------------------------------------------------------------
		�ܻ����� ���� ��������Ʈ�� ����ϴ� �Լ�(���Ϲ���)
		---------------------------------------------------------------------------*/
		void PutCspMonoUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(t<<2)));    // ��� ������ �������� �̵�

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								memset(tar+sbyte, color, xLimit-sbyte);
								doing = ENDPUTSPR;
							}
							else memset(tar+sbyte, color, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH + xLen;
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y+yLimit-1) * _LPITCH + xp;   // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(t<<2)));  // ��� ������ �������� �̵�

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
					xor   eax, eax;

next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;

next_b2:
					push  ecx;
					xor   eax, eax;
					lodsb;            // 0���� ���� �б�
					add   edi, eax;
					lodsb;            // �̹��� ���� �б�

					mov   ecx, eax;   

					mov   bl,  color; // EAX�� color�� 4����Ʈ ����
					mov   bh,  color;
					mov   ax,  bx;
					shl   eax, 16;
					mov   ax,  bx;

					add   esi, ecx;   // �ҽ��� �̸� �̵�

					shr   ecx, 1;
					jnc   next_c2;
					stosb;
next_c2:
					shr   ecx, 1;
					jnc   next_d2;
					stosw;
next_d2:
					rep   stosd;

					pop   ecx;
					loop  next_b2;

					pop   ecx;
					sub   edi, skip;
					loop  next_a2;
				}
			}
		}


		/*---------------------------------------------------------------------------
		�ܻ����� ���� ��������Ʈ�� ����ϴ� �Լ�(�����¿����)
		---------------------------------------------------------------------------*/
		void PutCspMonoUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;  // �ʰ�ġ

			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x) +(xLimit-1);  // ���� ��ǥ ���(�ʰ�ġ ����) - ����
				sor += (*(long*)(sor+(t<<2)));  // ��� ������ �������� �̵� - ����

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;
					//tar+=(xLimit-1);     // ������ ������ ������ �̵�
					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // ���� ��Ʈ��
								MemSet4b(tar-sbyte-temp+1, color, temp);
								doing = ENDPUTSPR;
							}
							else MemSet4b(tar-sbyte-pbyte+1, color, pbyte);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH - xLen;
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y+yLimit-1) * _LPITCH + xp + (xLen);   // ���� ��ǥ���(�ѵ�Ʈ �ʰ�)
				sor += (*(long*)(sor+(t<<2)));  // ��� ������ �������� �̵�

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
					xor   eax, eax;

next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;

next_b2:
					cld;
					push  ecx;
					xor   eax, eax;
					lodsb;            // 0���� ���� �б�
					sub   edi, eax;
					lodsb;            // �̹��� ���� �б�

					movzx ecx, al;   

					mov   bl,  color; // EAX�� color�� 4����Ʈ ����
					mov   bh,  color;
					mov   ax,  bx;
					shl   eax, 16;
					mov   ax,  bx;

					//std;
					add   esi,  ecx;   // �ҽ��� �̸� �̵�
					shr   ecx,  1;
					jnc   next_c2;
					dec   edi;
					mov   [edi],al;
					//

next_c2:
					shr   ecx,  1;
					jnc   next_d2;
					sub   edi,  2;
					mov   [edi],ax;

next_d2:

					cmp   ecx,  0;
					je    next_f2;
next_e2:
					sub   edi,4
						mov   [edi], eax;
					loop  next_e2;

next_f2:
					pop   ecx;
					loop  next_b2;     // ���ϼ���ŭ �ݺ�

					sub   edi, skip;
					pop   ecx;
					loop  next_a2;     // Y���̸�ŭ �ݺ�

				}
			}
		}



		/*---------------------------------------------------------------------------
		���������� ���� ��������Ʈ�� ���
		---------------------------------------------------------------------------*/
		void PutCspGlass8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[grade] ������ ã�´�.

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(y<<2)));    // ��� ������ �������� �̵�

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								GlassCpy(tar+sbyte, sor, xLimit-sbyte, pAT);
								doing = ENDPUTSPR;
							}
							else GlassCpy(tar+sbyte, sor, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH - xLen;
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y) * _LPITCH + xp;   // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(y<<2)));  // ��� ������ �������� �̵�

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0���� ���� �б�
					add   edi, eax;
					lodsb;            // �̹��� ���� �б�
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // �̹��� ������ 0�̸� ����
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, [esi];  // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					inc   edi;
					inc   esi;
					loop  next_c2;    // �̹�������ŭ �ݺ�
next_d2:	
					pop   ecx;
					loop  next_b2;    // ���ϼ���ŭ �ݺ�
					add   edi, skip;
					pop   ecx;
					loop  next_a2;    // ���μ���ŭ �ݺ�
				}
			}
		}


		/*---------------------------------------------------------------------------
		���������� ���� ��������Ʈ�� ���(�¿����)
		---------------------------------------------------------------------------*/
		void PutCspGlassLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[grade] ������ ã�´�.

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // ���� ��ǥ ���(�ʰ�ġ ����) - ����
				sor += (*(long*)(sor+(y<<2)));  // ��� ������ �������� �̵� - ����

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;
					//tar+=(xLimit-1);     // ������ ������ ������ �̵�
					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // ���� ��Ʈ��
								RvrsGlassCpy(tar-sbyte-temp+1, sor, temp, pAT);
								doing = ENDPUTSPR;
							}
							else RvrsGlassCpy(tar-sbyte-pbyte+1, sor, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH + xLen;
				int xLimit = xLen -x - h;       // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y) * _LPITCH + xp + (xLimit-1);   // ��ǥ���� (����)
				sor += (*(long*)(sor+(y<<2)));  // ��� ������ �������� �̵�

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0���� ���� �б�
					sub   edi, eax;
					lodsb;            // �̹��� ���� �б�
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // �̹��� ������ 0�̸� ����
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, [esi];  // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					dec   edi;
					inc   esi;
					loop  next_c2;    // �̹�������ŭ �ݺ�
next_d2:	
					pop   ecx;
					loop  next_b2;    // ���ϼ���ŭ �ݺ�
					add   edi, skip;
					pop   ecx;
					loop  next_a2;    // ���μ���ŭ �ݺ�
				}
			}
		}


		/*---------------------------------------------------------------------------
		���������� ���� ��������Ʈ�� ���(���Ϲ���)
		---------------------------------------------------------------------------*/
		void PutCspGlassUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[grade] ������ ã�´�.
			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(t<<2)));    // ��� ������ �������� �̵�

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								GlassCpy(tar+sbyte, sor, xLimit-sbyte, pAT);
								doing = ENDPUTSPR;
							}
							else GlassCpy(tar+sbyte, sor, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH + xLen;
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y+yLimit-1) * _LPITCH + xp;   // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(t<<2)));  // ��� ������ �������� �̵�
				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0���� ���� �б�
					add   edi, eax;
					lodsb;            // �̹��� ���� �б�
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // �̹��� ������ 0�̸� ����
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, [esi];  // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					inc   edi;
					inc   esi;
					loop  next_c2;    // �̹�������ŭ �ݺ�
next_d2:	
					pop   ecx;
					loop  next_b2;    // ���ϼ���ŭ �ݺ�
					sub   edi, skip;
					pop   ecx;
					loop  next_a2;    // ���μ���ŭ �ݺ�
				}
			}
		}


		/*---------------------------------------------------------------------------
		���������� ���� ��������Ʈ�� ���(�����¿����)
		---------------------------------------------------------------------------*/
		void PutCspGlassUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[grade] ������ ã�´�.

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x) +(xLimit-1);  // ���� ��ǥ ���(�ʰ�ġ ����) - ����
				sor += (*(long*)(sor+(t<<2)));  // ��� ������ �������� �̵� - ����

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;
					//tar+=(xLimit-1);     // ������ ������ ������ �̵�
					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // ���� ��Ʈ��
								RvrsGlassCpy(tar-sbyte-temp+1, sor, temp, pAT);
								doing = ENDPUTSPR;
							}
							else RvrsGlassCpy(tar-sbyte-pbyte+1, sor, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH - xLen;
				int xLimit = xLen -x - h;       // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y+yLimit-1) * _LPITCH + xp + (xLimit-1);   // ��ǥ���� (����)
				sor += (*(long*)(sor+(t<<2)));  // ��� ������ �������� �̵�

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0���� ���� �б�
					sub   edi, eax;
					lodsb;            // �̹��� ���� �б�
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // �̹��� ������ 0�̸� ����
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, [esi];  // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					dec   edi;
					inc   esi;
					loop  next_c2;    // �̹�������ŭ �ݺ�
next_d2:	
					pop   ecx;
					loop  next_b2;    // ���ϼ���ŭ �ݺ�
					sub   edi, skip;
					pop   ecx;
					loop  next_a2;    // ���μ���ŭ �ݺ�
				}
			}
		}

		/*---------------------------------------------------------------------------
		�ܻ� ���������� ���� ��������Ʈ�� ���
		---------------------------------------------------------------------------*/
		void PutCspMonoGlass8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[grade] ������ ã�´�.

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(y<<2)));    // ��� ������ �������� �̵�

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								GlassSet(tar+sbyte, color, xLimit-sbyte, pAT);
								doing = ENDPUTSPR;
							}
							else GlassSet(tar+sbyte, color, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH - xLen;
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y) * _LPITCH + xp;   // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(y<<2)));  // ��� ������ �������� �̵�

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0���� ���� �б�
					add   edi, eax;
					lodsb;            // �̹��� ���� �б�
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // �̹��� ������ 0�̸� ����
					add   esi, ecx;   // �ҽ��� �̸�����
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, color;  // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					inc   edi;
					//inc   esi;
					loop  next_c2;    // �̹�������ŭ �ݺ�
next_d2:	
					pop   ecx;
					loop  next_b2;    // ���ϼ���ŭ �ݺ�
					add   edi, skip;
					pop   ecx;
					loop  next_a2;    // ���μ���ŭ �ݺ�
				}
			}
		}


		/*---------------------------------------------------------------------------
		�ܻ� ���������� ���� ��������Ʈ�� ���(�¿����)
		---------------------------------------------------------------------------*/
		void PutCspMonoGlassLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[grade] ������ ã�´�.

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y) * _LPITCH + (xp+x) +(xLimit-1);  // ���� ��ǥ ���(�ʰ�ġ ����) - ����
				sor += (*(long*)(sor+(y<<2)));  // ��� ������ �������� �̵� - ����

				for(i=0;i<yLimit;i++,tar+=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;
					//tar+=(xLimit-1);     // ������ ������ ������ �̵�
					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // ���� ��Ʈ��
								RvrsGlassSet(tar-sbyte-temp+1, color, temp, pAT);
								doing = ENDPUTSPR;
							}
							else RvrsGlassSet(tar-sbyte-pbyte+1, color, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH + xLen;
				int xLimit = xLen -x - h;       // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y) * _LPITCH + xp + (xLimit-1);   // ��ǥ���� (����)
				sor += (*(long*)(sor+(y<<2)));  // ��� ������ �������� �̵�

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0���� ���� �б�
					sub   edi, eax;
					lodsb;            // �̹��� ���� �б�
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // �̹��� ������ 0�̸� ����
					add   esi, ecx;   // �ҽ��� �̸� ����
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, color;  // EAX + color
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					dec   edi;
					loop  next_c2;    // �̹�������ŭ �ݺ�
next_d2:	
					pop   ecx;
					loop  next_b2;    // ���ϼ���ŭ �ݺ�
					add   edi, skip;
					pop   ecx;
					loop  next_a2;    // ���μ���ŭ �ݺ�
				}
			}
		}


		/*---------------------------------------------------------------------------
		�ܻ� ���������� ���� ��������Ʈ�� ���(���Ϲ���)
		---------------------------------------------------------------------------*/
		void PutCspMonoGlassUD8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[grade] ������ ã�´�.
			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x); // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(t<<2)));    // ��� ������ �������� �̵�

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;

					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=x) { sbyte-=x; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > x) 
							{ 
								sor += x-sbyte; 
								pbyte = pbyte - (x-sbyte);
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								GlassSet(tar+sbyte, color, xLimit-sbyte, pAT);
								doing = ENDPUTSPR;
							}
							else GlassSet(tar+sbyte, color, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH + xLen;
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y+yLimit-1) * _LPITCH + xp;   // �������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��
				sor += (*(long*)(sor+(t<<2)));  // ��� ������ �������� �̵�
				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0���� ���� �б�
					add   edi, eax;
					lodsb;            // �̹��� ���� �б�
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // �̹��� ������ 0�̸� ����
					add   esi, ecx;   // �ҽ��� �̸� ����
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, color;  // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					inc   edi;
					//inc   esi;
					loop  next_c2;    // �̹�������ŭ �ݺ�
next_d2:	
					pop   ecx;
					loop  next_b2;    // ���ϼ���ŭ �ݺ�
					sub   edi, skip;
					pop   ecx;
					loop  next_a2;    // ���μ���ŭ �ݺ�
				}
			}
		}


		/*---------------------------------------------------------------------------
		�ܻ� ���������� ���� ��������Ʈ�� ���(�����¿����)
		---------------------------------------------------------------------------*/
		void PutCspMonoGlassUDLR8( int xp, int yp, int xLen, int yLen, char *tar, char *sor, char color, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // �ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+xLen <= _CX1 || yp+yLen <= _CY1 ) return;
			// Ŭ���� ���� ������ ������ ������� ����

			if( xp < _CX1 )          x = _CX1 - xp;
			if( yp < _CY1 )          y = _CY1 - yp;
			if( xp + xLen-1 > _CX2 ) h = (xp + xLen-1) - _CX2;
			if( yp + yLen-1 > _CY2 ) t = (yp + yLen-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[grade] ������ ã�´�.

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
			{
				int xLimit = xLen -x - h;           // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;           // Y������ ���� �ҽ��� ��Ʈ�� ���

				int i,cnt,doing;
				int sbyte,pbyte;
				int temp;

				tar += (yp+y+yLimit-1) * _LPITCH + (xp+x) +(xLimit-1);  // ���� ��ǥ ���(�ʰ�ġ ����) - ����
				sor += (*(long*)(sor+(t<<2)));  // ��� ������ �������� �̵� - ����

				for(i=0;i<yLimit;i++,tar-=_LPITCH)
				{
					cnt=(*(short*)sor);  // ���ϼ��� ����
					sor+=2;

					sbyte=0;
					doing=0;
					//tar+=(xLimit-1);     // ������ ������ ������ �̵�
					while(cnt--)         // ���ϼ� ��ŭ �ݺ�
					{
						sbyte+=(*(BYTE*)sor++);
						pbyte=(*(BYTE*)sor++);

						if(doing==0)  // ����� ���۵Ǵ� ���� ���
						{
							if(sbyte>=h) { sbyte-=h; doing=BEGINPUTSPR;}
							else if(sbyte+pbyte > h) 
							{ 
								sor += h-sbyte; 
								pbyte = (pbyte - (h-sbyte));
								sbyte = 0;
								doing = BEGINPUTSPR;
							}
						}

						if(doing==BEGINPUTSPR)   // ����� ��Ȳ�̸�
						{
							if(sbyte>=xLimit) doing = ENDPUTSPR;
							else if(sbyte + pbyte > xLimit)
							{
								temp = xLimit-sbyte; // ���� ��Ʈ��
								RvrsGlassSet(tar-sbyte-temp+1, color, temp, pAT);
								doing = ENDPUTSPR;
							}
							else RvrsGlassSet(tar-sbyte-pbyte+1, color, pbyte, pAT);
						}

						sor+=pbyte;
						sbyte+=pbyte;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���ʰ� �Ʒ��ʸ� �߻��� ���
			{
				int skip = _LPITCH - xLen;
				int xLimit = xLen -x - h;       // X������ ���� �ҽ��� ��Ʈ�� ���
				int yLimit = yLen -y - t;       // Y������ ���� �ҽ��� ��Ʈ�� ���

				tar += (yp+y+yLimit-1) * _LPITCH + xp + (xLimit-1);   // ��ǥ���� (����)
				sor += (*(long*)(sor+(t<<2)));  // ��� ������ �������� �̵�

				_asm
				{
					cld;
					mov   ecx, yLimit;
					mov   esi, sor;
					mov   edi, tar;
next_a2:
					push  ecx;     
					xor   ecx, ecx;   // clear
					mov   cx,  [esi]; // ���ϼ� �б�
					add   esi, 2;
next_b2:
					push  ecx;
					xor   eax, eax;   // clear
					lodsb;            // 0���� ���� �б�
					sub   edi, eax;
					lodsb;            // �̹��� ���� �б�
					mov   ecx, eax;
					cmp   ecx, 0;     
					je    next_d2;    // �̹��� ������ 0�̸� ����
					add   esi, ecx;   // �ҽ��� �̸� ����
next_c2:
					xor   eax,eax;    // Clear
					mov   al, [edi];
					shl   eax, 8;     // EAX * 256
					mov   al, color;  // EAX + ESI
					add   eax, pAT;
					mov   bl, [eax];
					mov   [edi],bl;
					dec   edi;
					//inc   esi;
					loop  next_c2;    // �̹�������ŭ �ݺ�
next_d2:	
					pop   ecx;
					loop  next_b2;    // ���ϼ���ŭ �ݺ�
					sub   edi, skip;
					pop   ecx;
					loop  next_a2;    // ���μ���ŭ �ݺ�
				}
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////



		void LowZoomSpr8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
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
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = *src;
							dest++;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
				sor += y * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = *src;
							dest--;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprUD8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
				sor += t * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL-1)* _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = *src;
							dest++;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void LowZoomSprUDLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
				sor += t * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL -1)* _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = *src;
							dest--;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void LowZoomSprMono8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
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
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = color;
							dest++;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprMonoLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
				sor += y * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = color;
							dest--;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprMonoUD8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
				sor += t * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL-1)* _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = color;
							dest++;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void LowZoomSprMonoUDLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
				sor += t * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL -1)* _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = color;
							dest--;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void LowZoomSprGlass8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[grade] ������ ã�´�.

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
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
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
							//-->if(*src) *dest = (char)pAlpha[grade][*dest][*src];
							dest++;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprGlassLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
				sor += y * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
							dest--;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprGlassUD8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
				sor += t * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL-1)* _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
							dest++;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void LowZoomSprGlassUDLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
				sor += t * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL -1)* _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
							dest--;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void LowZoomSprMonoGlass8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;
			// �������̺��� pAlpha[grade] ������ ã�´�.

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
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
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+(BYTE)color);
							dest++;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprMonoGlassLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
				sor += y * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += yp * _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+(BYTE)color);
							dest--;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}

		void LowZoomSprMonoGlassUD8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
				sor += t * xLen + x;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL-1)* _LPITCH + xp; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+(BYTE)color);
							dest++;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void LowZoomSprMonoGlassUDLR8(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;  // Ÿ���ʰ�ġ
			BYTE *pAT;                 // ���� ���̺��� ������ �����

			int xErt,yErt=0;
			register int xCnt, yCnt;
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL;

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���
				tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

				if(x) x=x*xLen/txl;
				if(y) y=y*yLen/tyl; 
				if(h) h=h*xLen/txl;
				if(t) t=t*yLen/tyl; 

				sorXL = xLen -x - h;         // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -y - t;         // Y������ ���� �ҽ��� ��Ʈ�� ���
				sor += t * xLen + h;
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
				tar += (yp + tarYL -1)* _LPITCH + xp + tarXL-1; 
			}

			yErt=0;
			yCnt = sorYL;
			for(; yCnt; yCnt--)
			{
				yErt += tarYL;
				while(yErt >= sorYL)
				{
					yErt -= sorYL;
					dest = tar;
					src = sor;
					xCnt = sorXL;
					xErt=0;

					for(; xCnt; xCnt--)
					{
						xErt += tarXL;
						while(xErt >= sorXL)
						{
							xErt-=sorXL;
							if(*src) *dest = (char)*(pAT+(((BYTE)*dest)<<8)+(BYTE)color);
							dest--;
						}
						src++;
					}
					tar-=_LPITCH;
				}
				sor+= xLen;
			}
		}


		void ZoomSprNoclip_Test(int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int xErt,yErt;
			register int xCnt, yCnt;
			char *dest, *src;

			tar += _LPITCH * yp + xp;

			yErt = 0;
			yCnt = yLen;
			for(; yCnt; yCnt--)
			{
				yErt += tyl;
				while(yErt >= yLen)
				{
					yErt -= yLen;
					dest = tar;
					src = sor;
					xCnt = xLen;
					xErt = 0;

					for(; xCnt; xCnt--)
					{
						xErt += txl;
						while(xErt >= xLen)
						{
							xErt-=xLen;
							if(*src) *dest = *src;
							dest++;
						}
						src++;
					}
					tar+=_LPITCH;
				}
				sor+= xLen;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////                             /////////////////////////////////
		////////////////////////////  ���ེ������Ʈ ����/�ƿ�   /////////////////////////////////
		////////////////////////////                             /////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////


		void LowZoomCsp8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
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
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}
							//dest += xErt/sorXL;
							//xErt = xErt%sorXL;

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = *src;
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;

							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}
							//dest += xErt/sorXL;
							//xErt = xErt%sorXL;

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = *src;
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = *src;
									//dest--;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = *src;
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspUD8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
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
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = *src;
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = *src;
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}

		void LowZoomCspUDLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor)
		{
			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = *src;
									//dest--;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = *src;
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspMono8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
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
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = color;
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = color;
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspMonoLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = color;
									//dest--;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = color;
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspMonoUD8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
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
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = color;
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = color;
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}

		void LowZoomCspMonoUDLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color)
		{
			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = color;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = color;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspGlass8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ
			BYTE *pAT;                        // ���� ���̺��� ������ �����

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
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
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspGlassLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ
			BYTE *pAT;                        // ���� ���̺��� ������ �����

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
									//dest--;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspGlassUD8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ
			BYTE *pAT;                        // ���� ���̺��� ������ �����

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
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
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}

		void LowZoomCspGlassUDLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ
			BYTE *pAT;                        // ���� ���̺��� ������ �����

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)*src));
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}



		void LowZoomCspMonoGlass8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ
			BYTE *pAT;                        // ���� ���̺��� ������ �����

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
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
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspMonoGlassLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ
			BYTE *pAT;                        // ���� ���̺��� ������ �����

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
									//dest--;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,sy+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
									//dest++;
								}
								src++;
							}
						}
						tar+=_LPITCH;
					}
				}
			}
		}


		void LowZoomCspMonoGlassUD8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ
			BYTE *pAT;                        // ���� ���̺��� ������ �����

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x);  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
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
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest++;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest++ = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
									//dest++;
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}

		void LowZoomCspMonoGlassUDLR8( int xp,int yp, int xLen, int yLen, LPSTR tar, int txl, int tyl, char *sor, char color, int grade)
		{
			// ���� ���̺��� �������� ������ ���
			if(!pAlpha) return;

			int x=0, y=0, h=0, t=0;         // Ÿ���ʰ�ġ
			int sx=0, sy=0, sh=0, st=0;     // �ҽ��ʰ�ġ
			BYTE *pAT;                        // ���� ���̺��� ������ �����

			int xErt,yErt=0;                // X,Y ������
			char *dest, *src;
			int sorXL, sorYL, tarXL, tarYL; // �ҽ��� Ÿ���� ��µ� X,Y����

			if(grade>=MAX_ALPHAGRADE) return;
			if( xp > _CX2 || yp > _CY2 || xp+txl <= _CX1 || yp+tyl <= _CY1 ) return;

			if( xp < _CX1 )         x = _CX1 - xp;
			if( yp < _CY1 )         y = _CY1 - yp;
			if( xp + txl-1 > _CX2 ) h = (xp + txl-1) - _CX2;
			if( yp + tyl-1 > _CY2 ) t = (yp + tyl-1) - _CY2;

			pAT = (BYTE *)pAlpha + (grade<<16); // pAlpha + 256 * 256 * grade;

			if(x||y||t||h)
			{
				tarXL = txl -x - h;           // X������ ���� Ÿ���� ��Ʈ�� ���
				tarYL = tyl -y - t;           // Y������ ���� Ÿ���� ��Ʈ�� ���

				if(x) sx=x*xLen/txl;          // �ҽ��� �ʰ�ġ
				if(y) sy=y*yLen/tyl; 
				if(h) sh=h*xLen/txl;
				if(t) st=t*yLen/tyl; 

				sorXL = xLen -sx - sh;        // X������ ���� �ҽ��� ��Ʈ�� ���
				sorYL = yLen -sy - st;        // Y������ ���� �ҽ��� ��Ʈ�� ���
			}
			else 
			{
				tarXL = txl;
				tarYL = tyl;
				sorXL = xLen;
				sorYL = yLen;
			}

			tar += (yp+y+tarYL-1) * _LPITCH + (xp+x) + tarXL-1;  //�������� ��ǥ�� �ʰ�ġ�� �����Ͽ� �ּҰ��

			if( x || h )         // �¿� Ŭ������ �߻��Ǹ�
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�

							tbyte+=sbyte+pbyte;

							if(first==TRUE)  // ����� ���۵Ǵ� ���� ���
							{
								if(tbyte>=sh) 
								{
									if(tbyte-pbyte >= sh) { sbyte=tbyte-sh-pbyte;}
									else 
									{ 
										sbyte=0; 
										src+=pbyte-(tbyte-sh);
										pbyte=tbyte-sh; 
									}
									first=FALSE;
								}
								else { src+=pbyte; continue;}
							}

							temp = sorXL+sh;
							if(tbyte>=temp)    // ����� �������� ���
							{
								if(tbyte-pbyte >= temp){ pbyte=0; sbyte=0;} //���� �ʿ䰡 �����Ƿ�
								else pbyte = temp-(tbyte-pbyte);
								ptnCnt=0; // ���̻� ������ ���� ���ϰ� �Ѵ�
							}

							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
			else    // Ŭ������ �߻����� �ʾҰų� ���� Ŭ���θ� �߻����� ���
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

						src = GET_CSPLINEPTR(sor,st+i); // ���� �ҽ��� �������� �̵�
						ptnCnt=(*(short*)src);
						src+=2;

						while(ptnCnt--)         // ���ϼ� ��ŭ �ݺ�
						{
							sbyte = (BYTE)(*src++);  // 0������ ���� �б�
							pbyte = (BYTE)(*src++);  // ����Ÿ�� ���� �б�


							xErt += tarXL*sbyte;
							while(xErt >= sorXL)   // 0���� ��ŵ(Ȯ��/����ϸ鼭)
							{
								xErt-=sorXL;
								dest--;
							}

							for(j=0; j<pbyte; j++) // ����Ÿ ���(Ȯ��/����ϸ鼭)
							{
								xErt += tarXL;
								while(xErt >= sorXL)
								{
									xErt-=sorXL;
									*dest-- = (char)*(pAT+(((BYTE)*dest)<<8)+((BYTE)color));
								}
								src++;
							}
						}
						tar-=_LPITCH;
					}
				}
			}
		}

	}//namespace GRAPHICGDI

}//namespace NMBASE

