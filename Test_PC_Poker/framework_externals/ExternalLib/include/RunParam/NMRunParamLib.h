// NMRunParamModuleLib.h: interface for the NMRunParamModuleLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMRUNPARAMLIB_H__FE27D416_83D5_4D19_BD28_95BEB0519D2B__INCLUDED_)
#define AFX_NMRUNPARAMLIB_H__FE27D416_83D5_4D19_BD28_95BEB0519D2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
// Define Lib Name

#if !defined(_LIB) && !defined(_LINKED_NMRUNPARAMLIB)
#ifndef _NMRUNPARAMLIBNAME
#define _NMRUNPARAMLIBNAME "NMRunParamLib"
#endif //#ifndef _NMRUNPARAMLIBNAME
#ifdef _DLL
#ifdef _DEBUG	// Debug multithread DLL
#pragma comment(lib, _NMRUNPARAMLIBNAME "_MDd")
#else			// multithread DLL
#pragma comment(lib, _NMRUNPARAMLIBNAME "_MD")
#endif // #ifdef _DEBUG
#else
#ifdef _DEBUG	// Debug multithread
#pragma comment(lib, _NMRUNPARAMLIBNAME "_MTd")
#else			// multithread
#pragma comment(lib, _NMRUNPARAMLIBNAME "_MT")
#endif // #ifdef _DEBUG
#endif // #ifdef _DLL
#define _LINKED_NMRUNPARAMLIB
#endif //#if !defined(_LIB) && !defined(_LINKED_NMRUNPARAMLIB)

