// AvatarDisplay.h : main header file for the AVATARDISPLAY DLL
//

#ifndef __AVATARDISPLAY_H__
#define __AVATARDISPLAY_H__

#include <windows.h>
#include <stdio.h>

typedef UINT HAVATAR;

/////////////////////////////////////////////////////////////////////////////

 BOOL InitAvatarDisplayDLL(const char *pstrBaseDir);
 BOOL DestroyAvatarDisplayDLL();
 HAVATAR CreateAvatarResource();
 BOOL DestroyAvatarResource(HAVATAR h);
 BOOL SetAvatar(HAVATAR h, short *pLayerItem);
 BOOL SetForceAvatar(HAVATAR h, short *pLayerItem, BOOL bAutoPair);
 BOOL SetBackgroundVisible(HAVATAR h, BOOL bShow);
 BOOL SetAvatarSize(HAVATAR h, int width, int height);
 BOOL SetAvatarOffset(HAVATAR h, int ox, int oy);
 BOOL ResetAvatarAnimation(HAVATAR h);
 BOOL IsAvatarDownloading(HAVATAR h);
 BOOL SetAvatarAnimationTime(HAVATAR h, WORD tick);
 BOOL SetAvatarBackBitmap(HAVATAR h, HBITMAP hBmp, int srcx, int srcy);
 BOOL DrawAvatarToMemory(int xp, int yp, HAVATAR h, LONG txl, LONG tyl, void *lpbit, LONG lpitch, BOOL bMirror, int TargetPixelFormat);
 BOOL DrawAvatarToDC(int xp, int yp, HAVATAR h, HDC hdcDest, BOOL bMirror);
 BOOL DrawAvatarToDC_SmoothZoom(int xp, int yp, HAVATAR h, HDC hdcDest, int tx, int ty, COLORREF crBackColor, BOOL bMirror);
 char* GetAvatarStatusString();


#endif // !defined(__AVATARDISPLAY_H__)
