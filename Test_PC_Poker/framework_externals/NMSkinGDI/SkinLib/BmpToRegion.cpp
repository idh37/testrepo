// xmas.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include<windows.h>
#include<stdlib.h>
#include<math.h>
#include "BmpToRegion.h"

namespace NMBASE
{
	namespace SKINGDI
	{
		BOOL CBmpRegion::SetWindowRgn(HWND hwnd, HBITMAP hbmp,  COLORREF cTransColor, COLORREF cTolerance)
		{
			if(!hwnd) return FALSE;
			if(hRgn) ::DeleteObject(hRgn);
			hRgn = BitmapToRegion(hbmp, cTransColor, cTolerance);
			if(!hRgn) return FALSE;
			return ::SetWindowRgn(hwnd, hRgn, TRUE);
		}



		HRGN BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance)
		{
			HRGN hRgn = NULL;

			if(hBmp)
			{
				// Create a memory DC inside which we will scan the bitmap content
				HDC hMemDC = CreateCompatibleDC(NULL);
				if(hMemDC)
				{
					// Get bitmap size
					BITMAP bm;
					GetObject(hBmp, sizeof(bm), &bm);

					// Create a 32 bits depth bitmap and select it into the memory DC 
					BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
						sizeof(BITMAPINFOHEADER),	// biSize 
						bm.bmWidth,					// biWidth; 
						bm.bmHeight,				// biHeight; 
						1,							// biPlanes; 
						32,							// biBitCount 
						BI_RGB,						// biCompression; 
						0,							// biSizeImage; 
						0,							// biXPelsPerMeter; 
						0,							// biYPelsPerMeter; 
						0,							// biClrUsed; 
						0							// biClrImportant; 
					};
					VOID * pbits32; 
					HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
					if(hbm32)
					{
						HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);

						// Create a DC just to copy the bitmap into the memory DC
						HDC hDC = CreateCompatibleDC(hMemDC);
						if(hDC)
						{
							// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits)
							BITMAP bm32;
							GetObject(hbm32, sizeof(bm32), &bm32);
							while (bm32.bmWidthBytes % 4)
								bm32.bmWidthBytes++;

							// Copy the bitmap into the memory DC
							HBITMAP holdBmp2 = (HBITMAP)SelectObject(hDC, hBmp);
							BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);

							// For better performances, we will use the ExtCreateRegion() function to create the
							// region. This function take a RGNDATA structure on entry. We will add rectangles by
							// amount of ALLOC_UNIT number in this structure.
#define ALLOC_UNIT	100
							DWORD maxRects = ALLOC_UNIT;
							HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
							RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
							pData->rdh.dwSize = sizeof(RGNDATAHEADER);
							pData->rdh.iType = RDH_RECTANGLES;
							pData->rdh.nCount = pData->rdh.nRgnSize = 0;
							SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

							BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;

							LONG *pXX = (LONG *)p32;
							// Keep on hand highest and lowest values for the "transparent" pixels
							BYTE lr = GetRValue(*pXX);//GetRValue(5);
							BYTE lg = GetGValue(*pXX);//GetGValue(5);
							BYTE lb = GetBValue(*pXX);//GetBValue(5);
							BYTE hr = min(0xff, lr + GetRValue(*pXX));//min(0xff, lr + GetRValue(5));
							BYTE hg = min(0xff, lg + GetGValue(*pXX));
							BYTE hb = min(0xff, lb + GetBValue(*pXX));

							// Scan each bitmap row from bottom to top (the bitmap is inverted vertically)

							for (int y = 0; y < bm.bmHeight; y++)
							{
								// Scan each bitmap pixel from left to right
								for (int x = 0; x < bm.bmWidth; x++)
								{
									// Search for a continuous range of "non transparent pixels"
									int x0 = x;
									LONG *p = (LONG *)p32 + x;
									while (x < bm.bmWidth)
									{
										BYTE b = GetRValue(*p);
										if(b >= lr && b <= hr)
										{
											b = GetGValue(*p);
											if(b >= lg && b <= hg)
											{
												b = GetBValue(*p);
												if(b >= lb && b <= hb)
													// This pixel is "transparent"
													break;
											}
										}
										p++;
										x++;
									}

									if(x > x0)
									{
										// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
										if(pData->rdh.nCount >= maxRects)
										{
											GlobalUnlock(hData);
											maxRects += ALLOC_UNIT;
											hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
											pData = (RGNDATA *)GlobalLock(hData);
										}
										RECT *pr = (RECT *)&pData->Buffer;
										SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
										if(x0 < pData->rdh.rcBound.left)
											pData->rdh.rcBound.left = x0;
										if(y < pData->rdh.rcBound.top)
											pData->rdh.rcBound.top = y;
										if(x > pData->rdh.rcBound.right)
											pData->rdh.rcBound.right = x;
										if(y+1 > pData->rdh.rcBound.bottom)
											pData->rdh.rcBound.bottom = y+1;
										pData->rdh.nCount++;

										// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
										// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
										if(pData->rdh.nCount == 2000)
										{
											HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
											if(hRgn)
											{
												CombineRgn(hRgn, hRgn, h, RGN_OR);
												DeleteObject(h);
											}
											else
												hRgn = h;
											pData->rdh.nCount = 0;
											SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
										}
									}
								}

								// Go to next row (remember, the bitmap is inverted vertically)
								p32 -= bm32.bmWidthBytes;
							}

							// Create or extend the region with the remaining rectangles
							HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
							if(hRgn)
							{
								CombineRgn(hRgn, hRgn, h, RGN_OR);
								DeleteObject(h);
							}
							else
								hRgn = h;

							// Clean up
							SelectObject(hDC, holdBmp2);
							DeleteDC(hDC);
							//�޸� �����ϴ� �������� 
							GlobalFree(hData);
						}

						DeleteObject(SelectObject(hMemDC, holdBmp));
					}

					//�޸� �����ϴ� �������� 
					DeleteObject(SelectObject(hMemDC, hbm32));
					DeleteDC(hMemDC);
				}	
			}

			return hRgn;
		}


		BOOL WindowResize2Bitmap(HWND hwnd, HGDIOBJ hbmp)
		{
			if(!hbmp || !hwnd)
				return FALSE;

			BITMAP bm = {0,};
			GetObject(hbmp, sizeof(bm), &bm);

			if(bm.bmWidth==0 || bm.bmHeight==0)
				return FALSE;

			// �������� ��ġ�� Z������ �����ϰ� ����� �����Ų��
			::SetWindowPos(hwnd, NULL, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE|SWP_NOZORDER);

			return TRUE;
		}


		BOOL WindowResize2Bitmap(HWND hwnd, LPCTSTR bmpfilename)
		{
			if(!bmpfilename || !hwnd)
				return FALSE;

			CBitmap bmp;

			HBITMAP hBitmap = NULL; 
			hBitmap = (HBITMAP)LoadImage(NULL, bmpfilename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
			if(!bmp.Attach(hBitmap))
				return FALSE;

			return WindowResize2Bitmap(hwnd, bmp.GetSafeHandle());
		}

	}//namespace SKINGDI


}//namespace NMBASE