//////////////////////////////////////////////////////////////////////////
// Define RunParam Name
/**
@defgroup GROUPERRCODE NMRunParamLib �����ڵ�
@{
*/
/// @brief - NMRunParamLib �����ڵ�
enum ERROR_NMRUNPARAM {
	ERROR_NMRUNPARAM_SUCCESS = 0,					///< ����(���� ����).
///	@details <b> ERROR_NMRUNPARAM_COMMON,
	ERROR_NMRUNPARAM_COMMON_NOT_IMPLEMENTED = 1001,	///< �����Ǿ����� ����.
	ERROR_NMRUNPARAM_COMMON_NOT_SUPPORT,			///< �������� ����.
///	@details <b> ERROR_NMRUNPARAM_LIB
	ERROR_NMRUNPARAM_LIB_NOTINIT = 2001,			///< ���� ���̺귯���� �ʱ�ȭ���� �ʾ���.
	ERROR_NMRUNPARAM_LIB_INVALID_PARAM,				///< �߸��� �Ķ����
	ERROR_NMRUNPARAM_LIB_DLL_NOT_EXISTS,			///< DLL ������ �������� ����
	ERROR_NMRUNPARAM_LIB_DLL_INVALID,				///< �߸��� DLL ����
	ERROR_NMRUNPARAM_LIB_DLL_CANT_LOAD,				///< DLL �� �ε��� �� ����.
	ERROR_NMRUNPARAM_LIB_DLL_NOT_LOADED,			///< DLL �� �ε���� ����.
	ERROR_NMRUNPARAM_LIB_DLL_ALREADY_LOADED,		///< DLL �� �̹� �ε��.
	ERROR_NMRUNPARAM_LIB_DLLPROC_NOT_FOUND,			///< DLL Export Function�� ã���� ����.
	ERROR_NMRUNPARAM_LIB_NOTEXIST_ENVVAR_COMMONPATH,	///< ���� ��� ��� ȯ�� ������ ã�� �� ����.
	ERROR_NMRUNPARAM_LIB_MISSING_PARAM,				///< �Ķ���Ͱ� ������.
///	@details <b> ERROR_NMRUNPARAM_API
	ERROR_NMRUNPARAM_API_INVALID_PARAM = 3001,		///< �߸��� �Ķ����.
	ERROR_NMRUNPARAM_API_NOTENOUGHBUFFER,			///< ������ ũ�Ⱑ ������� ����.
	ERROR_NMRUNPARAM_API_ENVVAR_NOTEXIST,			///< ȯ�溯���� �������� ����.
	ERROR_NMRUNPARAM_API_ENVVAR_SET_FAIL,			///< ȯ�� ���� ���� ����.
///	@details <b> ERROR_NMRUNPARAM_API_ENVVAR
	ERROR_NMRUNPARAM_API_ENVVAR_NOTEXISTS = 3101,	///< ȯ�溯���� �������� ����.
	ERROR_NMRUNPARAM_API_ENVVAR_FAIL_LOAD_DATA,		///< ȯ�� ������ �дµ� ������.
///	@details <b> ERROR_NMRUNPARAM_API_CMDARG
	ERROR_NMRUNPARAM_API_CMDARG_GET_FAIL = 3201,	///< ���� ���ڸ� �������µ� ������.
	ERROR_NMRUNPARAM_API_CMDARG_INVALIDPARAM,		///< ���� ���ڰ� �߸��Ǿ���.
	ERROR_NMRUNPARAM_API_CMDARG_NOTENOUGHBUFFER,	///< ���� ���ڸ� �������� ���� ������ ũ�Ⱑ ������.
	ERROR_NMRUNPARAM_API_CMDARG_OUT_OF_MEMORY,		///< ���� ���ڸ� �����ü� ����(�޸� ����).
	ERROR_NMRUNPARAM_API_CMDARG_NOT_EXISTS_PARAM,	///< ���� ���ڰ� �������� ����.
	ERROR_NMRUNPARAM_API_CMDARG_RUNPROGRAM_NOT_HAVE_PATH,	///< ���� ���α׷��� ��ΰ� �������� ����.
	ERROR_NMRUNPARAM_API_CMDARG_CURPATH_CANT_CHANGE,		///< ���� ��θ� ������ �� ����.
///	@details <b> ERROR_NMRUNPARAM_MODULE
	ERROR_NMRUNPARAM_MODULE_INVALID_PARAM = 5001,	///< �߸��� �Ķ����
	ERROR_NMRUNPARAM_MODULE_NOT_ENOUGH_MEM,			///< �޸� ����.
	ERROR_NMRUNPARAM_MODULE_ALREADY_CREATED,		///< ���� �Ķ���� ����� �̹� �����Ǿ���.
	ERROR_NMRUNPARAM_MODULE_NOTFOUND_DATA,			///< �����͸� ã�� �� ����.
	ERROR_NMRUNPARAM_MODULE_ENV_INVALID_FORMAT,		///< �߸��� ȯ�溯�� ����
	ERROR_NMRUNPARAM_MODULE_CMDARGKEY_NOTEXISTS,	///< ȯ�溯�� ��ȣȭ Ű���� �������� ����.
///	@details <b> ERROR_NMRUNPARAM_MODULE_DATAMAP
	ERROR_NMRUNPARAM_MODULE_DATAMAP_INSERT_FAIL = 5101,	///< ������ �ʿ� Insert ����
///	@details <b> ERROR_NMRUNPARAM_MODULE_PARAMDATA
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_NEW_FAIL,		///< �Ķ���� ����Ʈ ���� ����
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_SETDATA_FAIL,	///< �Ķ���� ������ ���� ����
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_INSERT_FAIL,	///< �Ķ���� ������ Insert ����
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_NOT_EXISTS,	///< �Ķ���� �����Ͱ� �������� ����.
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_EMPTYDATA,	///< �Ķ���� �����Ͱ� �������.
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_GETESCAPEDDATA_FAIL,	///< Ż�⹮�� ������ �������� ����.
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_NOTENOUGHBUFFER,		///< ������ ũ�Ⱑ ������� ����.
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_SETENV_FAIL,			///< ȯ�溯�� ���ÿ� ������.
	ERROR_NMRUNPARAM_MODULE_PARAMDATA_RUNPROGRAM_FAIL,		///< ���α׷� ���� ����.
///	@details <b> ERROR_NMRUNPARAM_MODULE_TDES
	ERROR_NMRUNPARAM_MODULE_TDES_INIT_FAIL = 5301,			///< TDES ��� �ʱ�ȭ ����
	ERROR_NMRUNPARAM_MODULE_TDES_ENCRYPT_FAIL,				///< TDES ��ȣȭ ����
	ERROR_NMRUNPARAM_MODULE_TDES_DECRYPT_FAIL,				///< TDES ��ȣȭ ����
	ERROR_NMRUNPARAM_MODULE_TDES_NOTENOUGHBUFFER,			///< ������ ũ�Ⱑ ������� ����.
	ERROR_NMRUNPARAM_MODULE_TDES_INVALID_DATALENGTH,		///< �߸��� ������ ����.
};
/**
@}
*/

