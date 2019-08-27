#pragma once
//#ifndef __H_UTIL_H__
//#define __H_UTIL_H__




extern float cos_look[360];
extern float sin_look[360];
extern void Build_Sin_Cos_Tables(void);

extern int GetRandomValue(int startVal, int endVal);		// 난수값 얻어 오기
extern float Linear(float v0, float v1, float t);
extern int GetBigNumDivide(int iBigNum, int *ilp1Num, int *ilp10Num, int *ilp100Num, int *ilp1000Num);

extern void FilePrintf(char *clpFileName, char *clpStr);
extern void XChange( int *a, int *b );
extern void GetColorbyARGB(DWORD dwARGB, int *ilpA, int *ilpR, int *ilpG, int *ilpB);

extern int GetAngle(int iStandardPixelPosX, int iStandardPixelPosY, int iCheckPixelPosX, int iCheckPixelPosY);
extern bool ValidWindowPixelFormat();


extern void SetMsgData(char *clpMsgData, int iMsgSize);
extern void GetMsgData(char *clpMsgData, char *clpData, int iMsgSize);
extern char* GetAllMsgData();
extern int GetAllMsgDataSize();
extern void MsgDataCleanup();
extern bool ImpactCehck(int iLeft1, int iTop1, int iRight1, int iBottom1, int iLeft2, int iTop2, int iRight2, int iBottom2 );
extern int GetLengthOfTwoPoint(int iPos_1X, int iPos_1Y, int iPos_2X, int iPos_2Y);
extern bool CheckPosInRect(int iCheckPosX, int iCheckPosY, int iRectLeft,int iRectTop,  int iRectRight, int iRectBottom);

extern int GetNowIntDate();
extern __int64 getFileSize(char *filename);

template <class A, class B> inline A Min(A a, B b)
{
    if(a > b) return b;
    else return a;
}

template <class A, class B> inline A Max(A a, B b)
{
    if(a > b) return a;
    else return b;
}
