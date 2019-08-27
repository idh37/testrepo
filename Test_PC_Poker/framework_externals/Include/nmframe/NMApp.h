#pragma once

#include "FrameDefine.h"

#define MSG_MODAL_DIALOG_TIMER			"modaldlg_timer_proc"	///< 모달 다이얼로그에서 주기적으로 호출

namespace NMBASE
{
	namespace FRAME
	{
		// 사운드 파일 타입
		enum SOCKETTYPE_ENUM
		{
			SOCKETTYPE_DEFAULT	= 0,		//< Default(이전에 사용되는 Socket)
			SOCKETTYPE_CAMEL	= 1,		//< Camel(서버팀 새로운 NetworkFrame)			
		};

		class NMFRAME_CLASS CNMApp : public CWinApp
		{
			DECLARE_MESSAGE_MAP();

		public:
			CNMApp();
			virtual ~CNMApp();

			void					RegisterDefaultHandlers();			///< 기본 Handlers 등록
			//virtual void			RegisterHandlers();					///< Handlers 등록
			void					RegisterPlatformConverters();		///< PlatformConverters 등록

			virtual BOOL 			LoadDefaultObjects( );				///< (필수)기본 Object 로드
			virtual BOOL 			LoadProcessObjects( );				///< Process Object 로드

			virtual BOOL			InitInstance( void );				///< Instance 초기화
			virtual int				ExitInstance( void );				///< Instance 종료

			virtual CFrameWnd*		MakeFrameInstance(int nWidth, int nHeight);			///< FrameInstance 생성
			virtual void*			MakeGameInstance();					///< GameInstance 생성

			virtual BOOL			SetLoinInfo();						///< (필수)로그인 정보를 얻는다.(ID/PW/IP/PORT....)

			virtual void			SetExceptionInfo();					///< (필수) 게임마다 Exception정보가 다름

			virtual LRESULT			ProcessModalDialog(WPARAM &, LPARAM &);

			virtual void			OnDraw();							///< GameLoop

			virtual void			OnUpdate();							///< Timer대신 매 틱마다 업데이트가 필요 할때...


			int						GetWindowWidth(){return m_nWindowWidth;}	///< 윈도우 해상도 Width
			int						GetWindowHeight(){return m_nWindowHeight;}	///< 윈도우 해상도 Height

			virtual BOOL			InitEngine(CWnd *pWindow);					///< User Engine 초기화


			void					SetExitFlag(BOOL bFlag){m_bExitFlag = bFlag;}	///< 종료시에는 Draw/Update를 멈춘다.
			BOOL					GetExitFlag(){return m_bExitFlag;}


			void					SetUseDefaultSock(SOCKETTYPE_ENUM bDefaultSock){m_eSocketType = bDefaultSock;}		///< DefaultSock사용(False: Camel Sock사용)
			SOCKETTYPE_ENUM			GetUseDefaultSock(){return m_eSocketType;}

		protected:
			virtual void			SetIcon();							///< ICon을 세팅한다.

			// QA쪽 요구로 뮤텍스 체크를 빼야할 경우 오버라이딩해서 쓰려고 virtual로 변경함[12/2/2011 guesty]
			virtual BOOL CheckMutex(LPCSTR lpAppName);

		private:
			virtual int				Run();								///< MFC 메인 루프

			void DestroyMutex();

			void SetCurPathToRunFilePath();

			virtual BOOL AnalyzeRunParam();									///< RunParam값을 얻어온다.

			BOOL CreateStrManager();			

		public:
			virtual BOOL			OnIdle(LONG lCount);

		protected:
			int						m_nWindowWidth;						///< (필수)윈도우 해상도 Width
			int						m_nWindowHeight;					///< (필수)윈도우 해상도 Height

		private:
			HANDLE					m_hMutex;

			BOOL					m_bExitFlag;						///< 종료시에는 Draw/Update를 멈춘다.

			SOCKETTYPE_ENUM			m_eSocketType;						///< SocketType
		};

	}//namespace FRAME

}//namespace NMBASE