//////////////////////////////////////////////////////////////////////////
// Define RunParam Name
/**
@defgroup GROUPNMPARAMNAME ����� �Ķ���� �̸� ����
@{
*/
#define NMPARAMNAME_AUTHCOOKIE		"AuthCookie"	///< ������Ű �Ķ���� �̸� ����
#define NMPARAMNAME_DATACOOKIE		"DataCookie"	///< ��������Ű �Ķ���� �̸� ����
#define NMPARAMNAME_CPCOOKIE		"CpCookie"		///< CP��Ű �Ķ���� �̸� ����
#define NMPARAMNAME_SERVPORT 		"ServPort"		///< ���� ��Ʈ �Ķ���� �̸� ����
#define NMPARAMNAME_SERVIP			"ServIP"		///< ���� ������ �Ķ���� �̸� ����
#define NMPARAMNAME_SPAREPARAM		"SpareParam"	///< ���� ���� �Ķ���� �̸� ����
/**
@}
*/

//////////////////////////////////////////////////////////////////////////
// Default 

typedef unsigned long	NMRUNPARAM_RETCODE;

//////////////////////////////////////////////////////////////////////////
// Function Definition

/// @brief NMRunParam API �Լ�
namespace NMRUNPARAMAPIFUNC
{
	/**
	@brif - �������� DLL ���� ��θ� �����Ѵ�. (������)
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	@param[in] szFileName - �������� ������ DLL ������ ��θ� �Է��Ѵ�.
	*/
	NMRUNPARAM_RETCODE	SetDLLFileName(LPCSTR szFileName);

	// convert return Code 2 String 
	/**
	@brif - @ref ERROR_NMRUNPARAM ������ �ڵ带 ���ڿ� �ڵ�� ��ȯ�Ѵ�.
	@returns �Էµ� ������ @ref ERROR_NMRUNPARAM �ڵ� ���� ���ڿ� �ڵ�� ��ȯ�� ���� ��ȯ�Ѵ�.
	@param[in] retCode - ���ڿ� �ڵ�� ��ȯ�� ������ @ref ERROR_NMRUNPARAM ���� �Է��Ѵ�.
	*/
	LPCSTR				ConvertRetCode2String(NMRUNPARAM_RETCODE retCode);

	/**
	@brif - ���� ��� �ڵ� ��(WinExec �� ShellExcute �� ���ϰ�)�� ���ڿ� �ڵ�� ��ȯ�Ѵ�.
	@returns �Էµ� ���� ��� �ڵ� ��(WinExec �� ShellExcute �� ���ϰ�)�� ���ڿ� �ڵ�� ��ȯ�� ���� ��ȯ�Ѵ�.
	@param[in] uErr - ���ڿ� �ڵ�� ��ȯ�� ���� ��� �ڵ� ��(WinExec �� ShellExcute �� ���ϰ�)�� �Է��Ѵ�.
	*/
	LPCSTR				ConvertRunProgError2String(UINT uErr);
};

/// @brief ȯ�� ���� ���� NMRunParam API �Լ�
namespace NMRUNPARAMAPIFUNC_ENVVAR
{
	/// @brief - ����� ȯ�� ���� �ڵ� ������.
	enum ENVVARNAME {
		ENVVARNAME_SITENAME = 0,	///< ����Ʈ �ڵ� ȯ�� ���� �ڵ�
		ENVVARNAME_COMMONPATH,		///< ���� ��� ȯ�� ���� �ڵ�
	};

