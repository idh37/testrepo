//
//                                   �׷��� ó�� ���
//
#include "StdAfx.h"
#include "Graphic.h"
#include "Jpegfile.h"
#include <math.h>

namespace NMBASE
{
	namespace GRAPHICGDI
	{
		#define TO_RADIAN(x) ((x) * 0.0174773f)
		#define FLOATROUNDINT(x) ( (int)((x)+0.5f) )
		#define FLOATROUND2(x) ( floor(10.0*((x)+0.05))/10.0 )


		#define _swap(a,b) {(a)^=(b); (b)^=(a); (a)^=(b);}

		// ��������Ʈ ��Ʈ �׽�Ʈ
		BOOL PointHitTest(xSprite *pSpr, int sprnum, int x, int y, DWORD opt)
		{
			if(pSpr==NULL) return FALSE;
			if(pSpr->spr==NULL) return FALSE;
			if(sprnum<0 || sprnum >= pSpr->sfhead.TotalSprNum) return FALSE;

			int tx = x;
			int ty = y;

			if(opt & RXY)
			{
				tx -= pSpr->spr[sprnum].rx;
				ty -= pSpr->spr[sprnum].ry;
			}

			if(opt & LR) 
			{
				tx = pSpr->spr[sprnum].xl - tx;
			}

			if(opt & UD) 
			{
				ty = pSpr->spr[sprnum].yl - ty;
			}

			if(pSpr->spr[sprnum].compress) 
				return _PointHitTestCsp(pSpr->spr[sprnum].xl, pSpr->spr[sprnum].yl, pSpr->spr[sprnum].data, tx, ty);
			else 
				return _PointHitTestSpr(pSpr->spr[sprnum].xl, pSpr->spr[sprnum].yl, pSpr->spr[sprnum].data, pSpr->spr[sprnum].colorkey, tx, ty);
		}

		// ���� ������ ��������Ʈ ��Ʈ �׽�Ʈ
		BOOL PointHitTest_Zoom(xSprite *pSpr, int sprnum, int x, int y, int w, int h, DWORD opt)
		{
			if(pSpr == NULL) return FALSE;
			if(pSpr->spr == NULL) return FALSE;
			if(sprnum < 0 || sprnum >= pSpr->sfhead.TotalSprNum) return FALSE;
			if(pSpr->spr[sprnum].xl == 0 || pSpr->spr[sprnum].yl == 0)
				return FALSE;
			if(w == 0 || h == 0)
				return FALSE;

			float tw = ((float)pSpr->spr[sprnum].xl / (float)w);
			float th = ((float)pSpr->spr[sprnum].yl / (float)h);

			int tx = FLOATROUNDINT(x * tw);
			int ty = FLOATROUNDINT(y * th);

			return PointHitTest(pSpr, sprnum, tx, ty, opt);
		}

		// ȸ���� ���� ������ ��������Ʈ ��Ʈ �׽�Ʈ
		BOOL PointHitTest_RotateZoom(xSprite *pSpr, int sprnum, int x, int y, int w, int h, float angle, DWORD opt)
		{
			if(pSpr == NULL) return FALSE;
			if(pSpr->spr == NULL) return FALSE;
			if(sprnum < 0 || sprnum >= pSpr->sfhead.TotalSprNum) return FALSE;
			if(pSpr->spr[sprnum].xl == 0 || pSpr->spr[sprnum].yl == 0)
				return FALSE;
			if(w == 0 || h == 0)
				return FALSE;

			if(angle != 0) {
				POINT rp;
				rp.x = x;
				rp.y = y;

				_RotatePoint(&rp, -angle);

				x = rp.x;
				y = rp.y;
			}

			return PointHitTest_Zoom(pSpr, sprnum, x, y, w, h, opt);
		}


		//////////////////////////////////////////////////


		CPage::CPage()
		{
			lpBmpInfo = NULL;
			lpBit     = NULL;
			lPitch    = 0;
			lPitchHalf= 0;
			nBitNum   = 8;
			nBPD      = 1;
			nColorNum = 0;
			lpLogPal  = NULL;
			hPal      = NULL;
			hBmp      = NULL;

			nAllocSize = 0;

			////////////

			bEnableTempClip = FALSE;

			////////////

			TClipX1 = 0;
			TClipY1 = 0;
			TClipX2 = 0;
			TClipY2 = 0;

			MirrorX = 0;

			////////////

			bAttached = FALSE;

			////////////

			xpFlip = 0;
			ypFlip = 0;
			iZoomX = 1;

			////////////

			bScnEffect = FALSE;

			iDrawingTrackBegin = -1;
			iDrawingTrackEnd = -1;
		}


		CPage::~CPage()
		{
			Destroy();
		}

		void CPage::Destroy()
		{
			if(bAttached) Detach();

			nColorNum = 0;
			nAllocSize = 0;
			bScnEffect = FALSE;

			if(lpBmpInfo != NULL){ delete lpBmpInfo; lpBmpInfo=NULL;}
			//if(lpBit != NULL) { delete lpBit; lpBit=NULL; }
			if(lpLogPal) { free( lpLogPal );  lpLogPal=NULL; }
			if(hPal) { DeleteObject(hPal); hPal=NULL;}
			if(hBmp) { DeleteObject(hBmp); hBmp=NULL;}
		}

		// �ӽ�Ŭ���� ������ �����Ѵ�
		void CPage::SetTempClip(int x1, int y1, int x2, int y2)
		{
			if(x1 > x2) _swap(x1, x2);
			if(y1 > y2) _swap(y1, y2);

			x1 = min(max(x1, 0), width-1);
			x2 = min(max(x2, 0), width-1);
			y1 = min(max(y1, 0), height-1);
			y2 = min(max(y2, 0), height-1);

			TClipX1 = x1;
			TClipY1 = y1;
			TClipX2 = x2;
			TClipY2 = y2;
		}

		// �ӽ� Ŭ���� ������ Ȱ��ȭ ���θ� ����
		void CPage::EnableTempClip(BOOL bEnable)
		{
			bEnableTempClip = bEnable;
		}

		// �̷���� ���� ��ǥ�� �����Ѵ�
		void CPage::SetMirrorX(int x)
		{
			MirrorX = x;
		}

		/*-----------------------------------------------------------------------------------------
		�������� �ʱ�ȭ �Ѵ�
		------------------------------------------------------------------------------------------*/
		BOOL CPage::Init(LONG xl, LONG yl, INT bit, RGBQUAD *lpPal, LPCTSTR palfname)
		{
			if(bAttached) Detach();

			//if(lpBit != NULL) { delete lpBit; lpBit=NULL; }
			if(lpBmpInfo != NULL){ delete lpBmpInfo; lpBmpInfo=NULL;}
			if(hBmp) { DeleteObject(hBmp); hBmp=NULL;}

			switch(bit)
			{
			case 4  : nBPD=1; nColorNum=16;  break;
			case 8  : nBPD=1; nColorNum=256; break;
			case 16 : nBPD=2; nColorNum=0;   break;
			case 24 : nBPD=3; nColorNum=0;   break;
			default : return FALSE;
			}

			nBitNum = bit;
			width = xl;
			lPitch = xl*nBPD;
			height = yl;

			lPitch = (lPitch+3)&~3;	// Pitch�� 4�� ����� �ǰ� �Ѵ�(while(lPitch%4) lPitch++;�İ� ����)
			// ���⼭�� lPitch�� ����Ʈ���̴�(���� ������ �ƴϴ�)
			lPitchHalf = lPitch/nBPD; // lPitch�� ����Ʈ���̹Ƿ� 8,16,23��Ʈ ����� ���� ������ ��´�.

			// ��Ʈ�� ���� ����ü�� ���� �޸� �Ҵ�
			nAllocSize = sizeof(BITMAPINFO) + sizeof(RGBQUAD) * nColorNum;
			//if(nColorNum != 0) nAllocSize -= sizeof(RGBQUAD);
			nAllocSize -= sizeof(RGBQUAD);
			lpBmpInfo = (LPBITMAPINFO)new char[nAllocSize + sizeof(RGBQUAD)];
			if(lpBmpInfo == NULL) return FALSE;

			// ���� ������ ����(��Ʈ�� �̹��� ������)
			//lpBit = (WORD*)new char[lPitch * yl];
			//if(lpBit == NULL) return FALSE;

			// ��Ʈ�� ���� �ش� ����
			lpBmpInfo->bmiHeader.biSize	         = sizeof (BITMAPINFOHEADER);
			lpBmpInfo->bmiHeader.biWidth         = width;      //width;
			lpBmpInfo->bmiHeader.biHeight        = -height;    // ������ �׸��� ������ ��µȴ�
			lpBmpInfo->bmiHeader.biPlanes        = 1;          // �׻�1
			lpBmpInfo->bmiHeader.biBitCount	     = nBitNum;    // ��Ʈ�� ��Ʈ��(16��Ʈ�� 5:6:5)
			lpBmpInfo->bmiHeader.biCompression	 = 0;          // BI_RGB�� ����(�����)
			lpBmpInfo->bmiHeader.biSizeImage     = lPitch*yl;  // lPitch; // �׻� 4�ǹ��
			lpBmpInfo->bmiHeader.biXPelsPerMeter = 0;
			lpBmpInfo->bmiHeader.biYPelsPerMeter = 0;
			lpBmpInfo->bmiHeader.biClrUsed	     = 0;          // ���� �� Į���(�׻� 0���� ����)
			lpBmpInfo->bmiHeader.biClrImportant	 = 0;          // �߿��� �����(�׻� 0���� ����)

			// �ķ�Ʈ ����
			if( nColorNum != 0) {
				if(palfname) OpenPal(palfname);
				else SetPal(lpPal);
			}

			// DIB�� ����
			ReCreateDIBSection();

			// �׷��� ���� �ʱ�ȭ
			InitGrpLib(xl, yl, lPitchHalf, 555);
			InitAlphaTable(555);

			return TRUE;
		}

		BOOL CPage::Attach(LONG xl, LONG yl, void *lpbit, LONG lpitch)
		{
			if(lpBmpInfo) return FALSE;
			if(hBmp) return FALSE;
			if(bAttached) return FALSE;

			nBPD=2; nColorNum=0;
			nBitNum = 16;
			width = xl;
			lPitch = lpitch;
			height = yl;
			//while(lPitch%4) lPitch++; // Pitch�� 4�� ����� �ǰ� �Ѵ�
			lPitchHalf = lPitch/nBPD; // lPitch�� ����Ʈ���̹Ƿ� 8,16,23��Ʈ ����� ���� ������ ��´�.
			lpBmpInfo = NULL;
			lpBit = (WORD*)lpbit;

			InitGrpLib(xl, yl, lPitch/2, 555);

			bAttached = TRUE;
			return TRUE;
		}

