#pragma once

class UPG_COMMON_EXPORT CGameDataManager
{
protected:
	enum GAME_DATA_TYPE
	{
		GDT_UNKNOWN = -1,
		GDT_INT64,
		GDT_UINT64,
		GDT_DOUBLE,
		GDT_STRING
	};

	class GAME_DATA
	{
	public:
		GAME_DATA_TYPE	m_nType;
		void			*m_pData;
	public:
		GAME_DATA();
		virtual ~GAME_DATA();

		void Release();
	};

	typedef std::map<string, GAME_DATA> MAP_GAME_DATA;

	MAP_GAME_DATA		m_mapData;
	CRITICAL_SECTION	m_cLock;
public:
	CGameDataManager(void);
	virtual ~CGameDataManager(void);

	void Release(void);

	void SetINT64(const std::string &strKey, const INT64 nData);
	void SetUINT64(const std::string &strKey, const UINT64 nData);
	void SetDouble(const std::string &strKey, const double dbData);
	void SetString(const std::string &strKey, const std::string strData);
	void SetBool(const std::string &strKey, const bool bData);

	INT64 &GetINT64(const std::string &strKey);
	UINT64 &GetUINT64(const std::string &strKey);
	double &GetDouble(const std::string &strKey);
	std::string &GetString(const std::string &strKey);
	bool GetBool(const std::string &strKey);

	static CGameDataManager& GetInstance();
	bool	IsKey(const std::string &strKey);
protected:
	GAME_DATA	&GetData(const std::string &strKey);
};

UPG_COMMON_EXPORT CGameDataManager& GDM();