	// Process Environment Function
	/**
	@brif - �־��� ȯ�� ���� �̸� �ڵ带 �������� ����� ȯ�� ������ �����´�.
	@returns �־��� ȯ�� ���� �̸� �ڵ带 �������� ����� ȯ�� ������ ��ȯ�Ѵ�.
	@param[in] eName - ������ ȯ�� ���� ���� ȯ�� ���� �̸� �ڵ带 �Է��Ѵ�.
	*/
	LPCSTR				GetEnvName(ENVVARNAME eName);
	/**
	@brif - �־��� ȯ�� ���� �̸��� �������� ����� ȯ�� ������ �����´�.
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	@param[in] szEnvName - ������ ȯ�� ���� ���� ȯ�� ���� �̸��� �Է��Ѵ�.
	@param[out] szBuffer - ������ ȯ�� ���� ���� ����� ������ �����͸� �Է��Ѵ�.
	@param[in] dwBufferLen - ������ ȯ�� ���� ���� ����� ������ ���̸� �Է��Ѵ�.
	*/
	NMRUNPARAM_RETCODE	GetEnvVar(LPCSTR szEnvName, LPSTR szBuffer, DWORD dwBufferLen);
	/**
	@brif - �־��� ȯ�� ���� �̸� �ڵ带 �������� ����� ȯ�� ������ �����´�.
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	@param[in] eName - ������ ȯ�� ���� ���� ȯ�� ���� �̸� �ڵ带 �Է��Ѵ�.
	@param[out] szBuffer - ������ ȯ�� ���� ���� ����� ������ �����͸� �Է��Ѵ�.
	@param[in] dwBufferLen - ������ ȯ�� ���� ���� ����� ������ ���̸� �Է��Ѵ�.
	*/
	NMRUNPARAM_RETCODE	GetEnvVar(ENVVARNAME eName, LPSTR szBuffer, DWORD dwBufferLen);
	

	/**
	@brif - �־��� ȯ�� ���� �̸� �ڵ带 �������� ����� ȯ�� ������ ���ڿ� ���̸� �����´�.
	@returns �Էµ� ȯ�� ���� �̸� �ڵ带 �������� ����� ȯ�� ������ ���ڿ� ���̸� ��ȯ�Ѵ�.
	@param[in] eName - ������ ȯ�� ���� ���� ȯ�� ���� �̸� �ڵ带 �Է��Ѵ�.
	*/
	DWORD				GetEnvVarLength(ENVVARNAME eName);
	/**
	@brif - �־��� ȯ�� ���� �̸��� �������� ����� ȯ�� ������ ���ڿ� ���̸� �����´�.
	@returns �Էµ� ȯ�� ���� �̸��� �������� ����� ȯ�� ������ ���ڿ� ���̸� ��ȯ�Ѵ�.
	@param[in] szEnvName - ������ ȯ�� ���� ���� ȯ�� ���� �̸��� �Է��Ѵ�.
	*/
	DWORD				GetEnvVarLength(LPCSTR szEnvName);

	/**
	@brif - �־��� ȯ�� ���� �̸� �ڵ忡 ȯ�� ���� ���� �����Ѵ�.
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	@param[in] eName - ������ ȯ�� ���� �̸� �ڵ� ���� �Է��Ѵ�.
	@param[in] szValue - ������ ȯ�� ���� ���� �Է��Ѵ�.
	*/
	NMRUNPARAM_RETCODE	SetEnvVar(ENVVARNAME eName, LPCSTR szValue);
	/**
	@brif - �־��� ȯ�� ���� �̸� �� ȯ�� ���� ���� �����Ѵ�.
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	@param[in] szEnvName - ������ ȯ�� ���� �̸��� �Է��Ѵ�.
	@param[in] szValue - ������ ȯ�� ���� ���� �Է��Ѵ�.
	*/
	NMRUNPARAM_RETCODE	SetEnvVar(LPCSTR szEnvName, LPCSTR szValue);

	/**
	@brif - ����Ʈ �ڵ� ȯ�� ���� ���� �����´�. (�̱���)
	@returns ����Ʈ �ڵ� ȯ�� ���� ���� ��ȯ�Ѵ�.
	*/
	LPCSTR				GetSiteName();
	/**
	@brif - ����Ʈ �ڵ� ȯ�� ���� ���� �����Ѵ�. (�̱���)
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	@param[in] szSiteName - ����Ʈ �ڵ� ȯ�� ���� ���� ����� ����Ʈ �̸��� �Է��Ѵ�.
	*/
	NMRUNPARAM_RETCODE	SetSiteName(LPCSTR szSiteName);
	
	/**
	@brif - ���� ��� ��θ� �����´�.
	@returns ���� ��� ��θ� ��ȯ�Ѵ�.
	*/
	LPCSTR				GetCommonPath();
	/**
	@brif - ���� ��� ��θ� �����Ѵ�.
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	@param[in] szCommonPath - ������ ���� ��� ��θ� �Է��Ѵ�.
	*/
	NMRUNPARAM_RETCODE	SetCommonPath(LPCSTR szCommonPath);
};