		BOOL CPage::Detach()
		{
			if(!bAttached) return FALSE;

			lpBmpInfo = NULL;
			hBmp = NULL;
			lpBit = NULL;
			width = height = 0;
			lPitch = lPitchHalf = 0;
			bAttached = FALSE;

			return TRUE;
		}

		BOOL CPage::ReCreateDIBSection()
		{
			if(hBmp) DeleteObject(hBmp);

			// DIB�� ����
			HWND ActiveWindow = GetActiveWindow();
			HDC ScreenDC = GetDC(ActiveWindow);
			hBmp = CreateDIBSection(ScreenDC, lpBmpInfo, DIB_RGB_COLORS, (VOID**)&lpBit, NULL, 0);
			ReleaseDC(ActiveWindow, ScreenDC);

			return (hBmp != NULL);
		}

		BOOL CPage::OpenPal(LPCTSTR filename)
		{
			if(nColorNum<=0) return FALSE;

			FILE *fp;
			RGBQUAD rgbpal[256];
			if( (fp = fopen( filename, "rb")) == NULL ) return FALSE;

			fread(rgbpal, nColorNum*sizeof(RGBQUAD), 1, fp);
			fclose(fp);

			return SetPal(rgbpal);
		}


		/*-----------------------------------------------------------------------------------------
		�������� ��Ʈ�� ���Ϸ� �����Ѵ�
		------------------------------------------------------------------------------------------*/
		BOOL CPage::SaveToBitmap(LPSTR fname)
		{
			if(!fname) return FALSE;
			if(!lpBit || !lpBmpInfo) return FALSE;
			if(nAllocSize <= 0) return FALSE;
			if(nBitNum != 16) return FALSE;

			FILE *fp = fopen(fname, "wb");
			if(!fp) return FALSE;

			int npitch = width * 3;
			while(npitch%4) npitch++;
			int allocsize = npitch * height;
			BYTE *pimage = new BYTE[allocsize];
			memset(pimage, 0, allocsize);
			BYTE *bit = pimage;

			// 16��Ʈ �̹����� 24��Ʈ�� ��ȯ(�̹����� ������)
			for(int i=height-1; i>=0; i--) {
				for(int j=0; j<width; j++) {
					WORD color = *((WORD*)lpBit + (i * lPitchHalf + j));
					// ����) BGR���̴�
					*(bit+(j*3)) = ((BYTE)B555(color)) << 3;	// B
					*(bit+(j*3)+1) = ((BYTE)G555(color)) << 3;	// G
					*(bit+(j*3)+2) = ((BYTE)R555(color)) << 3;	// R
				}
				bit += npitch;
			}

			BITMAPFILEHEADER	bmfHeader = {0,};
			BITMAPINFOHEADER    bmiHeader = {0,};
			memcpy(&bmiHeader, &lpBmpInfo->bmiHeader, sizeof(BITMAPINFOHEADER));

			bmiHeader.biBitCount = 24;
			bmiHeader.biHeight = height;

			bmfHeader.bfType = ((WORD)('M'<<8 | 'B'));	// bmp���� ǥ����;
			bmfHeader.bfSize = 14 + sizeof(BITMAPINFOHEADER) + allocsize;
			bmfHeader.bfOffBits = 14 + sizeof(BITMAPINFOHEADER);
			fwrite(&bmfHeader, 14, 1, fp);
			fwrite(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
			fwrite(pimage, allocsize, 1, fp);

			fclose(fp);

			if(pimage)
			{
				delete [] pimage;
			}

			return TRUE;
		}

		/*-----------------------------------------------------------------------------------------
		�������� JPG ���Ϸ� �����Ѵ�(quality = 0-100)
		------------------------------------------------------------------------------------------*/
		BOOL CPage::SaveToJpeg(LPSTR fname, int quality)
		{
			if(!fname) return FALSE;
			if(!lpBit || !lpBmpInfo) return FALSE;
			if(nAllocSize <= 0) return FALSE;
			if(nBitNum != 16) return FALSE;

			int npitch = width * 3;
			//while(npitch%4) npitch++;	// 4����Ʈ �����ڵ�
			int allocsize = npitch * height;
			BYTE *pimage = new BYTE[allocsize];
			memset(pimage, 0, allocsize);

			BYTE *bit = pimage;

			// 16��Ʈ �̹����� 24��Ʈ�� ��ȯ(�̹����� ������ �ʾƾ��Ѵ�)
			for(int i=0; i<height; i++) {
				for(int j=0; j<width; j++) {
					WORD color = *((WORD*)lpBit + (i * lPitchHalf + j));
					// RGB
					*(bit+(j*3)) = ((BYTE)R555(color)) << 3;	// R
					*(bit+(j*3)+1) = ((BYTE)G555(color)) << 3;	// G
					*(bit+(j*3)+2) = ((BYTE)B555(color)) << 3;	// B
				}
				bit += npitch;
			}

			BOOL bSuccess = JpegFile::RGBToJpegFile(fname,		// path
				pimage,		// RGB buffer
				width,		// pixels
				height,		// rows
				TRUE,		// TRUE = RGB, FALSE = Grayscale
				quality);	// 0 - 100

			if(pimage)
			{
				delete [] pimage;
			}

			return bSuccess;
		}

		/*-----------------------------------------------------------------------------------------
		�������� �ķ�Ʈ�� �����Ѵ�

		- �μ� : RGBQUAD�� ������
		------------------------------------------------------------------------------------------*/
		BOOL CPage::SetPal(RGBQUAD *lpPal)
		{
			if( nColorNum == 0)	
				return TRUE;        //�ķ�Ʈ�� �ʿ���ٸ�

			if(lpLogPal) 
			{
				free( lpLogPal );	// �޸� ����
				lpLogPal=NULL;
			} 

			lpLogPal = (LPLOGPALETTE)malloc( sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColorNum );

			if(NULL == lpLogPal)
				return FALSE;

			lpLogPal->palVersion    = 0x300;	    // �ȷ�Ʈ ���� �Է�
			lpLogPal->palNumEntries = nColorNum;    // �ȷ�Ʈ ����� �Է�

			int i;
			if(NULL == lpPal)	// �ķ�Ʈ �����Ͱ� NULL�̸� �⺻ �ķ�Ʈ�� �����Ѵ�
			{             
				for(i=0; i<nColorNum; i++)
				{
					lpLogPal->palPalEntry[i].peRed   = lpBmpInfo->bmiColors[i].rgbRed      = (BYTE)(((i>>5) & 0x07) * 255/7);
					lpLogPal->palPalEntry[i].peGreen = lpBmpInfo->bmiColors[i].rgbGreen    = (BYTE)(((i>>2) & 0x07) * 255/7);
					lpLogPal->palPalEntry[i].peBlue  = lpBmpInfo->bmiColors[i].rgbBlue     = (BYTE)(((i>>0) & 0x03) * 255/3);
					lpLogPal->palPalEntry[i].peFlags = lpBmpInfo->bmiColors[i].rgbReserved = (BYTE)0;
				}
			}
			else
			{
				for(i=0; i<nColorNum; i++)
				{
					lpLogPal->palPalEntry[i].peRed   = lpBmpInfo->bmiColors[i].rgbRed   = lpPal[i].rgbRed;
					lpLogPal->palPalEntry[i].peGreen = lpBmpInfo->bmiColors[i].rgbGreen = lpPal[i].rgbGreen;
					lpLogPal->palPalEntry[i].peBlue  = lpBmpInfo->bmiColors[i].rgbBlue  = lpPal[i].rgbBlue;
					lpLogPal->palPalEntry[i].peFlags = lpBmpInfo->bmiColors[i].rgbReserved = 0;
				}
			}

			if(hPal) 
			{
				DeleteObject(hPal); 
				hPal=NULL;
			}

			hPal = CreatePalette( lpLogPal );		

			if(hPal == NULL)
				return FALSE;

			return TRUE;
		}

		BOOL CPage::SetLogPal(LPLOGPALETTE lpPal)
		{
			if( nColorNum == 0)	//�ķ�Ʈ�� �ʿ���ٸ�
				return TRUE;

			if(lpLogPal!=NULL) { free( lpLogPal );  lpLogPal=NULL; } // �޸� ����

			LONG size = sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColorNum;
			lpLogPal = (LPLOGPALETTE)malloc( size );

			if(NULL == lpLogPal)
				return FALSE;

			memcpy(lpLogPal, lpPal, size);

			if(hPal) { DeleteObject(hPal); hPal=NULL;}
			hPal = CreatePalette( lpLogPal );		

			if(hPal == NULL) return FALSE;
			return TRUE;
		}


		BOOL CPage::SetPalHandle(HPALETTE h)
		{
			if(h==NULL) return FALSE;
			if(hPal) { DeleteObject(hPal); hPal=NULL;}
			hPal = h;
			return TRUE;
		}

		/*-----------------------------------------------------------------------------------------
		GDI ǥ���� Ư�� Į��� ä���
		------------------------------------------------------------------------------------------*/
		BOOL CPage::FillGDI(HDC hTarDC, int nXStart, int nYStart, COLORREF crColor)
		{
			BOOL check;
			check = FloodFill(hTarDC, nXStart, nYStart, crColor);
			if(check != 0)return TRUE;
			else return FALSE;
		}

		/*-----------------------------------------------------------------------------------------
		�ٸ� �������� ������ ���� ������ ǥ�鿡 �Ѹ���
		------------------------------------------------------------------------------------------*/
		BOOL CPage::PutFromPage(LONG xp, LONG yp, CPage *pPage, int blendgrade)
		{
			int w = pPage->width;
			int h = pPage->height;
			if(lpBit == NULL) return FALSE;
			if(pPage->lpBit == NULL) return FALSE;
			if(w==0 || h==0) return FALSE;

			InitGrpLib(width, height, lPitchHalf, 555);
			if(blendgrade >= 31) _PutImage(xp, yp, w, h, (WORD*)lpBit, (WORD*)pPage->lpBit);
			else _PutImage_GB(xp, yp, w, h, (WORD*)lpBit, (WORD*)pPage->lpBit, blendgrade);

			return TRUE;
		}

		/*-----------------------------------------------------------------------------------------
		�ٸ� �������� ������ �÷�Ű�� �����Ͽ� ���� ������ ǥ�鿡 �Ѹ���
		------------------------------------------------------------------------------------------*/
		BOOL CPage::PutFromPageWithColorKey(LONG xp, LONG yp, CPage *pPage, WORD colorkey, int blendgrade)
		{
			int w = pPage->width;
			int h = pPage->height;
			if(lpBit == NULL) return FALSE;
			if(pPage->lpBit == NULL) return FALSE;
			if(w==0 || h==0) return FALSE;

			InitGrpLib(width, height, lPitchHalf, 555);
			if(blendgrade >= 31) _PutSpr(xp, yp, w, h, (WORD*)lpBit, (WORD*)pPage->lpBit, colorkey);
			else _PutSpr_GB(xp, yp, w, h, (WORD*)lpBit, (WORD*)pPage->lpBit, colorkey, blendgrade);

			return TRUE;
		}

		/*-----------------------------------------------------------------------------------------
		�������� �ѷ��� Ŭ���̾�Ʈ�� Ư�� ��ǥ�� �������� ��ǥ�� ��ȯ��Ų��
		------------------------------------------------------------------------------------------*/
		void CPage::ToPagePos(LONG xp, LONG yp, LONG* X, LONG* Y)
		{
			*X = (xp-xpFlip)/iZoomX;
			*Y = (yp-ypFlip)/iZoomX;
		}

		/*-----------------------------------------------------------------------------------------
		�������� �ø��� GDIǥ�� ��ġ(Ŭ���̾�Ʈ ����)�� �����Ѵ�
		------------------------------------------------------------------------------------------*/
		void CPage::SetFlipPos(LONG xp, LONG yp, int zoom)
		{
			xpFlip = xp;
			ypFlip = yp;
			iZoomX = zoom;
		}

		/*-----------------------------------------------------------------------------------------
		�������� ������ GDI ǥ�鿡 ������� �Ѹ���(SetPagePos()�� ��ġ�� �ٲܼ� �ִ�)
		------------------------------------------------------------------------------------------*/
		BOOL CPage::Flip(HWND hwnd)
		{
			// �������� �ʱ�ȭ ���� �ʾҴٸ�
			if(  lpBit == NULL ) return FALSE;
			HDC hDC = GetDC(hwnd);
			BOOL rtn = Flip(hDC);
			ReleaseDC(hwnd, hDC);
			return rtn;
		}

		BOOL CPage::Flip(HDC hdc)
		{
			int rtn;

			// �������� �ʱ�ȭ ���� �ʾҴٸ�
			if(  lpBit == NULL ) return FALSE;

			HDC hDC = hdc;

			if( nColorNum != 0 && hPal != NULL)	//�ķ�Ʈ�� �ʿ��ϴٸ�
			{
				HPALETTE hOldPal;
				hOldPal = SelectPalette( hDC, hPal, TRUE ); //�ȷ�Ʈ ����
				if(hOldPal == NULL) return FALSE;
				rtn = RealizePalette( hDC );				 //�ȷ�Ʈ Ȱ��ȭ
				if(rtn==GDI_ERROR) return FALSE;             // GDI_ERROR = -1
			}

			int xl = width * iZoomX;
			int yl = height * iZoomX;

			rtn = StretchDIBits( hDC,
				xpFlip,
				ypFlip,
				xl,
				yl,
				0,
				0,
				width,
				height,
				lpBit,
				lpBmpInfo,
				DIB_RGB_COLORS,
				SRCCOPY );

			if(rtn == GDI_ERROR) return FALSE;
			return TRUE;
		}

		/*-----------------------------------------------------------------------------------------
		�������� ������ GDI ǥ�鿡 ������ Ȯ��� �Ѹ���(SetFlipPos()�� �̸�����)
		------------------------------------------------------------------------------------------*/
		BOOL CPage::FlipToGDI( HDC hdc, LONG xp, LONG yp, int zoom)
		{
			SetFlipPos(xp, yp, zoom);
			return Flip(hdc);
		}

		BOOL CPage::FlipToGDI( HWND hwnd, LONG xp, LONG yp, int zoom)
		{
			SetFlipPos(xp, yp, zoom);
			return Flip(hwnd);
		}

		/*-----------------------------------------------------------------------------------------
		�������� ������ GDI ǥ�鿡 Stretch�� �Ѹ���

		- �ҽ��� Ÿ���� RECT�� NULL�̸� ��ü �������� �����ȴ�
		------------------------------------------------------------------------------------------*/
		BOOL CPage::StretchFlip(HWND hwnd, RECT* rcDest, RECT* rcSrc )
		{
			int rtn;

			// �������� �ʱ�ȭ ���� �ʾҴٸ�
			if(  lpBit == NULL ) return FALSE;

			HDC hDC = GetDC(hwnd);

			if( nColorNum != 0 && hPal != NULL)	//�ķ�Ʈ�� �ʿ��ϴٸ�
			{
				HPALETTE hOldPal;
				hOldPal = SelectPalette( hDC, hPal, TRUE ); //�ȷ�Ʈ ����
				if(hOldPal == NULL) return FALSE;
				rtn = RealizePalette( hDC );				 //�ȷ�Ʈ Ȱ��ȭ
				if(rtn==GDI_ERROR) return FALSE;             // GDI_ERROR = -1
			}

			//int xl = width * iZoomX;
			//int yl = height * iZoomX;

			int sx, sy, sxl, syl;
			int dx, dy, dxl, dyl;

			if(rcDest==NULL) 
			{ 
				RECT rect;
				GetClientRect(hwnd, &rect);
				dx=0; dy=0; dxl=rect.right-rect.left+1; dyl=rect.bottom-rect.top+1;
			}
			else
			{
				dx = rcDest->left; dy = rcDest->top;
				dxl = rcDest->right - rcDest->left + 1; dyl = rcDest->bottom - rcDest->top + 1;
			}

			if(rcSrc==NULL) 
			{ 
				sx=0; sy=0; sxl=width; syl=height;
			}
			else
			{
				sx = rcSrc->left; sy = rcSrc->top;
				sxl = rcSrc->right - rcSrc->left + 1; syl = rcSrc->bottom - rcSrc->top + 1;
			}

			sy = height-sy-syl; // �׸��� ������ �����Ƿ� ��ǥ�� �����´�

			rtn = StretchDIBits( hDC,
				dx, dy, dxl, dyl,
				sx, sy, sxl, syl,
				lpBit,
				lpBmpInfo,
				DIB_RGB_COLORS,
				SRCCOPY );

			ReleaseDC(hwnd, hDC);

			if(rtn == GDI_ERROR) return FALSE;
			return TRUE;
		}


		/////////////////////
		//  ����� ����
		/////////////////////

		/*-----------------------------------------------------------------------------------------
		�������� ������ �׸���(flag�� TRUE�̸� XOR�� �׸���)
		------------------------------------------------------------------------------------------*/
		void CPage::HLine(int x, int y, int len, WORD color, BOOL flag)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(nBitNum == 16) {
				if(flag==FALSE) DrawHLine( x, y, len, (WORD*)lpBit, color );    // �ܻ� Į�� �����
				else if(flag==TRUE) DrawHLineXOR( x, y, len, (WORD*)lpBit);           // XOR �����
			}
			else if(nBitNum == 8) {
				DrawHLine8(x, y, len, (char*)lpBit, (char)color);
			}
		}

