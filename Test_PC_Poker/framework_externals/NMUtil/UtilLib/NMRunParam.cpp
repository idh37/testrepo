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
			@brif - �������� DLL ���� ��θ� �����Ѵ�. (������)
			@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
			@param[in] szFileName - �������� ������ DLL ������ ��θ� �Է��Ѵ�.
			*/
			NMRUNPARAM_RETCODE SetDLLFileName(LPCSTR szFileName)
			{
				return NMRUNPARAMAPIFUNC::SetDLLFileName(szFileName);
			}

			// convert return Code 2 String 
			/**
			@brif - @ref ERROR_NMRUNPARAM ������ �ڵ带 ���ڿ� �ڵ�� ��ȯ�Ѵ�.
			@returns �Էµ� ������ @ref ERROR_NMRUNPARAM �ڵ� ���� ���ڿ� �ڵ�� ��ȯ�� ���� ��ȯ�Ѵ�.
			@param[in] retCode - ���ڿ� �ڵ�� ��ȯ�� ������ @ref ERROR_NMRUNPARAM ���� �Է��Ѵ�.
			*/
			LPCSTR ConvertRetCode2String(NMRUNPARAM_RETCODE retCode)
			{
				return NMRUNPARAMAPIFUNC::ConvertRetCode2String(retCode);
			}

			/**
			@brif - ���� ��� �ڵ� ��(WinExec �� ShellExcute �� ���ϰ�)�� ���ڿ� �ڵ�� ��ȯ�Ѵ�.
			@returns �Էµ� ���� ��� �ڵ� ��(WinExec �� ShellExcute �� ���ϰ�)�� ���ڿ� �ڵ�� ��ȯ�� ���� ��ȯ�Ѵ�.
			@param[in] uErr - ���ڿ� �ڵ�� ��ȯ�� ���� ��� �ڵ� ��(WinExec �� ShellExcute �� ���ϰ�)�� �Է��Ѵ�.
			*/
			//LPCSTR ConvertRunProgError2String(UINT uErr)
			//{
			//	return NMRUNPARAMAPIFUNC::ConvertRunProgError2String(uErr);
			//}
		}; //namespace NMNMRUNPARAMAPIFUNC

		/// @brief ȯ�� ���� ���� NMRunParam API �Լ�
		namespace NMNMRUNPARAMAPIFUNC_ENVVAR
		{
			// Process Environment Function
			/*
			@brif - �־��� ȯ�� ���� �̸� �ڵ带 �������� ����� ȯ�� ������ �����´�.
			@returns �־��� ȯ�� ���� �̸� �ڵ带 �������� ����� ȯ�� ������ ��ȯ�Ѵ�.
			@param[in] eName - ������ ȯ�� ���� ���� ȯ�� ���� �̸� �ڵ带 �Է��Ѵ�.
			*/
			LPCSTR GetEnvName(NMRUNPARAMAPIFUNC_ENVVAR::ENVVARNAME eName)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::GetEnvName(eName);
			}

			/*
			@brif - �־��� ȯ�� ���� �̸��� �������� ����� ȯ�� ������ �����´�.
			@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
			@param[in] szEnvName - ������ ȯ�� ���� ���� ȯ�� ���� �̸��� �Է��Ѵ�.
			@param[out] szBuffer - ������ ȯ�� ���� ���� ����� ������ �����͸� �Է��Ѵ�.
			@param[in] dwBufferLen - ������ ȯ�� ���� ���� ����� ������ ���̸� �Է��Ѵ�.
			*/
			NMRUNPARAM_RETCODE GetEnvVar(LPCSTR szEnvName, LPSTR szBuffer, DWORD dwBufferLen)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::GetEnvVar(szEnvName, szBuffer, dwBufferLen);
			}

			/*
			@brif - �־��� ȯ�� ���� �̸� �ڵ带 �������� ����� ȯ�� ������ �����´�.
			@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
			@param[in] eName - ������ ȯ�� ���� ���� ȯ�� ���� �̸� �ڵ带 �Է��Ѵ�.
			@param[out] szBuffer - ������ ȯ�� ���� ���� ����� ������ �����͸� �Է��Ѵ�.
			@param[in] dwBufferLen - ������ ȯ�� ���� ���� ����� ������ ���̸� �Է��Ѵ�.
			*/
			NMRUNPARAM_RETCODE GetEnvVar(NMRUNPARAMAPIFUNC_ENVVAR::ENVVARNAME eName, LPSTR szBuffer, DWORD dwBufferLen)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::GetEnvVar(eName, szBuffer, dwBufferLen);
			}

			/*
			@brif - �־��� ȯ�� ���� �̸� �ڵ带 �������� ����� ȯ�� ������ ���ڿ� ���̸� �����´�.
			@returns �Էµ� ȯ�� ���� �̸� �ڵ带 �������� ����� ȯ�� ������ ���ڿ� ���̸� ��ȯ�Ѵ�.
			@param[in] eName - ������ ȯ�� ���� ���� ȯ�� ���� �̸� �ڵ带 �Է��Ѵ�.
			*/
			DWORD GetEnvVarLength(NMRUNPARAMAPIFUNC_ENVVAR::ENVVARNAME eName)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::GetEnvVarLength(eName);
			}

			/*
			@brif - �־��� ȯ�� ���� �̸��� �������� ����� ȯ�� ������ ���ڿ� ���̸� �����´�.
			@returns �Էµ� ȯ�� ���� �̸��� �������� ����� ȯ�� ������ ���ڿ� ���̸� ��ȯ�Ѵ�.
			@param[in] szEnvName - ������ ȯ�� ���� ���� ȯ�� ���� �̸��� �Է��Ѵ�.
			*/
			DWORD GetEnvVarLength(LPCSTR szEnvName)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::GetEnvVarLength(szEnvName);
			}

			/*
			@brif - �־��� ȯ�� ���� �̸� �ڵ忡 ȯ�� ���� ���� �����Ѵ�.
			@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
			@param[in] eName - ������ ȯ�� ���� �̸� �ڵ� ���� �Է��Ѵ�.
			@param[in] szValue - ������ ȯ�� ���� ���� �Է��Ѵ�.
			*/
			NMRUNPARAM_RETCODE SetEnvVar(NMRUNPARAMAPIFUNC_ENVVAR::ENVVARNAME eName, LPCSTR szValue)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::SetEnvVar(eName, szValue);
			}

			/*
			@brif - �־��� ȯ�� ���� �̸� �� ȯ�� ���� ���� �����Ѵ�.
			@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
			@param[in] szEnvName - ������ ȯ�� ���� �̸��� �Է��Ѵ�.
			@param[in] szValue - ������ ȯ�� ���� ���� �Է��Ѵ�.
			*/
			NMRUNPARAM_RETCODE SetEnvVar(LPCSTR szEnvName, LPCSTR szValue)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::SetEnvVar(szEnvName, szValue);
			}

			/*
			@brif - ����Ʈ �ڵ� ȯ�� ���� ���� �����´�. (�̱���)
			@returns ����Ʈ �ڵ� ȯ�� ���� ���� ��ȯ�Ѵ�.
			*/
			LPCSTR GetSiteName()
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::GetSiteName();
			}

			/*
			@brif - ����Ʈ �ڵ� ȯ�� ���� ���� �����Ѵ�. (�̱���)
			@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
			@param[in] szSiteName - ����Ʈ �ڵ� ȯ�� ���� ���� ����� ����Ʈ �̸��� �Է��Ѵ�.
			*/
			NMRUNPARAM_RETCODE SetSiteName(LPCSTR szSiteName)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::SetSiteName(szSiteName);
			}

			/*
			@brif - ���� ��� ��θ� �����´�.
			@returns ���� ��� ��θ� ��ȯ�Ѵ�.
			*/
			LPCSTR GetCommonPath()
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::GetCommonPath();
			}

			/*
			@brif - ���� ��� ��θ� �����Ѵ�.
			@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
			@param[in] szCommonPath - ������ ���� ��� ��θ� �Է��Ѵ�.
			*/
			NMRUNPARAM_RETCODE SetCommonPath(LPCSTR szCommonPath)
			{
				return NMRUNPARAMAPIFUNC_ENVVAR::SetCommonPath(szCommonPath);
			}
		}; //namespace NMNMRUNPARAMAPIFUNC_ENVVAR


		/// @brief Command line Argument ���� NMRunParam API �Լ�
		namespace NMNMRUNPARAMAPIFUNC_CMDARG
		{
			// Command line argument function
			/*
			@brif - Command line Argument ������ �����´�.
			@returns Command line Argument ������ ��ȯ�Ѵ�.
			*/
			DWORD GetCount()
			{
				return NMRUNPARAMAPIFUNC_CMDARG::GetCount();
			}

			/*
			@brif - ���õ� �ε����� Command line Argument ���� �����´�.
			@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
			@param[in] iArgIdx - ������ Command line Argument �� �ε����� �Է��Ѵ�.
			@param[out] szBuffer - �־��� �ε����� Command line Argument ���� ����� ���۸� �Է��Ѵ�.
			@param[in] dwBufferLen - �־��� �ε����� Command line Argument ���� ����� ������ ���̸� �Է��Ѵ�.
			*/
			NMRUNPARAM_RETCODE GetArg(unsigned short iArgIdx, LPSTR szBuffer, DWORD dwBufferLen)
			{
				return NMRUNPARAMAPIFUNC_CMDARG::GetArg(iArgIdx, szBuffer, dwBufferLen);
			}

			/*
			@brif - ���õ� �ε����� Command line Argument ���� ���ڿ� ���̸� �����´�.
			@returns ���õ� �ε����� Command line Argument ���� ���ڿ� ���̸� ��ȯ�Ѵ�.
			@param[in] iArgIdx - ������ Command line Argument �� �ε����� �Է��Ѵ�.
			*/
			DWORD GetArgLength(unsigned short iArgIdx)
			{
				return NMRUNPARAMAPIFUNC_CMDARG::GetArgLength(iArgIdx);
			}

			// Command line argument extended function
			/*
			@brif - ���� ����� ������ ��θ� �����´�.
			@returns ���� ����� ������ ��θ� ��ȯ�Ѵ�.
			*/
			LPCSTR GetRunFilePath()
			{
				return NMRUNPARAMAPIFUNC_CMDARG::GetRunFilePath();
			}

			/*
			@brif - ���� �Ķ���� ��ȣȭ Ű(Command line Argument �ε��� 1�� ��Ī�ϴ� ��)�� �����´�.
			@returns ���� �Ķ���� ��ȣȭ Ű(Command line Argument �ε��� 1�� ��Ī�ϴ� ��)�� ��ȯ�Ѵ�.
			*/
			LPCSTR GetRunParamKey()
			{
				return NMRUNPARAMAPIFUNC_CMDARG::GetRunParamKey();
			}

			/*
			@brif - ���� ���μ����� Current Directory �� �������� ��η� ��ȯ�Ѵ�.
			@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
			*/
			NMRUNPARAM_RETCODE SetCurPathToRunFilePath()
			{
				return NMRUNPARAMAPIFUNC_CMDARG::SetCurPathToRunFilePath();
			}
		}; //namespace NMNMRUNPARAMAPIFUNC_CMDARG

		/// @brief ���� �Ķ���� ��ȣȭ Ű ���� NMRunParam API �Լ�
		namespace NMNMRUNPARAMAPIFUNC_RUNPARAMKEY
		{
			/*
			@brif - ���� �Ķ���� ��ȣȭ Ű�� ���� �����Ѵ�.
			@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
			@param[out] szBuffer - ���� �Ķ���� ��ȣȭ Ű�� ����� ������ �����͸� �Է��Ѵ�.
			@param[in] dwLength - ���� �Ķ���� ��ȣȭ Ű�� ����� ������ ���̸� �Է��Ѵ�.
			*/
			NMRUNPARAM_RETCODE Create(LPSTR szBuffer, DWORD dwLength)
			{
				return NMRUNPARAMAPIFUNC_RUNPARAMKEY::Create(szBuffer, dwLength);
			}
			/*
			@brif - ���� ������ ���� �Ķ���� ��ȣȭ Ű�� ����Ǵµ� �ʿ��� ������ ���̸� �����´�.
			@returns ���� ������ ���� �Ķ���� ��ȣȭ Ű�� ����Ǵµ� �ʿ��� ������ ���̸� ��ȯ�Ѵ�.
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
		@brif - ���� �Ķ���͸� ȯ�溯���� ���� �о�´�.
		@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
		@param[in] szDecKey - ���� �Ķ���� ��ȣȭ Ű�� �Է��Ѵ�.@br
		�ش� ���� NULL�� ��� Command Line Argument 1��° ���� �о� ó���Ѵ�.
		@param[in] bClearEnv - ���� �Ķ���� �ε��� ȯ�� �������� �����͸� ���������� �Է��Ѵ�. true�� �Է��Ұ�� ���ŵǸ�, false�� �Է��� ��� ���ŵ��� �ʴ´�.
		*/
		NMRUNPARAM_RETCODE CNMNMRunParam::Load(LPCSTR szDecKey/* = NULL*/, bool bClearEnv/* = true*/)
		{
			return m_pParam->Load(szDecKey, bClearEnv);
		}

		/*
		@brif - ���� Ŭ������ ����� �Ķ���͸� ��ȣȭ�Ͽ� ȯ�� ������ �����Ѵ�.
		@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
		@param[in] szKey - ȯ�� ������ ����ɶ� ��ȣȭ�� Ű���� �Է��Ѵ�.
		*/
		NMRUNPARAM_RETCODE CNMNMRunParam::Save(LPCSTR szKey)
		{
			return m_pParam->Save(szKey);
		}

		/*
		@brif - �־��� ���α׷��� ���� Ŭ������ ����� �Ķ���͸� �����Ͽ� �����Ѵ�.
		@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
		@param[in] lpszRunFile - ������ ���α׷� ������ �Է��Ѵ�.
		@param[out] pRetExecCode - �־��� ���α׷��� ���� ������� ����� �����͸� �Է��Ѵ�. NULL�� ��� ������� �ʴ´�.
		@param[in] szEncKey - ���� Ŭ������ ����� �Ķ���Ͱ� ��ȣȭ�� ��ȣȭŰ�� �Է��Ѵ�. NULL�� ��� �ڵ����� �����Ͽ� �����Ѵ�.
		*/
		NMRUNPARAM_RETCODE CNMNMRunParam::RunProgram(LPCSTR lpszRunFile,UINT* pRetExecCode/* = NULL*/, LPCSTR szEncKey/* = NULL*/)
		{
			return m_pParam->RunProgram(lpszRunFile, pRetExecCode, szEncKey);
		}

		/*
		@brif - �־��� �̸��� ���� �Ķ���� ���� �����´�.
		@returns �־��� �̸��� ���� �Ķ���� ���� ��ȯ�Ѵ�.
		@param[in] lpszName - 
		*/
		LPCSTR CNMNMRunParam::GetParam(LPCSTR lpszName)
		{
			return m_pParam->GetParam(lpszName);
		}

		/*
		@brif - �־��� �̸��� ���� �Ķ���� ���� �����Ѵ�.
		@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
		@param[in] lpszName - ������ ���� �Ķ���� �̸��� �Է��Ѵ�.
		@param[in] lpszValue - ������ ���� �Ķ���� ���� �Է��Ѵ�.
		*/
		NMRUNPARAM_RETCODE CNMNMRunParam::SetParam(LPCSTR lpszName, LPCSTR lpszValue)
		{
			return m_pParam->SetParam(lpszName, lpszValue);
		}

		/*
		@brif - �־��� �̸��� ���� �Ķ���� ���� �����Ѵ�.
		@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
		@param[in] lpszName - ������ ���� �Ķ���� �̸��� �Է��Ѵ�.
		*/
		NMRUNPARAM_RETCODE CNMNMRunParam::RemoveParam(LPCSTR lpszName)
		{
			return m_pParam->RemoveParam(lpszName);
		}

		/*
		@brif - �־��� �ε����� ���� �Ķ���� �̸��� �����´�.
		@returns �־��� �ε����� ���� �Ķ���� �̸��� ��ȯ�Ѵ�.
		@param[in] dwIdx - �ε����� �Է��Ѵ�.
		*/
		LPCSTR CNMNMRunParam::GetParamName(DWORD dwIdx)
		{
			return m_pParam->GetParamName(dwIdx);
		}

		/*
		@brif - ����� ���� �Ķ���� ������ �����´�.
		@returns ����� ���� �Ķ���� ������ ��ȯ�Ѵ�.
		*/
		DWORD CNMNMRunParam::GetParamCount()
		{
			return m_pParam->GetParamCount();
		}

		/*
		@brif - �־��� �̸��� ���� �Ķ���Ͱ� �����ϴ��� üũ�Ѵ�,
		@returns �־��� �̸��� ���� �Ķ���Ͱ� �����ϸ� true����, �������� ������ false���� ��ȯ�Ѵ�.
		@param[in] lpszName - üũ�� ���� �Ķ������ �̸��� �Է��Ѵ�.
		*/
		bool CNMNMRunParam::ExistsParam(LPCSTR lpszName)
		{
			return m_pParam->ExistsParam(lpszName);
		}

	}//namespace UTIL

}//namespace NMBASE