/// @brief Command line Argument ���� NMRunParam API �Լ�
namespace NMRUNPARAMAPIFUNC_CMDARG
{
	/// @brif Command line Argument �ε��� ������.
	enum EDEFARGIDX {
		DEFARGIDX_RUNPARAMKEY = 1, ///< �Ϻ�ȣȭ Ű�� ��Ī�ϴ� Command line Argument �ε���
	};
	
	// Command line argument function
	/**
	@brif - Command line Argument ������ �����´�.
	@returns Command line Argument ������ ��ȯ�Ѵ�.
	*/
	DWORD				GetCount();
	
	/**
	@brif - ���õ� �ε����� Command line Argument ���� �����´�.
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	@param[in] iArgIdx - ������ Command line Argument �� �ε����� �Է��Ѵ�.
	@param[out] szBuffer - �־��� �ε����� Command line Argument ���� ����� ���۸� �Է��Ѵ�.
	@param[in] dwBufferLen - �־��� �ε����� Command line Argument ���� ����� ������ ���̸� �Է��Ѵ�.
	*/
	NMRUNPARAM_RETCODE	GetArg(unsigned short iArgIdx, LPSTR szBuffer, DWORD dwBufferLen);
	/**
	@brif - ���õ� �ε����� Command line Argument ���� ���ڿ� ���̸� �����´�.
	@returns ���õ� �ε����� Command line Argument ���� ���ڿ� ���̸� ��ȯ�Ѵ�.
	@param[in] iArgIdx - ������ Command line Argument �� �ε����� �Է��Ѵ�.
	*/
	DWORD				GetArgLength(unsigned short iArgIdx);
	
	// Command line argument extended function
	/**
	@brif - ���� ����� ������ ��θ� �����´�.
	@returns ���� ����� ������ ��θ� ��ȯ�Ѵ�.
	*/
	LPCSTR				GetRunFilePath();
	/**
	@brif - ���� �Ķ���� ��ȣȭ Ű(Command line Argument �ε��� 1�� ��Ī�ϴ� ��)�� �����´�.
	@returns ���� �Ķ���� ��ȣȭ Ű(Command line Argument �ε��� 1�� ��Ī�ϴ� ��)�� ��ȯ�Ѵ�.
	*/
	LPCSTR				GetRunParamKey();
	
	/**
	@brif - ���� ���μ����� Current Directory �� �������� ��η� ��ȯ�Ѵ�.
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	*/
	NMRUNPARAM_RETCODE	SetCurPathToRunFilePath();
};

/// @brief ���� �Ķ���� ��ȣȭ Ű ���� NMRunParam API �Լ�
namespace NMRUNPARAMAPIFUNC_RUNPARAMKEY
{
	/**
	@brif - ���� �Ķ���� ��ȣȭ Ű�� ���� �����Ѵ�.
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	@param[out] szBuffer - ���� �Ķ���� ��ȣȭ Ű�� ����� ������ �����͸� �Է��Ѵ�.
	@param[in] dwLength - ���� �Ķ���� ��ȣȭ Ű�� ����� ������ ���̸� �Է��Ѵ�.
	*/
	NMRUNPARAM_RETCODE	Create(LPSTR szBuffer, DWORD dwLength);
	/**
	@brif - ���� ������ ���� �Ķ���� ��ȣȭ Ű�� ����Ǵµ� �ʿ��� ������ ���̸� �����´�.
	@returns ���� ������ ���� �Ķ���� ��ȣȭ Ű�� ����Ǵµ� �ʿ��� ������ ���̸� ��ȯ�Ѵ�.
	*/
	DWORD				GetNeedLength();	
};





//////////////////////////////////////////////////////////////////////////
// Class Definition

/// @brief ���� �Ķ���� Ŭ����
class CNMRunParam
{
public:
	/** @brif - CNMRunParam Ŭ���� ������ */
	CNMRunParam();
	/** @brif - CNMRunParam Ŭ���� �Ҹ��� */
	virtual ~CNMRunParam();

