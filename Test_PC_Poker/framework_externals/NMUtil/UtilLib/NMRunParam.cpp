// HTransRegion.cpp: implementation of the CHTransRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NMRunParam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace NMBASE
{
	namespace UTIL
	{

		namespace NMNMRUNPARAMAPIFUNC
		{
			/**
			@brif - 수동으로 DLL 파일 경로를 선택한다. (디버깅용)
			@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
			@param[in] szFileName - 수동으로 설정할 DLL 파일의 경로를 입력한다.
			*/
			NMRUNPARAM_RETCODE SetDLLFileName(LPCSTR szFileName)
			{
				return NMRUNPARAMAPIFUNC::SetDLLFileName(szFileName);
			}

			// convert return Code 2 String 
			/**
			@brif - @ref ERROR_NMRUNPARAM 열거자 코드를 문자열 코드로 변환한다.
			@returns 입력된 열거자 @ref ERROR_NMRUNPARAM 코드 값을 문자열 코드로 변환한 값을 반환한다.
			@param[in] retCode - 문자열 코드로 변환할 열거자 @ref ERROR_NMRUNPARAM 값을 입력한다.
			*/
			LPCSTR ConvertRetCode2String(NMRUNPARAM_RETCODE retCode)
			{
				return NMRUNPARAMAPIFUNC::ConvertRetCode2String(retCode);
			}

			/**
			@brif - 실행 결과 코드 값(WinExec 나 ShellExcute 의 리턴값)을 문자열 코드로 변환한다.
			@returns 입력된 실행 결과 코드 값(WinExec 나 ShellExcute 의 리턴값)을 문자열 코드로 변환한 값을 반환한다.
			@param[in] uErr - 문자열 코드로 변환할 실행 결과 코드 값(WinExec 나 ShellExcute 의 리턴값)을 입력한다.
			*/
			//LPCSTR ConvertRunProgError2String(UINT uErr)
			//{
			//	return NMRUNPARAMAPIFUNC::ConvertRunProgError2String(uErr);
			//}
		}; //namespace NMNMRUNPARAMAPIFUNC

		/// @brief 환견 변수 관련 NMRunParam API 함수
		namespace NMNMRUNPARAMAPIFUNC_ENVVAR
		{
			// Process Environment Function
			/*
			@brif - 주어진 환경 변수 이름 코드를 바탕으로 저장된 환경 변수를 가져온다.
			@returns 주어진 환경 변수 이름 코드를 바탕으로 저장된 환경 변수를 반환한다.
			@param[in] eName - 가져올 환경 변수 값의 환경 변수 이름 코드를 입력한다.
			*/
			LPCSTR GetEnvName(NMRUNPARAMAPIFUNC_ENVVAR::ENVVARNAME eName)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::GetEnvName(eName);
			}

			/*
			@brif - 주어진 환경 변수 이름을 바탕으로 저장된 환경 변수를 가져온다.
			@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
			@param[in] szEnvName - 가져올 환경 변수 값의 환경 변수 이름를 입력한다.
			@param[out] szBuffer - 가져올 환경 변수 값이 저장될 버퍼의 포인터를 입력한다.
			@param[in] dwBufferLen - 가져올 환경 변수 값이 저장될 버퍼의 길이를 입력한다.
			*/
			NMRUNPARAM_RETCODE GetEnvVar(LPCSTR szEnvName, LPSTR szBuffer, DWORD dwBufferLen)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::GetEnvVar(szEnvName, szBuffer, dwBufferLen);
			}

			/*
			@brif - 주어진 환경 변수 이름 코드를 바탕으로 저장된 환경 변수를 가져온다.
			@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
			@param[in] eName - 가져올 환경 변수 값의 환경 변수 이름 코드를 입력한다.
			@param[out] szBuffer - 가져올 환경 변수 값이 저장될 버퍼의 포인터를 입력한다.
			@param[in] dwBufferLen - 가져올 환경 변수 값이 저장될 버퍼의 길이를 입력한다.
			*/
			NMRUNPARAM_RETCODE GetEnvVar(NMRUNPARAMAPIFUNC_ENVVAR::ENVVARNAME eName, LPSTR szBuffer, DWORD dwBufferLen)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::GetEnvVar(eName, szBuffer, dwBufferLen);
			}

			/*
			@brif - 주어진 환경 변수 이름 코드를 바탕으로 저장된 환경 변수의 문자열 길이를 가져온다.
			@returns 입력된 환경 변수 이름 코드를 바탕으로 저장된 환경 변수의 문자열 길이를 반환한다.
			@param[in] eName - 가져올 환경 변수 값의 환경 변수 이름 코드를 입력한다.
			*/
			DWORD GetEnvVarLength(NMRUNPARAMAPIFUNC_ENVVAR::ENVVARNAME eName)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::GetEnvVarLength(eName);
			}

			/*
			@brif - 주어진 환경 변수 이름을 바탕으로 저장된 환경 변수의 문자열 길이를 가져온다.
			@returns 입력된 환경 변수 이름을 바탕으로 저장된 환경 변수의 문자열 길이를 반환한다.
			@param[in] szEnvName - 가져올 환경 변수 값의 환경 변수 이름을 입력한다.
			*/
			DWORD GetEnvVarLength(LPCSTR szEnvName)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::GetEnvVarLength(szEnvName);
			}

			/*
			@brif - 주어진 환경 변수 이름 코드에 환경 변수 값을 저장한다.
			@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
			@param[in] eName - 저장할 환경 변수 이름 코드 값을 입력한다.
			@param[in] szValue - 저장할 환경 변수 값을 입력한다.
			*/
			NMRUNPARAM_RETCODE SetEnvVar(NMRUNPARAMAPIFUNC_ENVVAR::ENVVARNAME eName, LPCSTR szValue)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::SetEnvVar(eName, szValue);
			}

			/*
			@brif - 주어진 환경 변수 이름 에 환경 변수 값을 저장한다.
			@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
			@param[in] szEnvName - 저장할 환경 변수 이름을 입력한다.
			@param[in] szValue - 저장할 환경 변수 값을 입력한다.
			*/
			NMRUNPARAM_RETCODE SetEnvVar(LPCSTR szEnvName, LPCSTR szValue)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::SetEnvVar(szEnvName, szValue);
			}

			/*
			@brif - 사이트 코드 환경 변수 값을 가져온다. (미구현)
			@returns 사이트 코드 환경 변수 값을 반환한다.
			*/
			LPCSTR GetSiteName()
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::GetSiteName();
			}

			/*
			@brif - 사이트 코드 환경 변수 값을 설정한다. (미구현)
			@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
			@param[in] szSiteName - 사이트 코드 환경 변수 값에 저장될 사이트 이름을 입력한다.
			*/
			NMRUNPARAM_RETCODE SetSiteName(LPCSTR szSiteName)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::SetSiteName(szSiteName);
			}

			/*
			@brif - 공통 모듈 경로를 가져온다.
			@returns 공통 모듈 경로를 반환한다.
			*/
			LPCSTR GetCommonPath()
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::GetCommonPath();
			}

			/*
			@brif - 공통 모듈 경로를 설정한다.
			@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
			@param[in] szCommonPath - 설정할 공통 모듈 경로를 입력한다.
			*/
			NMRUNPARAM_RETCODE SetCommonPath(LPCSTR szCommonPath)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::SetCommonPath(szCommonPath);
			}
		}; //namespace NMNMRUNPARAMAPIFUNC_ENVVAR


		/// @brief Command line Argument 관련 NMRunParam API 함수
		namespace NMNMRUNPARAMAPIFUNC_CMDARG
		{
			// Command line argument function
			/*
			@brif - Command line Argument 갯수를 가져온다.
			@returns Command line Argument 갯수를 반환한다.
			*/
			DWORD GetCount()
			{
				return NMRUNPARAMAPIFUNC_CMDARG::GetCount();
			}

			/*
			@brif - 지시된 인덱스의 Command line Argument 값을 가져온다.
			@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
			@param[in] iArgIdx - 가져올 Command line Argument 의 인덱스를 입력한다.
			@param[out] szBuffer - 주어진 인덱스의 Command line Argument 값이 저장될 버퍼를 입력한다.
			@param[in] dwBufferLen - 주어진 인덱스의 Command line Argument 값이 저장될 버퍼의 길이를 입력한다.
			*/
			NMRUNPARAM_RETCODE GetArg(unsigned short iArgIdx, LPSTR szBuffer, DWORD dwBufferLen)
			{
				return NMRUNPARAMAPIFUNC_CMDARG::GetArg(iArgIdx, szBuffer, dwBufferLen);
			}

			/*
			@brif - 지시된 인덱스의 Command line Argument 값의 문자열 길이를 가져온다.
			@returns 지시된 인덱스의 Command line Argument 값의 문자열 길이를 반환한다.
			@param[in] iArgIdx - 가져올 Command line Argument 의 인덱스를 입력한다.
			*/
			DWORD GetArgLength(unsigned short iArgIdx)
			{
				return NMRUNPARAMAPIFUNC_CMDARG::GetArgLength(iArgIdx);
			}

			// Command line argument extended function
			/*
			@brif - 현재 실행된 파일의 경로를 가져온다.
			@returns 현재 실행된 파일의 경로를 반환한다.
			*/
			LPCSTR GetRunFilePath()
			{
				return NMRUNPARAMAPIFUNC_CMDARG::GetRunFilePath();
			}

			/*
			@brif - 실행 파라미터 복호화 키(Command line Argument 인덱스 1이 지칭하는 값)을 가져온다.
			@returns 실행 파라미터 복호화 키(Command line Argument 인덱스 1이 지칭하는 값)을 반환한다.
			*/
			LPCSTR GetRunParamKey()
			{
				return NMRUNPARAMAPIFUNC_CMDARG::GetRunParamKey();
			}

			/*
			@brif - 현재 프로세스의 Current Directory 를 실행파일 경로로 변환한다.
			@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
			*/
			NMRUNPARAM_RETCODE SetCurPathToRunFilePath()
			{
				return NMRUNPARAMAPIFUNC_CMDARG::SetCurPathToRunFilePath();
			}
		}; //namespace NMNMRUNPARAMAPIFUNC_CMDARG

		/// @brief 실행 파라미터 암호화 키 관련 NMRunParam API 함수
		namespace NMNMRUNPARAMAPIFUNC_RUNPARAMKEY
		{
			/*
			@brif - 실행 파라미터 암호화 키를 새로 생성한다.
			@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
			@param[out] szBuffer - 실행 파라미터 암호화 키가 저장될 버퍼의 포인터를 입력한다.
			@param[in] dwLength - 실행 파라미터 암호화 키가 저장될 버퍼의 길이를 입력한다.
			*/
			NMRUNPARAM_RETCODE Create(LPSTR szBuffer, DWORD dwLength)
			{
				return NMRUNPARAMAPIFUNC_RUNPARAMKEY::Create(szBuffer, dwLength);
			}
			/*
			@brif - 새로 생성될 실행 파라미터 암호화 키를 저장되는데 필요한 버퍼의 길이를 가져온다.
			@returns 새로 생성될 실행 파라미터 암호화 키를 저장되는데 필요한 버퍼의 길이를 반환한다.
			*/
			//DWORD GetNeedLength();
			//{
			//	return NMRUNPARAMAPIFUNC_RUNPARAMKEY::GetNeedLength();
			//}
		}; //namespace NMNMRUNPARAMAPIFUNC_RUNPARAMKEY

		CNMNMRunParam::CNMNMRunParam()
		{
			m_pParam = new CNMRunParam;
		}

		CNMNMRunParam::~CNMNMRunParam()
		{
			if(m_pParam) delete m_pParam;
			m_pParam = NULL;
		}

		/*
		@brif - 실행 파라미터를 환경변수로 부터 읽어온다.
		@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
		@param[in] szDecKey - 실행 파라미터 복호화 키를 입력한다.@br
		해당 값이 NULL일 경우 Command Line Argument 1번째 값을 읽어 처리한다.
		@param[in] bClearEnv - 실행 파라미터 로드후 환경 변수에서 데이터를 제거할지를 입력한다. true를 입력할경우 제거되며, false를 입력할 경우 제거되지 않는다.
		*/
		NMRUNPARAM_RETCODE CNMNMRunParam::Load(LPCSTR szDecKey/* = NULL*/, bool bClearEnv/* = true*/)
		{
			return m_pParam->Load(szDecKey, bClearEnv);
		}

		/*
		@brif - 현재 클래스에 저장된 파라미터를 암호화하여 환경 변수에 저장한다.
		@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
		@param[in] szKey - 환경 변수에 저장될때 암호화할 키값을 입력한다.
		*/
		NMRUNPARAM_RETCODE CNMNMRunParam::Save(LPCSTR szKey)
		{
			return m_pParam->Save(szKey);
		}

		/*
		@brif - 주어진 프로그램을 현재 클래스의 저장된 파라미터를 전달하여 실행한다.
		@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
		@param[in] lpszRunFile - 실행할 프로그램 파일을 입력한다.
		@param[out] pRetExecCode - 주어진 프로그램의 실행 결과값이 저장될 포인터를 입력한다. NULL일 경우 저장되지 않는다.
		@param[in] szEncKey - 현재 클래스의 저장된 파라미터가 암호화될 암호화키를 입력한다. NULL일 경우 자동으로 생성하여 전달한다.
		*/
		NMRUNPARAM_RETCODE CNMNMRunParam::RunProgram(LPCSTR lpszRunFile,UINT* pRetExecCode/* = NULL*/, LPCSTR szEncKey/* = NULL*/)
		{
			return m_pParam->RunProgram(lpszRunFile, pRetExecCode, szEncKey);
		}

		/*
		@brif - 주어진 이름의 실행 파라미터 값을 가져온다.
		@returns 주어진 이름의 실행 파라미터 값을 반환한다.
		@param[in] lpszName - 
		*/
		LPCSTR CNMNMRunParam::GetParam(LPCSTR lpszName)
		{
			return m_pParam->GetParam(lpszName);
		}

		/*
		@brif - 주어진 이름의 실행 파라미터 값을 설정한다.
		@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
		@param[in] lpszName - 설정할 실행 파라미터 이름을 입력한다.
		@param[in] lpszValue - 설정할 실행 파라미터 값을 입력한다.
		*/
		NMRUNPARAM_RETCODE CNMNMRunParam::SetParam(LPCSTR lpszName, LPCSTR lpszValue)
		{
			return m_pParam->SetParam(lpszName, lpszValue);
		}

		/*
		@brif - 주어진 이름의 실행 파라미터 값을 제거한다.
		@returns 성공하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 반환하고, 실패하였을 경우 @ref ERROR_NMRUNPARAM_SUCCESS 를 제외한 열거자 @ref ERROR_NMRUNPARAM 값들 중에 하나를 반환한다.
		@param[in] lpszName - 제거할 실행 파라미터 이름을 입력한다.
		*/
		NMRUNPARAM_RETCODE CNMNMRunParam::RemoveParam(LPCSTR lpszName)
		{
			return m_pParam->RemoveParam(lpszName);
		}

		/*
		@brif - 주어진 인덱스의 실행 파라미터 이름을 가져온다.
		@returns 주어진 인덱스의 실행 파라미터 이름을 반환한다.
		@param[in] dwIdx - 인덱스를 입력한다.
		*/
		LPCSTR CNMNMRunParam::GetParamName(DWORD dwIdx)
		{
			return m_pParam->GetParamName(dwIdx);
		}

		/*
		@brif - 저장된 실행 파라미터 갯수를 가져온다.
		@returns 저장된 실행 파라미터 갯수를 반환한다.
		*/
		DWORD CNMNMRunParam::GetParamCount()
		{
			return m_pParam->GetParamCount();
		}

		/*
		@brif - 주어진 이름의 실행 파라미터가 존재하는지 체크한다,
		@returns 주어진 이름의 실행 파라미터가 존재하면 true값을, 존재하지 않으면 false값을 반환한다.
		@param[in] lpszName - 체크할 실행 파라미터의 이름을 입력한다.
		*/
		bool CNMNMRunParam::ExistsParam(LPCSTR lpszName)
		{
			return m_pParam->ExistsParam(lpszName);
		}

	}//namespace UTIL

}//namespace NMBASE