		/*-----------------------------------------------------------------------------------------
		�������� �������� �׸���(flag�� TRUE�̸� XOR�� �׸���)
		------------------------------------------------------------------------------------------*/
		void CPage::VLine(int x, int y, int len, WORD color, BOOL flag)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(nBitNum == 16) {
				if(flag==FALSE) DrawVLine( x, y, len, (WORD*)lpBit, color );    // �ܻ� Į�� �����
				else if(flag==TRUE) DrawVLineXOR( x, y, len, (WORD*)lpBit);           // XOR �����
			}
			else if(nBitNum == 8) {
				DrawVLine8(x, y, len, (char*)lpBit, (char)color);
			}
		}

		/*-----------------------------------------------------------------------------------------
		�������� ���� �׸���(�극���� �˰���)
		------------------------------------------------------------------------------------------*/
		void CPage::DrawLine(int x1, int y1, int x2, int y2, WORD color)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(nBitNum == 16) {
				DrawBresLine( x1, y1, x2, y2, (WORD*)lpBit, color );    // �� �׸���(�극���� �˰���)
			}
		}

		void CPage::DrawLineXOR(int x1, int y1, int x2, int y2)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(nBitNum == 16) {
				DrawBresLineXOR( x1, y1, x2, y2, (WORD*)lpBit);    // �� �׸���(�극���� �˰���)
			}
		}

		void CPage::DrawLineGB(int x1, int y1, int x2, int y2, WORD color, int grade)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(nBitNum == 16) {
				DrawBresLineGB( x1, y1, x2, y2, (WORD*)lpBit, color, grade );    // �� �׸���(�극���� �˰���)
			}
		}

		/*-----------------------------------------------------------------------------------------
		�������� ���� �� �ڽ��� �׸���(flag�� TRUE�̸� XOR�� �׸���)
		------------------------------------------------------------------------------------------*/
		void CPage::DrawEmptyBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(x1>x2) _swap(x1,x2);
			if(y1>y2) _swap(y1,y2);

			if(nBitNum == 16) {
				if(flag==FALSE) DrawBox( x1, y1, x2, y2, (WORD*)lpBit, color );    // �ܻ� Į�� �����
				else if(flag==TRUE) DrawBoxXOR( x1, y1, x2, y2, (WORD*)lpBit);           // XOR �����
			}
			else if(nBitNum == 8) {
				DrawBox8(x1, y1, x2, y2, (char*)lpBit, (char)color);
			}
		}

		/*-----------------------------------------------------------------------------------------
		�������� ���� �� �ڽ��� �׸���
		------------------------------------------------------------------------------------------*/
		void CPage::DrawFillBox(int x1, int y1, int x2, int y2, WORD color)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(x1>x2) _swap(x1,x2);
			if(y1>y2) _swap(y1,y2);

			if(nBitNum == 16) {
				FillBox( x1, y1, x2, y2, (WORD*)lpBit, color );
			}
			else if(nBitNum == 8) {
				FillBox8( x1, y1, x2, y2, (char*)lpBit, (char)color );
			}
		}

		/*-----------------------------------------------------------------------------------------
		�������� ���� �� ������ �ڽ��� �׸���
		------------------------------------------------------------------------------------------*/
		void CPage::DrawFillBoxGB(int x1, int y1, int x2, int y2, WORD color, int grade)
		{
			if(lpBit==NULL) return;
			InitGrpLib(width, height, lPitchHalf, 555);

			if(x1>x2) _swap(x1,x2);
			if(y1>y2) _swap(y1,y2);

			if(nBitNum == 16) {
				FillBoxGB( x1, y1, x2, y2, (WORD*)lpBit, color, grade);
			}
			else if(nBitNum == 8) {
				FillBoxGlass8(x1, y1, x2, y2, (char*)lpBit, (char)color, grade);
			}
		}

		/*-----------------------------------------------------------------------------------------
		���� �ڽ��� �׸���
		------------------------------------------------------------------------------------------*/
		void CPage::DrawSelectBox(int x1, int y1, int x2, int y2, WORD color, BOOL flag)
		{
			if(lpBit==NULL)
			{
				return;
			}

			InitGrpLib(width, height, lPitchHalf, 555);

			if(x1>x2)
				_swap(x1,x2);
			if(y1>y2)
				_swap(y1,y2);

			if(nBitNum == 16) 
			{
				if(flag) 
				{
					DrawXORSelectBox(x1, y1, x2, y2, (WORD*)lpBit);
				}
				else
				{
					DrawSelectBox(x1, y1, x2, y2, *(WORD*)lpBit, color);
				}
			}
			else if(nBitNum == 8) 
			{
			}
		}

		/*--------------------------------------------------------------------------------------
		ȸ�� X-Box ��� �Լ�
		----------------------------------------------------------------------------------------*/
		void CPage::DrawRotateXBox(int x1, int y1, int x2, int y2, float angle, WORD color)
		{
			POINT pt[4];
			pt[0].x = x1;	pt[0].y = y1;
			pt[1].x = x2;	pt[1].y = y1;
			pt[2].x = x2;	pt[2].y = y2;
			pt[3].x = x1;	pt[3].y = y2;

			for(int i=1; i<=4; i++) {
				if(angle != 0) {
					if(i < 4)
						_AxisRotatePoint(&pt[i], angle, pt[0]);
				}

				int n = i%4;

				DrawLine(pt[i-1].x, pt[i-1].y, pt[n].x, pt[n].y, color);
			}

			DrawLine(pt[0].x, pt[0].y, pt[2].x, pt[2].y, color);
			DrawLine(pt[1].x, pt[1].y, pt[3].x, pt[3].y, color);
		}


		///////////////////////////
		// 8��Ʈ �̹��� ��� �Լ� 
		//////////////////////////


		/*--------------------------------------------------------------------------------------
		8��Ʈ �ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::PutSprAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int opt, int grade, BYTE color)
		{
			if(pSpr8==NULL) return FALSE;
			if(pSpr8->spr==NULL) return FALSE;
			if(sprnum<0 || sprnum >= pSpr8->sfhead.TotalSprNum) return FALSE;
			PutSprAuto(x, y, &pSpr8->spr[sprnum], opt, grade, color);
			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		8��Ʈ �ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::PutSprAuto(int x, int y, SPRITE8 *pSpr, int opt, int grade, BYTE color)
		{
			if(pSpr==NULL) return FALSE;
			if(lpBit==NULL) return FALSE;
			if(nBitNum != 8) return FALSE;

			if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
				InitGrpLib(width, height, lPitchHalf, 555);

			if(opt & TEMPCLIP) {
				opt = (~(opt&TEMPCLIP))&opt;
				SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
			}
			else {
				SetClipArea(0, 0, width-1, height-1);
			}

			// ������ ����̸�(�����ǥ ����)
			if(opt & MIRROR)
			{
				opt = (~(opt&MIRROR))&opt;
				opt |= LR;
				if(opt & RXY)
				{
					x = MirrorX + (MirrorX - (x + pSpr->rx + pSpr->xl));
					y += pSpr->ry;
					opt = (~(opt&RXY))&opt;
				}
				else x = MirrorX + (MirrorX - (x + pSpr->xl));
			}
			else if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }


			if(!(opt&GB) && !(opt&GLOW))  // �Ϲ�����ΰ�?
			{
				if(pSpr->compress) // �����ΰ�?
				{
					if(!(opt&LR) && !(opt&UD)) PutCsp8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
					else if((opt&LR) && !(opt&UD)) PutCspLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
					else if(!(opt&LR) && (opt&UD)) PutCspUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
					else PutCspUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) PutSpr8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
					else if((opt&LR) && !(opt&UD)) PutSprLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
					else if(!(opt&LR) && (opt&UD)) PutSprUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
					else PutSprUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data);
				}
			}
			else if(!(opt&GB) && (opt&GLOW))  // �Ϲ� �ܻ� ����ΰ�?
			{
				if(pSpr->compress) // �����ΰ�?
				{
					if(!(opt&LR) && !(opt&UD)) PutCspMono8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
					else if((opt&LR) && !(opt&UD)) PutCspMonoLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
					else if(!(opt&LR) && (opt&UD)) PutCspMonoUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
					else PutCspMonoUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) PutSprMono8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
					else if((opt&LR) && !(opt&UD)) PutSprMonoLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
					else if(!(opt&LR) && (opt&UD)) PutSprMonoUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
					else PutSprMonoUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color);
				}
			}
			else if((opt&GB) && !(opt&GLOW)) // ����������ΰ�?
			{
				if(pSpr->compress) // �����ΰ�?
				{
					if(!(opt&LR) && !(opt&UD)) PutCspGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
					else if((opt&LR) && !(opt&UD)) PutCspGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
					else if(!(opt&LR) && (opt&UD)) PutCspGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
					else PutCspGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) PutSprGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
					else if((opt&LR) && !(opt&UD)) PutSprGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
					else if(!(opt&LR) && (opt&UD)) PutSprGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
					else PutSprGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, grade);
				}
			}
			else if((opt&GB) && (opt&GLOW)) // �ܻ� ����������ΰ�?
			{
				if(pSpr->compress) // �����ΰ�?
				{
					if(!(opt&LR) && !(opt&UD)) PutCspMonoGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
					else if((opt&LR) && !(opt&UD)) PutCspMonoGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
					else if(!(opt&LR) && (opt&UD)) PutCspMonoGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
					else PutCspMonoGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) PutSprMonoGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
					else if((opt&LR) && !(opt&UD)) PutSprMonoGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
					else if(!(opt&LR) && (opt&UD)) PutSprMonoGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
					else PutSprMonoGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, pSpr->data, color, grade);
				}
			}

			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		8��Ʈ Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::ZoomSprAuto(int x, int y, CSprite8 *pSpr8, int sprnum, int tx, int ty, int opt, int grade, BYTE color)
		{
			if(pSpr8==NULL) return FALSE;
			if(pSpr8->spr==NULL) return FALSE;
			if(sprnum<0 || sprnum >= pSpr8->sfhead.TotalSprNum) return FALSE;
			ZoomSprAuto(x, y, &pSpr8->spr[sprnum], tx, ty, opt, grade, color);
			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		8��Ʈ Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::ZoomSprAuto(int x, int y, SPRITE8 *pSpr, int tx, int ty, int opt, int grade, BYTE color)
		{
			if(pSpr==NULL) return FALSE;
			if(lpBit==NULL) return FALSE;
			if(nBitNum != 8) return FALSE;

			if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
				InitGrpLib(width, height, lPitchHalf, 555);

			if(opt & TEMPCLIP) {
				opt = (~(opt&TEMPCLIP))&opt;
				SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
			}
			else {
				SetClipArea(0, 0, width-1, height-1);
			}

			if(opt & PERCENT) { tx = pSpr->xl*tx/100; ty = pSpr->yl*ty/100; }	// 100% �������� Ȯ��,���
			if(opt & RXY) { x+=pSpr->rx*tx/pSpr->xl; y+=pSpr->ry*ty/pSpr->yl; }	// �����ǥ�� ����
			if(opt & CENTER) { x -= (tx>>1); y -= (ty>>1); }					// ĳ���͸� ��ǥ�� �߽ɿ� ��ġ��Ų��

			// 1x Zoom
			if(tx == pSpr->xl && ty == pSpr->yl) { PutSprAuto(x, y, pSpr, opt, grade, color); return TRUE; }

			if(!(opt&GB) && !(opt&GLOW))  // �Ϲ�����ΰ�?
			{
				if(pSpr->compress) // �����ΰ�?
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomCsp8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
					else if((opt&LR) && !(opt&UD)) LowZoomCspLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
					else if(!(opt&LR) && (opt&UD)) LowZoomCspUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
					else LowZoomCspUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomSpr8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
					else if((opt&LR) && !(opt&UD)) LowZoomSprLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
					else if(!(opt&LR) && (opt&UD)) LowZoomSprUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
					else LowZoomSprUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data);
				}
			}
			else if(!(opt&GB) && (opt&GLOW))  // �Ϲ� �ܻ� ����ΰ�?
			{
				if(pSpr->compress) // �����ΰ�?
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomCspMono8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
					else if((opt&LR) && !(opt&UD)) LowZoomCspMonoLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
					else if(!(opt&LR) && (opt&UD)) LowZoomCspMonoUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
					else LowZoomCspMonoUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomSprMono8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
					else if((opt&LR) && !(opt&UD)) LowZoomSprMonoLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
					else if(!(opt&LR) && (opt&UD)) LowZoomSprMonoUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
					else LowZoomSprMonoUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color);
				}
			}
			else if((opt&GB) && !(opt&GLOW)) // ����������ΰ�?
			{
				if(pSpr->compress) // �����ΰ�?
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomCspGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
					else if((opt&LR) && !(opt&UD)) LowZoomCspGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
					else if(!(opt&LR) && (opt&UD)) LowZoomCspGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
					else LowZoomCspGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomSprGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
					else if((opt&LR) && !(opt&UD)) LowZoomSprGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
					else if(!(opt&LR) && (opt&UD)) LowZoomSprGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
					else LowZoomSprGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, grade);
				}
			}
			else if((opt&GB) && (opt&GLOW)) // �ܻ� ����������ΰ�?
			{
				if(pSpr->compress) // �����ΰ�?
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomCspMonoGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
					else if((opt&LR) && !(opt&UD)) LowZoomCspMonoGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
					else if(!(opt&LR) && (opt&UD)) LowZoomCspMonoGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
					else LowZoomCspMonoGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
				}
				else
				{
					if(!(opt&LR) && !(opt&UD)) LowZoomSprMonoGlass8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
					else if((opt&LR) && !(opt&UD)) LowZoomSprMonoGlassLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
					else if(!(opt&LR) && (opt&UD)) LowZoomSprMonoGlassUD8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
					else LowZoomSprMonoGlassUDLR8(x, y, pSpr->xl, pSpr->yl, (char*)lpBit, tx, ty, pSpr->data, color, grade);
				}
			}

			return TRUE;
		}


		/////////////////////////////
		//  16��Ʈ ��������Ʈ ����
		/////////////////////////////

		/*--------------------------------------------------------------------------------------
		�ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::PutSprAuto(int x, int y, CPage *pPage, int opt, int grade, WORD color)
		{
			SPRITE30 *pSprite = new SPRITE30;
			pSprite->xl = pPage->width;
			pSprite->yl = pPage->height;
			pSprite->compress = 0;
			pSprite->data = (WORD *)pPage->lpBit;
			pSprite->colorkey = xSprite::RGB555(0,0,255);
			pSprite->pAlpha = NULL;

			BOOL bRet = PutSprAuto(x, y, pSprite, opt, grade, color);

			delete pSprite;

			return bRet;
		}

		/*--------------------------------------------------------------------------------------
		�ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::PutSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt, int grade, WORD color)
		{
			if(pXSpr==NULL) return FALSE;
			if(pXSpr->spr==NULL) return FALSE;
			if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;
			PutSprAuto(x, y, &pXSpr->spr[sprnum], opt, grade, color);
			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		�ڵ� ��������Ʈ ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::PutSprAuto(int x, int y, SPRITE30 *pSpr, int opt, int grade, WORD color)
		{
			if(pSpr==NULL) return FALSE;
			if(lpBit==NULL) return FALSE;
			if(nBitNum != 16) return FALSE;

			if(bEnableTempClip)
				opt |= TEMPCLIP;

			if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
				InitGrpLib(width, height, lPitchHalf, 555);

			if(opt & TEMPCLIP) {
				opt = (~(opt&TEMPCLIP))&opt;
				SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
			}
			else {
				SetClipArea(0, 0, width-1, height-1);
			}

			if((pSpr->PixelFormat != 5558 && pSpr->PixelFormat != 8888)  || pSpr->pAlpha==NULL) {
				// ���� ���̺��� ���� �����̶�� ȿ�� �÷��� ����
				opt = (~(opt&AB))&opt;
			}
			else if(pSpr->PixelFormat != 5558 || pSpr->pAlpha==NULL) {
				// ���� ���̺��� ���� �����̶�� ȿ�� �÷��� ����
				opt = (~(opt&AB))&opt;
			}
			// ������ ����̸�(�����ǥ ����)
			if(opt & MIRROR)
			{
				opt = (~(opt&MIRROR))&opt;
				//opt &= ~MIRROR;
				opt |= LR;
				if(opt & RXY)
				{
					x = MirrorX + (MirrorX - (x + pSpr->rx + pSpr->xl));
					opt = (~(opt&RXY))&opt;
				}
				else x = MirrorX + (MirrorX - (x + pSpr->xl));
				y+=pSpr->ry;
			}
			else if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }

			if(grade < 0) grade = 0;
			if(grade > 31) grade = 31;

			if(pSpr->compress)
			{
				if(opt&LR)
				{
					opt = (~(opt&LR))&opt;
					if(opt==NULL) _PutCspLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&HB) _PutCspLR_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&GB) _PutCspLR_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&LE) _PutCspLR_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&DE) _PutCspLR_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&AB) _PutCspLR_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->pAlpha, grade);
					else if(opt&GLOW) _PutCspLR_Glow(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color, grade);
					else if(opt&GRAYSCL) _PutCspLR_Grayscl(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&MONO) _PutCspLR_Mono(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color);
					else _PutCspLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
				}
				else if(opt&UD)
				{
					opt = (~(opt&UD))&opt;
					if(opt==NULL) _PutCspUD(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&HB) _PutCspUD_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&GB) _PutCspUD_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&LE) _PutCspUD_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&DE) _PutCspUD_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&AB) _PutCspUD_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->pAlpha, grade);
					else if(opt&GLOW) _PutCspUD_Glow(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color, grade);
					else if(opt&GRAYSCL) _PutCspUD_Grayscl(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&MONO) _PutCspUD_Mono(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color);
					else _PutCspUD(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
				}
				else
				{
					if(opt==NULL) _PutCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&ANTI) _PutCsp_Anti(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&HB) _PutCsp_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
					else if(opt&GB) _PutCsp_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&LE) _PutCsp_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&DE) _PutCsp_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, grade);
					else if(opt&AB) _PutCsp_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->pAlpha, grade);
					else if(opt&GLOW) _PutCsp_Glow(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color, grade);
					else if(opt&GRAYSCL)
					{	
						if( pSpr->pAlpha )
						{
							_PutCsp_Grayscl_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->pAlpha,grade);
						}
						else
						{
							_PutCsp_Grayscl(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
						}

					}
					else if(opt&MONO)
					{
						if( pSpr->pAlpha )
						{
							_PutCsp_Mono_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data,  pSpr->pAlpha, color,grade);
						}
						else
						{
							_PutCsp_Mono(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, color);
						}

					}
					else _PutCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
				}
			}
			else
			{
				if(opt==NULL) _PutSpr(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey);
				else if(opt&HB) _PutSpr_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey, 15);	// GBȿ���� �ӽ÷� ���
				else if(opt&GB) _PutSpr_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey, grade);
				else if(opt&LE) _PutSpr_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey, grade);
				else if(opt&DE) _PutSpr_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey, grade);
				else if(opt&AB) _PutSpr_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey, pSpr->pAlpha, grade);
				else if(opt&LR) _PutSprLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey);
				else if(opt&UD) _PutSprUD(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey);
				else if(opt&GRAYSCL)
				{
					if( pSpr->pAlpha )
					{
						_PutSpr_Grayscl_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data,pSpr->pAlpha,grade);
					}
					else
					{
						_PutSpr_Grayscl(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data,grade);
					}
				}
				else if(opt&MONO)
				{
					if( pSpr->pAlpha )
					{
						_PutSpr_Mono_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data,pSpr->pAlpha, color,grade);
					}
					else
					{
						_PutSpr_Mono(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey, color,grade);
					}
				}
				else _PutSpr(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data, pSpr->colorkey);
			}

			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::ZoomSprAuto(int x, int y, CPage *pPage, int tx, int ty, int opt, int grade, WORD color)
		{
			SPRITE30 *pSprite = new SPRITE30;
			pSprite->xl = pPage->width;
			pSprite->yl = pPage->height;
			pSprite->compress = 0;
			pSprite->data = (WORD *)pPage->lpBit;
			pSprite->colorkey = xSprite::RGB555(0,0,255);
			pSprite->pAlpha = NULL;

			BOOL bRet = ZoomSprAuto(x, y, pSprite, tx, ty, opt, grade, color);

			delete pSprite;

			return bRet;
		}

		/*--------------------------------------------------------------------------------------
		Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::ZoomSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, int opt, int grade, WORD color)
		{
			if(pXSpr==NULL) return FALSE;
			if(pXSpr->spr==NULL) return FALSE;
			if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;
			ZoomSprAuto(x, y, &pXSpr->spr[sprnum], tx, ty, opt, grade, color);
			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::ZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, int opt, int grade, WORD color)
		{
			if(pSpr==NULL) return FALSE;
			if(lpBit==NULL) return FALSE;
			if(nBitNum != 16) return FALSE;

			if(bEnableTempClip)
				opt |= TEMPCLIP;

			if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
				InitGrpLib(width, height, lPitchHalf, 555);

			if(opt & TEMPCLIP) {
				opt = (~(opt&TEMPCLIP))&opt;
				SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
			}
			else {
				SetClipArea(0, 0, width-1, height-1);
			}

			if((pSpr->PixelFormat != 5558 && pSpr->PixelFormat != 8888)  || pSpr->pAlpha==NULL) {
				// ���� ���̺��� ���� �����̶�� ȿ�� �÷��� ����
				opt = (~(opt&AB))&opt;
			}
			else if(pSpr->PixelFormat != 5558 || pSpr->pAlpha==NULL) {
				// ���� ���̺��� ���� �����̶�� ȿ�� �÷��� ����
				opt = (~(opt&AB))&opt;
			}


			if(opt & PERCENT) { tx = pSpr->xl*tx/100; ty = pSpr->yl*ty/100; }	// 100% �������� Ȯ��,���
			if(opt & RXY) { x+=pSpr->rx*tx/pSpr->xl; y+=pSpr->ry*ty/pSpr->yl; }	// �����ǥ�� ����
			if(opt & CENTER) { x -= (tx>>1); y -= (ty>>1); }					// ĳ���͸� ��ǥ�� �߽ɿ� ��ġ��Ų��

			if(grade < 0) grade = 0;
			if(grade > 31) grade = 31;

			// 1x Zoom
			if(tx == pSpr->xl && ty == pSpr->yl) { 
				PutSprAuto(x, y, pSpr, opt, grade, color); 
				return TRUE; 
			}

			if(pSpr->compress)
			{
				if(opt&LR)
				{
					opt = (~(opt&LR))&opt;
					if(opt==NULL) _ZoomCspLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
					else if(opt&HB) _ZoomCspLR_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
					else if(opt&GB) _ZoomCspLR_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&LE) _ZoomCspLR_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&DE) _ZoomCspLR_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&AB) _ZoomCspLR_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->pAlpha, grade);
					else _ZoomCspLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
				}
				else if(opt&UD)
				{
					opt = (~(opt&UD))&opt;
					if(opt==NULL) _ZoomCspUD(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
					else if(opt&HB) _ZoomCspUD_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
					else if(opt&GB) _ZoomCspUD_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&LE) _ZoomCspUD_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&DE) _ZoomCspUD_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&AB) _ZoomCspUD_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->pAlpha, grade);
					else _ZoomCspUD(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
				}
				else
				{
					if(opt==NULL) _ZoomCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
					else if(opt&HB) _ZoomCsp_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
					else if(opt&GB) _ZoomCsp_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&LE) _ZoomCsp_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&DE) _ZoomCsp_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, grade);
					else if(opt&AB) _ZoomCsp_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->pAlpha, grade);
					else if(opt&MONO) _ZoomCsp_Mono(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, pSpr->pAlpha, grade,color);
					else if(opt&GRAYSCL) _ZoomCsp_Grayscl(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data,  pSpr->pAlpha , grade);
					else _ZoomCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data);
				}
			}
			else
			{
				if(opt&LR)
				{
					opt = (~(opt&LR))&opt;
					if(opt==NULL) _ZoomSprLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey);
					else if(opt&HB) _ZoomSprLR_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, 15);	// GBȿ�� �Լ��� ���(�ӽ� ����)
					else if(opt&GB) _ZoomSprLR_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
					else if(opt&LE) _ZoomSprLR_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
					else if(opt&DE) _ZoomSprLR_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
					else if(opt&AB) _ZoomSprLR_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, pSpr->pAlpha, grade);
					else _ZoomSprLR(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey);

				}
				else if(opt&UD)
				{	
					opt = (~(opt&UD))&opt;
					if(opt==NULL) _ZoomSprUD(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey);
					else if(opt&HB) _ZoomSprUD_HB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, 15);	// GBȿ�� �Լ��� ���(�ӽ� ����)
					else if(opt&GB) _ZoomSprUD_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
					else if(opt&LE) _ZoomSprUD_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
					else if(opt&DE) _ZoomSprUD_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
					else if(opt&AB) _ZoomSprUD_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, pSpr->pAlpha, grade);
					else _ZoomSprUD(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey);

				}
				else if(opt==NULL) _ZoomSpr(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey);
				else if(opt&HB) _ZoomSpr_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, 15);	// GBȿ�� �Լ��� ���(�ӽ� ����)
				else if(opt&GB) _ZoomSpr_GB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
				else if(opt&LE) _ZoomSpr_LE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
				else if(opt&DE) _ZoomSpr_DE(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, grade);
				else if(opt&AB) _ZoomSpr_AB(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, pSpr->pAlpha, grade);
				else if(opt&MONO) _ZoomSpr_Mono(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey, pSpr->pAlpha, grade,color);
				else if(opt&GRAYSCL) _ZoomSpr_Grayscl(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data,  pSpr->pAlpha , grade);
				else _ZoomSpr(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, pSpr->data, pSpr->colorkey);
			}

			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		Rotate & Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::RotateZoomSprAuto(int x, int y, CPage *pPage, int tx, int ty, float angle, int opt, int grade, WORD color)
		{
			SPRITE30 *pSprite = new SPRITE30;
			pSprite->xl = pPage->width;
			pSprite->yl = pPage->height;
			pSprite->compress = 0;
			pSprite->data = (WORD *)pPage->lpBit;
			pSprite->colorkey = xSprite::RGB555(0,0,255);
			pSprite->pAlpha = NULL;

			BOOL bRet = RotateZoomSprAuto(x, y, pSprite, tx, ty, angle, opt, grade, color);

			delete pSprite;

			return bRet;
		}

		/*--------------------------------------------------------------------------------------
		Rotate & Zoom ��������Ʈ ��� �Լ�(����/����� ��� ��밡��, �¿�/�����ø� ����)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::RotateZoomSprAuto(int x, int y, xSprite *pXSpr, int sprnum, int tx, int ty, float angle, int opt, int grade, WORD color)
		{
			if(pXSpr==NULL) return FALSE;
			if(pXSpr->spr==NULL) return FALSE;
			if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;
			RotateZoomSprAuto(x, y, &pXSpr->spr[sprnum], tx, ty, angle, opt, grade, color);
			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		Rotate & Zoom ��������Ʈ ��� �Լ�
		----------------------------------------------------------------------------------------*/
		BOOL CPage::RotateZoomSprAuto(int x, int y, SPRITE30 *pSpr, int tx, int ty, float angle, int opt, int grade, WORD color)
		{
			if(pSpr==NULL) return FALSE;
			if(lpBit==NULL) return FALSE;
			if(nBitNum != 16) return FALSE;

			if(bEnableTempClip)
				opt |= TEMPCLIP;

			if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
				InitGrpLib(width, height, lPitchHalf, 555);

			if(opt & TEMPCLIP) {
				opt = (~(opt&TEMPCLIP))&opt;
				SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
			}
			else {
				SetClipArea(0, 0, width-1, height-1);
			}

			if((pSpr->PixelFormat != 5558 && pSpr->PixelFormat != 8888)  || pSpr->pAlpha==NULL) {
				// ���� ���̺��� ���� �����̶�� ȿ�� �÷��� ����
				opt = (~(opt&AB))&opt;
			}
			else if(pSpr->PixelFormat != 5558 || pSpr->pAlpha==NULL) {
				// ���� ���̺��� ���� �����̶�� ȿ�� �÷��� ����
				opt = (~(opt&AB))&opt;
			}

			if(opt & PERCENT) { tx = pSpr->xl*tx/100; ty = pSpr->yl*ty/100; }	// 100% �������� Ȯ��,���
			if(opt & RXY) { x += pSpr->rx*tx/pSpr->xl; y += pSpr->ry*ty/pSpr->yl; }	// �����ǥ�� ����
			if(opt & CENTER) { 
				// ĳ���͸� ��ǥ�� �߽ɿ� ��ġ��Ų��
				x -= (tx>>1); 
				y -= (ty>>1);

				// ȸ���� ��ŭ ��ǥ�� �����Ѵ�
				if(angle != 0) {
					POINT dt, odt;
					dt.x = -(tx>>1);
					dt.y = -(ty>>1);

					odt = dt;

					_RotatePoint(&dt, angle);

					x += dt.x - odt.x;
					y += dt.y - odt.y;
				}
			}

			if(grade < 0) grade = 0;
			if(grade > 31) grade = 31;

			if(angle == 0.0f) {
				if(tx == pSpr->xl && ty == pSpr->yl) { 
					// ȸ�� �� �� �������� ��� �Ϲ� ��������Ʈ ���
					PutSprAuto(x, y, pSpr, opt, grade, color); 
					return TRUE; 
				}
				else {
					// �ܸ� ����� ��� �� ��������Ʈ ���
					ZoomSprAuto(x, y, pSpr, tx, ty, opt, grade, color);
					return TRUE;
				}
			}

			if(pSpr->compress)
			{
				/*
				if(opt&LR)
				{
				opt = (~(opt&LR))&opt;
				//_RotateZoomCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, angle, pSpr->data);
				}
				else if(opt&UD)
				{
				opt = (~(opt&UD))&opt;
				//_RotateZoomCsp(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, tx, ty, angle, pSpr->data);
				}
				else
				{
				// ���� �̱���
				}

				if(pSpr->data) {
				// ����� ��������Ʈ�� �� �� ȸ���� �������� ����(���� �� �� �ֵ��� X�ڽ��� ǥ����)
				DrawRotateXBox(x, y, x+tx-1, y+ty-1, angle, RGBmix(31,0,0));
				}
				*/
				_SetRenderTarget((WORD*)lpBit);
				_SetTexture(pSpr->data, pSpr->xl, pSpr->yl, pSpr->pAlpha);
				_SetTextureColorKey(pSpr->colorkey);
				_SetBlendingFunc_HLineTexture(CSPHLineTexture_GB);
				_SetTextureAlpha(grade);

				if(opt==NULL) _SetBlendingFunc_HLineTexture(CSPHLineTexture);
				else if(opt&HB) _SetBlendingFunc_HLineTexture(CSPHLineTexture_HB);
				else if(opt&GB) _SetBlendingFunc_HLineTexture(CSPHLineTexture_GB);
				else if(opt&LE) _SetBlendingFunc_HLineTexture(CSPHLineTexture_LE);
				else if(opt&DE) _SetBlendingFunc_HLineTexture(CSPHLineTexture_DE);
				else if(opt&AB && pSpr->pAlpha) _SetBlendingFunc_HLineTexture(CSPHLineTexture_AB); 
				else _SetBlendingFunc_HLineTexture(CSPHLineTexture);
				_RotateZoomSpr(x, y, tx, ty, angle);


			}
			else
			{
				_SetRenderTarget((WORD*)lpBit);
				_SetTexture(pSpr->data, pSpr->xl, pSpr->yl, pSpr->pAlpha);
				_SetTextureColorKey(pSpr->colorkey);
				_SetBlendingFunc_HLineTexture(HLineTexture_GB);
				_SetTextureAlpha(grade);

				if(opt==NULL) _SetBlendingFunc_HLineTexture(HLineTexture);
				else if(opt&HB) _SetBlendingFunc_HLineTexture(HLineTexture_HB);
				else if(opt&GB) _SetBlendingFunc_HLineTexture(HLineTexture_GB);
				else if(opt&LE) _SetBlendingFunc_HLineTexture(HLineTexture_LE);
				else if(opt&DE) _SetBlendingFunc_HLineTexture(HLineTexture_DE);
				else if(opt&AB && pSpr->pAlpha) _SetBlendingFunc_HLineTexture(HLineTexture_AB); 
				else _SetBlendingFunc_HLineTexture(HLineTexture);

				_RotateZoomSpr(x, y, tx, ty, angle);
			}

			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		�ڵ� �̹��� ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::PutImageAuto(int x, int y, xSprite *pXSpr, int sprnum, int opt, int grade, WORD color)
		{
			if(pXSpr==NULL) return FALSE;
			if(pXSpr->spr==NULL) return FALSE;
			if(sprnum<0 || sprnum >= pXSpr->sfhead.TotalSprNum) return FALSE;
			PutImageAuto(x, y, &pXSpr->spr[sprnum], opt, grade, color);
			return TRUE;
		}

		/*--------------------------------------------------------------------------------------
		�ڵ� �̹��� ��� �Լ�(����/����� �ڵ�����, ������/�ø�/�ܻ���� ����)
		----------------------------------------------------------------------------------------*/
		BOOL CPage::PutImageAuto(int x, int y, SPRITE30 *pSpr, int opt, int grade, WORD color)
		{
			if(pSpr==NULL) return FALSE;
			if(lpBit==NULL) return FALSE;
			if(nBitNum != 16) return FALSE;

			if(bEnableTempClip)
				opt |= TEMPCLIP;

			if((int)width != _MXL || (int)height != _MYL || (DWORD)lPitchHalf != _LPITCH) 
				InitGrpLib(width, height, lPitchHalf, 555);

			if(opt & TEMPCLIP) {
				opt = (~(opt&TEMPCLIP))&opt;
				SetClipArea(TClipX1, TClipY1, TClipX2, TClipY2);
			}
			else {
				SetClipArea(0, 0, width-1, height-1);
			}

			if(opt & RXY) { x+=pSpr->rx; y+=pSpr->ry; opt = (~(opt&RXY))&opt; }

			if(grade < 0) grade = 0;
			if(grade > 31) grade = 31;

			if(pSpr->compress)
			{
				//if(opt==NULL) _PutCsp(x, y, pSpr->xl, pSpr->yl, lpBit, pSpr->data);
			}
			else
			{
				_PutImage(x, y, pSpr->xl, pSpr->yl, (WORD*)lpBit, pSpr->data);
			}

			return TRUE;
		}


		///////// �ִϸ��̼� ����


		//--------------------------------------------------------------------------------------
		//      ��� �ִϸ��̼� ��� �Լ�
		//--------------------------------------------------------------------------------------
		BOOL CPage::DrawScene(int x, int y, xSprite *pXSpr, int scn, DWORD time, BOOL loop, int n_opt, int n_grade, WORD n_color)
		{
			if(!pXSpr) 
				return FALSE;

			SceneData* pScene = pXSpr->scene.GetScenePt(scn);

			if(!pScene)
				return FALSE;

			// �־��� �ð��� �÷��� �Ǿ�� �� �������� ã�´�
			int frame = pScene->SearchFrame(time, loop);

			if(frame < 0)
				return FALSE;

			// ������ ������ ��Ÿ���� ����
			float delta = pScene->GetFrameDelta(frame, time, loop);

			return DrawFrameOfScene(x, y, pXSpr, scn, frame, delta, n_opt, n_grade, n_color);
		}

		//--------------------------------------------------------------------------------------
		//      ��� �ִϸ��̼��� Ư�� ������ ��� �Լ�
		//--------------------------------------------------------------------------------------
		BOOL CPage::DrawFrameOfScene(int x, int y, xSprite *pXSpr, int scn, int frame, float fdelta, int n_opt, int n_grade, WORD n_color)
		{
			SceneData* pScene = pXSpr->scene.GetScenePt(scn);

			if(!pScene) 
				return FALSE;

			if(pScene->TotFrameNum == 0)
				return FALSE;

			FrameData* pFrame = pScene->GetPtrFrame(frame);

			if(!pFrame)
				return FALSE;

			if(pFrame->TotCellNum == 0)
				return FALSE;

			for(int i=0; i<pFrame->TotCellNum; i++)
			{
				DRAWINGCELLDESC desc;

				// �� ��� ������ ����
				if(!GetDrawingCellInfo(x, y, pXSpr, scn, frame, i, fdelta, &desc))
					continue;

				// �������̵� �ɼ��� �����Ǿ� �ִٸ� �� ������ ��ü
				if(n_opt != 0) {
					if(n_opt & LR || n_opt & UD || n_opt & TEMPCLIP || n_opt & MIRROR || n_opt & PERCENT || n_opt & RXY || n_opt & CENTER) {
						// ���� ������ �ΰ� �ɼ��� ���
						desc.opt |= n_opt;
					}

					if(n_opt & HB || n_opt & GB || n_opt & GLOW || n_opt & GRAYSCL || n_opt & MONO || n_opt & ANTI || n_opt & LE || n_opt & DE || n_opt & AB) {
						// ������ �Ұ����� ȿ�� �ɼ��� ���
						desc.opt = n_opt;
						desc.grade = n_grade; 
						desc.color = n_color;
					}
				}

				if(pXSpr->spr[desc.sprno].xl == desc.width && pXSpr->spr[desc.sprno].yl == desc.height && desc.angle == 0) {
					PutSprAuto((int)desc.xp, (int)desc.yp, pXSpr, desc.sprno, desc.opt, desc.grade, desc.color);
				}
				else if(desc.angle == 0){
					if(desc.width != 0 && desc.height != 0)
						ZoomSprAuto((int)desc.xp, (int)desc.yp, pXSpr, desc.sprno, (int)desc.width, (int)desc.height, desc.opt, desc.grade, desc.color);
				} 
				else {
					if(desc.width != 0 && desc.height != 0)
						RotateZoomSprAuto((int)desc.xp, (int)desc.yp, pXSpr, desc.sprno, (int)desc.width, (int)desc.height, (float)desc.angle, desc.opt, desc.grade, desc.color);
				}
			}

			return TRUE;
		}

		void CPage::SetSceneEffect(SCNEFFECTDESC* pEffect)
		{
			if(!pEffect) {
				bScnEffect = FALSE;
				return;
			}

			bScnEffect = TRUE;
			ScnEffect = *pEffect;
		}

		//--------------------------------------------------------------------------------------
		//      ����� Ư�� ������ ���� �� ��� ������ ����
		//--------------------------------------------------------------------------------------
		BOOL CPage::GetDrawingCellInfo(int x, int y, xSprite *pXSpr, int scn, int frame, int cell, float fdelta, DRAWINGCELLDESC* pDesc)
		{
			if(!pXSpr) return FALSE;

			SceneData* pScene = pXSpr->scene.GetScenePt(scn);
			if(!pScene) return FALSE;

			FrameData* pFrame = pScene->GetPtrFrame(frame);
			if(!pFrame) return FALSE;

			CellData* pCell = pFrame->GetPtrCell(cell);
			if(!pCell) return FALSE;

			if(pCell->SprNo < 0 || pCell->SprNo >= pXSpr->sfhead.TotalSprNum)
				return FALSE;

			float xp = (float)(x + pCell->Rx);
			float yp = (float)(y + pCell->Ry);
			int sprno = pCell->SprNo;
			float w = (float)pXSpr->spr[sprno].xl;
			float h = (float)pXSpr->spr[sprno].yl;
			int opt = 0;
			float grade = (float)pCell->Grade;
			WORD color = pCell->Color;

			switch(pCell->Effect) 
			{
			case 1 : opt |= HB; break;
			case 2 : opt |= GB; break;
			case 3 : opt |= LE; break;
			case 4 : opt |= DE; break;
			case 5 : opt |= GLOW; break;
			case 6 : opt |= MONO; break;
			case 7 : opt |= GRAYSCL; break;
			case 8 : opt |= AB; break;
			}

			if(pCell->Mirror == 1) {
				// �¿� ������
				if(opt & LR) opt &= ~LR;
				else opt |= LR;
			}
			if(pCell->Mirror == 2) {
				// ���� ������
				if(opt & UD) opt &= ~UD;
				else opt |= UD;
			}

			float zoomx = pCell->ZoomX;
			float zoomy = pCell->ZoomY;
			float angle = (float)pCell->RotAngle;

			if(pCell->Interpol == 1) {
				// ���������̼�
				int nf = frame + 1;
				if(nf >= pScene->TotFrameNum) nf = 0;

				// ���� �������� ������ �����Ϳ� �� �����͸� ����
				FrameData* pFrame2 = pScene->GetPtrFrame(nf);
				CellData* pCell2 = pFrame2->GetPtrCell(cell);

				if(pCell2) {

					float delta = 0;
					float bias = (float)(pCell->Bias / 100.0);

					if(bias < 0.0001f && bias > -0.0001f) bias = 0;
					if(bias > 1.0f) bias = 1.0f;
					if(bias < -1.0f) bias = -1.0f;

					if(bias == 0) {
						delta = fdelta;
					}
					else if(bias < 0) {
						float ang = fdelta * TO_RADIAN(90);
						float m = (float)(1.0 - cos(ang));
						delta = fdelta + ((fdelta - m) * bias);
					}
					else {
						float ang = fdelta * TO_RADIAN(90);
						float m = (float)sin(ang);
						delta = fdelta + ((m - fdelta) * bias);
					}

					// ��ǥ ����
					xp += ((pCell2->Rx - pCell->Rx) * delta);
					yp += ((pCell2->Ry - pCell->Ry) * delta);

					// ���İ� ����
					grade += FLOATROUNDINT((pCell2->Grade - pCell->Grade) * delta);

					// �÷� ����
					if(pCell2->Effect == 5 || pCell2->Effect == 6) {
						int r1 = (int)R555(pCell->Color);
						int g1 = (int)G555(pCell->Color);
						int b1 = (int)B555(pCell->Color);
						int r2 = (int)R555(pCell2->Color);
						int g2 = (int)G555(pCell2->Color);
						int b2 = (int)B555(pCell2->Color);

						int r3 = r1 + FLOATROUNDINT((r2 - r1) * delta);
						int g3 = g1 + FLOATROUNDINT((g2 - g1) * delta);
						int b3 = b1 + FLOATROUNDINT((b2 - b1) * delta);

						color = RGBmix(r3, g3, b3);
					}

					// �� ���� ����
					zoomx += ((pCell2->ZoomX - pCell->ZoomX) * delta);
					zoomy += ((pCell2->ZoomY - pCell->ZoomY) * delta);

					// ȸ�� ���� ����
					angle += ((pCell2->RotAngle - pCell->RotAngle) * delta);
				}
			}

			// Ȯ�� ��� ���� ���� ��� ��ǥ
			float oxp = xp;
			float oyp = yp;

			if(zoomx != 100 || zoomy != 100) {	
				if(zoomx >= 0) w = (pXSpr->spr[sprno].xl * zoomx/100.0f);
				if(zoomy >= 0) h = (pXSpr->spr[sprno].yl * zoomy/100.0f);

				// �߽���ǥ�� �������� Ȯ��/��ҵǵ��� ����
				float mx = (float)pXSpr->spr[sprno].mx;
				float my = (float)pXSpr->spr[sprno].my;
				xp += mx - (mx * zoomx/100.0f);
				yp += my - (my * zoomy/100.0f);
			}

			if(angle != 0) {
				// �߽� ��ǥ�� �������� ȸ���ǵ��� ����
				float mx = (float)pXSpr->spr[sprno].mx;
				float my = (float)pXSpr->spr[sprno].my;

				_AxisRotatePoint(&xp, &yp, angle, (oxp + mx), (oyp + my));
			}

			// �� �׸��� ����Ʈ�� ����Ǿ� �ִٸ�
			if(bScnEffect) {
				if(ScnEffect.alpha >= 0 && ScnEffect.alpha < 31) {
					if(!(opt & GB || opt & LE || opt & DE || opt & AB)) {
						// ������ ������� �ʴ� ������ ��� �ٴܰ� �������� ���� ����
						grade = 31;
						opt |= GB;
					}

					grade = (grade * (ScnEffect.alpha / 31.0f));
				}

				if(ScnEffect.zoomx >= 0 && ScnEffect.zoomx != 100) {
					w = (w * (ScnEffect.zoomx / 100.0f));
					// �� ��� ��ǥ�� �������� Ȯ��/��ҵǵ��� ����
					float mx = (float)(ScnEffect.mx - xp);
					xp += mx - (mx * ScnEffect.zoomx/100.0f);
				}

				if(ScnEffect.zoomy >= 0 && ScnEffect.zoomy != 100) {
					h = (h * (ScnEffect.zoomy / 100.0f));
					// �� ��� ��ǥ�� �������� Ȯ��/��ҵǵ��� ����
					float my = (float)(ScnEffect.my - yp);
					yp += my - (my * ScnEffect.zoomy/100.0f);
				}

				if(ScnEffect.angle != 0) {
					// �� ��� ��ǥ�� �������� ȸ���ǵ��� ����
					_AxisRotatePoint(&xp, &yp, (float)ScnEffect.angle, (float)ScnEffect.mx, (float)ScnEffect.my);
					angle += (float)ScnEffect.angle;
				}
			}


			pDesc->xp = float(xp);
			pDesc->yp = float(yp);
			pDesc->sprno = sprno;
			pDesc->width = float(w);
			pDesc->height = float(h);
			pDesc->angle = angle;
			pDesc->opt = opt;
			pDesc->grade = FLOATROUNDINT(grade);
			pDesc->color = color;

			return TRUE;
		}

		//--------------------------------------------------------------------------------------
		//      ��Ƽ������ �׷��� Ʈ���� ���� ����(��� ��Ƽ���� ���� ����)
		//--------------------------------------------------------------------------------------
		void CPage::SetDrawingTrackScope(int begin/*=-1*/, int end/*=-1*/)
		{
			iDrawingTrackBegin = begin;
			iDrawingTrackEnd = end;
		}

		//--------------------------------------------------------------------------------------
		//      ��Ƽ�� �ִϸ��̼� ��� �Լ�
		//--------------------------------------------------------------------------------------
		BOOL CPage::DrawMultiScene(int x, int y, xSprite *pXSpr, int multiscn, DWORD time, BOOL loop, int n_opt, int n_grade, WORD n_color)
		{
			if(!pXSpr) 
				return FALSE;

			MSceneData* pMScene = pXSpr->multiscene.GetMScenePt(multiscn);

			if(!pMScene)
				return FALSE;

			if(loop && pMScene->TotPlayTime > 0) {
				if(time > 0 ) {
					time = time % pMScene->TotPlayTime;
					if(time == 0)
						time = pMScene->TotPlayTime;
				}
				else {
					time = time % pMScene->TotPlayTime;
				}
			}

			pMScene->UpdateAffectClipboxTrack();

			for(int track=0; track<pMScene->TotTrackNum; track++) 
			{
				// ���Ƿ� ������ Ʈ�� ��ȿ ������ �ʰ��ϴ��� �˻�
				if(iDrawingTrackBegin >= 0 && track < iDrawingTrackBegin)
					continue;
				if(iDrawingTrackEnd >= 0 && track > iDrawingTrackEnd)
					continue;

				TrackData* pTrack = pMScene->GetPtrTrack(track);
				if(pTrack) {
					// ���� Ʈ���̸� ��ŵ
					if(pTrack->bHideTrack)
						continue;

					switch(pTrack->TrackType)
					{
					case TTYPE_SCENE:
						{
							DWORD stime, etime;
							if(pXSpr->CalcTrackPlayTime(multiscn, track, &stime, &etime)) {
								if(time < stime || time > etime) 
									continue;

								BOOL bSetTempClip = FALSE;
								// �ӽ� Ŭ���� ���� ���� ���� ����
								RECT orgtmpclip;
								BOOL orgbclip;

								if(pTrack->AffectClipboxTrack >= 0) {
									TrackData* pClipTrack = pMScene->GetPtrTrack(pTrack->AffectClipboxTrack);
									if(pClipTrack && !pClipTrack->bHideTrack && pClipTrack->TrackType == TTYPE_CLIPBOX) {
										TRACK_CLIPBOX* pCbxData = (TRACK_CLIPBOX*)pClipTrack->pData;
										DWORD t1 = pClipTrack->Delay;
										DWORD t2 = pClipTrack->Delay + pCbxData->LifeTime;
										if(time >= t1 && time <= t2) {
											CPI_CLIPBOX cpi;
											if(pClipTrack->GetInterpol_Clipbox(time, &cpi)) {
												bSetTempClip = TRUE;
												// �ӽ� Ŭ���� ���� ���� ���� ����
												orgtmpclip.left = TClipX1;
												orgtmpclip.top = TClipY1;
												orgtmpclip.right = TClipX2;
												orgtmpclip.bottom = TClipY2;
												orgbclip = bEnableTempClip;
												// �ӽ� Ŭ���� ������ Ŭ���ڽ��� ���߾� ����
												OffsetRect(&cpi.Rect, x, y);
												if(bEnableTempClip) {
													// �̹� Ŭ������ �����Ǿ� �ִٸ� Ŭ���ڽ� ������ ���� �޴´�
													SetTempClip(max(cpi.Rect.left, TClipX1), 
														max(cpi.Rect.top, TClipY1), 
														min(cpi.Rect.right, TClipX2), 
														min(cpi.Rect.bottom, TClipY2));
												}
												else {
													SetTempClip(cpi.Rect.left, cpi.Rect.top, cpi.Rect.right, cpi.Rect.bottom);
												}

												EnableTempClip(TRUE);
											}
										}
									}
								}

								CPI_SCENE cpi;
								if(pTrack->GetInterpol_Scene(time, &cpi)) {	
									TRACK_SCENE* pScnData = (TRACK_SCENE*)pTrack->pData;
									BOOL bscnloop = TRUE;
									if(pScnData->LifeTime != 0) {
										// ������Ÿ���� 0�� �ƴ� ���� �ݺ� ȸ����ŭ �÷����� ���Ŀ� ������ �����Ӹ� ǥ�õǵ��� �Ѵ�
										SceneData* pScene = pXSpr->GetScenePt(pScnData->SceneNo);
										if(pScene) 
											if(cpi.DeltaTime >  pScene->TotPlayTime * pScnData->RepeatCnt)
												bscnloop = FALSE;
									}

									if(cpi.Alpha != 31 || cpi.ZoomX != 100 || cpi.ZoomY != 100 || cpi.Angle != 0) {
										SCNEFFECTDESC effect;
										effect.mx = x + cpi.Pos.x + pScnData->CenterX;
										effect.my = y + cpi.Pos.y + pScnData->CenterY;
										effect.alpha = cpi.Alpha;
										effect.zoomx = cpi.ZoomX;
										effect.zoomy = cpi.ZoomY;
										effect.angle = cpi.Angle;

										SetSceneEffect(&effect);
									}
									else {
										SetSceneEffect(NULL);
									}

									DrawScene(x + cpi.Pos.x, y + cpi.Pos.y, pXSpr, pScnData->SceneNo, cpi.DeltaTime, bscnloop, n_opt, n_grade, n_color);

									if(bScnEffect)
										SetSceneEffect(NULL);
								}

								if(bSetTempClip) {
									SetTempClip(orgtmpclip.left, orgtmpclip.top, orgtmpclip.right, orgtmpclip.bottom);
									EnableTempClip(orgbclip);
								}
							}
						} break;

					case TTYPE_SOUND:
						{
							//TRACK_SOUND* pSndData = (TRACK_SOUND*)pTrack->pData;
						} break;

					case TTYPE_CLIPBOX:
						{
							//TRACK_CLIPBOX* pCbxData = (TRACK_CLIPBOX*)pTrack->pData;
						} break;

					case TTYPE_SCRIPT:
						{
							//TRACK_SCRIPT* pScrData = (TRACK_SCRIPT*)pTrack->pData;
						} break;

					default:
						break;
					}
				}
			}

			return TRUE;
		}

		//--------------------------------------------------------------------------------------
		//      ��Ƽ���� Ư�� �� Ʈ���� �� ��� ������ ��� �Լ�
		//--------------------------------------------------------------------------------------
		BOOL CPage::GetMSceneDrawingCellInfo(int x, int y, xSprite *pXSpr, int multiscn, int track, int cell, DWORD time, BOOL loop, DRAWINGCELLDESC* pDesc)
		{
			if(!pXSpr) 
				return FALSE;

			MSceneData* pMScene = pXSpr->multiscene.GetMScenePt(multiscn);

			if(!pMScene)
				return FALSE;

			if(loop && pMScene->TotPlayTime > 0)
			{
				if(time > 0 )
				{
					time = time % pMScene->TotPlayTime;
					if(time == 0)
					{
						time = pMScene->TotPlayTime;
					}
				}
				else
				{
					time = time % pMScene->TotPlayTime;
				}
			}


			TrackData* pTrack = pMScene->GetPtrTrack(track);
			if(!pTrack || !pTrack->pData) 
				return FALSE;

			if(pTrack->TrackType != TTYPE_SCENE)
				return FALSE;

			CPI_SCENE cpi;
			if(!pTrack->GetInterpol_Scene(time, &cpi)) 
				return FALSE;

			TRACK_SCENE* pScnData = (TRACK_SCENE*)pTrack->pData;
			int scn = pScnData->SceneNo;

			SceneData* pScene = pXSpr->scene.GetScenePt(scn);

			if(!pScene)
				return FALSE;

			if(time < pTrack->Delay)
				return FALSE;

			if(pScnData->LifeTime == 0)
			{
				if(time > pTrack->Delay + (pScene->TotPlayTime * pScnData->RepeatCnt))
					return FALSE;
			}
			else 
			{
				if(time > pTrack->Delay + pScnData->LifeTime)
					return FALSE;
			}

			BOOL bscnloop = TRUE;
			if(pScnData->LifeTime != 0)
			{
				// ������Ÿ���� 0�� �ƴ� ���� �ݺ� ȸ����ŭ �÷����� ���Ŀ� ������ �����Ӹ� ǥ�õǵ��� �Ѵ�
				SceneData* pScene2 = pXSpr->GetScenePt(pScnData->SceneNo);

				if(pScene2) 
				{
					if(cpi.DeltaTime >  pScene2->TotPlayTime * pScnData->RepeatCnt)
					{
						bscnloop = FALSE;
					}
				}
			}

			// �ð��� �� �������� ��ȯ
			time -= pTrack->Delay;

			// �־��� �ð��� �÷��� �Ǿ�� �� �������� ã�´�
			int frame = pScene->SearchFrame(time, bscnloop);

			if(frame < 0)
				return FALSE;

			// ������ ������ ��Ÿ���� ����
			float delta = pScene->GetFrameDelta(frame, time, bscnloop);

			if(cpi.Alpha != 31 || cpi.ZoomX != 100 || cpi.ZoomY != 100 || cpi.Angle != 0)
			{
				SCNEFFECTDESC effect;
				effect.mx = x + cpi.Pos.x + pScnData->CenterX;
				effect.my = y + cpi.Pos.y + pScnData->CenterY;
				effect.alpha = cpi.Alpha;
				effect.zoomx = cpi.ZoomX;
				effect.zoomy = cpi.ZoomY;
				effect.angle = cpi.Angle;

				SetSceneEffect(&effect);
			}
			else
			{
				SetSceneEffect(NULL);
			}

			// �� ��� ������ ����
			if(!GetDrawingCellInfo(x + cpi.Pos.x, y + cpi.Pos.y, pXSpr, scn, frame, cell, delta, pDesc))
				return FALSE;

			if(bScnEffect)
				SetSceneEffect(NULL);

			return TRUE;
		}

		WORD GetRGBmix(int nR, int nG, int nB)
		{
			return RGBmix(nR, nG, nB);
		}

	}//namespace GRAPHICGDI

}//namespace NMBASE

