#pragma once

UPG_BASE_EXPORT extern void ResistryGame(void);
UPG_BASE_EXPORT extern CFrameWnd *CreateMainFrame(void);
UPG_BASE_EXPORT extern void DestroyMainFrame(void);
UPG_BASE_EXPORT extern BOOL CheckMutex(LPCSTR lpAppName);
UPG_BASE_EXPORT extern BOOL CreateStrManager(void);
UPG_BASE_EXPORT extern void InitGoGameDirect(void);
UPG_BASE_EXPORT extern void SetCurPathToRunFilePath(void);
UPG_BASE_EXPORT extern bool AnalyzeArgument(void);
UPG_BASE_EXPORT extern void MainFrameDraw(void);