	/**
	@brif - ���� �Ķ���͸� ȯ�溯���� ���� �о�´�.
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	@param[in] szDecKey - ���� �Ķ���� ��ȣȭ Ű�� �Է��Ѵ�.@br
						�ش� ���� NULL�� ��� Command Line Argument 1��° ���� �о� ó���Ѵ�.
	@param[in] bClearEnv - ���� �Ķ���� �ε��� ȯ�� �������� �����͸� ���������� �Է��Ѵ�. true�� �Է��Ұ�� ���ŵǸ�, false�� �Է��� ��� ���ŵ��� �ʴ´�.
	*/
	NMRUNPARAM_RETCODE	Load(LPCSTR szDecKey = NULL, bool bClearEnv = true);
	/**
	@brif - ���� Ŭ������ ����� �Ķ���͸� ��ȣȭ�Ͽ� ȯ�� ������ �����Ѵ�.
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	@param[in] szKey - ȯ�� ������ ����ɶ� ��ȣȭ�� Ű���� �Է��Ѵ�.
	*/
	NMRUNPARAM_RETCODE	Save(LPCSTR szKey);
	/**
	@brif - �־��� ���α׷��� ���� Ŭ������ ����� �Ķ���͸� �����Ͽ� �����Ѵ�.
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	@param[in] lpszRunFile - ������ ���α׷� ������ �Է��Ѵ�.
	@param[out] pRetExecCode - �־��� ���α׷��� ���� ������� ����� �����͸� �Է��Ѵ�. NULL�� ��� ������� �ʴ´�.
	@param[in] szEncKey - ���� Ŭ������ ����� �Ķ���Ͱ� ��ȣȭ�� ��ȣȭŰ�� �Է��Ѵ�. NULL�� ��� �ڵ����� �����Ͽ� �����Ѵ�.
	*/
	NMRUNPARAM_RETCODE	RunProgram(LPCSTR lpszRunFile,UINT* pRetExecCode = NULL, LPCSTR szEncKey = NULL);

	/**
	@brif - �־��� �̸��� ���� �Ķ���� ���� �����´�.
	@returns �־��� �̸��� ���� �Ķ���� ���� ��ȯ�Ѵ�.
	@param[in] lpszName - 
	*/
	LPCSTR				GetParam(LPCSTR lpszName);
	/**
	@brif - �־��� �̸��� ���� �Ķ���� ���� �����Ѵ�.
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	@param[in] lpszName - ������ ���� �Ķ���� �̸��� �Է��Ѵ�.
	@param[in] lpszValue - ������ ���� �Ķ���� ���� �Է��Ѵ�.
	*/
	NMRUNPARAM_RETCODE	SetParam(LPCSTR lpszName, LPCSTR lpszValue);
	/**
	@brif - �־��� �̸��� ���� �Ķ���� ���� �����Ѵ�.
	@returns �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ��ȯ�ϰ�, �����Ͽ��� ��� @ref ERROR_NMRUNPARAM_SUCCESS �� ������ ������ @ref ERROR_NMRUNPARAM ���� �߿� �ϳ��� ��ȯ�Ѵ�.
	@param[in] lpszName - ������ ���� �Ķ���� �̸��� �Է��Ѵ�.
	*/
	NMRUNPARAM_RETCODE	RemoveParam(LPCSTR lpszName);

	/**
	@brif - �־��� �ε����� ���� �Ķ���� �̸��� �����´�.
	@returns �־��� �ε����� ���� �Ķ���� �̸��� ��ȯ�Ѵ�.
	@param[in] dwIdx - �ε����� �Է��Ѵ�.
	*/
	LPCSTR				GetParamName(DWORD dwIdx);
	/**
	@brif - ����� ���� �Ķ���� ������ �����´�.
	@returns ����� ���� �Ķ���� ������ ��ȯ�Ѵ�.
	*/
	DWORD				GetParamCount();

	/**
	@brif - �־��� �̸��� ���� �Ķ���Ͱ� �����ϴ��� üũ�Ѵ�,
	@returns �־��� �̸��� ���� �Ķ���Ͱ� �����ϸ� true����, �������� ������ false���� ��ȯ�Ѵ�.
	@param[in] lpszName - üũ�� ���� �Ķ������ �̸��� �Է��Ѵ�.
	*/
	bool				ExistsParam(LPCSTR lpszName);
	
};
#endif // !defined(AFX_NMRUNPARAMLIB_H__FE27D416_83D5_4D19_BD28_95BEB0519D2B__INCLUDED_)
