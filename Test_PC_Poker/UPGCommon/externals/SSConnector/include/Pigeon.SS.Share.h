#pragma once

// DarCompiler 2.0.0.1 ver.

#include <Camel/Dar2/Dar2BaseMsg.h>
#include <Camel/Dar2/DAR2Rule_STREAM.h>
#if defined(__JSON_LIB_SUPPORT__)
#include <Camel/Dar2/DAR2Rule_JSON.h>
#endif // defined(__JSON_LIB_SUPPORT__)


namespace Camel
{
	namespace Dar2
	{
		namespace SS
		{

			#if !defined __DarEnum_SS_LoginResult__MessageId_3043390339__
			#define __DarEnum_SS_LoginResult__MessageId_3043390339__
			enum LoginResult
			{
				LOGIN_SUCCESS = 0,
				LOGIN_FAILED = 1,
				LOGIN_DUPLICATE = 2,
				LOGIN_AUTU_FAILED = 3,
				LOGOUT_SUCCESS = 4,
				LOGOUT_FAILED = 5,
			};

			// Camel::Dar2::SS::LoginResult enum => string
			inline const char* EnumToString(Camel::Dar2::SS::LoginResult eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::LOGIN_SUCCESS == eEnumValue)
				{
					return "Camel::Dar2::SS::LOGIN_SUCCESS";
				}
				if ( Camel::Dar2::SS::LOGIN_FAILED == eEnumValue)
				{
					return "Camel::Dar2::SS::LOGIN_FAILED";
				}
				if ( Camel::Dar2::SS::LOGIN_DUPLICATE == eEnumValue)
				{
					return "Camel::Dar2::SS::LOGIN_DUPLICATE";
				}
				if ( Camel::Dar2::SS::LOGIN_AUTU_FAILED == eEnumValue)
				{
					return "Camel::Dar2::SS::LOGIN_AUTU_FAILED";
				}
				if ( Camel::Dar2::SS::LOGOUT_SUCCESS == eEnumValue)
				{
					return "Camel::Dar2::SS::LOGOUT_SUCCESS";
				}
				if ( Camel::Dar2::SS::LOGOUT_FAILED == eEnumValue)
				{
					return "Camel::Dar2::SS::LOGOUT_FAILED";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::LoginResult string => enum
			inline Camel::Dar2::SS::LoginResult StringToEnum(const char* szStringValue, Camel::Dar2::SS::LoginResult eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::LOGIN_SUCCESS", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::LOGIN_SUCCESS;
				}
				if ( strcmp("Camel::Dar2::SS::LOGIN_FAILED", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::LOGIN_FAILED;
				}
				if ( strcmp("Camel::Dar2::SS::LOGIN_DUPLICATE", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::LOGIN_DUPLICATE;
				}
				if ( strcmp("Camel::Dar2::SS::LOGIN_AUTU_FAILED", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::LOGIN_AUTU_FAILED;
				}
				if ( strcmp("Camel::Dar2::SS::LOGOUT_SUCCESS", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::LOGOUT_SUCCESS;
				}
				if ( strcmp("Camel::Dar2::SS::LOGOUT_FAILED", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::LOGOUT_FAILED;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_LoginResult__MessageId_3043390339__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_BoxPosition__MessageId_808408265__
			#define __DarEnum_SS_BoxPosition__MessageId_808408265__
			enum BoxPosition
			{
				ALL = 0,
				IN_BOX = 1,
				IN_STORAGE = 2,
			};

			// Camel::Dar2::SS::BoxPosition enum => string
			inline const char* EnumToString(Camel::Dar2::SS::BoxPosition eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::ALL == eEnumValue)
				{
					return "Camel::Dar2::SS::ALL";
				}
				if ( Camel::Dar2::SS::IN_BOX == eEnumValue)
				{
					return "Camel::Dar2::SS::IN_BOX";
				}
				if ( Camel::Dar2::SS::IN_STORAGE == eEnumValue)
				{
					return "Camel::Dar2::SS::IN_STORAGE";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::BoxPosition string => enum
			inline Camel::Dar2::SS::BoxPosition StringToEnum(const char* szStringValue, Camel::Dar2::SS::BoxPosition eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::ALL", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::ALL;
				}
				if ( strcmp("Camel::Dar2::SS::IN_BOX", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::IN_BOX;
				}
				if ( strcmp("Camel::Dar2::SS::IN_STORAGE", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::IN_STORAGE;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_BoxPosition__MessageId_808408265__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_SetOption__MessageId_890469168__
			#define __DarEnum_SS_SetOption__MessageId_890469168__
			enum SetOption
			{
				WHISPER = 1,
				MEMO = 2,
				CHAT = 3,
			};

			// Camel::Dar2::SS::SetOption enum => string
			inline const char* EnumToString(Camel::Dar2::SS::SetOption eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::WHISPER == eEnumValue)
				{
					return "Camel::Dar2::SS::WHISPER";
				}
				if ( Camel::Dar2::SS::MEMO == eEnumValue)
				{
					return "Camel::Dar2::SS::MEMO";
				}
				if ( Camel::Dar2::SS::CHAT == eEnumValue)
				{
					return "Camel::Dar2::SS::CHAT";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::SetOption string => enum
			inline Camel::Dar2::SS::SetOption StringToEnum(const char* szStringValue, Camel::Dar2::SS::SetOption eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::WHISPER", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::WHISPER;
				}
				if ( strcmp("Camel::Dar2::SS::MEMO", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::MEMO;
				}
				if ( strcmp("Camel::Dar2::SS::CHAT", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::CHAT;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_SetOption__MessageId_890469168__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_RecvState__MessageId_949200092__
			#define __DarEnum_SS_RecvState__MessageId_949200092__
			enum RecvState
			{
				RECV_STATE_ALL = 0,
				RECV_STATE_NOT = 1,
				RECV_STATE_ONLY_FRIEND = 2,
				ERCV_STATE_MAX = 3,
			};

			// Camel::Dar2::SS::RecvState enum => string
			inline const char* EnumToString(Camel::Dar2::SS::RecvState eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::RECV_STATE_ALL == eEnumValue)
				{
					return "Camel::Dar2::SS::RECV_STATE_ALL";
				}
				if ( Camel::Dar2::SS::RECV_STATE_NOT == eEnumValue)
				{
					return "Camel::Dar2::SS::RECV_STATE_NOT";
				}
				if ( Camel::Dar2::SS::RECV_STATE_ONLY_FRIEND == eEnumValue)
				{
					return "Camel::Dar2::SS::RECV_STATE_ONLY_FRIEND";
				}
				if ( Camel::Dar2::SS::ERCV_STATE_MAX == eEnumValue)
				{
					return "Camel::Dar2::SS::ERCV_STATE_MAX";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::RecvState string => enum
			inline Camel::Dar2::SS::RecvState StringToEnum(const char* szStringValue, Camel::Dar2::SS::RecvState eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::RECV_STATE_ALL", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::RECV_STATE_ALL;
				}
				if ( strcmp("Camel::Dar2::SS::RECV_STATE_NOT", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::RECV_STATE_NOT;
				}
				if ( strcmp("Camel::Dar2::SS::RECV_STATE_ONLY_FRIEND", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::RECV_STATE_ONLY_FRIEND;
				}
				if ( strcmp("Camel::Dar2::SS::ERCV_STATE_MAX", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::ERCV_STATE_MAX;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_RecvState__MessageId_949200092__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_Show__MessageId_3507751084__
			#define __DarEnum_SS_Show__MessageId_3507751084__
			enum Show
			{
				SHOW_AVAILABLE = 0,
				SHOW_AWAY = 1,
				SHOW_CHAT = 2,
				SHOW_DND = 3,
				SHOW_XA = 4,
				SHOW_MAX = 5,
			};

			// Camel::Dar2::SS::Show enum => string
			inline const char* EnumToString(Camel::Dar2::SS::Show eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::SHOW_AVAILABLE == eEnumValue)
				{
					return "Camel::Dar2::SS::SHOW_AVAILABLE";
				}
				if ( Camel::Dar2::SS::SHOW_AWAY == eEnumValue)
				{
					return "Camel::Dar2::SS::SHOW_AWAY";
				}
				if ( Camel::Dar2::SS::SHOW_CHAT == eEnumValue)
				{
					return "Camel::Dar2::SS::SHOW_CHAT";
				}
				if ( Camel::Dar2::SS::SHOW_DND == eEnumValue)
				{
					return "Camel::Dar2::SS::SHOW_DND";
				}
				if ( Camel::Dar2::SS::SHOW_XA == eEnumValue)
				{
					return "Camel::Dar2::SS::SHOW_XA";
				}
				if ( Camel::Dar2::SS::SHOW_MAX == eEnumValue)
				{
					return "Camel::Dar2::SS::SHOW_MAX";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::Show string => enum
			inline Camel::Dar2::SS::Show StringToEnum(const char* szStringValue, Camel::Dar2::SS::Show eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::SHOW_AVAILABLE", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SHOW_AVAILABLE;
				}
				if ( strcmp("Camel::Dar2::SS::SHOW_AWAY", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SHOW_AWAY;
				}
				if ( strcmp("Camel::Dar2::SS::SHOW_CHAT", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SHOW_CHAT;
				}
				if ( strcmp("Camel::Dar2::SS::SHOW_DND", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SHOW_DND;
				}
				if ( strcmp("Camel::Dar2::SS::SHOW_XA", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SHOW_XA;
				}
				if ( strcmp("Camel::Dar2::SS::SHOW_MAX", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SHOW_MAX;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_Show__MessageId_3507751084__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_SearchCategory__MessageId_2625218879__
			#define __DarEnum_SS_SearchCategory__MessageId_2625218879__
			enum SearchCategory
			{
				SEARCH_CONTENT = 0,
				SEARCH_ID = 1,
				SEARCH_NICKNAME = 2,
				SEARCH_ID_OR_NICKNAME = 3,
				SEARCH_MAX = 4,
			};

			// Camel::Dar2::SS::SearchCategory enum => string
			inline const char* EnumToString(Camel::Dar2::SS::SearchCategory eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::SEARCH_CONTENT == eEnumValue)
				{
					return "Camel::Dar2::SS::SEARCH_CONTENT";
				}
				if ( Camel::Dar2::SS::SEARCH_ID == eEnumValue)
				{
					return "Camel::Dar2::SS::SEARCH_ID";
				}
				if ( Camel::Dar2::SS::SEARCH_NICKNAME == eEnumValue)
				{
					return "Camel::Dar2::SS::SEARCH_NICKNAME";
				}
				if ( Camel::Dar2::SS::SEARCH_ID_OR_NICKNAME == eEnumValue)
				{
					return "Camel::Dar2::SS::SEARCH_ID_OR_NICKNAME";
				}
				if ( Camel::Dar2::SS::SEARCH_MAX == eEnumValue)
				{
					return "Camel::Dar2::SS::SEARCH_MAX";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::SearchCategory string => enum
			inline Camel::Dar2::SS::SearchCategory StringToEnum(const char* szStringValue, Camel::Dar2::SS::SearchCategory eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::SEARCH_CONTENT", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SEARCH_CONTENT;
				}
				if ( strcmp("Camel::Dar2::SS::SEARCH_ID", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SEARCH_ID;
				}
				if ( strcmp("Camel::Dar2::SS::SEARCH_NICKNAME", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SEARCH_NICKNAME;
				}
				if ( strcmp("Camel::Dar2::SS::SEARCH_ID_OR_NICKNAME", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SEARCH_ID_OR_NICKNAME;
				}
				if ( strcmp("Camel::Dar2::SS::SEARCH_MAX", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SEARCH_MAX;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_SearchCategory__MessageId_2625218879__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_MUCContentType__MessageId_1809230605__
			#define __DarEnum_SS_MUCContentType__MessageId_1809230605__
			enum MUCContentType
			{
				MCT_TEXT = 1,
				MCT_RTF = 2,
				MCT_JSON = 3,
				MCT_XML = 4,
				MCT_CUSTOM = 5,
				MCT_MAX = 6,
			};

			// Camel::Dar2::SS::MUCContentType enum => string
			inline const char* EnumToString(Camel::Dar2::SS::MUCContentType eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::MCT_TEXT == eEnumValue)
				{
					return "Camel::Dar2::SS::MCT_TEXT";
				}
				if ( Camel::Dar2::SS::MCT_RTF == eEnumValue)
				{
					return "Camel::Dar2::SS::MCT_RTF";
				}
				if ( Camel::Dar2::SS::MCT_JSON == eEnumValue)
				{
					return "Camel::Dar2::SS::MCT_JSON";
				}
				if ( Camel::Dar2::SS::MCT_XML == eEnumValue)
				{
					return "Camel::Dar2::SS::MCT_XML";
				}
				if ( Camel::Dar2::SS::MCT_CUSTOM == eEnumValue)
				{
					return "Camel::Dar2::SS::MCT_CUSTOM";
				}
				if ( Camel::Dar2::SS::MCT_MAX == eEnumValue)
				{
					return "Camel::Dar2::SS::MCT_MAX";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::MUCContentType string => enum
			inline Camel::Dar2::SS::MUCContentType StringToEnum(const char* szStringValue, Camel::Dar2::SS::MUCContentType eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::MCT_TEXT", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::MCT_TEXT;
				}
				if ( strcmp("Camel::Dar2::SS::MCT_RTF", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::MCT_RTF;
				}
				if ( strcmp("Camel::Dar2::SS::MCT_JSON", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::MCT_JSON;
				}
				if ( strcmp("Camel::Dar2::SS::MCT_XML", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::MCT_XML;
				}
				if ( strcmp("Camel::Dar2::SS::MCT_CUSTOM", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::MCT_CUSTOM;
				}
				if ( strcmp("Camel::Dar2::SS::MCT_MAX", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::MCT_MAX;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_MUCContentType__MessageId_1809230605__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_ContentType__MessageId_3062077000__
			#define __DarEnum_SS_ContentType__MessageId_3062077000__
			enum ContentType
			{
				CT_TEXT = 1,
				CT_RTF = 2,
				CT_JSON = 3,
				CT_XML = 4,
				CT_CUSTOM = 5,
				CT_MAX = 6,
			};

			// Camel::Dar2::SS::ContentType enum => string
			inline const char* EnumToString(Camel::Dar2::SS::ContentType eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::CT_TEXT == eEnumValue)
				{
					return "Camel::Dar2::SS::CT_TEXT";
				}
				if ( Camel::Dar2::SS::CT_RTF == eEnumValue)
				{
					return "Camel::Dar2::SS::CT_RTF";
				}
				if ( Camel::Dar2::SS::CT_JSON == eEnumValue)
				{
					return "Camel::Dar2::SS::CT_JSON";
				}
				if ( Camel::Dar2::SS::CT_XML == eEnumValue)
				{
					return "Camel::Dar2::SS::CT_XML";
				}
				if ( Camel::Dar2::SS::CT_CUSTOM == eEnumValue)
				{
					return "Camel::Dar2::SS::CT_CUSTOM";
				}
				if ( Camel::Dar2::SS::CT_MAX == eEnumValue)
				{
					return "Camel::Dar2::SS::CT_MAX";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::ContentType string => enum
			inline Camel::Dar2::SS::ContentType StringToEnum(const char* szStringValue, Camel::Dar2::SS::ContentType eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::CT_TEXT", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::CT_TEXT;
				}
				if ( strcmp("Camel::Dar2::SS::CT_RTF", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::CT_RTF;
				}
				if ( strcmp("Camel::Dar2::SS::CT_JSON", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::CT_JSON;
				}
				if ( strcmp("Camel::Dar2::SS::CT_XML", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::CT_XML;
				}
				if ( strcmp("Camel::Dar2::SS::CT_CUSTOM", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::CT_CUSTOM;
				}
				if ( strcmp("Camel::Dar2::SS::CT_MAX", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::CT_MAX;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_ContentType__MessageId_3062077000__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_MUCRoomAffiliation__MessageId_821875091__
			#define __DarEnum_SS_MUCRoomAffiliation__MessageId_821875091__
			enum MUCRoomAffiliation
			{
				AFF_NONE = 1,
				AFF_OUTCAT = 2,
				AFF_MEMBER = 4,
				AFF_ADMIN = 8,
				AFF_OWNER = 16,
				AFF_MAX = 32,
			};

			// Camel::Dar2::SS::MUCRoomAffiliation enum => string
			inline const char* EnumToString(Camel::Dar2::SS::MUCRoomAffiliation eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::AFF_NONE == eEnumValue)
				{
					return "Camel::Dar2::SS::AFF_NONE";
				}
				if ( Camel::Dar2::SS::AFF_OUTCAT == eEnumValue)
				{
					return "Camel::Dar2::SS::AFF_OUTCAT";
				}
				if ( Camel::Dar2::SS::AFF_MEMBER == eEnumValue)
				{
					return "Camel::Dar2::SS::AFF_MEMBER";
				}
				if ( Camel::Dar2::SS::AFF_ADMIN == eEnumValue)
				{
					return "Camel::Dar2::SS::AFF_ADMIN";
				}
				if ( Camel::Dar2::SS::AFF_OWNER == eEnumValue)
				{
					return "Camel::Dar2::SS::AFF_OWNER";
				}
				if ( Camel::Dar2::SS::AFF_MAX == eEnumValue)
				{
					return "Camel::Dar2::SS::AFF_MAX";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::MUCRoomAffiliation string => enum
			inline Camel::Dar2::SS::MUCRoomAffiliation StringToEnum(const char* szStringValue, Camel::Dar2::SS::MUCRoomAffiliation eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::AFF_NONE", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::AFF_NONE;
				}
				if ( strcmp("Camel::Dar2::SS::AFF_OUTCAT", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::AFF_OUTCAT;
				}
				if ( strcmp("Camel::Dar2::SS::AFF_MEMBER", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::AFF_MEMBER;
				}
				if ( strcmp("Camel::Dar2::SS::AFF_ADMIN", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::AFF_ADMIN;
				}
				if ( strcmp("Camel::Dar2::SS::AFF_OWNER", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::AFF_OWNER;
				}
				if ( strcmp("Camel::Dar2::SS::AFF_MAX", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::AFF_MAX;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_MUCRoomAffiliation__MessageId_821875091__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_MUCRoomRole__MessageId_1080942335__
			#define __DarEnum_SS_MUCRoomRole__MessageId_1080942335__
			enum MUCRoomRole
			{
				ROLE_NONE = 1,
				ROLE_VISITOR = 2,
				ROLE_PARTICIPANT = 4,
				ROLE_MODERATOR = 8,
				ROLE_VOICEOFF = 16,
				ROLE_MAX = 32,
			};

			// Camel::Dar2::SS::MUCRoomRole enum => string
			inline const char* EnumToString(Camel::Dar2::SS::MUCRoomRole eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::ROLE_NONE == eEnumValue)
				{
					return "Camel::Dar2::SS::ROLE_NONE";
				}
				if ( Camel::Dar2::SS::ROLE_VISITOR == eEnumValue)
				{
					return "Camel::Dar2::SS::ROLE_VISITOR";
				}
				if ( Camel::Dar2::SS::ROLE_PARTICIPANT == eEnumValue)
				{
					return "Camel::Dar2::SS::ROLE_PARTICIPANT";
				}
				if ( Camel::Dar2::SS::ROLE_MODERATOR == eEnumValue)
				{
					return "Camel::Dar2::SS::ROLE_MODERATOR";
				}
				if ( Camel::Dar2::SS::ROLE_VOICEOFF == eEnumValue)
				{
					return "Camel::Dar2::SS::ROLE_VOICEOFF";
				}
				if ( Camel::Dar2::SS::ROLE_MAX == eEnumValue)
				{
					return "Camel::Dar2::SS::ROLE_MAX";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::MUCRoomRole string => enum
			inline Camel::Dar2::SS::MUCRoomRole StringToEnum(const char* szStringValue, Camel::Dar2::SS::MUCRoomRole eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::ROLE_NONE", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::ROLE_NONE;
				}
				if ( strcmp("Camel::Dar2::SS::ROLE_VISITOR", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::ROLE_VISITOR;
				}
				if ( strcmp("Camel::Dar2::SS::ROLE_PARTICIPANT", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::ROLE_PARTICIPANT;
				}
				if ( strcmp("Camel::Dar2::SS::ROLE_MODERATOR", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::ROLE_MODERATOR;
				}
				if ( strcmp("Camel::Dar2::SS::ROLE_VOICEOFF", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::ROLE_VOICEOFF;
				}
				if ( strcmp("Camel::Dar2::SS::ROLE_MAX", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::ROLE_MAX;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_MUCRoomRole__MessageId_1080942335__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_WHOIS__MessageId_3665605589__
			#define __DarEnum_SS_WHOIS__MessageId_3665605589__
			enum WHOIS
			{
				WHOIS_MODERATORS = 0,
				WHOIS_ANYONE = 1,
			};

			// Camel::Dar2::SS::WHOIS enum => string
			inline const char* EnumToString(Camel::Dar2::SS::WHOIS eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::WHOIS_MODERATORS == eEnumValue)
				{
					return "Camel::Dar2::SS::WHOIS_MODERATORS";
				}
				if ( Camel::Dar2::SS::WHOIS_ANYONE == eEnumValue)
				{
					return "Camel::Dar2::SS::WHOIS_ANYONE";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::WHOIS string => enum
			inline Camel::Dar2::SS::WHOIS StringToEnum(const char* szStringValue, Camel::Dar2::SS::WHOIS eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::WHOIS_MODERATORS", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::WHOIS_MODERATORS;
				}
				if ( strcmp("Camel::Dar2::SS::WHOIS_ANYONE", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::WHOIS_ANYONE;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_WHOIS__MessageId_3665605589__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_SubscriptionState__MessageId_3034503761__
			#define __DarEnum_SS_SubscriptionState__MessageId_3034503761__
			enum SubscriptionState
			{
				SBSC_NONE = 0,
				SBSC_FROM = 10,
				SBSC_TO = 20,
				SBSC_BOTH = 30,
				SBSC_MAX = 31,
			};

			// Camel::Dar2::SS::SubscriptionState enum => string
			inline const char* EnumToString(Camel::Dar2::SS::SubscriptionState eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::SBSC_NONE == eEnumValue)
				{
					return "Camel::Dar2::SS::SBSC_NONE";
				}
				if ( Camel::Dar2::SS::SBSC_FROM == eEnumValue)
				{
					return "Camel::Dar2::SS::SBSC_FROM";
				}
				if ( Camel::Dar2::SS::SBSC_TO == eEnumValue)
				{
					return "Camel::Dar2::SS::SBSC_TO";
				}
				if ( Camel::Dar2::SS::SBSC_BOTH == eEnumValue)
				{
					return "Camel::Dar2::SS::SBSC_BOTH";
				}
				if ( Camel::Dar2::SS::SBSC_MAX == eEnumValue)
				{
					return "Camel::Dar2::SS::SBSC_MAX";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::SubscriptionState string => enum
			inline Camel::Dar2::SS::SubscriptionState StringToEnum(const char* szStringValue, Camel::Dar2::SS::SubscriptionState eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::SBSC_NONE", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SBSC_NONE;
				}
				if ( strcmp("Camel::Dar2::SS::SBSC_FROM", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SBSC_FROM;
				}
				if ( strcmp("Camel::Dar2::SS::SBSC_TO", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SBSC_TO;
				}
				if ( strcmp("Camel::Dar2::SS::SBSC_BOTH", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SBSC_BOTH;
				}
				if ( strcmp("Camel::Dar2::SS::SBSC_MAX", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::SBSC_MAX;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_SubscriptionState__MessageId_3034503761__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_AskState__MessageId_1520185131__
			#define __DarEnum_SS_AskState__MessageId_1520185131__
			enum AskState
			{
				ASK_PENDING_NONE = 0,
				ASK_PENDING_IN = 1,
				ASK_PENDING_OUT = 2,
				ASK_PENDING_INOUT = 3,
				ASK_MAX = 4,
			};

			// Camel::Dar2::SS::AskState enum => string
			inline const char* EnumToString(Camel::Dar2::SS::AskState eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::ASK_PENDING_NONE == eEnumValue)
				{
					return "Camel::Dar2::SS::ASK_PENDING_NONE";
				}
				if ( Camel::Dar2::SS::ASK_PENDING_IN == eEnumValue)
				{
					return "Camel::Dar2::SS::ASK_PENDING_IN";
				}
				if ( Camel::Dar2::SS::ASK_PENDING_OUT == eEnumValue)
				{
					return "Camel::Dar2::SS::ASK_PENDING_OUT";
				}
				if ( Camel::Dar2::SS::ASK_PENDING_INOUT == eEnumValue)
				{
					return "Camel::Dar2::SS::ASK_PENDING_INOUT";
				}
				if ( Camel::Dar2::SS::ASK_MAX == eEnumValue)
				{
					return "Camel::Dar2::SS::ASK_MAX";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::AskState string => enum
			inline Camel::Dar2::SS::AskState StringToEnum(const char* szStringValue, Camel::Dar2::SS::AskState eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::ASK_PENDING_NONE", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::ASK_PENDING_NONE;
				}
				if ( strcmp("Camel::Dar2::SS::ASK_PENDING_IN", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::ASK_PENDING_IN;
				}
				if ( strcmp("Camel::Dar2::SS::ASK_PENDING_OUT", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::ASK_PENDING_OUT;
				}
				if ( strcmp("Camel::Dar2::SS::ASK_PENDING_INOUT", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::ASK_PENDING_INOUT;
				}
				if ( strcmp("Camel::Dar2::SS::ASK_MAX", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::ASK_MAX;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_AskState__MessageId_1520185131__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_RevisionDataType__MessageId_420017526__
			#define __DarEnum_SS_RevisionDataType__MessageId_420017526__
			enum RevisionDataType
			{
				RDT_ACCOUNTMASTER = 0,
				RDT_ROSTER = 1,
				RDT_GROUP = 2,
				RDT_GAME_INFO = 3,
				RDT_MAX = 4,
			};

			// Camel::Dar2::SS::RevisionDataType enum => string
			inline const char* EnumToString(Camel::Dar2::SS::RevisionDataType eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::RDT_ACCOUNTMASTER == eEnumValue)
				{
					return "Camel::Dar2::SS::RDT_ACCOUNTMASTER";
				}
				if ( Camel::Dar2::SS::RDT_ROSTER == eEnumValue)
				{
					return "Camel::Dar2::SS::RDT_ROSTER";
				}
				if ( Camel::Dar2::SS::RDT_GROUP == eEnumValue)
				{
					return "Camel::Dar2::SS::RDT_GROUP";
				}
				if ( Camel::Dar2::SS::RDT_GAME_INFO == eEnumValue)
				{
					return "Camel::Dar2::SS::RDT_GAME_INFO";
				}
				if ( Camel::Dar2::SS::RDT_MAX == eEnumValue)
				{
					return "Camel::Dar2::SS::RDT_MAX";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::RevisionDataType string => enum
			inline Camel::Dar2::SS::RevisionDataType StringToEnum(const char* szStringValue, Camel::Dar2::SS::RevisionDataType eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::RDT_ACCOUNTMASTER", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::RDT_ACCOUNTMASTER;
				}
				if ( strcmp("Camel::Dar2::SS::RDT_ROSTER", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::RDT_ROSTER;
				}
				if ( strcmp("Camel::Dar2::SS::RDT_GROUP", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::RDT_GROUP;
				}
				if ( strcmp("Camel::Dar2::SS::RDT_GAME_INFO", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::RDT_GAME_INFO;
				}
				if ( strcmp("Camel::Dar2::SS::RDT_MAX", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::RDT_MAX;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_RevisionDataType__MessageId_420017526__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_UUID_CODE__MessageId_3062834012__
			#define __DarEnum_SS_UUID_CODE__MessageId_3062834012__
			enum UUID_CODE
			{
				INVALID_UUID = 0,
			};

			// Camel::Dar2::SS::UUID_CODE enum => string
			inline const char* EnumToString(Camel::Dar2::SS::UUID_CODE eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::INVALID_UUID == eEnumValue)
				{
					return "Camel::Dar2::SS::INVALID_UUID";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::UUID_CODE string => enum
			inline Camel::Dar2::SS::UUID_CODE StringToEnum(const char* szStringValue, Camel::Dar2::SS::UUID_CODE eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::INVALID_UUID", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::INVALID_UUID;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_UUID_CODE__MessageId_3062834012__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_RegistNewDevice_ErrorCode__MessageId_3369328855__
			#define __DarEnum_SS_RegistNewDevice_ErrorCode__MessageId_3369328855__
			enum RegistNewDevice_ErrorCode
			{
				RND_SUCCESS = 0,
				RND_FAIL_ALREADY_EXIST = 100,
				RND_FAIL_INVALID_JID = 101,
				RND_FAIL_INVALID_UDID = 102,
				RND_FAIL_INVALID_SERVICECODE = 103,
				RND_FAIL_UNKONW_REASON = 104,
			};

			// Camel::Dar2::SS::RegistNewDevice_ErrorCode enum => string
			inline const char* EnumToString(Camel::Dar2::SS::RegistNewDevice_ErrorCode eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::RND_SUCCESS == eEnumValue)
				{
					return "Camel::Dar2::SS::RND_SUCCESS";
				}
				if ( Camel::Dar2::SS::RND_FAIL_ALREADY_EXIST == eEnumValue)
				{
					return "Camel::Dar2::SS::RND_FAIL_ALREADY_EXIST";
				}
				if ( Camel::Dar2::SS::RND_FAIL_INVALID_JID == eEnumValue)
				{
					return "Camel::Dar2::SS::RND_FAIL_INVALID_JID";
				}
				if ( Camel::Dar2::SS::RND_FAIL_INVALID_UDID == eEnumValue)
				{
					return "Camel::Dar2::SS::RND_FAIL_INVALID_UDID";
				}
				if ( Camel::Dar2::SS::RND_FAIL_INVALID_SERVICECODE == eEnumValue)
				{
					return "Camel::Dar2::SS::RND_FAIL_INVALID_SERVICECODE";
				}
				if ( Camel::Dar2::SS::RND_FAIL_UNKONW_REASON == eEnumValue)
				{
					return "Camel::Dar2::SS::RND_FAIL_UNKONW_REASON";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::RegistNewDevice_ErrorCode string => enum
			inline Camel::Dar2::SS::RegistNewDevice_ErrorCode StringToEnum(const char* szStringValue, Camel::Dar2::SS::RegistNewDevice_ErrorCode eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::RND_SUCCESS", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::RND_SUCCESS;
				}
				if ( strcmp("Camel::Dar2::SS::RND_FAIL_ALREADY_EXIST", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::RND_FAIL_ALREADY_EXIST;
				}
				if ( strcmp("Camel::Dar2::SS::RND_FAIL_INVALID_JID", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::RND_FAIL_INVALID_JID;
				}
				if ( strcmp("Camel::Dar2::SS::RND_FAIL_INVALID_UDID", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::RND_FAIL_INVALID_UDID;
				}
				if ( strcmp("Camel::Dar2::SS::RND_FAIL_INVALID_SERVICECODE", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::RND_FAIL_INVALID_SERVICECODE;
				}
				if ( strcmp("Camel::Dar2::SS::RND_FAIL_UNKONW_REASON", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::RND_FAIL_UNKONW_REASON;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_RegistNewDevice_ErrorCode__MessageId_3369328855__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_MobileDeviceType__MessageId_1729302997__
			#define __DarEnum_SS_MobileDeviceType__MessageId_1729302997__
			enum MobileDeviceType
			{
				MDT_ANDROID = 0,
				MDT_IPHONE = 1,
				MDT_WINDOW = 2,
				MDT_SIMBIAN = 3,
				UNKNOWN_DEVICE_TYPE = 4,
			};

			// Camel::Dar2::SS::MobileDeviceType enum => string
			inline const char* EnumToString(Camel::Dar2::SS::MobileDeviceType eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::MDT_ANDROID == eEnumValue)
				{
					return "Camel::Dar2::SS::MDT_ANDROID";
				}
				if ( Camel::Dar2::SS::MDT_IPHONE == eEnumValue)
				{
					return "Camel::Dar2::SS::MDT_IPHONE";
				}
				if ( Camel::Dar2::SS::MDT_WINDOW == eEnumValue)
				{
					return "Camel::Dar2::SS::MDT_WINDOW";
				}
				if ( Camel::Dar2::SS::MDT_SIMBIAN == eEnumValue)
				{
					return "Camel::Dar2::SS::MDT_SIMBIAN";
				}
				if ( Camel::Dar2::SS::UNKNOWN_DEVICE_TYPE == eEnumValue)
				{
					return "Camel::Dar2::SS::UNKNOWN_DEVICE_TYPE";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::MobileDeviceType string => enum
			inline Camel::Dar2::SS::MobileDeviceType StringToEnum(const char* szStringValue, Camel::Dar2::SS::MobileDeviceType eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::MDT_ANDROID", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::MDT_ANDROID;
				}
				if ( strcmp("Camel::Dar2::SS::MDT_IPHONE", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::MDT_IPHONE;
				}
				if ( strcmp("Camel::Dar2::SS::MDT_WINDOW", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::MDT_WINDOW;
				}
				if ( strcmp("Camel::Dar2::SS::MDT_SIMBIAN", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::MDT_SIMBIAN;
				}
				if ( strcmp("Camel::Dar2::SS::UNKNOWN_DEVICE_TYPE", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::UNKNOWN_DEVICE_TYPE;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_MobileDeviceType__MessageId_1729302997__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_UpdateMobileDeviceInfo_ErrorCode__MessageId_3654690598__
			#define __DarEnum_SS_UpdateMobileDeviceInfo_ErrorCode__MessageId_3654690598__
			enum UpdateMobileDeviceInfo_ErrorCode
			{
				UMD_SUCCESS = 0,
				UMD_FAIL_NOT_FOUND_UUID = 100,
				UMD_FAIL_INVALID_JID = 101,
				UMD_FAIL_INVALID_UUID = 102,
				UMD_FAIL_UNKNOWN_REASON = 103,
			};

			// Camel::Dar2::SS::UpdateMobileDeviceInfo_ErrorCode enum => string
			inline const char* EnumToString(Camel::Dar2::SS::UpdateMobileDeviceInfo_ErrorCode eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::UMD_SUCCESS == eEnumValue)
				{
					return "Camel::Dar2::SS::UMD_SUCCESS";
				}
				if ( Camel::Dar2::SS::UMD_FAIL_NOT_FOUND_UUID == eEnumValue)
				{
					return "Camel::Dar2::SS::UMD_FAIL_NOT_FOUND_UUID";
				}
				if ( Camel::Dar2::SS::UMD_FAIL_INVALID_JID == eEnumValue)
				{
					return "Camel::Dar2::SS::UMD_FAIL_INVALID_JID";
				}
				if ( Camel::Dar2::SS::UMD_FAIL_INVALID_UUID == eEnumValue)
				{
					return "Camel::Dar2::SS::UMD_FAIL_INVALID_UUID";
				}
				if ( Camel::Dar2::SS::UMD_FAIL_UNKNOWN_REASON == eEnumValue)
				{
					return "Camel::Dar2::SS::UMD_FAIL_UNKNOWN_REASON";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::UpdateMobileDeviceInfo_ErrorCode string => enum
			inline Camel::Dar2::SS::UpdateMobileDeviceInfo_ErrorCode StringToEnum(const char* szStringValue, Camel::Dar2::SS::UpdateMobileDeviceInfo_ErrorCode eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::UMD_SUCCESS", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::UMD_SUCCESS;
				}
				if ( strcmp("Camel::Dar2::SS::UMD_FAIL_NOT_FOUND_UUID", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::UMD_FAIL_NOT_FOUND_UUID;
				}
				if ( strcmp("Camel::Dar2::SS::UMD_FAIL_INVALID_JID", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::UMD_FAIL_INVALID_JID;
				}
				if ( strcmp("Camel::Dar2::SS::UMD_FAIL_INVALID_UUID", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::UMD_FAIL_INVALID_UUID;
				}
				if ( strcmp("Camel::Dar2::SS::UMD_FAIL_UNKNOWN_REASON", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::UMD_FAIL_UNKNOWN_REASON;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_UpdateMobileDeviceInfo_ErrorCode__MessageId_3654690598__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_UpdatePushRegistrationID_ErrorCode__MessageId_3152764634__
			#define __DarEnum_SS_UpdatePushRegistrationID_ErrorCode__MessageId_3152764634__
			enum UpdatePushRegistrationID_ErrorCode
			{
				UPR_SUCCESS = 0,
				UPR_FAIL_NOT_FOUND_UUID = 100,
				UPR_FAIL_INVALID_JID = 101,
				UPR_FAIL_INVALID_UUID = 102,
				UPR_FAIL_UNKNOWN_REASON = 103,
			};

			// Camel::Dar2::SS::UpdatePushRegistrationID_ErrorCode enum => string
			inline const char* EnumToString(Camel::Dar2::SS::UpdatePushRegistrationID_ErrorCode eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::UPR_SUCCESS == eEnumValue)
				{
					return "Camel::Dar2::SS::UPR_SUCCESS";
				}
				if ( Camel::Dar2::SS::UPR_FAIL_NOT_FOUND_UUID == eEnumValue)
				{
					return "Camel::Dar2::SS::UPR_FAIL_NOT_FOUND_UUID";
				}
				if ( Camel::Dar2::SS::UPR_FAIL_INVALID_JID == eEnumValue)
				{
					return "Camel::Dar2::SS::UPR_FAIL_INVALID_JID";
				}
				if ( Camel::Dar2::SS::UPR_FAIL_INVALID_UUID == eEnumValue)
				{
					return "Camel::Dar2::SS::UPR_FAIL_INVALID_UUID";
				}
				if ( Camel::Dar2::SS::UPR_FAIL_UNKNOWN_REASON == eEnumValue)
				{
					return "Camel::Dar2::SS::UPR_FAIL_UNKNOWN_REASON";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::UpdatePushRegistrationID_ErrorCode string => enum
			inline Camel::Dar2::SS::UpdatePushRegistrationID_ErrorCode StringToEnum(const char* szStringValue, Camel::Dar2::SS::UpdatePushRegistrationID_ErrorCode eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::UPR_SUCCESS", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::UPR_SUCCESS;
				}
				if ( strcmp("Camel::Dar2::SS::UPR_FAIL_NOT_FOUND_UUID", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::UPR_FAIL_NOT_FOUND_UUID;
				}
				if ( strcmp("Camel::Dar2::SS::UPR_FAIL_INVALID_JID", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::UPR_FAIL_INVALID_JID;
				}
				if ( strcmp("Camel::Dar2::SS::UPR_FAIL_INVALID_UUID", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::UPR_FAIL_INVALID_UUID;
				}
				if ( strcmp("Camel::Dar2::SS::UPR_FAIL_UNKNOWN_REASON", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::UPR_FAIL_UNKNOWN_REASON;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_UpdatePushRegistrationID_ErrorCode__MessageId_3152764634__
		}
		namespace SS
		{

			#if !defined __DarEnum_SS_UnRegistDevice_ErrorCode__MessageId_2367419280__
			#define __DarEnum_SS_UnRegistDevice_ErrorCode__MessageId_2367419280__
			enum UnRegistDevice_ErrorCode
			{
				URD_SUCCESS = 0,
				URD_FAIL_NOT_FOUND_UUID = 100,
				URD_FAIL_INVALID_JID = 101,
				URD_FAIL_INVALID_UUID = 102,
				URD_FAIL_UNKNOWN_REASON = 200,
			};

			// Camel::Dar2::SS::UnRegistDevice_ErrorCode enum => string
			inline const char* EnumToString(Camel::Dar2::SS::UnRegistDevice_ErrorCode eEnumValue, const char* szDefaultReturnString)
			{
				if ( Camel::Dar2::SS::URD_SUCCESS == eEnumValue)
				{
					return "Camel::Dar2::SS::URD_SUCCESS";
				}
				if ( Camel::Dar2::SS::URD_FAIL_NOT_FOUND_UUID == eEnumValue)
				{
					return "Camel::Dar2::SS::URD_FAIL_NOT_FOUND_UUID";
				}
				if ( Camel::Dar2::SS::URD_FAIL_INVALID_JID == eEnumValue)
				{
					return "Camel::Dar2::SS::URD_FAIL_INVALID_JID";
				}
				if ( Camel::Dar2::SS::URD_FAIL_INVALID_UUID == eEnumValue)
				{
					return "Camel::Dar2::SS::URD_FAIL_INVALID_UUID";
				}
				if ( Camel::Dar2::SS::URD_FAIL_UNKNOWN_REASON == eEnumValue)
				{
					return "Camel::Dar2::SS::URD_FAIL_UNKNOWN_REASON";
				}
				return szDefaultReturnString;
			}

			// Camel::Dar2::SS::UnRegistDevice_ErrorCode string => enum
			inline Camel::Dar2::SS::UnRegistDevice_ErrorCode StringToEnum(const char* szStringValue, Camel::Dar2::SS::UnRegistDevice_ErrorCode eDefaultReturnEnum)
			{
				if ( NULL == szStringValue)
				{
					return eDefaultReturnEnum;
				}
				if ( strcmp("Camel::Dar2::SS::URD_SUCCESS", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::URD_SUCCESS;
				}
				if ( strcmp("Camel::Dar2::SS::URD_FAIL_NOT_FOUND_UUID", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::URD_FAIL_NOT_FOUND_UUID;
				}
				if ( strcmp("Camel::Dar2::SS::URD_FAIL_INVALID_JID", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::URD_FAIL_INVALID_JID;
				}
				if ( strcmp("Camel::Dar2::SS::URD_FAIL_INVALID_UUID", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::URD_FAIL_INVALID_UUID;
				}
				if ( strcmp("Camel::Dar2::SS::URD_FAIL_UNKNOWN_REASON", szStringValue) == 0) 
				{
					return Camel::Dar2::SS::URD_FAIL_UNKNOWN_REASON;
				}
				return eDefaultReturnEnum;
			}
			#endif     // !defined __DarEnum_SS_UnRegistDevice_ErrorCode__MessageId_2367419280__
		}
		namespace SS
		{
			namespace Feature
			{

				#if !defined __DarEnum_SS_Feature_AnonymousType__MessageId_3603130370__
				#define __DarEnum_SS_Feature_AnonymousType__MessageId_3603130370__
				enum AnonymousType
				{
					Non_AnonymousRoom = 1,
					Semi_AnonymousRoom = 2,
					Fully_AnnoymousRoom = 3,
				};

				// Camel::Dar2::SS::Feature::AnonymousType enum => string
				inline const char* EnumToString(Camel::Dar2::SS::Feature::AnonymousType eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar2::SS::Feature::Non_AnonymousRoom == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::Non_AnonymousRoom";
					}
					if ( Camel::Dar2::SS::Feature::Semi_AnonymousRoom == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::Semi_AnonymousRoom";
					}
					if ( Camel::Dar2::SS::Feature::Fully_AnnoymousRoom == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::Fully_AnnoymousRoom";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar2::SS::Feature::AnonymousType string => enum
				inline Camel::Dar2::SS::Feature::AnonymousType StringToEnum(const char* szStringValue, Camel::Dar2::SS::Feature::AnonymousType eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::Non_AnonymousRoom", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::Non_AnonymousRoom;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::Semi_AnonymousRoom", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::Semi_AnonymousRoom;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::Fully_AnnoymousRoom", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::Fully_AnnoymousRoom;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_SS_Feature_AnonymousType__MessageId_3603130370__
			}
		}
		namespace SS
		{
			namespace Feature
			{

				#if !defined __DarEnum_SS_Feature_PublicType__MessageId_61487320__
				#define __DarEnum_SS_Feature_PublicType__MessageId_61487320__
				enum PublicType
				{
					HiddenRoom = 1,
					PublicRoom = 2,
				};

				// Camel::Dar2::SS::Feature::PublicType enum => string
				inline const char* EnumToString(Camel::Dar2::SS::Feature::PublicType eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar2::SS::Feature::HiddenRoom == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::HiddenRoom";
					}
					if ( Camel::Dar2::SS::Feature::PublicRoom == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::PublicRoom";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar2::SS::Feature::PublicType string => enum
				inline Camel::Dar2::SS::Feature::PublicType StringToEnum(const char* szStringValue, Camel::Dar2::SS::Feature::PublicType eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::HiddenRoom", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::HiddenRoom;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::PublicRoom", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::PublicRoom;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_SS_Feature_PublicType__MessageId_61487320__
			}
		}
		namespace SS
		{
			namespace Feature
			{

				#if !defined __DarEnum_SS_Feature_OpenType__MessageId_2084815499__
				#define __DarEnum_SS_Feature_OpenType__MessageId_2084815499__
				enum OpenType
				{
					Members_onlyRoom = 1,
					OpenRoom = 2,
				};

				// Camel::Dar2::SS::Feature::OpenType enum => string
				inline const char* EnumToString(Camel::Dar2::SS::Feature::OpenType eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar2::SS::Feature::Members_onlyRoom == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::Members_onlyRoom";
					}
					if ( Camel::Dar2::SS::Feature::OpenRoom == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::OpenRoom";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar2::SS::Feature::OpenType string => enum
				inline Camel::Dar2::SS::Feature::OpenType StringToEnum(const char* szStringValue, Camel::Dar2::SS::Feature::OpenType eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::Members_onlyRoom", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::Members_onlyRoom;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::OpenRoom", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::OpenRoom;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_SS_Feature_OpenType__MessageId_2084815499__
			}
		}
		namespace SS
		{
			namespace Feature
			{

				#if !defined __DarEnum_SS_Feature_ModerateType__MessageId_655294442__
				#define __DarEnum_SS_Feature_ModerateType__MessageId_655294442__
				enum ModerateType
				{
					ModeratedRoom = 1,
					UnModeratedRoom = 2,
				};

				// Camel::Dar2::SS::Feature::ModerateType enum => string
				inline const char* EnumToString(Camel::Dar2::SS::Feature::ModerateType eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar2::SS::Feature::ModeratedRoom == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::ModeratedRoom";
					}
					if ( Camel::Dar2::SS::Feature::UnModeratedRoom == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::UnModeratedRoom";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar2::SS::Feature::ModerateType string => enum
				inline Camel::Dar2::SS::Feature::ModerateType StringToEnum(const char* szStringValue, Camel::Dar2::SS::Feature::ModerateType eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::ModeratedRoom", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::ModeratedRoom;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::UnModeratedRoom", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::UnModeratedRoom;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_SS_Feature_ModerateType__MessageId_655294442__
			}
		}
		namespace SS
		{
			namespace Feature
			{

				#if !defined __DarEnum_SS_Feature_SecureType__MessageId_1771229056__
				#define __DarEnum_SS_Feature_SecureType__MessageId_1771229056__
				enum SecureType
				{
					Password_PretectedRoom = 1,
					UnSecuredRoom = 2,
				};

				// Camel::Dar2::SS::Feature::SecureType enum => string
				inline const char* EnumToString(Camel::Dar2::SS::Feature::SecureType eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar2::SS::Feature::Password_PretectedRoom == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::Password_PretectedRoom";
					}
					if ( Camel::Dar2::SS::Feature::UnSecuredRoom == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::UnSecuredRoom";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar2::SS::Feature::SecureType string => enum
				inline Camel::Dar2::SS::Feature::SecureType StringToEnum(const char* szStringValue, Camel::Dar2::SS::Feature::SecureType eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::Password_PretectedRoom", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::Password_PretectedRoom;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::UnSecuredRoom", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::UnSecuredRoom;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_SS_Feature_SecureType__MessageId_1771229056__
			}
		}
		namespace SS
		{
			namespace Feature
			{

				#if !defined __DarEnum_SS_Feature_PersistentType__MessageId_607330218__
				#define __DarEnum_SS_Feature_PersistentType__MessageId_607330218__
				enum PersistentType
				{
					PersistentRoom = 1,
					TempararyRoom = 2,
				};

				// Camel::Dar2::SS::Feature::PersistentType enum => string
				inline const char* EnumToString(Camel::Dar2::SS::Feature::PersistentType eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar2::SS::Feature::PersistentRoom == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::PersistentRoom";
					}
					if ( Camel::Dar2::SS::Feature::TempararyRoom == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::TempararyRoom";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar2::SS::Feature::PersistentType string => enum
				inline Camel::Dar2::SS::Feature::PersistentType StringToEnum(const char* szStringValue, Camel::Dar2::SS::Feature::PersistentType eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::PersistentRoom", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::PersistentRoom;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::TempararyRoom", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::TempararyRoom;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_SS_Feature_PersistentType__MessageId_607330218__
			}
		}
		namespace SS
		{
			namespace Feature
			{

				#if !defined __DarEnum_SS_Feature_RoomVoiceOnOffType__MessageId_751725028__
				#define __DarEnum_SS_Feature_RoomVoiceOnOffType__MessageId_751725028__
				enum RoomVoiceOnOffType
				{
					VoiceOn = 1,
					VoiceOff = 2,
				};

				// Camel::Dar2::SS::Feature::RoomVoiceOnOffType enum => string
				inline const char* EnumToString(Camel::Dar2::SS::Feature::RoomVoiceOnOffType eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar2::SS::Feature::VoiceOn == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::VoiceOn";
					}
					if ( Camel::Dar2::SS::Feature::VoiceOff == eEnumValue)
					{
						return "Camel::Dar2::SS::Feature::VoiceOff";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar2::SS::Feature::RoomVoiceOnOffType string => enum
				inline Camel::Dar2::SS::Feature::RoomVoiceOnOffType StringToEnum(const char* szStringValue, Camel::Dar2::SS::Feature::RoomVoiceOnOffType eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::VoiceOn", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::VoiceOn;
					}
					if ( strcmp("Camel::Dar2::SS::Feature::VoiceOff", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Feature::VoiceOff;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_SS_Feature_RoomVoiceOnOffType__MessageId_751725028__
			}
		}
		namespace SS
		{
			namespace Filter
			{

				#if !defined __DarEnum_SS_Filter_KeyType__MessageId_1596263452__
				#define __DarEnum_SS_Filter_KeyType__MessageId_1596263452__
				enum KeyType
				{
					ServiceCode = 1,
					CategoryCode = 2,
					RoomName = 3,
					RoomID = 4,
					ID = 5,
					CreatorID = 6,
				};

				// Camel::Dar2::SS::Filter::KeyType enum => string
				inline const char* EnumToString(Camel::Dar2::SS::Filter::KeyType eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar2::SS::Filter::ServiceCode == eEnumValue)
					{
						return "Camel::Dar2::SS::Filter::ServiceCode";
					}
					if ( Camel::Dar2::SS::Filter::CategoryCode == eEnumValue)
					{
						return "Camel::Dar2::SS::Filter::CategoryCode";
					}
					if ( Camel::Dar2::SS::Filter::RoomName == eEnumValue)
					{
						return "Camel::Dar2::SS::Filter::RoomName";
					}
					if ( Camel::Dar2::SS::Filter::RoomID == eEnumValue)
					{
						return "Camel::Dar2::SS::Filter::RoomID";
					}
					if ( Camel::Dar2::SS::Filter::ID == eEnumValue)
					{
						return "Camel::Dar2::SS::Filter::ID";
					}
					if ( Camel::Dar2::SS::Filter::CreatorID == eEnumValue)
					{
						return "Camel::Dar2::SS::Filter::CreatorID";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar2::SS::Filter::KeyType string => enum
				inline Camel::Dar2::SS::Filter::KeyType StringToEnum(const char* szStringValue, Camel::Dar2::SS::Filter::KeyType eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar2::SS::Filter::ServiceCode", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Filter::ServiceCode;
					}
					if ( strcmp("Camel::Dar2::SS::Filter::CategoryCode", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Filter::CategoryCode;
					}
					if ( strcmp("Camel::Dar2::SS::Filter::RoomName", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Filter::RoomName;
					}
					if ( strcmp("Camel::Dar2::SS::Filter::RoomID", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Filter::RoomID;
					}
					if ( strcmp("Camel::Dar2::SS::Filter::ID", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Filter::ID;
					}
					if ( strcmp("Camel::Dar2::SS::Filter::CreatorID", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Filter::CreatorID;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_SS_Filter_KeyType__MessageId_1596263452__
			}
		}
		namespace SS
		{
			namespace Filter
			{

				#if !defined __DarEnum_SS_Filter_FilterType__MessageId_2606072249__
				#define __DarEnum_SS_Filter_FilterType__MessageId_2606072249__
				enum FilterType
				{
					Full_Matching = 0,
					FrontSub_Matching = 1,
				};

				// Camel::Dar2::SS::Filter::FilterType enum => string
				inline const char* EnumToString(Camel::Dar2::SS::Filter::FilterType eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar2::SS::Filter::Full_Matching == eEnumValue)
					{
						return "Camel::Dar2::SS::Filter::Full_Matching";
					}
					if ( Camel::Dar2::SS::Filter::FrontSub_Matching == eEnumValue)
					{
						return "Camel::Dar2::SS::Filter::FrontSub_Matching";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar2::SS::Filter::FilterType string => enum
				inline Camel::Dar2::SS::Filter::FilterType StringToEnum(const char* szStringValue, Camel::Dar2::SS::Filter::FilterType eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar2::SS::Filter::Full_Matching", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Filter::Full_Matching;
					}
					if ( strcmp("Camel::Dar2::SS::Filter::FrontSub_Matching", szStringValue) == 0) 
					{
						return Camel::Dar2::SS::Filter::FrontSub_Matching;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_SS_Filter_FilterType__MessageId_2606072249__
			}
		}

		#if !defined __DarMessageProtocol_SS_MemoInfo__MessageId_947399097__
		#define __DarMessageProtocol_SS_MemoInfo__MessageId_947399097__
		namespace SS
		{
			class MemoInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=947399097U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MemoInfo";	};

				// const member valiable declare

				// member valiable declare
				short unreadCount;
				short totalCount;
				Camel::Dar2::SS::BoxPosition boxType;

				// Optional member valiable declare

				MemoInfo()
				{
					Init();
				}

				virtual ~MemoInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					unreadCount = static_cast<short>(0);
					totalCount = static_cast<short>(0);
					boxType = static_cast<Camel::Dar2::SS::BoxPosition>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					::memset(&unreadCount, 0, sizeof(unreadCount));
					::memset(&totalCount, 0, sizeof(totalCount));
					::memset(&boxType, 0, sizeof(boxType));

					// Optional member variable clear
				}

				bool Copy(const MemoInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					unreadCount = rhs.unreadCount;
					totalCount = rhs.totalCount;
					boxType = rhs.boxType;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member unreadCount
					nLength += sizeof(unreadCount);
					// GetLength Member totalCount
					nLength += sizeof(totalCount);
					// GetLength Member boxType
					nLength += sizeof(boxType);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save<short>( stream, unreadCount))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<short>( stream, totalCount))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::BoxPosition>( stream, boxType))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false ==  DAR2Rule_STREAM::Load<short>( clMessageBlock, unreadCount, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<short>( clMessageBlock, totalCount, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::BoxPosition>( clMessageBlock, boxType, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member unreadCount
					Json_root["unreadCount"] = unreadCount;
					// Save Member totalCount
					Json_root["totalCount"] = totalCount;
					// Save Member boxType
					Json_root["boxType"] = boxType;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member unreadCount
					if (! Json_root["unreadCount"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["unreadCount"], unreadCount))	{	return false;	}
					}
					// Load Member totalCount
					if (! Json_root["totalCount"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["totalCount"], totalCount))	{	return false;	}
					}
					// Load Member boxType
					if (! Json_root["boxType"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["boxType"], temp))	{	return false;	}
						boxType = static_cast<Camel::Dar2::SS::BoxPosition>(temp);
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MemoInfo__MessageId_947399097__

		#if !defined __DarMessageProtocol_SS_Memo__MessageId_1362639540__
		#define __DarMessageProtocol_SS_Memo__MessageId_1362639540__
		namespace SS
		{
			class Memo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1362639540U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::Memo";	};

				// const member valiable declare

				// member valiable declare
				std::string node;
				std::string senderID;
				std::string senderNickname;
				std::string senderCN;
				std::string content;
				std::string recvTime;
				Camel::Dar2::SS::BoxPosition boxType;
				bool read;
				int senderAvatarInfo[26];

				// Optional member valiable declare

				Memo()
				{
					Init();
				}

				virtual ~Memo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					node = "";
					senderID = "";
					senderNickname = "";
					senderCN = "";
					content = "";
					recvTime = "";
					boxType = static_cast<Camel::Dar2::SS::BoxPosition>(0);
					read = static_cast<bool>(0);
					for ( int nIndex = 0; nIndex < 26 ; ++nIndex)
					{
						senderAvatarInfo[nIndex] = static_cast<int>(0);
					}

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					node = "";
					senderID = "";
					senderNickname = "";
					senderCN = "";
					content = "";
					recvTime = "";
					::memset(&boxType, 0, sizeof(boxType));
					::memset(&read, 0, sizeof(read));
					::memset(senderAvatarInfo, 0, sizeof(senderAvatarInfo));

					// Optional member variable clear
				}

				bool Copy(const Memo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					node = rhs.node;
					senderID = rhs.senderID;
					senderNickname = rhs.senderNickname;
					senderCN = rhs.senderCN;
					content = rhs.content;
					recvTime = rhs.recvTime;
					boxType = rhs.boxType;
					read = rhs.read;
					for ( int i=0; i<26; ++i)
					{
						senderAvatarInfo[i] = rhs.senderAvatarInfo[i];
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member node
					nLength += sizeof(int);
					nLength += node.length();
					// GetLength Member senderID
					nLength += sizeof(int);
					nLength += senderID.length();
					// GetLength Member senderNickname
					nLength += sizeof(int);
					nLength += senderNickname.length();
					// GetLength Member senderCN
					nLength += sizeof(int);
					nLength += senderCN.length();
					// GetLength Member content
					nLength += sizeof(int);
					nLength += content.length();
					// GetLength Member recvTime
					nLength += sizeof(int);
					nLength += recvTime.length();
					// GetLength Member boxType
					nLength += sizeof(boxType);
					// GetLength Member read
					nLength += sizeof(read);
					// GetLength Member senderAvatarInfo
					nLength += sizeof(senderAvatarInfo);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, node))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, senderID))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, senderNickname))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, senderCN))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, content))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, recvTime))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::BoxPosition>( stream, boxType))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<bool>( stream, read))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, senderAvatarInfo, 26))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, node, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, senderID, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, senderNickname, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, senderCN, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, content, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, recvTime, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::BoxPosition>( clMessageBlock, boxType, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<bool>( clMessageBlock, read, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load<int>( clMessageBlock, senderAvatarInfo, 26, bRequireChangeByteOrder))
					{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member node
					Json_root["node"] = node;
					// Save Member senderID
					Json_root["senderID"] = senderID;
					// Save Member senderNickname
					Json_root["senderNickname"] = senderNickname;
					// Save Member senderCN
					Json_root["senderCN"] = senderCN;
					// Save Member content
					Json_root["content"] = content;
					// Save Member recvTime
					Json_root["recvTime"] = recvTime;
					// Save Member boxType
					Json_root["boxType"] = boxType;
					// Save Member read
					Json_root["read"] = read;
					// Save Member senderAvatarInfo
					{
						Json::Value JsonValue_senderAvatarInfo;
						for ( int i=0; i<26; ++i)
						{
							JsonValue_senderAvatarInfo.append( senderAvatarInfo[i]);
						}
						Json_root["senderAvatarInfo"] = JsonValue_senderAvatarInfo;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member node
					if (! Json_root["node"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["node"], node))	{	return false;	}
					}
					// Load Member senderID
					if (! Json_root["senderID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["senderID"], senderID))	{	return false;	}
					}
					// Load Member senderNickname
					if (! Json_root["senderNickname"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["senderNickname"], senderNickname))	{	return false;	}
					}
					// Load Member senderCN
					if (! Json_root["senderCN"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["senderCN"], senderCN))	{	return false;	}
					}
					// Load Member content
					if (! Json_root["content"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["content"], content))	{	return false;	}
					}
					// Load Member recvTime
					if (! Json_root["recvTime"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["recvTime"], recvTime))	{	return false;	}
					}
					// Load Member boxType
					if (! Json_root["boxType"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["boxType"], temp))	{	return false;	}
						boxType = static_cast<Camel::Dar2::SS::BoxPosition>(temp);
					}
					// Load Member read
					if (! Json_root["read"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["read"], read))	{	return false;	}
					}
					// Load Member senderAvatarInfo
					if ( (! Json_root["senderAvatarInfo"].isNull()) && (Json_root["senderAvatarInfo"].isArray()))
					{
						unsigned int n_senderAvatarInfo_Size = Json_root["senderAvatarInfo"].size();
						if ( n_senderAvatarInfo_Size > 26)	{	return false;	}
						for ( unsigned int nIndex = 0; nIndex < n_senderAvatarInfo_Size ; ++nIndex)
						{
							if (false == DAR2Rule_JSON::Load(Json_root["senderAvatarInfo"][nIndex], senderAvatarInfo[nIndex]))	{	return false;	}
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_Memo__MessageId_1362639540__

		#if !defined __DarMessageProtocol_SS_PresenceInfo__MessageId_3989506152__
		#define __DarMessageProtocol_SS_PresenceInfo__MessageId_3989506152__
		namespace SS
		{
			class PresenceInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=3989506152U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::PresenceInfo";	};

				// const member valiable declare

				// member valiable declare
				Camel::Dar2::SS::Show show;
				std::string status;
				int priority;
				std::string gameinfo;
				std::string userID;

				// Optional member valiable declare

				PresenceInfo()
				{
					Init();
				}

				virtual ~PresenceInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					show = static_cast<Camel::Dar2::SS::Show>(0);
					status = "";
					priority = static_cast<int>(0);
					gameinfo = "";
					userID = "";

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					::memset(&show, 0, sizeof(show));
					status = "";
					::memset(&priority, 0, sizeof(priority));
					gameinfo = "";
					userID = "";

					// Optional member variable clear
				}

				bool Copy(const PresenceInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					show = rhs.show;
					status = rhs.status;
					priority = rhs.priority;
					gameinfo = rhs.gameinfo;
					userID = rhs.userID;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member show
					nLength += sizeof(show);
					// GetLength Member status
					nLength += sizeof(int);
					nLength += status.length();
					// GetLength Member priority
					nLength += sizeof(priority);
					// GetLength Member gameinfo
					nLength += sizeof(int);
					nLength += gameinfo.length();
					// GetLength Member userID
					nLength += sizeof(int);
					nLength += userID.length();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::Show>( stream, show))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, status))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, priority))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, gameinfo))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, userID))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::Show>( clMessageBlock, show, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, status, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, priority, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, gameinfo, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, userID, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member show
					Json_root["show"] = show;
					// Save Member status
					Json_root["status"] = status;
					// Save Member priority
					Json_root["priority"] = priority;
					// Save Member gameinfo
					Json_root["gameinfo"] = gameinfo;
					// Save Member userID
					Json_root["userID"] = userID;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member show
					if (! Json_root["show"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["show"], temp))	{	return false;	}
						show = static_cast<Camel::Dar2::SS::Show>(temp);
					}
					// Load Member status
					if (! Json_root["status"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["status"], status))	{	return false;	}
					}
					// Load Member priority
					if (! Json_root["priority"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["priority"], priority))	{	return false;	}
					}
					// Load Member gameinfo
					if (! Json_root["gameinfo"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["gameinfo"], gameinfo))	{	return false;	}
					}
					// Load Member userID
					if (! Json_root["userID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["userID"], userID))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_PresenceInfo__MessageId_3989506152__

		#if !defined __DarMessageProtocol_SS_PresenceInfoResult__MessageId_2699743704__
		#define __DarMessageProtocol_SS_PresenceInfoResult__MessageId_2699743704__
		namespace SS
		{
			class PresenceInfoResult : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2699743704U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::PresenceInfoResult";	};

				// const member valiable declare

				// member valiable declare
				int result;
				std::string targetID;
				Camel::Dar2::SS::PresenceInfo presenceInfo;

				// Optional member valiable declare

				PresenceInfoResult()
				{
					Init();
				}

				virtual ~PresenceInfoResult()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					result = static_cast<int>(0);
					targetID = "";
					presenceInfo.Clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					::memset(&result, 0, sizeof(result));
					targetID = "";
					presenceInfo.Clear();

					// Optional member variable clear
				}

				bool Copy(const PresenceInfoResult& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					result = rhs.result;
					targetID = rhs.targetID;
					presenceInfo.Copy(rhs.presenceInfo);

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member result
					nLength += sizeof(result);
					// GetLength Member targetID
					nLength += sizeof(int);
					nLength += targetID.length();
					// GetLength Member presenceInfo
					nLength += presenceInfo.GetLength();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, result))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, targetID))		{    return false;     }
					if (false == presenceInfo.Save( stream))        {    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, result, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, targetID, bRequireChangeByteOrder))		{    return false;     }
					if ( false == presenceInfo.Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {      return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member result
					Json_root["result"] = result;
					// Save Member targetID
					Json_root["targetID"] = targetID;
					// Save Member presenceInfo
					{
						Json::Value JsonValue_presenceInfo;
						if (false == presenceInfo.SaveJSON( JsonValue_presenceInfo))        {    return false;     }
						Json_root["presenceInfo"] = JsonValue_presenceInfo;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member result
					if (! Json_root["result"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["result"], result))	{	return false;	}
					}
					// Load Member targetID
					if (! Json_root["targetID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["targetID"], targetID))	{	return false;	}
					}
					// Load Member presenceInfo
					if (! Json_root["presenceInfo"].isNull())
					{
						if (false == presenceInfo.LoadJSON( Json_root["presenceInfo"]))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_PresenceInfoResult__MessageId_2699743704__

		#if !defined __DarMessageProtocol_SS_PresenceInfoResultList__MessageId_3010289154__
		#define __DarMessageProtocol_SS_PresenceInfoResultList__MessageId_3010289154__
		namespace SS
		{
			class PresenceInfoResultList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=3010289154U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::PresenceInfoResultList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::PresenceInfoResult* > results;

				// Optional member valiable declare

				PresenceInfoResultList()
				{
					Init();
				}

				virtual ~PresenceInfoResultList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::PresenceInfoResult* >::iterator iter = results.begin(); iter != results.end(); ++iter)
					{
						Camel::Dar2::SS::PresenceInfoResult* pItem = (*iter);
						delete pItem;
					}
					results.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::PresenceInfoResult* >::iterator iter = results.begin(); iter != results.end(); ++iter)
					{
						Camel::Dar2::SS::PresenceInfoResult* pItem = (*iter);
						delete pItem;
					}
					results.clear();

					// Optional member variable clear
				}

				bool Copy(const PresenceInfoResultList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::PresenceInfoResult* >::const_iterator iter = rhs.results.begin(); iter != rhs.results.end(); ++iter)
					{
						Camel::Dar2::SS::PresenceInfoResult* pItem = new Camel::Dar2::SS::PresenceInfoResult;
						pItem->Copy(*(*iter));
						results.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member results
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::PresenceInfoResult* >::iterator iter = results.begin(); iter != results.end(); ++iter)
					{
						Camel::Dar2::SS::PresenceInfoResult* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_resultsSize = results.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_resultsSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::PresenceInfoResult* >::iterator iter = results.begin(); iter != results.end(); ++iter)
					{
						Camel::Dar2::SS::PresenceInfoResult* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_resultsSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_resultsSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_resultsSize ; ++nIndex)
					{
						Camel::Dar2::SS::PresenceInfoResult* pTemp = new Camel::Dar2::SS::PresenceInfoResult;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						results.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member results
					{
						Json::Value JsonValue_results;
						for ( std::list<Camel::Dar2::SS::PresenceInfoResult* >::const_iterator iter = results.begin(); iter != results.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::PresenceInfoResult* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_results.append( JsonValue_temp);
							}
						Json_root["results"] = JsonValue_results;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member results
					if ( (! Json_root["results"].isNull()) && (Json_root["results"].isArray()))
					{
						unsigned int n_resultsSize = Json_root["results"].size();
						for ( unsigned int nIndex = 0; nIndex < n_resultsSize ; ++nIndex)
						{
							Camel::Dar2::SS::PresenceInfoResult* temp = new Camel::Dar2::SS::PresenceInfoResult;
							if (false ==  temp->LoadJSON( Json_root["results"][nIndex]))		{    delete temp; return false;     }
							results.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_PresenceInfoResultList__MessageId_3010289154__

		#if !defined __DarMessageProtocol_SS_MemoList__MessageId_3968423730__
		#define __DarMessageProtocol_SS_MemoList__MessageId_3968423730__
		namespace SS
		{
			class MemoList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=3968423730U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MemoList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::Memo* > memoList;

				// Optional member valiable declare

				MemoList()
				{
					Init();
				}

				virtual ~MemoList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::Memo* >::iterator iter = memoList.begin(); iter != memoList.end(); ++iter)
					{
						Camel::Dar2::SS::Memo* pItem = (*iter);
						delete pItem;
					}
					memoList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::Memo* >::iterator iter = memoList.begin(); iter != memoList.end(); ++iter)
					{
						Camel::Dar2::SS::Memo* pItem = (*iter);
						delete pItem;
					}
					memoList.clear();

					// Optional member variable clear
				}

				bool Copy(const MemoList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::Memo* >::const_iterator iter = rhs.memoList.begin(); iter != rhs.memoList.end(); ++iter)
					{
						Camel::Dar2::SS::Memo* pItem = new Camel::Dar2::SS::Memo;
						pItem->Copy(*(*iter));
						memoList.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member memoList
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::Memo* >::iterator iter = memoList.begin(); iter != memoList.end(); ++iter)
					{
						Camel::Dar2::SS::Memo* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_memoListSize = memoList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_memoListSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::Memo* >::iterator iter = memoList.begin(); iter != memoList.end(); ++iter)
					{
						Camel::Dar2::SS::Memo* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_memoListSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_memoListSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_memoListSize ; ++nIndex)
					{
						Camel::Dar2::SS::Memo* pTemp = new Camel::Dar2::SS::Memo;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						memoList.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member memoList
					{
						Json::Value JsonValue_memoList;
						for ( std::list<Camel::Dar2::SS::Memo* >::const_iterator iter = memoList.begin(); iter != memoList.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::Memo* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_memoList.append( JsonValue_temp);
							}
						Json_root["memoList"] = JsonValue_memoList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member memoList
					if ( (! Json_root["memoList"].isNull()) && (Json_root["memoList"].isArray()))
					{
						unsigned int n_memoListSize = Json_root["memoList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_memoListSize ; ++nIndex)
						{
							Camel::Dar2::SS::Memo* temp = new Camel::Dar2::SS::Memo;
							if (false ==  temp->LoadJSON( Json_root["memoList"][nIndex]))		{    delete temp; return false;     }
							memoList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MemoList__MessageId_3968423730__

		#if !defined __DarMessageProtocol_SS_RemoveMemoNodeList__MessageId_3730197246__
		#define __DarMessageProtocol_SS_RemoveMemoNodeList__MessageId_3730197246__
		namespace SS
		{
			class RemoveMemoNodeList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=3730197246U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::RemoveMemoNodeList";	};

				// const member valiable declare

				// member valiable declare
				std::list<std::string> nodeList;

				// Optional member valiable declare

				RemoveMemoNodeList()
				{
					Init();
				}

				virtual ~RemoveMemoNodeList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					nodeList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					nodeList.clear();

					// Optional member variable clear
				}

				bool Copy(const RemoveMemoNodeList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					nodeList = rhs.nodeList;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member nodeList
					nLength += sizeof(int);
					for ( std::list<std::string>::iterator iter = nodeList.begin(); iter != nodeList.end(); ++iter)
					{
						nLength += sizeof(int);
						nLength += (*iter).length();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_nodeList_Size = nodeList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_nodeList_Size ))      {      return false;     }
					for ( std::list<std::string>::iterator iter = nodeList.begin(); iter != nodeList.end(); ++iter)
					{
						std::string& rRef = (*iter);
						if (false ==  DAR2Rule_STREAM::Save( stream, rRef))		{    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_nodeList_Size = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_nodeList_Size, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_nodeList_Size ; ++nIndex)
					{
						std::string temp;
						if (false == DAR2Rule_STREAM::Load( clMessageBlock, temp, bRequireChangeByteOrder))		{    return false;     }
						nodeList.push_back( temp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member nodeList
					{
						Json::Value JsonValue_nodeList;
						for ( std::list<std::string>::const_iterator iter = nodeList.begin(); iter != nodeList.end(); ++iter)
						{
							const std::string& rRef = (*iter);
							JsonValue_nodeList.append(rRef);
						}
						Json_root["nodeList"] = JsonValue_nodeList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member nodeList
					if ( (! Json_root["nodeList"].isNull()) && (Json_root["nodeList"].isArray()))
					{
						unsigned int n_nodeList_Size = Json_root["nodeList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_nodeList_Size ; ++nIndex)
						{
							std::string temp;
							if (false ==  DAR2Rule_JSON::Load( Json_root["nodeList"][nIndex], temp))		{    return false;     }
							nodeList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_RemoveMemoNodeList__MessageId_3730197246__

		#if !defined __DarMessageProtocol_SS_StoredMemoNodeList__MessageId_3245429377__
		#define __DarMessageProtocol_SS_StoredMemoNodeList__MessageId_3245429377__
		namespace SS
		{
			class StoredMemoNodeList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=3245429377U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::StoredMemoNodeList";	};

				// const member valiable declare

				// member valiable declare
				std::list<std::string> nodeList;

				// Optional member valiable declare

				StoredMemoNodeList()
				{
					Init();
				}

				virtual ~StoredMemoNodeList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					nodeList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					nodeList.clear();

					// Optional member variable clear
				}

				bool Copy(const StoredMemoNodeList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					nodeList = rhs.nodeList;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member nodeList
					nLength += sizeof(int);
					for ( std::list<std::string>::iterator iter = nodeList.begin(); iter != nodeList.end(); ++iter)
					{
						nLength += sizeof(int);
						nLength += (*iter).length();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_nodeList_Size = nodeList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_nodeList_Size ))      {      return false;     }
					for ( std::list<std::string>::iterator iter = nodeList.begin(); iter != nodeList.end(); ++iter)
					{
						std::string& rRef = (*iter);
						if (false ==  DAR2Rule_STREAM::Save( stream, rRef))		{    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_nodeList_Size = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_nodeList_Size, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_nodeList_Size ; ++nIndex)
					{
						std::string temp;
						if (false == DAR2Rule_STREAM::Load( clMessageBlock, temp, bRequireChangeByteOrder))		{    return false;     }
						nodeList.push_back( temp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member nodeList
					{
						Json::Value JsonValue_nodeList;
						for ( std::list<std::string>::const_iterator iter = nodeList.begin(); iter != nodeList.end(); ++iter)
						{
							const std::string& rRef = (*iter);
							JsonValue_nodeList.append(rRef);
						}
						Json_root["nodeList"] = JsonValue_nodeList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member nodeList
					if ( (! Json_root["nodeList"].isNull()) && (Json_root["nodeList"].isArray()))
					{
						unsigned int n_nodeList_Size = Json_root["nodeList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_nodeList_Size ; ++nIndex)
						{
							std::string temp;
							if (false ==  DAR2Rule_JSON::Load( Json_root["nodeList"][nIndex], temp))		{    return false;     }
							nodeList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_StoredMemoNodeList__MessageId_3245429377__

		#if !defined __DarMessageProtocol_SS_MemoNodeList__MessageId_509953968__
		#define __DarMessageProtocol_SS_MemoNodeList__MessageId_509953968__
		namespace SS
		{
			class MemoNodeList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=509953968U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MemoNodeList";	};

				// const member valiable declare

				// member valiable declare
				std::list<std::string> nodeList;

				// Optional member valiable declare

				MemoNodeList()
				{
					Init();
				}

				virtual ~MemoNodeList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					nodeList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					nodeList.clear();

					// Optional member variable clear
				}

				bool Copy(const MemoNodeList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					nodeList = rhs.nodeList;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member nodeList
					nLength += sizeof(int);
					for ( std::list<std::string>::iterator iter = nodeList.begin(); iter != nodeList.end(); ++iter)
					{
						nLength += sizeof(int);
						nLength += (*iter).length();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_nodeList_Size = nodeList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_nodeList_Size ))      {      return false;     }
					for ( std::list<std::string>::iterator iter = nodeList.begin(); iter != nodeList.end(); ++iter)
					{
						std::string& rRef = (*iter);
						if (false ==  DAR2Rule_STREAM::Save( stream, rRef))		{    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_nodeList_Size = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_nodeList_Size, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_nodeList_Size ; ++nIndex)
					{
						std::string temp;
						if (false == DAR2Rule_STREAM::Load( clMessageBlock, temp, bRequireChangeByteOrder))		{    return false;     }
						nodeList.push_back( temp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member nodeList
					{
						Json::Value JsonValue_nodeList;
						for ( std::list<std::string>::const_iterator iter = nodeList.begin(); iter != nodeList.end(); ++iter)
						{
							const std::string& rRef = (*iter);
							JsonValue_nodeList.append(rRef);
						}
						Json_root["nodeList"] = JsonValue_nodeList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member nodeList
					if ( (! Json_root["nodeList"].isNull()) && (Json_root["nodeList"].isArray()))
					{
						unsigned int n_nodeList_Size = Json_root["nodeList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_nodeList_Size ; ++nIndex)
						{
							std::string temp;
							if (false ==  DAR2Rule_JSON::Load( Json_root["nodeList"][nIndex], temp))		{    return false;     }
							nodeList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MemoNodeList__MessageId_509953968__

		#if !defined __DarMessageProtocol_SS_ReturnString__MessageId_553970190__
		#define __DarMessageProtocol_SS_ReturnString__MessageId_553970190__
		namespace SS
		{
			class ReturnString : public IDarMsg
			{
			public:
				static const unsigned int  msgid=553970190U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::ReturnString";	};

				// const member valiable declare

				// member valiable declare
				std::string str;

				// Optional member valiable declare

				ReturnString()
				{
					Init();
				}

				virtual ~ReturnString()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					str = "";

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					str = "";

					// Optional member variable clear
				}

				bool Copy(const ReturnString& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					str = rhs.str;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member str
					nLength += sizeof(int);
					nLength += str.length();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, str))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, str, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member str
					Json_root["str"] = str;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member str
					if (! Json_root["str"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["str"], str))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_ReturnString__MessageId_553970190__

		#if !defined __DarMessageProtocol_SS_ReceiveOption__MessageId_2240785203__
		#define __DarMessageProtocol_SS_ReceiveOption__MessageId_2240785203__
		namespace SS
		{
			class ReceiveOption : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2240785203U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::ReceiveOption";	};

				// const member valiable declare

				// member valiable declare
				std::string optionName;
				Camel::Dar2::SS::RecvState state;

				// Optional member valiable declare

				ReceiveOption()
				{
					Init();
				}

				virtual ~ReceiveOption()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					optionName = "";
					state = static_cast<Camel::Dar2::SS::RecvState>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					optionName = "";
					::memset(&state, 0, sizeof(state));

					// Optional member variable clear
				}

				bool Copy(const ReceiveOption& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					optionName = rhs.optionName;
					state = rhs.state;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member optionName
					nLength += sizeof(int);
					nLength += optionName.length();
					// GetLength Member state
					nLength += sizeof(state);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, optionName))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::RecvState>( stream, state))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, optionName, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::RecvState>( clMessageBlock, state, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member optionName
					Json_root["optionName"] = optionName;
					// Save Member state
					Json_root["state"] = state;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member optionName
					if (! Json_root["optionName"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["optionName"], optionName))	{	return false;	}
					}
					// Load Member state
					if (! Json_root["state"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["state"], temp))	{	return false;	}
						state = static_cast<Camel::Dar2::SS::RecvState>(temp);
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_ReceiveOption__MessageId_2240785203__

		#if !defined __DarMessageProtocol_SS_ReceiveOptionList__MessageId_3259475448__
		#define __DarMessageProtocol_SS_ReceiveOptionList__MessageId_3259475448__
		namespace SS
		{
			class ReceiveOptionList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=3259475448U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::ReceiveOptionList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::ReceiveOption* > optionList;

				// Optional member valiable declare

				ReceiveOptionList()
				{
					Init();
				}

				virtual ~ReceiveOptionList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::ReceiveOption* >::iterator iter = optionList.begin(); iter != optionList.end(); ++iter)
					{
						Camel::Dar2::SS::ReceiveOption* pItem = (*iter);
						delete pItem;
					}
					optionList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::ReceiveOption* >::iterator iter = optionList.begin(); iter != optionList.end(); ++iter)
					{
						Camel::Dar2::SS::ReceiveOption* pItem = (*iter);
						delete pItem;
					}
					optionList.clear();

					// Optional member variable clear
				}

				bool Copy(const ReceiveOptionList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::ReceiveOption* >::const_iterator iter = rhs.optionList.begin(); iter != rhs.optionList.end(); ++iter)
					{
						Camel::Dar2::SS::ReceiveOption* pItem = new Camel::Dar2::SS::ReceiveOption;
						pItem->Copy(*(*iter));
						optionList.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member optionList
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::ReceiveOption* >::iterator iter = optionList.begin(); iter != optionList.end(); ++iter)
					{
						Camel::Dar2::SS::ReceiveOption* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_optionListSize = optionList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_optionListSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::ReceiveOption* >::iterator iter = optionList.begin(); iter != optionList.end(); ++iter)
					{
						Camel::Dar2::SS::ReceiveOption* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_optionListSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_optionListSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_optionListSize ; ++nIndex)
					{
						Camel::Dar2::SS::ReceiveOption* pTemp = new Camel::Dar2::SS::ReceiveOption;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						optionList.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member optionList
					{
						Json::Value JsonValue_optionList;
						for ( std::list<Camel::Dar2::SS::ReceiveOption* >::const_iterator iter = optionList.begin(); iter != optionList.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::ReceiveOption* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_optionList.append( JsonValue_temp);
							}
						Json_root["optionList"] = JsonValue_optionList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member optionList
					if ( (! Json_root["optionList"].isNull()) && (Json_root["optionList"].isArray()))
					{
						unsigned int n_optionListSize = Json_root["optionList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_optionListSize ; ++nIndex)
						{
							Camel::Dar2::SS::ReceiveOption* temp = new Camel::Dar2::SS::ReceiveOption;
							if (false ==  temp->LoadJSON( Json_root["optionList"][nIndex]))		{    delete temp; return false;     }
							optionList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_ReceiveOptionList__MessageId_3259475448__

		#if !defined __DarMessageProtocol_SS_SendMemoResult__MessageId_3315580708__
		#define __DarMessageProtocol_SS_SendMemoResult__MessageId_3315580708__
		namespace SS
		{
			class SendMemoResult : public IDarMsg
			{
			public:
				static const unsigned int  msgid=3315580708U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::SendMemoResult";	};

				// const member valiable declare

				// member valiable declare
				std::string receiverID;
				short result;

				// Optional member valiable declare

				SendMemoResult()
				{
					Init();
				}

				virtual ~SendMemoResult()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					receiverID = "";
					result = static_cast<short>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					receiverID = "";
					::memset(&result, 0, sizeof(result));

					// Optional member variable clear
				}

				bool Copy(const SendMemoResult& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					receiverID = rhs.receiverID;
					result = rhs.result;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member receiverID
					nLength += sizeof(int);
					nLength += receiverID.length();
					// GetLength Member result
					nLength += sizeof(result);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, receiverID))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<short>( stream, result))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, receiverID, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<short>( clMessageBlock, result, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member receiverID
					Json_root["receiverID"] = receiverID;
					// Save Member result
					Json_root["result"] = result;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member receiverID
					if (! Json_root["receiverID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["receiverID"], receiverID))	{	return false;	}
					}
					// Load Member result
					if (! Json_root["result"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["result"], result))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_SendMemoResult__MessageId_3315580708__

		#if !defined __DarMessageProtocol_SS_SendMemoResultList__MessageId_974624124__
		#define __DarMessageProtocol_SS_SendMemoResultList__MessageId_974624124__
		namespace SS
		{
			class SendMemoResultList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=974624124U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::SendMemoResultList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::SendMemoResult* > results;

				// Optional member valiable declare

				SendMemoResultList()
				{
					Init();
				}

				virtual ~SendMemoResultList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::SendMemoResult* >::iterator iter = results.begin(); iter != results.end(); ++iter)
					{
						Camel::Dar2::SS::SendMemoResult* pItem = (*iter);
						delete pItem;
					}
					results.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::SendMemoResult* >::iterator iter = results.begin(); iter != results.end(); ++iter)
					{
						Camel::Dar2::SS::SendMemoResult* pItem = (*iter);
						delete pItem;
					}
					results.clear();

					// Optional member variable clear
				}

				bool Copy(const SendMemoResultList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::SendMemoResult* >::const_iterator iter = rhs.results.begin(); iter != rhs.results.end(); ++iter)
					{
						Camel::Dar2::SS::SendMemoResult* pItem = new Camel::Dar2::SS::SendMemoResult;
						pItem->Copy(*(*iter));
						results.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member results
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::SendMemoResult* >::iterator iter = results.begin(); iter != results.end(); ++iter)
					{
						Camel::Dar2::SS::SendMemoResult* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_resultsSize = results.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_resultsSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::SendMemoResult* >::iterator iter = results.begin(); iter != results.end(); ++iter)
					{
						Camel::Dar2::SS::SendMemoResult* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_resultsSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_resultsSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_resultsSize ; ++nIndex)
					{
						Camel::Dar2::SS::SendMemoResult* pTemp = new Camel::Dar2::SS::SendMemoResult;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						results.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member results
					{
						Json::Value JsonValue_results;
						for ( std::list<Camel::Dar2::SS::SendMemoResult* >::const_iterator iter = results.begin(); iter != results.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::SendMemoResult* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_results.append( JsonValue_temp);
							}
						Json_root["results"] = JsonValue_results;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member results
					if ( (! Json_root["results"].isNull()) && (Json_root["results"].isArray()))
					{
						unsigned int n_resultsSize = Json_root["results"].size();
						for ( unsigned int nIndex = 0; nIndex < n_resultsSize ; ++nIndex)
						{
							Camel::Dar2::SS::SendMemoResult* temp = new Camel::Dar2::SS::SendMemoResult;
							if (false ==  temp->LoadJSON( Json_root["results"][nIndex]))		{    delete temp; return false;     }
							results.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_SendMemoResultList__MessageId_974624124__

		#if !defined __DarMessageProtocol_SS_CategoryInfo__MessageId_2127400876__
		#define __DarMessageProtocol_SS_CategoryInfo__MessageId_2127400876__
		namespace SS
		{
			class CategoryInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2127400876U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::CategoryInfo";	};

				// const member valiable declare

				// member valiable declare
				std::string code;
				std::string desc;
				std::string serviceurl;

				// Optional member valiable declare

				CategoryInfo()
				{
					Init();
				}

				virtual ~CategoryInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					code = "";
					desc = "";
					serviceurl = "";

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					code = "";
					desc = "";
					serviceurl = "";

					// Optional member variable clear
				}

				bool Copy(const CategoryInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					code = rhs.code;
					desc = rhs.desc;
					serviceurl = rhs.serviceurl;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member code
					nLength += sizeof(int);
					nLength += code.length();
					// GetLength Member desc
					nLength += sizeof(int);
					nLength += desc.length();
					// GetLength Member serviceurl
					nLength += sizeof(int);
					nLength += serviceurl.length();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, code))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, desc))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, serviceurl))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, code, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, desc, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, serviceurl, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member code
					Json_root["code"] = code;
					// Save Member desc
					Json_root["desc"] = desc;
					// Save Member serviceurl
					Json_root["serviceurl"] = serviceurl;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member code
					if (! Json_root["code"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["code"], code))	{	return false;	}
					}
					// Load Member desc
					if (! Json_root["desc"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["desc"], desc))	{	return false;	}
					}
					// Load Member serviceurl
					if (! Json_root["serviceurl"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["serviceurl"], serviceurl))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_CategoryInfo__MessageId_2127400876__

		#if !defined __DarMessageProtocol_SS_MucRoomType__MessageId_2720795742__
		#define __DarMessageProtocol_SS_MucRoomType__MessageId_2720795742__
		namespace SS
		{
			class MucRoomType : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2720795742U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MucRoomType";	};

				// const member valiable declare

				// member valiable declare
				Camel::Dar2::SS::Feature::PublicType publicType;
				Camel::Dar2::SS::Feature::PersistentType persistentType;
				Camel::Dar2::SS::Feature::SecureType secureType;
				Camel::Dar2::SS::Feature::ModerateType moderateType;
				Camel::Dar2::SS::Feature::AnonymousType anonymousType;
				Camel::Dar2::SS::Feature::OpenType openType;
				Camel::Dar2::SS::Feature::RoomVoiceOnOffType voiceonoffType;

				// Optional member valiable declare

				MucRoomType()
				{
					Init();
				}

				virtual ~MucRoomType()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					publicType = static_cast<Camel::Dar2::SS::Feature::PublicType>(0);
					persistentType = static_cast<Camel::Dar2::SS::Feature::PersistentType>(0);
					secureType = static_cast<Camel::Dar2::SS::Feature::SecureType>(0);
					moderateType = static_cast<Camel::Dar2::SS::Feature::ModerateType>(0);
					anonymousType = static_cast<Camel::Dar2::SS::Feature::AnonymousType>(0);
					openType = static_cast<Camel::Dar2::SS::Feature::OpenType>(0);
					voiceonoffType = static_cast<Camel::Dar2::SS::Feature::RoomVoiceOnOffType>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					::memset(&publicType, 0, sizeof(publicType));
					::memset(&persistentType, 0, sizeof(persistentType));
					::memset(&secureType, 0, sizeof(secureType));
					::memset(&moderateType, 0, sizeof(moderateType));
					::memset(&anonymousType, 0, sizeof(anonymousType));
					::memset(&openType, 0, sizeof(openType));
					::memset(&voiceonoffType, 0, sizeof(voiceonoffType));

					// Optional member variable clear
				}

				bool Copy(const MucRoomType& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					publicType = rhs.publicType;
					persistentType = rhs.persistentType;
					secureType = rhs.secureType;
					moderateType = rhs.moderateType;
					anonymousType = rhs.anonymousType;
					openType = rhs.openType;
					voiceonoffType = rhs.voiceonoffType;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member publicType
					nLength += sizeof(publicType);
					// GetLength Member persistentType
					nLength += sizeof(persistentType);
					// GetLength Member secureType
					nLength += sizeof(secureType);
					// GetLength Member moderateType
					nLength += sizeof(moderateType);
					// GetLength Member anonymousType
					nLength += sizeof(anonymousType);
					// GetLength Member openType
					nLength += sizeof(openType);
					// GetLength Member voiceonoffType
					nLength += sizeof(voiceonoffType);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::Feature::PublicType>( stream, publicType))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::Feature::PersistentType>( stream, persistentType))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::Feature::SecureType>( stream, secureType))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::Feature::ModerateType>( stream, moderateType))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::Feature::AnonymousType>( stream, anonymousType))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::Feature::OpenType>( stream, openType))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::Feature::RoomVoiceOnOffType>( stream, voiceonoffType))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::Feature::PublicType>( clMessageBlock, publicType, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::Feature::PersistentType>( clMessageBlock, persistentType, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::Feature::SecureType>( clMessageBlock, secureType, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::Feature::ModerateType>( clMessageBlock, moderateType, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::Feature::AnonymousType>( clMessageBlock, anonymousType, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::Feature::OpenType>( clMessageBlock, openType, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::Feature::RoomVoiceOnOffType>( clMessageBlock, voiceonoffType, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member publicType
					Json_root["publicType"] = publicType;
					// Save Member persistentType
					Json_root["persistentType"] = persistentType;
					// Save Member secureType
					Json_root["secureType"] = secureType;
					// Save Member moderateType
					Json_root["moderateType"] = moderateType;
					// Save Member anonymousType
					Json_root["anonymousType"] = anonymousType;
					// Save Member openType
					Json_root["openType"] = openType;
					// Save Member voiceonoffType
					Json_root["voiceonoffType"] = voiceonoffType;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member publicType
					if (! Json_root["publicType"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["publicType"], temp))	{	return false;	}
						publicType = static_cast<Camel::Dar2::SS::Feature::PublicType>(temp);
					}
					// Load Member persistentType
					if (! Json_root["persistentType"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["persistentType"], temp))	{	return false;	}
						persistentType = static_cast<Camel::Dar2::SS::Feature::PersistentType>(temp);
					}
					// Load Member secureType
					if (! Json_root["secureType"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["secureType"], temp))	{	return false;	}
						secureType = static_cast<Camel::Dar2::SS::Feature::SecureType>(temp);
					}
					// Load Member moderateType
					if (! Json_root["moderateType"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["moderateType"], temp))	{	return false;	}
						moderateType = static_cast<Camel::Dar2::SS::Feature::ModerateType>(temp);
					}
					// Load Member anonymousType
					if (! Json_root["anonymousType"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["anonymousType"], temp))	{	return false;	}
						anonymousType = static_cast<Camel::Dar2::SS::Feature::AnonymousType>(temp);
					}
					// Load Member openType
					if (! Json_root["openType"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["openType"], temp))	{	return false;	}
						openType = static_cast<Camel::Dar2::SS::Feature::OpenType>(temp);
					}
					// Load Member voiceonoffType
					if (! Json_root["voiceonoffType"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["voiceonoffType"], temp))	{	return false;	}
						voiceonoffType = static_cast<Camel::Dar2::SS::Feature::RoomVoiceOnOffType>(temp);
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MucRoomType__MessageId_2720795742__

		#if !defined __DarMessageProtocol_SS_MucRoomExtInfo__MessageId_2443321872__
		#define __DarMessageProtocol_SS_MucRoomExtInfo__MessageId_2443321872__
		namespace SS
		{
			class MucRoomExtInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2443321872U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MucRoomExtInfo";	};

				// const member valiable declare

				// member valiable declare
				std::string description;
				std::string subject;
				bool canChangeSubject;
				int occupants;

				// Optional member valiable declare

				MucRoomExtInfo()
				{
					Init();
				}

				virtual ~MucRoomExtInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					description = "";
					subject = "";
					canChangeSubject = static_cast<bool>(0);
					occupants = static_cast<int>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					description = "";
					subject = "";
					::memset(&canChangeSubject, 0, sizeof(canChangeSubject));
					::memset(&occupants, 0, sizeof(occupants));

					// Optional member variable clear
				}

				bool Copy(const MucRoomExtInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					description = rhs.description;
					subject = rhs.subject;
					canChangeSubject = rhs.canChangeSubject;
					occupants = rhs.occupants;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member description
					nLength += sizeof(int);
					nLength += description.length();
					// GetLength Member subject
					nLength += sizeof(int);
					nLength += subject.length();
					// GetLength Member canChangeSubject
					nLength += sizeof(canChangeSubject);
					// GetLength Member occupants
					nLength += sizeof(occupants);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, description))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, subject))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<bool>( stream, canChangeSubject))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, occupants))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, description, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, subject, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<bool>( clMessageBlock, canChangeSubject, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, occupants, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member description
					Json_root["description"] = description;
					// Save Member subject
					Json_root["subject"] = subject;
					// Save Member canChangeSubject
					Json_root["canChangeSubject"] = canChangeSubject;
					// Save Member occupants
					Json_root["occupants"] = occupants;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member description
					if (! Json_root["description"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["description"], description))	{	return false;	}
					}
					// Load Member subject
					if (! Json_root["subject"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["subject"], subject))	{	return false;	}
					}
					// Load Member canChangeSubject
					if (! Json_root["canChangeSubject"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["canChangeSubject"], canChangeSubject))	{	return false;	}
					}
					// Load Member occupants
					if (! Json_root["occupants"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["occupants"], occupants))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MucRoomExtInfo__MessageId_2443321872__

		#if !defined __DarMessageProtocol_SS_MUCUser__MessageId_3263596948__
		#define __DarMessageProtocol_SS_MUCUser__MessageId_3263596948__
		namespace SS
		{
			class MUCUser : public IDarMsg
			{
			public:
				static const unsigned int  msgid=3263596948U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MUCUser";	};

				// const member valiable declare

				// member valiable declare
				std::string roomID;
				Camel::Dar2::SS::MUCRoomAffiliation affiliation;
				Camel::Dar2::SS::MUCRoomRole role;

				// Optional member valiable declare

				MUCUser()
				{
					Init();
				}

				virtual ~MUCUser()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					roomID = "";
					affiliation = static_cast<Camel::Dar2::SS::MUCRoomAffiliation>(0);
					role = static_cast<Camel::Dar2::SS::MUCRoomRole>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					roomID = "";
					::memset(&affiliation, 0, sizeof(affiliation));
					::memset(&role, 0, sizeof(role));

					// Optional member variable clear
				}

				bool Copy(const MUCUser& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					roomID = rhs.roomID;
					affiliation = rhs.affiliation;
					role = rhs.role;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member roomID
					nLength += sizeof(int);
					nLength += roomID.length();
					// GetLength Member affiliation
					nLength += sizeof(affiliation);
					// GetLength Member role
					nLength += sizeof(role);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, roomID))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::MUCRoomAffiliation>( stream, affiliation))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::MUCRoomRole>( stream, role))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, roomID, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::MUCRoomAffiliation>( clMessageBlock, affiliation, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::MUCRoomRole>( clMessageBlock, role, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member roomID
					Json_root["roomID"] = roomID;
					// Save Member affiliation
					Json_root["affiliation"] = affiliation;
					// Save Member role
					Json_root["role"] = role;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member roomID
					if (! Json_root["roomID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["roomID"], roomID))	{	return false;	}
					}
					// Load Member affiliation
					if (! Json_root["affiliation"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["affiliation"], temp))	{	return false;	}
						affiliation = static_cast<Camel::Dar2::SS::MUCRoomAffiliation>(temp);
					}
					// Load Member role
					if (! Json_root["role"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["role"], temp))	{	return false;	}
						role = static_cast<Camel::Dar2::SS::MUCRoomRole>(temp);
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MUCUser__MessageId_3263596948__

		#if !defined __DarMessageProtocol_SS_MUCUserInfo__MessageId_901362779__
		#define __DarMessageProtocol_SS_MUCUserInfo__MessageId_901362779__
		namespace SS
		{
			class MUCUserInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=901362779U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MUCUserInfo";	};

				// const member valiable declare

				// member valiable declare
				std::string id;
				std::string nickname;
				int avatarInfo[26];
				Camel::Dar2::SS::MUCUser mucUser;

				// Optional member valiable declare

				MUCUserInfo()
				{
					Init();
				}

				virtual ~MUCUserInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					id = "";
					nickname = "";
					for ( int nIndex = 0; nIndex < 26 ; ++nIndex)
					{
						avatarInfo[nIndex] = static_cast<int>(0);
					}
					mucUser.Clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					id = "";
					nickname = "";
					::memset(avatarInfo, 0, sizeof(avatarInfo));
					mucUser.Clear();

					// Optional member variable clear
				}

				bool Copy(const MUCUserInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					id = rhs.id;
					nickname = rhs.nickname;
					for ( int i=0; i<26; ++i)
					{
						avatarInfo[i] = rhs.avatarInfo[i];
					}
					mucUser.Copy(rhs.mucUser);

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member id
					nLength += sizeof(int);
					nLength += id.length();
					// GetLength Member nickname
					nLength += sizeof(int);
					nLength += nickname.length();
					// GetLength Member avatarInfo
					nLength += sizeof(avatarInfo);
					// GetLength Member mucUser
					nLength += mucUser.GetLength();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, id))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, nickname))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, avatarInfo, 26))		{    return false;     }
					if (false == mucUser.Save( stream))        {    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, id, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, nickname, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load<int>( clMessageBlock, avatarInfo, 26, bRequireChangeByteOrder))
					{    return false;     }
					if ( false == mucUser.Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {      return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member id
					Json_root["id"] = id;
					// Save Member nickname
					Json_root["nickname"] = nickname;
					// Save Member avatarInfo
					{
						Json::Value JsonValue_avatarInfo;
						for ( int i=0; i<26; ++i)
						{
							JsonValue_avatarInfo.append( avatarInfo[i]);
						}
						Json_root["avatarInfo"] = JsonValue_avatarInfo;
					}
					// Save Member mucUser
					{
						Json::Value JsonValue_mucUser;
						if (false == mucUser.SaveJSON( JsonValue_mucUser))        {    return false;     }
						Json_root["mucUser"] = JsonValue_mucUser;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member id
					if (! Json_root["id"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["id"], id))	{	return false;	}
					}
					// Load Member nickname
					if (! Json_root["nickname"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nickname"], nickname))	{	return false;	}
					}
					// Load Member avatarInfo
					if ( (! Json_root["avatarInfo"].isNull()) && (Json_root["avatarInfo"].isArray()))
					{
						unsigned int n_avatarInfo_Size = Json_root["avatarInfo"].size();
						if ( n_avatarInfo_Size > 26)	{	return false;	}
						for ( unsigned int nIndex = 0; nIndex < n_avatarInfo_Size ; ++nIndex)
						{
							if (false == DAR2Rule_JSON::Load(Json_root["avatarInfo"][nIndex], avatarInfo[nIndex]))	{	return false;	}
						}
					}
					// Load Member mucUser
					if (! Json_root["mucUser"].isNull())
					{
						if (false == mucUser.LoadJSON( Json_root["mucUser"]))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MUCUserInfo__MessageId_901362779__

		#if !defined __DarMessageProtocol_SS_MUCRoomInfo__MessageId_3683337176__
		#define __DarMessageProtocol_SS_MUCRoomInfo__MessageId_3683337176__
		namespace SS
		{
			class MUCRoomInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=3683337176U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MUCRoomInfo";	};

				// const member valiable declare

				// member valiable declare
				std::string roomID;
				std::string roomname;
				std::string creatorID;
				std::string password;
				Camel::Dar2::SS::MucRoomType info;
				Camel::Dar2::SS::MucRoomExtInfo extInfo;
				int currentuser;
				int maxuser;

				// Optional member valiable declare

				MUCRoomInfo()
				{
					Init();
				}

				virtual ~MUCRoomInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					roomID = "";
					roomname = "";
					creatorID = "";
					password = "";
					info.Clear();
					extInfo.Clear();
					currentuser = static_cast<int>(0);
					maxuser = static_cast<int>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					roomID = "";
					roomname = "";
					creatorID = "";
					password = "";
					info.Clear();
					extInfo.Clear();
					::memset(&currentuser, 0, sizeof(currentuser));
					::memset(&maxuser, 0, sizeof(maxuser));

					// Optional member variable clear
				}

				bool Copy(const MUCRoomInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					roomID = rhs.roomID;
					roomname = rhs.roomname;
					creatorID = rhs.creatorID;
					password = rhs.password;
					info.Copy(rhs.info);
					extInfo.Copy(rhs.extInfo);
					currentuser = rhs.currentuser;
					maxuser = rhs.maxuser;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member roomID
					nLength += sizeof(int);
					nLength += roomID.length();
					// GetLength Member roomname
					nLength += sizeof(int);
					nLength += roomname.length();
					// GetLength Member creatorID
					nLength += sizeof(int);
					nLength += creatorID.length();
					// GetLength Member password
					nLength += sizeof(int);
					nLength += password.length();
					// GetLength Member info
					nLength += info.GetLength();
					// GetLength Member extInfo
					nLength += extInfo.GetLength();
					// GetLength Member currentuser
					nLength += sizeof(currentuser);
					// GetLength Member maxuser
					nLength += sizeof(maxuser);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, roomID))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, roomname))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, creatorID))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, password))		{    return false;     }
					if (false == info.Save( stream))        {    return false;     }
					if (false == extInfo.Save( stream))        {    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, currentuser))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, maxuser))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, roomID, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, roomname, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, creatorID, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, password, bRequireChangeByteOrder))		{    return false;     }
					if ( false == info.Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {      return false;     }
					if ( false == extInfo.Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {      return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, currentuser, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, maxuser, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member roomID
					Json_root["roomID"] = roomID;
					// Save Member roomname
					Json_root["roomname"] = roomname;
					// Save Member creatorID
					Json_root["creatorID"] = creatorID;
					// Save Member password
					Json_root["password"] = password;
					// Save Member info
					{
						Json::Value JsonValue_info;
						if (false == info.SaveJSON( JsonValue_info))        {    return false;     }
						Json_root["info"] = JsonValue_info;
					}
					// Save Member extInfo
					{
						Json::Value JsonValue_extInfo;
						if (false == extInfo.SaveJSON( JsonValue_extInfo))        {    return false;     }
						Json_root["extInfo"] = JsonValue_extInfo;
					}
					// Save Member currentuser
					Json_root["currentuser"] = currentuser;
					// Save Member maxuser
					Json_root["maxuser"] = maxuser;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member roomID
					if (! Json_root["roomID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["roomID"], roomID))	{	return false;	}
					}
					// Load Member roomname
					if (! Json_root["roomname"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["roomname"], roomname))	{	return false;	}
					}
					// Load Member creatorID
					if (! Json_root["creatorID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["creatorID"], creatorID))	{	return false;	}
					}
					// Load Member password
					if (! Json_root["password"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["password"], password))	{	return false;	}
					}
					// Load Member info
					if (! Json_root["info"].isNull())
					{
						if (false == info.LoadJSON( Json_root["info"]))	{	return false;	}
					}
					// Load Member extInfo
					if (! Json_root["extInfo"].isNull())
					{
						if (false == extInfo.LoadJSON( Json_root["extInfo"]))	{	return false;	}
					}
					// Load Member currentuser
					if (! Json_root["currentuser"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["currentuser"], currentuser))	{	return false;	}
					}
					// Load Member maxuser
					if (! Json_root["maxuser"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["maxuser"], maxuser))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MUCRoomInfo__MessageId_3683337176__

		#if !defined __DarMessageProtocol_SS_MUCJoinRoomInfo__MessageId_2866939553__
		#define __DarMessageProtocol_SS_MUCJoinRoomInfo__MessageId_2866939553__
		namespace SS
		{
			class MUCJoinRoomInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2866939553U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MUCJoinRoomInfo";	};

				// const member valiable declare

				// member valiable declare
				std::string roomID;
				std::string roomNickname;
				std::string password;
				std::string servicecode;

				// Optional member valiable declare

				MUCJoinRoomInfo()
				{
					Init();
				}

				virtual ~MUCJoinRoomInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					roomID = "";
					roomNickname = "";
					password = "";
					servicecode = "";

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					roomID = "";
					roomNickname = "";
					password = "";
					servicecode = "";

					// Optional member variable clear
				}

				bool Copy(const MUCJoinRoomInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					roomID = rhs.roomID;
					roomNickname = rhs.roomNickname;
					password = rhs.password;
					servicecode = rhs.servicecode;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member roomID
					nLength += sizeof(int);
					nLength += roomID.length();
					// GetLength Member roomNickname
					nLength += sizeof(int);
					nLength += roomNickname.length();
					// GetLength Member password
					nLength += sizeof(int);
					nLength += password.length();
					// GetLength Member servicecode
					nLength += sizeof(int);
					nLength += servicecode.length();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, roomID))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, roomNickname))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, password))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, servicecode))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, roomID, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, roomNickname, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, password, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, servicecode, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member roomID
					Json_root["roomID"] = roomID;
					// Save Member roomNickname
					Json_root["roomNickname"] = roomNickname;
					// Save Member password
					Json_root["password"] = password;
					// Save Member servicecode
					Json_root["servicecode"] = servicecode;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member roomID
					if (! Json_root["roomID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["roomID"], roomID))	{	return false;	}
					}
					// Load Member roomNickname
					if (! Json_root["roomNickname"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["roomNickname"], roomNickname))	{	return false;	}
					}
					// Load Member password
					if (! Json_root["password"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["password"], password))	{	return false;	}
					}
					// Load Member servicecode
					if (! Json_root["servicecode"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["servicecode"], servicecode))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MUCJoinRoomInfo__MessageId_2866939553__

		#if !defined __DarMessageProtocol_SS_Filter_FilterInfo__MessageId_266869564__
		#define __DarMessageProtocol_SS_Filter_FilterInfo__MessageId_266869564__
		namespace SS
		{
			namespace Filter
			{
				class FilterInfo : public IDarMsg
				{
				public:
					static const unsigned int  msgid=266869564U;

					virtual unsigned int GetMessageId()		{	return msgid;	};
					virtual  const char* GetMessageName()	{	return "SS::Filter::FilterInfo";	};

					// const member valiable declare

					// member valiable declare
					Camel::Dar2::SS::Filter::KeyType key;
					Camel::Dar2::SS::Filter::FilterType type;
					std::string value;

					// Optional member valiable declare

					FilterInfo()
					{
						Init();
					}

					virtual ~FilterInfo()
					{
						Clear();
					}

					virtual void Init()
					{
						// member variable init
						key = static_cast<Camel::Dar2::SS::Filter::KeyType>(0);
						type = static_cast<Camel::Dar2::SS::Filter::FilterType>(0);
						value = "";

						// Optional member variable init
					}

					virtual void Clear()
					{
						// member variable clear
						::memset(&key, 0, sizeof(key));
						::memset(&type, 0, sizeof(type));
						value = "";

						// Optional member variable clear
					}

					bool Copy(const FilterInfo& rhs)
					{
						if (this == &rhs)
						{
							return false;
						}
						Init();
						// member variable copy
						key = rhs.key;
						type = rhs.type;
						value = rhs.value;

						// Optional member variable copy
						return true;
					}

					// GetLength Function
					virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
					{
						// Stream Format
						if ( IDarMsg::STREAM == eRule)
						{
							return _GetLengthSTREAM( );
						}
						return -1;
					}

					// GetLength STREAM Format
					int _GetLengthSTREAM( )
					{
						int nLength = 0;
						// GetLength Member key
						nLength += sizeof(key);
						// GetLength Member type
						nLength += sizeof(type);
						// GetLength Member value
						nLength += sizeof(int);
						nLength += value.length();

						nLength += sizeof(unsigned char);

						return nLength;
					}

					// Save Function
					virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
					{
						// Stream Format
						if ( IDarMsg::STREAM == eRule)
						{
							return _SaveSTREAM( stream);
						}
						return false;
					}

					virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
					{
						// STREAM Format
						int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
						MessageBlock clBlock;
						if ( false == clBlock.SetData(stream))
						{
							return false;
						}
						bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
						Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
						return bRet;
					}
					bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
					{
						// STREAM Initial Format
						if ( IDarMsg::STREAM == eRule)
						{
							return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
						}
						return false;
					}

					// save STREAM Format
					bool _SaveSTREAM( Base::System::Stream::Handle stream)
					{
						// member variable save
						if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::Filter::KeyType>( stream, key))		{    return false;     }
						if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::Filter::FilterType>( stream, type))		{    return false;     }
						if (false ==  DAR2Rule_STREAM::Save( stream, value))		{    return false;     }

						// Optional member count save
						unsigned char cOptionalMemberCount = (unsigned char)(0);
						if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

						// Optional member variable save
						return true;
					}

					bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
					{
						if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::Filter::KeyType>( clMessageBlock, key, bRequireChangeByteOrder))		{    return false;     }
						if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::Filter::FilterType>( clMessageBlock, type, bRequireChangeByteOrder))		{    return false;     }
						if (false == DAR2Rule_STREAM::Load( clMessageBlock, value, bRequireChangeByteOrder))		{    return false;     }
						unsigned char cOptionalMemberCnt = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
						for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
						{
							unsigned short nOptionalTypeHash = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
							unsigned short nOptionalType = nOptionalTypeHash & 7;

							if (0 == nOptionalType) // 1byte
							{
								if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
							}
							else if (1 == nOptionalType) // 2byte
							{
								if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
							}
							else if (2 == nOptionalType) // 4byte
							{
								if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
							}
							else if (3 == nOptionalType) // 8byte
							{
								if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
							}
							else if (4 == nOptionalType) // variable type
							{
								int nSkipSize = 0;
								if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
								if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
							}
						}
						return true;
					}

					#if defined(__JSON_LIB_SUPPORT__)
					// save JSON Format
					virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
					{
						Json::Value Json_root;
						if (false == SaveJSON(Json_root))	{	return false;	}
						if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
						else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
						return true;
					}

					bool SaveJSON(Json::Value& Json_root) const
					{
						// Save Member key
						Json_root["key"] = key;
						// Save Member type
						Json_root["type"] = type;
						// Save Member value
						Json_root["value"] = value;
						return true;
					}

					// Load json format
					virtual bool LoadJSON(const std::string& strJsonFormat)
					{
						Json::Value root;
						Json::Reader reader;
						if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
						return LoadJSON(root);
					}

					bool LoadJSON(const Json::Value& Json_root)
					{
						Init();
						// Load Member key
						if (! Json_root["key"].isNull())
						{
							int temp;
							if (false == DAR2Rule_JSON::Load(Json_root["key"], temp))	{	return false;	}
							key = static_cast<Camel::Dar2::SS::Filter::KeyType>(temp);
						}
						// Load Member type
						if (! Json_root["type"].isNull())
						{
							int temp;
							if (false == DAR2Rule_JSON::Load(Json_root["type"], temp))	{	return false;	}
							type = static_cast<Camel::Dar2::SS::Filter::FilterType>(temp);
						}
						// Load Member value
						if (! Json_root["value"].isNull())
						{
							if (false == DAR2Rule_JSON::Load(Json_root["value"], value))	{	return false;	}
						}
						return true;
					}

					#endif // defined(__JSON_LIB_SUPPORT__)
				};
				// const member valiable


			};
		};
		#endif     // !defined __DarMessageProtocol_SS_Filter_FilterInfo__MessageId_266869564__

		#if !defined __DarMessageProtocol_SS_MUCRoomServiceList__MessageId_2087767830__
		#define __DarMessageProtocol_SS_MUCRoomServiceList__MessageId_2087767830__
		namespace SS
		{
			class MUCRoomServiceList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2087767830U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MUCRoomServiceList";	};

				// const member valiable declare

				// member valiable declare
				std::string service;
				std::vector<Camel::Dar2::SS::CategoryInfo* > categorylist;

				// Optional member valiable declare

				MUCRoomServiceList()
				{
					Init();
				}

				virtual ~MUCRoomServiceList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					service = "";
					for ( std::vector<Camel::Dar2::SS::CategoryInfo* >::iterator iter = categorylist.begin(); iter != categorylist.end(); ++iter)
					{
						Camel::Dar2::SS::CategoryInfo* pItem = (*iter);
						delete pItem;
					}
					categorylist.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					service = "";
					for ( std::vector<Camel::Dar2::SS::CategoryInfo* >::iterator iter = categorylist.begin(); iter != categorylist.end(); ++iter)
					{
						Camel::Dar2::SS::CategoryInfo* pItem = (*iter);
						delete pItem;
					}
					categorylist.clear();

					// Optional member variable clear
				}

				bool Copy(const MUCRoomServiceList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					service = rhs.service;
					for ( std::vector<Camel::Dar2::SS::CategoryInfo* >::const_iterator iter = rhs.categorylist.begin(); iter != rhs.categorylist.end(); ++iter)
					{
						Camel::Dar2::SS::CategoryInfo* pItem = new Camel::Dar2::SS::CategoryInfo;
						pItem->Copy(*(*iter));
						categorylist.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member service
					nLength += sizeof(int);
					nLength += service.length();
					// GetLength Member categorylist
					nLength += sizeof(int);
					for ( std::vector<Camel::Dar2::SS::CategoryInfo* >::iterator iter = categorylist.begin(); iter != categorylist.end(); ++iter)
					{
						Camel::Dar2::SS::CategoryInfo* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, service))		{    return false;     }
					unsigned int n_categorylistSize = categorylist.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_categorylistSize ))      {      return false;     }
					for ( std::vector<Camel::Dar2::SS::CategoryInfo* >::iterator iter = categorylist.begin(); iter != categorylist.end(); ++iter)
					{
						Camel::Dar2::SS::CategoryInfo* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, service, bRequireChangeByteOrder))		{    return false;     }
					unsigned int n_categorylistSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_categorylistSize, bRequireChangeByteOrder))      {      return false;     }
					categorylist.reserve( n_categorylistSize);
					for ( unsigned int nIndex = 0; nIndex < n_categorylistSize ; ++nIndex)
					{
						Camel::Dar2::SS::CategoryInfo* pTemp = new Camel::Dar2::SS::CategoryInfo;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						categorylist.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member service
					Json_root["service"] = service;
					// Save Member categorylist
					{
						Json::Value JsonValue_categorylist;
						for ( std::vector<Camel::Dar2::SS::CategoryInfo* >::const_iterator iter = categorylist.begin(); iter != categorylist.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::CategoryInfo* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_categorylist.append( JsonValue_temp);
							}
						Json_root["categorylist"] = JsonValue_categorylist;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member service
					if (! Json_root["service"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["service"], service))	{	return false;	}
					}
					// Load Member categorylist
					if ( (! Json_root["categorylist"].isNull()) && (Json_root["categorylist"].isArray()))
					{
						unsigned int n_categorylistSize = Json_root["categorylist"].size();
						for ( unsigned int nIndex = 0; nIndex < n_categorylistSize ; ++nIndex)
						{
							Camel::Dar2::SS::CategoryInfo* temp = new Camel::Dar2::SS::CategoryInfo;
							if (false ==  temp->LoadJSON( Json_root["categorylist"][nIndex]))		{    delete temp;	return false;     }
							categorylist.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MUCRoomServiceList__MessageId_2087767830__

		#if !defined __DarMessageProtocol_SS_MUCRoomList__MessageId_1562615385__
		#define __DarMessageProtocol_SS_MUCRoomList__MessageId_1562615385__
		namespace SS
		{
			class MUCRoomList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1562615385U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MUCRoomList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::MUCRoomInfo* > roomList;

				// Optional member valiable declare

				MUCRoomList()
				{
					Init();
				}

				virtual ~MUCRoomList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::MUCRoomInfo* >::iterator iter = roomList.begin(); iter != roomList.end(); ++iter)
					{
						Camel::Dar2::SS::MUCRoomInfo* pItem = (*iter);
						delete pItem;
					}
					roomList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::MUCRoomInfo* >::iterator iter = roomList.begin(); iter != roomList.end(); ++iter)
					{
						Camel::Dar2::SS::MUCRoomInfo* pItem = (*iter);
						delete pItem;
					}
					roomList.clear();

					// Optional member variable clear
				}

				bool Copy(const MUCRoomList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::MUCRoomInfo* >::const_iterator iter = rhs.roomList.begin(); iter != rhs.roomList.end(); ++iter)
					{
						Camel::Dar2::SS::MUCRoomInfo* pItem = new Camel::Dar2::SS::MUCRoomInfo;
						pItem->Copy(*(*iter));
						roomList.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member roomList
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::MUCRoomInfo* >::iterator iter = roomList.begin(); iter != roomList.end(); ++iter)
					{
						Camel::Dar2::SS::MUCRoomInfo* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_roomListSize = roomList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_roomListSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::MUCRoomInfo* >::iterator iter = roomList.begin(); iter != roomList.end(); ++iter)
					{
						Camel::Dar2::SS::MUCRoomInfo* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_roomListSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_roomListSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_roomListSize ; ++nIndex)
					{
						Camel::Dar2::SS::MUCRoomInfo* pTemp = new Camel::Dar2::SS::MUCRoomInfo;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						roomList.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member roomList
					{
						Json::Value JsonValue_roomList;
						for ( std::list<Camel::Dar2::SS::MUCRoomInfo* >::const_iterator iter = roomList.begin(); iter != roomList.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::MUCRoomInfo* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_roomList.append( JsonValue_temp);
							}
						Json_root["roomList"] = JsonValue_roomList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member roomList
					if ( (! Json_root["roomList"].isNull()) && (Json_root["roomList"].isArray()))
					{
						unsigned int n_roomListSize = Json_root["roomList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_roomListSize ; ++nIndex)
						{
							Camel::Dar2::SS::MUCRoomInfo* temp = new Camel::Dar2::SS::MUCRoomInfo;
							if (false ==  temp->LoadJSON( Json_root["roomList"][nIndex]))		{    delete temp; return false;     }
							roomList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MUCRoomList__MessageId_1562615385__

		#if !defined __DarMessageProtocol_SS_MucRoleCheck__MessageId_777541396__
		#define __DarMessageProtocol_SS_MucRoleCheck__MessageId_777541396__
		namespace SS
		{
			class MucRoleCheck : public IDarMsg
			{
			public:
				static const unsigned int  msgid=777541396U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MucRoleCheck";	};

				// const member valiable declare

				// member valiable declare
				bool moderator;
				bool participant;
				bool visitor;

				// Optional member valiable declare

				MucRoleCheck()
				{
					Init();
				}

				virtual ~MucRoleCheck()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					moderator = static_cast<bool>(0);
					participant = static_cast<bool>(0);
					visitor = static_cast<bool>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					::memset(&moderator, 0, sizeof(moderator));
					::memset(&participant, 0, sizeof(participant));
					::memset(&visitor, 0, sizeof(visitor));

					// Optional member variable clear
				}

				bool Copy(const MucRoleCheck& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					moderator = rhs.moderator;
					participant = rhs.participant;
					visitor = rhs.visitor;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member moderator
					nLength += sizeof(moderator);
					// GetLength Member participant
					nLength += sizeof(participant);
					// GetLength Member visitor
					nLength += sizeof(visitor);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save<bool>( stream, moderator))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<bool>( stream, participant))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<bool>( stream, visitor))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false ==  DAR2Rule_STREAM::Load<bool>( clMessageBlock, moderator, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<bool>( clMessageBlock, participant, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<bool>( clMessageBlock, visitor, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member moderator
					Json_root["moderator"] = moderator;
					// Save Member participant
					Json_root["participant"] = participant;
					// Save Member visitor
					Json_root["visitor"] = visitor;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member moderator
					if (! Json_root["moderator"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["moderator"], moderator))	{	return false;	}
					}
					// Load Member participant
					if (! Json_root["participant"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["participant"], participant))	{	return false;	}
					}
					// Load Member visitor
					if (! Json_root["visitor"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["visitor"], visitor))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MucRoleCheck__MessageId_777541396__

		#if !defined __DarMessageProtocol_SS_MucRoomConfig__MessageId_254619786__
		#define __DarMessageProtocol_SS_MucRoomConfig__MessageId_254619786__
		namespace SS
		{
			class MucRoomConfig : public IDarMsg
			{
			public:
				static const unsigned int  msgid=254619786U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MucRoomConfig";	};

				// const member valiable declare

				// member valiable declare
				std::string roomname;
				std::string roomdesc;
				bool enableLogging;
				bool changeSubject;
				bool allowInvites;
				int maxUsers;
				Camel::Dar2::SS::MucRoleCheck presenceBroadcast;
				Camel::Dar2::SS::MucRoleCheck getMemberList;
				Camel::Dar2::SS::MucRoomType roomType;
				std::string category;
				std::string type;
				std::string password;
				Camel::Dar2::SS::WHOIS whois;
				std::list<std::string> roomAdmins;
				std::list<std::string> roomOwners;
				std::string pubsub;

				// Optional member valiable declare

				MucRoomConfig()
				{
					Init();
				}

				virtual ~MucRoomConfig()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					roomname = "";
					roomdesc = "";
					enableLogging = static_cast<bool>(0);
					changeSubject = static_cast<bool>(0);
					allowInvites = static_cast<bool>(0);
					maxUsers = static_cast<int>(0);
					presenceBroadcast.Clear();
					getMemberList.Clear();
					roomType.Clear();
					category = "";
					type = "";
					password = "";
					whois = static_cast<Camel::Dar2::SS::WHOIS>(0);
					roomAdmins.clear();
					roomOwners.clear();
					pubsub = "";

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					roomname = "";
					roomdesc = "";
					::memset(&enableLogging, 0, sizeof(enableLogging));
					::memset(&changeSubject, 0, sizeof(changeSubject));
					::memset(&allowInvites, 0, sizeof(allowInvites));
					::memset(&maxUsers, 0, sizeof(maxUsers));
					presenceBroadcast.Clear();
					getMemberList.Clear();
					roomType.Clear();
					category = "";
					type = "";
					password = "";
					::memset(&whois, 0, sizeof(whois));
					roomAdmins.clear();
					roomOwners.clear();
					pubsub = "";

					// Optional member variable clear
				}

				bool Copy(const MucRoomConfig& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					roomname = rhs.roomname;
					roomdesc = rhs.roomdesc;
					enableLogging = rhs.enableLogging;
					changeSubject = rhs.changeSubject;
					allowInvites = rhs.allowInvites;
					maxUsers = rhs.maxUsers;
					presenceBroadcast.Copy(rhs.presenceBroadcast);
					getMemberList.Copy(rhs.getMemberList);
					roomType.Copy(rhs.roomType);
					category = rhs.category;
					type = rhs.type;
					password = rhs.password;
					whois = rhs.whois;
					roomAdmins = rhs.roomAdmins;
					roomOwners = rhs.roomOwners;
					pubsub = rhs.pubsub;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member roomname
					nLength += sizeof(int);
					nLength += roomname.length();
					// GetLength Member roomdesc
					nLength += sizeof(int);
					nLength += roomdesc.length();
					// GetLength Member enableLogging
					nLength += sizeof(enableLogging);
					// GetLength Member changeSubject
					nLength += sizeof(changeSubject);
					// GetLength Member allowInvites
					nLength += sizeof(allowInvites);
					// GetLength Member maxUsers
					nLength += sizeof(maxUsers);
					// GetLength Member presenceBroadcast
					nLength += presenceBroadcast.GetLength();
					// GetLength Member getMemberList
					nLength += getMemberList.GetLength();
					// GetLength Member roomType
					nLength += roomType.GetLength();
					// GetLength Member category
					nLength += sizeof(int);
					nLength += category.length();
					// GetLength Member type
					nLength += sizeof(int);
					nLength += type.length();
					// GetLength Member password
					nLength += sizeof(int);
					nLength += password.length();
					// GetLength Member whois
					nLength += sizeof(whois);
					// GetLength Member roomAdmins
					nLength += sizeof(int);
					for ( std::list<std::string>::iterator iter = roomAdmins.begin(); iter != roomAdmins.end(); ++iter)
					{
						nLength += sizeof(int);
						nLength += (*iter).length();
					}
					// GetLength Member roomOwners
					nLength += sizeof(int);
					for ( std::list<std::string>::iterator iter = roomOwners.begin(); iter != roomOwners.end(); ++iter)
					{
						nLength += sizeof(int);
						nLength += (*iter).length();
					}
					// GetLength Member pubsub
					nLength += sizeof(int);
					nLength += pubsub.length();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, roomname))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, roomdesc))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<bool>( stream, enableLogging))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<bool>( stream, changeSubject))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<bool>( stream, allowInvites))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, maxUsers))		{    return false;     }
					if (false == presenceBroadcast.Save( stream))        {    return false;     }
					if (false == getMemberList.Save( stream))        {    return false;     }
					if (false == roomType.Save( stream))        {    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, category))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, type))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, password))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::WHOIS>( stream, whois))		{    return false;     }
					unsigned int n_roomAdmins_Size = roomAdmins.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_roomAdmins_Size ))      {      return false;     }
					for ( std::list<std::string>::iterator iter = roomAdmins.begin(); iter != roomAdmins.end(); ++iter)
					{
						std::string& rRef = (*iter);
						if (false ==  DAR2Rule_STREAM::Save( stream, rRef))		{    return false;     }
					}
					unsigned int n_roomOwners_Size = roomOwners.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_roomOwners_Size ))      {      return false;     }
					for ( std::list<std::string>::iterator iter = roomOwners.begin(); iter != roomOwners.end(); ++iter)
					{
						std::string& rRef = (*iter);
						if (false ==  DAR2Rule_STREAM::Save( stream, rRef))		{    return false;     }
					}
					if (false ==  DAR2Rule_STREAM::Save( stream, pubsub))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, roomname, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, roomdesc, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<bool>( clMessageBlock, enableLogging, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<bool>( clMessageBlock, changeSubject, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<bool>( clMessageBlock, allowInvites, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, maxUsers, bRequireChangeByteOrder))		{    return false;     }
					if ( false == presenceBroadcast.Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {      return false;     }
					if ( false == getMemberList.Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {      return false;     }
					if ( false == roomType.Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {      return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, category, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, type, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, password, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::WHOIS>( clMessageBlock, whois, bRequireChangeByteOrder))		{    return false;     }
					unsigned int n_roomAdmins_Size = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_roomAdmins_Size, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_roomAdmins_Size ; ++nIndex)
					{
						std::string temp;
						if (false == DAR2Rule_STREAM::Load( clMessageBlock, temp, bRequireChangeByteOrder))		{    return false;     }
						roomAdmins.push_back( temp);
					}
					unsigned int n_roomOwners_Size = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_roomOwners_Size, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_roomOwners_Size ; ++nIndex)
					{
						std::string temp;
						if (false == DAR2Rule_STREAM::Load( clMessageBlock, temp, bRequireChangeByteOrder))		{    return false;     }
						roomOwners.push_back( temp);
					}
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, pubsub, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member roomname
					Json_root["roomname"] = roomname;
					// Save Member roomdesc
					Json_root["roomdesc"] = roomdesc;
					// Save Member enableLogging
					Json_root["enableLogging"] = enableLogging;
					// Save Member changeSubject
					Json_root["changeSubject"] = changeSubject;
					// Save Member allowInvites
					Json_root["allowInvites"] = allowInvites;
					// Save Member maxUsers
					Json_root["maxUsers"] = maxUsers;
					// Save Member presenceBroadcast
					{
						Json::Value JsonValue_presenceBroadcast;
						if (false == presenceBroadcast.SaveJSON( JsonValue_presenceBroadcast))        {    return false;     }
						Json_root["presenceBroadcast"] = JsonValue_presenceBroadcast;
					}
					// Save Member getMemberList
					{
						Json::Value JsonValue_getMemberList;
						if (false == getMemberList.SaveJSON( JsonValue_getMemberList))        {    return false;     }
						Json_root["getMemberList"] = JsonValue_getMemberList;
					}
					// Save Member roomType
					{
						Json::Value JsonValue_roomType;
						if (false == roomType.SaveJSON( JsonValue_roomType))        {    return false;     }
						Json_root["roomType"] = JsonValue_roomType;
					}
					// Save Member category
					Json_root["category"] = category;
					// Save Member type
					Json_root["type"] = type;
					// Save Member password
					Json_root["password"] = password;
					// Save Member whois
					Json_root["whois"] = whois;
					// Save Member roomAdmins
					{
						Json::Value JsonValue_roomAdmins;
						for ( std::list<std::string>::const_iterator iter = roomAdmins.begin(); iter != roomAdmins.end(); ++iter)
						{
							const std::string& rRef = (*iter);
							JsonValue_roomAdmins.append(rRef);
						}
						Json_root["roomAdmins"] = JsonValue_roomAdmins;
					}
					// Save Member roomOwners
					{
						Json::Value JsonValue_roomOwners;
						for ( std::list<std::string>::const_iterator iter = roomOwners.begin(); iter != roomOwners.end(); ++iter)
						{
							const std::string& rRef = (*iter);
							JsonValue_roomOwners.append(rRef);
						}
						Json_root["roomOwners"] = JsonValue_roomOwners;
					}
					// Save Member pubsub
					Json_root["pubsub"] = pubsub;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member roomname
					if (! Json_root["roomname"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["roomname"], roomname))	{	return false;	}
					}
					// Load Member roomdesc
					if (! Json_root["roomdesc"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["roomdesc"], roomdesc))	{	return false;	}
					}
					// Load Member enableLogging
					if (! Json_root["enableLogging"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["enableLogging"], enableLogging))	{	return false;	}
					}
					// Load Member changeSubject
					if (! Json_root["changeSubject"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["changeSubject"], changeSubject))	{	return false;	}
					}
					// Load Member allowInvites
					if (! Json_root["allowInvites"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["allowInvites"], allowInvites))	{	return false;	}
					}
					// Load Member maxUsers
					if (! Json_root["maxUsers"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["maxUsers"], maxUsers))	{	return false;	}
					}
					// Load Member presenceBroadcast
					if (! Json_root["presenceBroadcast"].isNull())
					{
						if (false == presenceBroadcast.LoadJSON( Json_root["presenceBroadcast"]))	{	return false;	}
					}
					// Load Member getMemberList
					if (! Json_root["getMemberList"].isNull())
					{
						if (false == getMemberList.LoadJSON( Json_root["getMemberList"]))	{	return false;	}
					}
					// Load Member roomType
					if (! Json_root["roomType"].isNull())
					{
						if (false == roomType.LoadJSON( Json_root["roomType"]))	{	return false;	}
					}
					// Load Member category
					if (! Json_root["category"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["category"], category))	{	return false;	}
					}
					// Load Member type
					if (! Json_root["type"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["type"], type))	{	return false;	}
					}
					// Load Member password
					if (! Json_root["password"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["password"], password))	{	return false;	}
					}
					// Load Member whois
					if (! Json_root["whois"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["whois"], temp))	{	return false;	}
						whois = static_cast<Camel::Dar2::SS::WHOIS>(temp);
					}
					// Load Member roomAdmins
					if ( (! Json_root["roomAdmins"].isNull()) && (Json_root["roomAdmins"].isArray()))
					{
						unsigned int n_roomAdmins_Size = Json_root["roomAdmins"].size();
						for ( unsigned int nIndex = 0; nIndex < n_roomAdmins_Size ; ++nIndex)
						{
							std::string temp;
							if (false ==  DAR2Rule_JSON::Load( Json_root["roomAdmins"][nIndex], temp))		{    return false;     }
							roomAdmins.push_back( temp);
						}
					}
					// Load Member roomOwners
					if ( (! Json_root["roomOwners"].isNull()) && (Json_root["roomOwners"].isArray()))
					{
						unsigned int n_roomOwners_Size = Json_root["roomOwners"].size();
						for ( unsigned int nIndex = 0; nIndex < n_roomOwners_Size ; ++nIndex)
						{
							std::string temp;
							if (false ==  DAR2Rule_JSON::Load( Json_root["roomOwners"][nIndex], temp))		{    return false;     }
							roomOwners.push_back( temp);
						}
					}
					// Load Member pubsub
					if (! Json_root["pubsub"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["pubsub"], pubsub))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MucRoomConfig__MessageId_254619786__

		#if !defined __DarMessageProtocol_SS_MucRoomFilter__MessageId_906491909__
		#define __DarMessageProtocol_SS_MucRoomFilter__MessageId_906491909__
		namespace SS
		{
			class MucRoomFilter : public IDarMsg
			{
			public:
				static const unsigned int  msgid=906491909U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MucRoomFilter";	};

				// const member valiable declare

				// member valiable declare
				std::vector<Camel::Dar2::SS::Filter::FilterInfo* > filterlist;

				// Optional member valiable declare

				MucRoomFilter()
				{
					Init();
				}

				virtual ~MucRoomFilter()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::vector<Camel::Dar2::SS::Filter::FilterInfo* >::iterator iter = filterlist.begin(); iter != filterlist.end(); ++iter)
					{
						Camel::Dar2::SS::Filter::FilterInfo* pItem = (*iter);
						delete pItem;
					}
					filterlist.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::vector<Camel::Dar2::SS::Filter::FilterInfo* >::iterator iter = filterlist.begin(); iter != filterlist.end(); ++iter)
					{
						Camel::Dar2::SS::Filter::FilterInfo* pItem = (*iter);
						delete pItem;
					}
					filterlist.clear();

					// Optional member variable clear
				}

				bool Copy(const MucRoomFilter& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::vector<Camel::Dar2::SS::Filter::FilterInfo* >::const_iterator iter = rhs.filterlist.begin(); iter != rhs.filterlist.end(); ++iter)
					{
						Camel::Dar2::SS::Filter::FilterInfo* pItem = new Camel::Dar2::SS::Filter::FilterInfo;
						pItem->Copy(*(*iter));
						filterlist.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member filterlist
					nLength += sizeof(int);
					for ( std::vector<Camel::Dar2::SS::Filter::FilterInfo* >::iterator iter = filterlist.begin(); iter != filterlist.end(); ++iter)
					{
						Camel::Dar2::SS::Filter::FilterInfo* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_filterlistSize = filterlist.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_filterlistSize ))      {      return false;     }
					for ( std::vector<Camel::Dar2::SS::Filter::FilterInfo* >::iterator iter = filterlist.begin(); iter != filterlist.end(); ++iter)
					{
						Camel::Dar2::SS::Filter::FilterInfo* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_filterlistSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_filterlistSize, bRequireChangeByteOrder))      {      return false;     }
					filterlist.reserve( n_filterlistSize);
					for ( unsigned int nIndex = 0; nIndex < n_filterlistSize ; ++nIndex)
					{
						Camel::Dar2::SS::Filter::FilterInfo* pTemp = new Camel::Dar2::SS::Filter::FilterInfo;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						filterlist.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member filterlist
					{
						Json::Value JsonValue_filterlist;
						for ( std::vector<Camel::Dar2::SS::Filter::FilterInfo* >::const_iterator iter = filterlist.begin(); iter != filterlist.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::Filter::FilterInfo* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_filterlist.append( JsonValue_temp);
							}
						Json_root["filterlist"] = JsonValue_filterlist;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member filterlist
					if ( (! Json_root["filterlist"].isNull()) && (Json_root["filterlist"].isArray()))
					{
						unsigned int n_filterlistSize = Json_root["filterlist"].size();
						for ( unsigned int nIndex = 0; nIndex < n_filterlistSize ; ++nIndex)
						{
							Camel::Dar2::SS::Filter::FilterInfo* temp = new Camel::Dar2::SS::Filter::FilterInfo;
							if (false ==  temp->LoadJSON( Json_root["filterlist"][nIndex]))		{    delete temp;	return false;     }
							filterlist.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MucRoomFilter__MessageId_906491909__

		#if !defined __DarMessageProtocol_SS_UserIDList__MessageId_1342651887__
		#define __DarMessageProtocol_SS_UserIDList__MessageId_1342651887__
		namespace SS
		{
			class UserIDList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1342651887U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::UserIDList";	};

				// const member valiable declare

				// member valiable declare
				std::list<std::string> idList;

				// Optional member valiable declare

				UserIDList()
				{
					Init();
				}

				virtual ~UserIDList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					idList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					idList.clear();

					// Optional member variable clear
				}

				bool Copy(const UserIDList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					idList = rhs.idList;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member idList
					nLength += sizeof(int);
					for ( std::list<std::string>::iterator iter = idList.begin(); iter != idList.end(); ++iter)
					{
						nLength += sizeof(int);
						nLength += (*iter).length();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_idList_Size = idList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_idList_Size ))      {      return false;     }
					for ( std::list<std::string>::iterator iter = idList.begin(); iter != idList.end(); ++iter)
					{
						std::string& rRef = (*iter);
						if (false ==  DAR2Rule_STREAM::Save( stream, rRef))		{    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_idList_Size = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_idList_Size, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_idList_Size ; ++nIndex)
					{
						std::string temp;
						if (false == DAR2Rule_STREAM::Load( clMessageBlock, temp, bRequireChangeByteOrder))		{    return false;     }
						idList.push_back( temp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member idList
					{
						Json::Value JsonValue_idList;
						for ( std::list<std::string>::const_iterator iter = idList.begin(); iter != idList.end(); ++iter)
						{
							const std::string& rRef = (*iter);
							JsonValue_idList.append(rRef);
						}
						Json_root["idList"] = JsonValue_idList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member idList
					if ( (! Json_root["idList"].isNull()) && (Json_root["idList"].isArray()))
					{
						unsigned int n_idList_Size = Json_root["idList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_idList_Size ; ++nIndex)
						{
							std::string temp;
							if (false ==  DAR2Rule_JSON::Load( Json_root["idList"][nIndex], temp))		{    return false;     }
							idList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_UserIDList__MessageId_1342651887__

		#if !defined __DarMessageProtocol_SS_UserProfile__MessageId_3121797981__
		#define __DarMessageProtocol_SS_UserProfile__MessageId_3121797981__
		namespace SS
		{
			class UserProfile : public IDarMsg
			{
			public:
				static const unsigned int  msgid=3121797981U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::UserProfile";	};

				// const member valiable declare

				// member valiable declare
				std::string id;
				std::string nickname;
				std::string cn;
				int avatarInfo[26];
				int result;

				// Optional member valiable declare

				UserProfile()
				{
					Init();
				}

				virtual ~UserProfile()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					id = "";
					nickname = "";
					cn = "";
					for ( int nIndex = 0; nIndex < 26 ; ++nIndex)
					{
						avatarInfo[nIndex] = static_cast<int>(0);
					}
					result = static_cast<int>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					id = "";
					nickname = "";
					cn = "";
					::memset(avatarInfo, 0, sizeof(avatarInfo));
					::memset(&result, 0, sizeof(result));

					// Optional member variable clear
				}

				bool Copy(const UserProfile& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					id = rhs.id;
					nickname = rhs.nickname;
					cn = rhs.cn;
					for ( int i=0; i<26; ++i)
					{
						avatarInfo[i] = rhs.avatarInfo[i];
					}
					result = rhs.result;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member id
					nLength += sizeof(int);
					nLength += id.length();
					// GetLength Member nickname
					nLength += sizeof(int);
					nLength += nickname.length();
					// GetLength Member cn
					nLength += sizeof(int);
					nLength += cn.length();
					// GetLength Member avatarInfo
					nLength += sizeof(avatarInfo);
					// GetLength Member result
					nLength += sizeof(result);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, id))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, nickname))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, cn))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, avatarInfo, 26))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, result))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, id, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, nickname, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, cn, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load<int>( clMessageBlock, avatarInfo, 26, bRequireChangeByteOrder))
					{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, result, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member id
					Json_root["id"] = id;
					// Save Member nickname
					Json_root["nickname"] = nickname;
					// Save Member cn
					Json_root["cn"] = cn;
					// Save Member avatarInfo
					{
						Json::Value JsonValue_avatarInfo;
						for ( int i=0; i<26; ++i)
						{
							JsonValue_avatarInfo.append( avatarInfo[i]);
						}
						Json_root["avatarInfo"] = JsonValue_avatarInfo;
					}
					// Save Member result
					Json_root["result"] = result;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member id
					if (! Json_root["id"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["id"], id))	{	return false;	}
					}
					// Load Member nickname
					if (! Json_root["nickname"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nickname"], nickname))	{	return false;	}
					}
					// Load Member cn
					if (! Json_root["cn"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["cn"], cn))	{	return false;	}
					}
					// Load Member avatarInfo
					if ( (! Json_root["avatarInfo"].isNull()) && (Json_root["avatarInfo"].isArray()))
					{
						unsigned int n_avatarInfo_Size = Json_root["avatarInfo"].size();
						if ( n_avatarInfo_Size > 26)	{	return false;	}
						for ( unsigned int nIndex = 0; nIndex < n_avatarInfo_Size ; ++nIndex)
						{
							if (false == DAR2Rule_JSON::Load(Json_root["avatarInfo"][nIndex], avatarInfo[nIndex]))	{	return false;	}
						}
					}
					// Load Member result
					if (! Json_root["result"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["result"], result))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_UserProfile__MessageId_3121797981__

		#if !defined __DarMessageProtocol_SS_UserProfileList__MessageId_2201497346__
		#define __DarMessageProtocol_SS_UserProfileList__MessageId_2201497346__
		namespace SS
		{
			class UserProfileList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2201497346U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::UserProfileList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::UserProfile* > profileList;

				// Optional member valiable declare

				UserProfileList()
				{
					Init();
				}

				virtual ~UserProfileList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::UserProfile* >::iterator iter = profileList.begin(); iter != profileList.end(); ++iter)
					{
						Camel::Dar2::SS::UserProfile* pItem = (*iter);
						delete pItem;
					}
					profileList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::UserProfile* >::iterator iter = profileList.begin(); iter != profileList.end(); ++iter)
					{
						Camel::Dar2::SS::UserProfile* pItem = (*iter);
						delete pItem;
					}
					profileList.clear();

					// Optional member variable clear
				}

				bool Copy(const UserProfileList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::UserProfile* >::const_iterator iter = rhs.profileList.begin(); iter != rhs.profileList.end(); ++iter)
					{
						Camel::Dar2::SS::UserProfile* pItem = new Camel::Dar2::SS::UserProfile;
						pItem->Copy(*(*iter));
						profileList.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member profileList
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::UserProfile* >::iterator iter = profileList.begin(); iter != profileList.end(); ++iter)
					{
						Camel::Dar2::SS::UserProfile* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_profileListSize = profileList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_profileListSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::UserProfile* >::iterator iter = profileList.begin(); iter != profileList.end(); ++iter)
					{
						Camel::Dar2::SS::UserProfile* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_profileListSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_profileListSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_profileListSize ; ++nIndex)
					{
						Camel::Dar2::SS::UserProfile* pTemp = new Camel::Dar2::SS::UserProfile;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						profileList.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member profileList
					{
						Json::Value JsonValue_profileList;
						for ( std::list<Camel::Dar2::SS::UserProfile* >::const_iterator iter = profileList.begin(); iter != profileList.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::UserProfile* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_profileList.append( JsonValue_temp);
							}
						Json_root["profileList"] = JsonValue_profileList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member profileList
					if ( (! Json_root["profileList"].isNull()) && (Json_root["profileList"].isArray()))
					{
						unsigned int n_profileListSize = Json_root["profileList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_profileListSize ; ++nIndex)
						{
							Camel::Dar2::SS::UserProfile* temp = new Camel::Dar2::SS::UserProfile;
							if (false ==  temp->LoadJSON( Json_root["profileList"][nIndex]))		{    delete temp; return false;     }
							profileList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_UserProfileList__MessageId_2201497346__

		#if !defined __DarMessageProtocol_SS_BuddyPresenceInfo__MessageId_2927833985__
		#define __DarMessageProtocol_SS_BuddyPresenceInfo__MessageId_2927833985__
		namespace SS
		{
			class BuddyPresenceInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2927833985U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::BuddyPresenceInfo";	};

				// const member valiable declare

				// member valiable declare
				Camel::Dar2::SS::Show show;
				std::string status;
				int priority;
				std::string gameinfo;

				// Optional member valiable declare

				BuddyPresenceInfo()
				{
					Init();
				}

				virtual ~BuddyPresenceInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					show = static_cast<Camel::Dar2::SS::Show>(0);
					status = "";
					priority = static_cast<int>(0);
					gameinfo = "";

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					::memset(&show, 0, sizeof(show));
					status = "";
					::memset(&priority, 0, sizeof(priority));
					gameinfo = "";

					// Optional member variable clear
				}

				bool Copy(const BuddyPresenceInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					show = rhs.show;
					status = rhs.status;
					priority = rhs.priority;
					gameinfo = rhs.gameinfo;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member show
					nLength += sizeof(show);
					// GetLength Member status
					nLength += sizeof(int);
					nLength += status.length();
					// GetLength Member priority
					nLength += sizeof(priority);
					// GetLength Member gameinfo
					nLength += sizeof(int);
					nLength += gameinfo.length();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::Show>( stream, show))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, status))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, priority))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, gameinfo))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::Show>( clMessageBlock, show, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, status, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, priority, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, gameinfo, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member show
					Json_root["show"] = show;
					// Save Member status
					Json_root["status"] = status;
					// Save Member priority
					Json_root["priority"] = priority;
					// Save Member gameinfo
					Json_root["gameinfo"] = gameinfo;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member show
					if (! Json_root["show"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["show"], temp))	{	return false;	}
						show = static_cast<Camel::Dar2::SS::Show>(temp);
					}
					// Load Member status
					if (! Json_root["status"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["status"], status))	{	return false;	}
					}
					// Load Member priority
					if (! Json_root["priority"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["priority"], priority))	{	return false;	}
					}
					// Load Member gameinfo
					if (! Json_root["gameinfo"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["gameinfo"], gameinfo))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_BuddyPresenceInfo__MessageId_2927833985__

		#if !defined __DarMessageProtocol_SS_BuddyProfileInfo__MessageId_254229526__
		#define __DarMessageProtocol_SS_BuddyProfileInfo__MessageId_254229526__
		namespace SS
		{
			class BuddyProfileInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=254229526U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::BuddyProfileInfo";	};

				// const member valiable declare

				// member valiable declare
				std::string nickname;
				std::string userCN;
				int avatarInfo[26];

				// Optional member valiable declare

				BuddyProfileInfo()
				{
					Init();
				}

				virtual ~BuddyProfileInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					nickname = "";
					userCN = "";
					for ( int nIndex = 0; nIndex < 26 ; ++nIndex)
					{
						avatarInfo[nIndex] = static_cast<int>(0);
					}

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					nickname = "";
					userCN = "";
					::memset(avatarInfo, 0, sizeof(avatarInfo));

					// Optional member variable clear
				}

				bool Copy(const BuddyProfileInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					nickname = rhs.nickname;
					userCN = rhs.userCN;
					for ( int i=0; i<26; ++i)
					{
						avatarInfo[i] = rhs.avatarInfo[i];
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member nickname
					nLength += sizeof(int);
					nLength += nickname.length();
					// GetLength Member userCN
					nLength += sizeof(int);
					nLength += userCN.length();
					// GetLength Member avatarInfo
					nLength += sizeof(avatarInfo);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, nickname))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, userCN))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, avatarInfo, 26))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, nickname, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, userCN, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load<int>( clMessageBlock, avatarInfo, 26, bRequireChangeByteOrder))
					{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member nickname
					Json_root["nickname"] = nickname;
					// Save Member userCN
					Json_root["userCN"] = userCN;
					// Save Member avatarInfo
					{
						Json::Value JsonValue_avatarInfo;
						for ( int i=0; i<26; ++i)
						{
							JsonValue_avatarInfo.append( avatarInfo[i]);
						}
						Json_root["avatarInfo"] = JsonValue_avatarInfo;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member nickname
					if (! Json_root["nickname"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nickname"], nickname))	{	return false;	}
					}
					// Load Member userCN
					if (! Json_root["userCN"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["userCN"], userCN))	{	return false;	}
					}
					// Load Member avatarInfo
					if ( (! Json_root["avatarInfo"].isNull()) && (Json_root["avatarInfo"].isArray()))
					{
						unsigned int n_avatarInfo_Size = Json_root["avatarInfo"].size();
						if ( n_avatarInfo_Size > 26)	{	return false;	}
						for ( unsigned int nIndex = 0; nIndex < n_avatarInfo_Size ; ++nIndex)
						{
							if (false == DAR2Rule_JSON::Load(Json_root["avatarInfo"][nIndex], avatarInfo[nIndex]))	{	return false;	}
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_BuddyProfileInfo__MessageId_254229526__

		#if !defined __DarMessageProtocol_SS_BuddyInfo__MessageId_1122850692__
		#define __DarMessageProtocol_SS_BuddyInfo__MessageId_1122850692__
		namespace SS
		{
			class BuddyInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1122850692U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::BuddyInfo";	};

				// const member valiable declare

				// member valiable declare
				std::string userID;
				std::string memo;
				Camel::Dar2::SS::SubscriptionState subscription;
				Camel::Dar2::SS::AskState ask;
				Camel::Dar2::SS::BuddyPresenceInfo presence;
				Camel::Dar2::SS::BuddyProfileInfo profile;

				// Optional member valiable declare

				BuddyInfo()
				{
					Init();
				}

				virtual ~BuddyInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					userID = "";
					memo = "";
					subscription = static_cast<Camel::Dar2::SS::SubscriptionState>(0);
					ask = static_cast<Camel::Dar2::SS::AskState>(0);
					presence.Clear();
					profile.Clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					userID = "";
					memo = "";
					::memset(&subscription, 0, sizeof(subscription));
					::memset(&ask, 0, sizeof(ask));
					presence.Clear();
					profile.Clear();

					// Optional member variable clear
				}

				bool Copy(const BuddyInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					userID = rhs.userID;
					memo = rhs.memo;
					subscription = rhs.subscription;
					ask = rhs.ask;
					presence.Copy(rhs.presence);
					profile.Copy(rhs.profile);

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member userID
					nLength += sizeof(int);
					nLength += userID.length();
					// GetLength Member memo
					nLength += sizeof(int);
					nLength += memo.length();
					// GetLength Member subscription
					nLength += sizeof(subscription);
					// GetLength Member ask
					nLength += sizeof(ask);
					// GetLength Member presence
					nLength += presence.GetLength();
					// GetLength Member profile
					nLength += profile.GetLength();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, userID))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, memo))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::SubscriptionState>( stream, subscription))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::AskState>( stream, ask))		{    return false;     }
					if (false == presence.Save( stream))        {    return false;     }
					if (false == profile.Save( stream))        {    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, userID, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, memo, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::SubscriptionState>( clMessageBlock, subscription, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::AskState>( clMessageBlock, ask, bRequireChangeByteOrder))		{    return false;     }
					if ( false == presence.Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {      return false;     }
					if ( false == profile.Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {      return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member userID
					Json_root["userID"] = userID;
					// Save Member memo
					Json_root["memo"] = memo;
					// Save Member subscription
					Json_root["subscription"] = subscription;
					// Save Member ask
					Json_root["ask"] = ask;
					// Save Member presence
					{
						Json::Value JsonValue_presence;
						if (false == presence.SaveJSON( JsonValue_presence))        {    return false;     }
						Json_root["presence"] = JsonValue_presence;
					}
					// Save Member profile
					{
						Json::Value JsonValue_profile;
						if (false == profile.SaveJSON( JsonValue_profile))        {    return false;     }
						Json_root["profile"] = JsonValue_profile;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member userID
					if (! Json_root["userID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["userID"], userID))	{	return false;	}
					}
					// Load Member memo
					if (! Json_root["memo"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["memo"], memo))	{	return false;	}
					}
					// Load Member subscription
					if (! Json_root["subscription"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["subscription"], temp))	{	return false;	}
						subscription = static_cast<Camel::Dar2::SS::SubscriptionState>(temp);
					}
					// Load Member ask
					if (! Json_root["ask"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["ask"], temp))	{	return false;	}
						ask = static_cast<Camel::Dar2::SS::AskState>(temp);
					}
					// Load Member presence
					if (! Json_root["presence"].isNull())
					{
						if (false == presence.LoadJSON( Json_root["presence"]))	{	return false;	}
					}
					// Load Member profile
					if (! Json_root["profile"].isNull())
					{
						if (false == profile.LoadJSON( Json_root["profile"]))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_BuddyInfo__MessageId_1122850692__

		#if !defined __DarMessageProtocol_SS_BuddyInfoList__MessageId_257855812__
		#define __DarMessageProtocol_SS_BuddyInfoList__MessageId_257855812__
		namespace SS
		{
			class BuddyInfoList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=257855812U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::BuddyInfoList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::BuddyInfo* > buddies;

				// Optional member valiable declare

				BuddyInfoList()
				{
					Init();
				}

				virtual ~BuddyInfoList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::BuddyInfo* >::iterator iter = buddies.begin(); iter != buddies.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyInfo* pItem = (*iter);
						delete pItem;
					}
					buddies.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::BuddyInfo* >::iterator iter = buddies.begin(); iter != buddies.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyInfo* pItem = (*iter);
						delete pItem;
					}
					buddies.clear();

					// Optional member variable clear
				}

				bool Copy(const BuddyInfoList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::BuddyInfo* >::const_iterator iter = rhs.buddies.begin(); iter != rhs.buddies.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyInfo* pItem = new Camel::Dar2::SS::BuddyInfo;
						pItem->Copy(*(*iter));
						buddies.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member buddies
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::BuddyInfo* >::iterator iter = buddies.begin(); iter != buddies.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyInfo* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_buddiesSize = buddies.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_buddiesSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::BuddyInfo* >::iterator iter = buddies.begin(); iter != buddies.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyInfo* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_buddiesSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_buddiesSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_buddiesSize ; ++nIndex)
					{
						Camel::Dar2::SS::BuddyInfo* pTemp = new Camel::Dar2::SS::BuddyInfo;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						buddies.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member buddies
					{
						Json::Value JsonValue_buddies;
						for ( std::list<Camel::Dar2::SS::BuddyInfo* >::const_iterator iter = buddies.begin(); iter != buddies.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::BuddyInfo* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_buddies.append( JsonValue_temp);
							}
						Json_root["buddies"] = JsonValue_buddies;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member buddies
					if ( (! Json_root["buddies"].isNull()) && (Json_root["buddies"].isArray()))
					{
						unsigned int n_buddiesSize = Json_root["buddies"].size();
						for ( unsigned int nIndex = 0; nIndex < n_buddiesSize ; ++nIndex)
						{
							Camel::Dar2::SS::BuddyInfo* temp = new Camel::Dar2::SS::BuddyInfo;
							if (false ==  temp->LoadJSON( Json_root["buddies"][nIndex]))		{    delete temp; return false;     }
							buddies.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_BuddyInfoList__MessageId_257855812__

		#if !defined __DarMessageProtocol_SS_BuddyGroupEntry__MessageId_354501204__
		#define __DarMessageProtocol_SS_BuddyGroupEntry__MessageId_354501204__
		namespace SS
		{
			class BuddyGroupEntry : public IDarMsg
			{
			public:
				static const unsigned int  msgid=354501204U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::BuddyGroupEntry";	};

				// const member valiable declare

				// member valiable declare
				std::string groupName;
				int rank;
				std::list<Camel::Dar2::SS::BuddyInfo* > buddies;

				// Optional member valiable declare

				BuddyGroupEntry()
				{
					Init();
				}

				virtual ~BuddyGroupEntry()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					groupName = "";
					rank = static_cast<int>(0);
					for ( std::list<Camel::Dar2::SS::BuddyInfo* >::iterator iter = buddies.begin(); iter != buddies.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyInfo* pItem = (*iter);
						delete pItem;
					}
					buddies.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					groupName = "";
					::memset(&rank, 0, sizeof(rank));
					for ( std::list<Camel::Dar2::SS::BuddyInfo* >::iterator iter = buddies.begin(); iter != buddies.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyInfo* pItem = (*iter);
						delete pItem;
					}
					buddies.clear();

					// Optional member variable clear
				}

				bool Copy(const BuddyGroupEntry& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					groupName = rhs.groupName;
					rank = rhs.rank;
					for ( std::list<Camel::Dar2::SS::BuddyInfo* >::const_iterator iter = rhs.buddies.begin(); iter != rhs.buddies.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyInfo* pItem = new Camel::Dar2::SS::BuddyInfo;
						pItem->Copy(*(*iter));
						buddies.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member groupName
					nLength += sizeof(int);
					nLength += groupName.length();
					// GetLength Member rank
					nLength += sizeof(rank);
					// GetLength Member buddies
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::BuddyInfo* >::iterator iter = buddies.begin(); iter != buddies.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyInfo* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, groupName))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, rank))		{    return false;     }
					unsigned int n_buddiesSize = buddies.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_buddiesSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::BuddyInfo* >::iterator iter = buddies.begin(); iter != buddies.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyInfo* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, groupName, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, rank, bRequireChangeByteOrder))		{    return false;     }
					unsigned int n_buddiesSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_buddiesSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_buddiesSize ; ++nIndex)
					{
						Camel::Dar2::SS::BuddyInfo* pTemp = new Camel::Dar2::SS::BuddyInfo;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						buddies.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member groupName
					Json_root["groupName"] = groupName;
					// Save Member rank
					Json_root["rank"] = rank;
					// Save Member buddies
					{
						Json::Value JsonValue_buddies;
						for ( std::list<Camel::Dar2::SS::BuddyInfo* >::const_iterator iter = buddies.begin(); iter != buddies.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::BuddyInfo* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_buddies.append( JsonValue_temp);
							}
						Json_root["buddies"] = JsonValue_buddies;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member groupName
					if (! Json_root["groupName"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["groupName"], groupName))	{	return false;	}
					}
					// Load Member rank
					if (! Json_root["rank"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["rank"], rank))	{	return false;	}
					}
					// Load Member buddies
					if ( (! Json_root["buddies"].isNull()) && (Json_root["buddies"].isArray()))
					{
						unsigned int n_buddiesSize = Json_root["buddies"].size();
						for ( unsigned int nIndex = 0; nIndex < n_buddiesSize ; ++nIndex)
						{
							Camel::Dar2::SS::BuddyInfo* temp = new Camel::Dar2::SS::BuddyInfo;
							if (false ==  temp->LoadJSON( Json_root["buddies"][nIndex]))		{    delete temp; return false;     }
							buddies.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_BuddyGroupEntry__MessageId_354501204__

		#if !defined __DarMessageProtocol_SS_BuddyGroupEntryList__MessageId_1287934362__
		#define __DarMessageProtocol_SS_BuddyGroupEntryList__MessageId_1287934362__
		namespace SS
		{
			class BuddyGroupEntryList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1287934362U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::BuddyGroupEntryList";	};

				// const member valiable declare

				// member valiable declare
				int revision;
				std::list<Camel::Dar2::SS::BuddyGroupEntry* > entrys;

				// Optional member valiable declare

				BuddyGroupEntryList()
				{
					Init();
				}

				virtual ~BuddyGroupEntryList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					revision = static_cast<int>(0);
					for ( std::list<Camel::Dar2::SS::BuddyGroupEntry* >::iterator iter = entrys.begin(); iter != entrys.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyGroupEntry* pItem = (*iter);
						delete pItem;
					}
					entrys.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					::memset(&revision, 0, sizeof(revision));
					for ( std::list<Camel::Dar2::SS::BuddyGroupEntry* >::iterator iter = entrys.begin(); iter != entrys.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyGroupEntry* pItem = (*iter);
						delete pItem;
					}
					entrys.clear();

					// Optional member variable clear
				}

				bool Copy(const BuddyGroupEntryList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					revision = rhs.revision;
					for ( std::list<Camel::Dar2::SS::BuddyGroupEntry* >::const_iterator iter = rhs.entrys.begin(); iter != rhs.entrys.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyGroupEntry* pItem = new Camel::Dar2::SS::BuddyGroupEntry;
						pItem->Copy(*(*iter));
						entrys.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member revision
					nLength += sizeof(revision);
					// GetLength Member entrys
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::BuddyGroupEntry* >::iterator iter = entrys.begin(); iter != entrys.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyGroupEntry* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, revision))		{    return false;     }
					unsigned int n_entrysSize = entrys.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_entrysSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::BuddyGroupEntry* >::iterator iter = entrys.begin(); iter != entrys.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyGroupEntry* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, revision, bRequireChangeByteOrder))		{    return false;     }
					unsigned int n_entrysSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_entrysSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_entrysSize ; ++nIndex)
					{
						Camel::Dar2::SS::BuddyGroupEntry* pTemp = new Camel::Dar2::SS::BuddyGroupEntry;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						entrys.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member revision
					Json_root["revision"] = revision;
					// Save Member entrys
					{
						Json::Value JsonValue_entrys;
						for ( std::list<Camel::Dar2::SS::BuddyGroupEntry* >::const_iterator iter = entrys.begin(); iter != entrys.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::BuddyGroupEntry* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_entrys.append( JsonValue_temp);
							}
						Json_root["entrys"] = JsonValue_entrys;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member revision
					if (! Json_root["revision"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["revision"], revision))	{	return false;	}
					}
					// Load Member entrys
					if ( (! Json_root["entrys"].isNull()) && (Json_root["entrys"].isArray()))
					{
						unsigned int n_entrysSize = Json_root["entrys"].size();
						for ( unsigned int nIndex = 0; nIndex < n_entrysSize ; ++nIndex)
						{
							Camel::Dar2::SS::BuddyGroupEntry* temp = new Camel::Dar2::SS::BuddyGroupEntry;
							if (false ==  temp->LoadJSON( Json_root["entrys"][nIndex]))		{    delete temp; return false;     }
							entrys.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_BuddyGroupEntryList__MessageId_1287934362__

		#if !defined __DarMessageProtocol_SS_BlackBuddy__MessageId_2906225961__
		#define __DarMessageProtocol_SS_BlackBuddy__MessageId_2906225961__
		namespace SS
		{
			class BlackBuddy : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2906225961U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::BlackBuddy";	};

				// const member valiable declare

				// member valiable declare
				std::string buddyID;
				std::string buddyNickname;

				// Optional member valiable declare

				BlackBuddy()
				{
					Init();
				}

				virtual ~BlackBuddy()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					buddyID = "";
					buddyNickname = "";

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					buddyID = "";
					buddyNickname = "";

					// Optional member variable clear
				}

				bool Copy(const BlackBuddy& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					buddyID = rhs.buddyID;
					buddyNickname = rhs.buddyNickname;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member buddyID
					nLength += sizeof(int);
					nLength += buddyID.length();
					// GetLength Member buddyNickname
					nLength += sizeof(int);
					nLength += buddyNickname.length();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, buddyID))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, buddyNickname))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, buddyID, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, buddyNickname, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member buddyID
					Json_root["buddyID"] = buddyID;
					// Save Member buddyNickname
					Json_root["buddyNickname"] = buddyNickname;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member buddyID
					if (! Json_root["buddyID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["buddyID"], buddyID))	{	return false;	}
					}
					// Load Member buddyNickname
					if (! Json_root["buddyNickname"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["buddyNickname"], buddyNickname))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_BlackBuddy__MessageId_2906225961__

		#if !defined __DarMessageProtocol_SS_BlackList__MessageId_4225262896__
		#define __DarMessageProtocol_SS_BlackList__MessageId_4225262896__
		namespace SS
		{
			class BlackList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=4225262896U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::BlackList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::BlackBuddy* > blackBuddies;

				// Optional member valiable declare

				BlackList()
				{
					Init();
				}

				virtual ~BlackList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::BlackBuddy* >::iterator iter = blackBuddies.begin(); iter != blackBuddies.end(); ++iter)
					{
						Camel::Dar2::SS::BlackBuddy* pItem = (*iter);
						delete pItem;
					}
					blackBuddies.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::BlackBuddy* >::iterator iter = blackBuddies.begin(); iter != blackBuddies.end(); ++iter)
					{
						Camel::Dar2::SS::BlackBuddy* pItem = (*iter);
						delete pItem;
					}
					blackBuddies.clear();

					// Optional member variable clear
				}

				bool Copy(const BlackList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::BlackBuddy* >::const_iterator iter = rhs.blackBuddies.begin(); iter != rhs.blackBuddies.end(); ++iter)
					{
						Camel::Dar2::SS::BlackBuddy* pItem = new Camel::Dar2::SS::BlackBuddy;
						pItem->Copy(*(*iter));
						blackBuddies.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member blackBuddies
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::BlackBuddy* >::iterator iter = blackBuddies.begin(); iter != blackBuddies.end(); ++iter)
					{
						Camel::Dar2::SS::BlackBuddy* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_blackBuddiesSize = blackBuddies.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_blackBuddiesSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::BlackBuddy* >::iterator iter = blackBuddies.begin(); iter != blackBuddies.end(); ++iter)
					{
						Camel::Dar2::SS::BlackBuddy* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_blackBuddiesSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_blackBuddiesSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_blackBuddiesSize ; ++nIndex)
					{
						Camel::Dar2::SS::BlackBuddy* pTemp = new Camel::Dar2::SS::BlackBuddy;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						blackBuddies.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member blackBuddies
					{
						Json::Value JsonValue_blackBuddies;
						for ( std::list<Camel::Dar2::SS::BlackBuddy* >::const_iterator iter = blackBuddies.begin(); iter != blackBuddies.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::BlackBuddy* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_blackBuddies.append( JsonValue_temp);
							}
						Json_root["blackBuddies"] = JsonValue_blackBuddies;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member blackBuddies
					if ( (! Json_root["blackBuddies"].isNull()) && (Json_root["blackBuddies"].isArray()))
					{
						unsigned int n_blackBuddiesSize = Json_root["blackBuddies"].size();
						for ( unsigned int nIndex = 0; nIndex < n_blackBuddiesSize ; ++nIndex)
						{
							Camel::Dar2::SS::BlackBuddy* temp = new Camel::Dar2::SS::BlackBuddy;
							if (false ==  temp->LoadJSON( Json_root["blackBuddies"][nIndex]))		{    delete temp; return false;     }
							blackBuddies.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_BlackList__MessageId_4225262896__

		#if !defined __DarMessageProtocol_SS_BuddyGroup__MessageId_2241818345__
		#define __DarMessageProtocol_SS_BuddyGroup__MessageId_2241818345__
		namespace SS
		{
			class BuddyGroup : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2241818345U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::BuddyGroup";	};

				// const member valiable declare

				// member valiable declare
				std::string groupName;
				int rank;

				// Optional member valiable declare

				BuddyGroup()
				{
					Init();
				}

				virtual ~BuddyGroup()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					groupName = "";
					rank = static_cast<int>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					groupName = "";
					::memset(&rank, 0, sizeof(rank));

					// Optional member variable clear
				}

				bool Copy(const BuddyGroup& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					groupName = rhs.groupName;
					rank = rhs.rank;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member groupName
					nLength += sizeof(int);
					nLength += groupName.length();
					// GetLength Member rank
					nLength += sizeof(rank);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, groupName))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, rank))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, groupName, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, rank, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member groupName
					Json_root["groupName"] = groupName;
					// Save Member rank
					Json_root["rank"] = rank;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member groupName
					if (! Json_root["groupName"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["groupName"], groupName))	{	return false;	}
					}
					// Load Member rank
					if (! Json_root["rank"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["rank"], rank))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_BuddyGroup__MessageId_2241818345__

		#if !defined __DarMessageProtocol_SS_BuddyGroupList__MessageId_387845780__
		#define __DarMessageProtocol_SS_BuddyGroupList__MessageId_387845780__
		namespace SS
		{
			class BuddyGroupList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=387845780U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::BuddyGroupList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::BuddyGroup* > groupList;

				// Optional member valiable declare

				BuddyGroupList()
				{
					Init();
				}

				virtual ~BuddyGroupList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::BuddyGroup* >::iterator iter = groupList.begin(); iter != groupList.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyGroup* pItem = (*iter);
						delete pItem;
					}
					groupList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::BuddyGroup* >::iterator iter = groupList.begin(); iter != groupList.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyGroup* pItem = (*iter);
						delete pItem;
					}
					groupList.clear();

					// Optional member variable clear
				}

				bool Copy(const BuddyGroupList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::BuddyGroup* >::const_iterator iter = rhs.groupList.begin(); iter != rhs.groupList.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyGroup* pItem = new Camel::Dar2::SS::BuddyGroup;
						pItem->Copy(*(*iter));
						groupList.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member groupList
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::BuddyGroup* >::iterator iter = groupList.begin(); iter != groupList.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyGroup* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_groupListSize = groupList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_groupListSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::BuddyGroup* >::iterator iter = groupList.begin(); iter != groupList.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyGroup* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_groupListSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_groupListSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_groupListSize ; ++nIndex)
					{
						Camel::Dar2::SS::BuddyGroup* pTemp = new Camel::Dar2::SS::BuddyGroup;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						groupList.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member groupList
					{
						Json::Value JsonValue_groupList;
						for ( std::list<Camel::Dar2::SS::BuddyGroup* >::const_iterator iter = groupList.begin(); iter != groupList.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::BuddyGroup* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_groupList.append( JsonValue_temp);
							}
						Json_root["groupList"] = JsonValue_groupList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member groupList
					if ( (! Json_root["groupList"].isNull()) && (Json_root["groupList"].isArray()))
					{
						unsigned int n_groupListSize = Json_root["groupList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_groupListSize ; ++nIndex)
						{
							Camel::Dar2::SS::BuddyGroup* temp = new Camel::Dar2::SS::BuddyGroup;
							if (false ==  temp->LoadJSON( Json_root["groupList"][nIndex]))		{    delete temp; return false;     }
							groupList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_BuddyGroupList__MessageId_387845780__

		#if !defined __DarMessageProtocol_SS_SelectedBuddyInfo__MessageId_2464624977__
		#define __DarMessageProtocol_SS_SelectedBuddyInfo__MessageId_2464624977__
		namespace SS
		{
			class SelectedBuddyInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2464624977U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::SelectedBuddyInfo";	};

				// const member valiable declare

				// member valiable declare
				std::string buddyID;
				std::string groupName;

				// Optional member valiable declare

				SelectedBuddyInfo()
				{
					Init();
				}

				virtual ~SelectedBuddyInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					buddyID = "";
					groupName = "";

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					buddyID = "";
					groupName = "";

					// Optional member variable clear
				}

				bool Copy(const SelectedBuddyInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					buddyID = rhs.buddyID;
					groupName = rhs.groupName;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member buddyID
					nLength += sizeof(int);
					nLength += buddyID.length();
					// GetLength Member groupName
					nLength += sizeof(int);
					nLength += groupName.length();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, buddyID))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, groupName))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, buddyID, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, groupName, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member buddyID
					Json_root["buddyID"] = buddyID;
					// Save Member groupName
					Json_root["groupName"] = groupName;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member buddyID
					if (! Json_root["buddyID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["buddyID"], buddyID))	{	return false;	}
					}
					// Load Member groupName
					if (! Json_root["groupName"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["groupName"], groupName))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_SelectedBuddyInfo__MessageId_2464624977__

		#if !defined __DarMessageProtocol_SS_SelectedBuddyList__MessageId_1829052660__
		#define __DarMessageProtocol_SS_SelectedBuddyList__MessageId_1829052660__
		namespace SS
		{
			class SelectedBuddyList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1829052660U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::SelectedBuddyList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::SelectedBuddyInfo* > selectedBuddyList;

				// Optional member valiable declare

				SelectedBuddyList()
				{
					Init();
				}

				virtual ~SelectedBuddyList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::SelectedBuddyInfo* >::iterator iter = selectedBuddyList.begin(); iter != selectedBuddyList.end(); ++iter)
					{
						Camel::Dar2::SS::SelectedBuddyInfo* pItem = (*iter);
						delete pItem;
					}
					selectedBuddyList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::SelectedBuddyInfo* >::iterator iter = selectedBuddyList.begin(); iter != selectedBuddyList.end(); ++iter)
					{
						Camel::Dar2::SS::SelectedBuddyInfo* pItem = (*iter);
						delete pItem;
					}
					selectedBuddyList.clear();

					// Optional member variable clear
				}

				bool Copy(const SelectedBuddyList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::SelectedBuddyInfo* >::const_iterator iter = rhs.selectedBuddyList.begin(); iter != rhs.selectedBuddyList.end(); ++iter)
					{
						Camel::Dar2::SS::SelectedBuddyInfo* pItem = new Camel::Dar2::SS::SelectedBuddyInfo;
						pItem->Copy(*(*iter));
						selectedBuddyList.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member selectedBuddyList
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::SelectedBuddyInfo* >::iterator iter = selectedBuddyList.begin(); iter != selectedBuddyList.end(); ++iter)
					{
						Camel::Dar2::SS::SelectedBuddyInfo* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_selectedBuddyListSize = selectedBuddyList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_selectedBuddyListSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::SelectedBuddyInfo* >::iterator iter = selectedBuddyList.begin(); iter != selectedBuddyList.end(); ++iter)
					{
						Camel::Dar2::SS::SelectedBuddyInfo* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_selectedBuddyListSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_selectedBuddyListSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_selectedBuddyListSize ; ++nIndex)
					{
						Camel::Dar2::SS::SelectedBuddyInfo* pTemp = new Camel::Dar2::SS::SelectedBuddyInfo;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						selectedBuddyList.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member selectedBuddyList
					{
						Json::Value JsonValue_selectedBuddyList;
						for ( std::list<Camel::Dar2::SS::SelectedBuddyInfo* >::const_iterator iter = selectedBuddyList.begin(); iter != selectedBuddyList.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::SelectedBuddyInfo* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_selectedBuddyList.append( JsonValue_temp);
							}
						Json_root["selectedBuddyList"] = JsonValue_selectedBuddyList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member selectedBuddyList
					if ( (! Json_root["selectedBuddyList"].isNull()) && (Json_root["selectedBuddyList"].isArray()))
					{
						unsigned int n_selectedBuddyListSize = Json_root["selectedBuddyList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_selectedBuddyListSize ; ++nIndex)
						{
							Camel::Dar2::SS::SelectedBuddyInfo* temp = new Camel::Dar2::SS::SelectedBuddyInfo;
							if (false ==  temp->LoadJSON( Json_root["selectedBuddyList"][nIndex]))		{    delete temp; return false;     }
							selectedBuddyList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_SelectedBuddyList__MessageId_1829052660__

		#if !defined __DarMessageProtocol_SS_BuddyResult__MessageId_507053702__
		#define __DarMessageProtocol_SS_BuddyResult__MessageId_507053702__
		namespace SS
		{
			class BuddyResult : public IDarMsg
			{
			public:
				static const unsigned int  msgid=507053702U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::BuddyResult";	};

				// const member valiable declare

				// member valiable declare
				std::string buddyID;
				std::string groupName;
				int result;

				// Optional member valiable declare

				BuddyResult()
				{
					Init();
				}

				virtual ~BuddyResult()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					buddyID = "";
					groupName = "";
					result = static_cast<int>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					buddyID = "";
					groupName = "";
					::memset(&result, 0, sizeof(result));

					// Optional member variable clear
				}

				bool Copy(const BuddyResult& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					buddyID = rhs.buddyID;
					groupName = rhs.groupName;
					result = rhs.result;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member buddyID
					nLength += sizeof(int);
					nLength += buddyID.length();
					// GetLength Member groupName
					nLength += sizeof(int);
					nLength += groupName.length();
					// GetLength Member result
					nLength += sizeof(result);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, buddyID))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, groupName))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, result))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, buddyID, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, groupName, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, result, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member buddyID
					Json_root["buddyID"] = buddyID;
					// Save Member groupName
					Json_root["groupName"] = groupName;
					// Save Member result
					Json_root["result"] = result;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member buddyID
					if (! Json_root["buddyID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["buddyID"], buddyID))	{	return false;	}
					}
					// Load Member groupName
					if (! Json_root["groupName"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["groupName"], groupName))	{	return false;	}
					}
					// Load Member result
					if (! Json_root["result"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["result"], result))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_BuddyResult__MessageId_507053702__

		#if !defined __DarMessageProtocol_SS_BuddyResultList__MessageId_1149865278__
		#define __DarMessageProtocol_SS_BuddyResultList__MessageId_1149865278__
		namespace SS
		{
			class BuddyResultList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1149865278U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::BuddyResultList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::BuddyResult* > results;

				// Optional member valiable declare

				BuddyResultList()
				{
					Init();
				}

				virtual ~BuddyResultList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::BuddyResult* >::iterator iter = results.begin(); iter != results.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyResult* pItem = (*iter);
						delete pItem;
					}
					results.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::BuddyResult* >::iterator iter = results.begin(); iter != results.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyResult* pItem = (*iter);
						delete pItem;
					}
					results.clear();

					// Optional member variable clear
				}

				bool Copy(const BuddyResultList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::BuddyResult* >::const_iterator iter = rhs.results.begin(); iter != rhs.results.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyResult* pItem = new Camel::Dar2::SS::BuddyResult;
						pItem->Copy(*(*iter));
						results.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member results
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::BuddyResult* >::iterator iter = results.begin(); iter != results.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyResult* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_resultsSize = results.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_resultsSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::BuddyResult* >::iterator iter = results.begin(); iter != results.end(); ++iter)
					{
						Camel::Dar2::SS::BuddyResult* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_resultsSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_resultsSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_resultsSize ; ++nIndex)
					{
						Camel::Dar2::SS::BuddyResult* pTemp = new Camel::Dar2::SS::BuddyResult;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						results.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member results
					{
						Json::Value JsonValue_results;
						for ( std::list<Camel::Dar2::SS::BuddyResult* >::const_iterator iter = results.begin(); iter != results.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::BuddyResult* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_results.append( JsonValue_temp);
							}
						Json_root["results"] = JsonValue_results;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member results
					if ( (! Json_root["results"].isNull()) && (Json_root["results"].isArray()))
					{
						unsigned int n_resultsSize = Json_root["results"].size();
						for ( unsigned int nIndex = 0; nIndex < n_resultsSize ; ++nIndex)
						{
							Camel::Dar2::SS::BuddyResult* temp = new Camel::Dar2::SS::BuddyResult;
							if (false ==  temp->LoadJSON( Json_root["results"][nIndex]))		{    delete temp; return false;     }
							results.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_BuddyResultList__MessageId_1149865278__

		#if !defined __DarMessageProtocol_SS_VCard__MessageId_925629938__
		#define __DarMessageProtocol_SS_VCard__MessageId_925629938__
		namespace SS
		{
			class VCard : public IDarMsg
			{
			public:
				static const unsigned int  msgid=925629938U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::VCard";	};

				// const member valiable declare

				// member valiable declare
				std::string type;
				std::string vCardSyntext;

				// Optional member valiable declare

				VCard()
				{
					Init();
				}

				virtual ~VCard()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					type = "";
					vCardSyntext = "";

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					type = "";
					vCardSyntext = "";

					// Optional member variable clear
				}

				bool Copy(const VCard& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					type = rhs.type;
					vCardSyntext = rhs.vCardSyntext;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member type
					nLength += sizeof(int);
					nLength += type.length();
					// GetLength Member vCardSyntext
					nLength += sizeof(int);
					nLength += vCardSyntext.length();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, type))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, vCardSyntext))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, type, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, vCardSyntext, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member type
					Json_root["type"] = type;
					// Save Member vCardSyntext
					Json_root["vCardSyntext"] = vCardSyntext;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member type
					if (! Json_root["type"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["type"], type))	{	return false;	}
					}
					// Load Member vCardSyntext
					if (! Json_root["vCardSyntext"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["vCardSyntext"], vCardSyntext))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_VCard__MessageId_925629938__

		#if !defined __DarMessageProtocol_SS_VCardList__MessageId_590313750__
		#define __DarMessageProtocol_SS_VCardList__MessageId_590313750__
		namespace SS
		{
			class VCardList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=590313750U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::VCardList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::VCard* > vcardList;

				// Optional member valiable declare

				VCardList()
				{
					Init();
				}

				virtual ~VCardList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::VCard* >::iterator iter = vcardList.begin(); iter != vcardList.end(); ++iter)
					{
						Camel::Dar2::SS::VCard* pItem = (*iter);
						delete pItem;
					}
					vcardList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::VCard* >::iterator iter = vcardList.begin(); iter != vcardList.end(); ++iter)
					{
						Camel::Dar2::SS::VCard* pItem = (*iter);
						delete pItem;
					}
					vcardList.clear();

					// Optional member variable clear
				}

				bool Copy(const VCardList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::VCard* >::const_iterator iter = rhs.vcardList.begin(); iter != rhs.vcardList.end(); ++iter)
					{
						Camel::Dar2::SS::VCard* pItem = new Camel::Dar2::SS::VCard;
						pItem->Copy(*(*iter));
						vcardList.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member vcardList
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::VCard* >::iterator iter = vcardList.begin(); iter != vcardList.end(); ++iter)
					{
						Camel::Dar2::SS::VCard* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_vcardListSize = vcardList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_vcardListSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::VCard* >::iterator iter = vcardList.begin(); iter != vcardList.end(); ++iter)
					{
						Camel::Dar2::SS::VCard* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_vcardListSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_vcardListSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_vcardListSize ; ++nIndex)
					{
						Camel::Dar2::SS::VCard* pTemp = new Camel::Dar2::SS::VCard;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						vcardList.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member vcardList
					{
						Json::Value JsonValue_vcardList;
						for ( std::list<Camel::Dar2::SS::VCard* >::const_iterator iter = vcardList.begin(); iter != vcardList.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::VCard* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_vcardList.append( JsonValue_temp);
							}
						Json_root["vcardList"] = JsonValue_vcardList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member vcardList
					if ( (! Json_root["vcardList"].isNull()) && (Json_root["vcardList"].isArray()))
					{
						unsigned int n_vcardListSize = Json_root["vcardList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_vcardListSize ; ++nIndex)
						{
							Camel::Dar2::SS::VCard* temp = new Camel::Dar2::SS::VCard;
							if (false ==  temp->LoadJSON( Json_root["vcardList"][nIndex]))		{    delete temp; return false;     }
							vcardList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_VCardList__MessageId_590313750__

		#if !defined __DarMessageProtocol_SS_VCard2__MessageId_2615987886__
		#define __DarMessageProtocol_SS_VCard2__MessageId_2615987886__
		namespace SS
		{
			class VCard2 : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2615987886U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::VCard2";	};

				// const member valiable declare

				// member valiable declare
				std::string key;
				std::string description;
				Camel::Dar2::SS::ContentType contentType;
				std::string content;
				std::string creationDate;
				std::string modificationDate;

				// Optional member valiable declare

				VCard2()
				{
					Init();
				}

				virtual ~VCard2()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					key = "";
					description = "";
					contentType = static_cast<Camel::Dar2::SS::ContentType>(0);
					content = "";
					creationDate = "";
					modificationDate = "";

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					key = "";
					description = "";
					::memset(&contentType, 0, sizeof(contentType));
					content = "";
					creationDate = "";
					modificationDate = "";

					// Optional member variable clear
				}

				bool Copy(const VCard2& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					key = rhs.key;
					description = rhs.description;
					contentType = rhs.contentType;
					content = rhs.content;
					creationDate = rhs.creationDate;
					modificationDate = rhs.modificationDate;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member key
					nLength += sizeof(int);
					nLength += key.length();
					// GetLength Member description
					nLength += sizeof(int);
					nLength += description.length();
					// GetLength Member contentType
					nLength += sizeof(contentType);
					// GetLength Member content
					nLength += sizeof(int);
					nLength += content.length();
					// GetLength Member creationDate
					nLength += sizeof(int);
					nLength += creationDate.length();
					// GetLength Member modificationDate
					nLength += sizeof(int);
					nLength += modificationDate.length();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, key))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, description))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::ContentType>( stream, contentType))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, content))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, creationDate))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, modificationDate))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, key, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, description, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::ContentType>( clMessageBlock, contentType, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, content, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, creationDate, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, modificationDate, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member key
					Json_root["key"] = key;
					// Save Member description
					Json_root["description"] = description;
					// Save Member contentType
					Json_root["contentType"] = contentType;
					// Save Member content
					Json_root["content"] = content;
					// Save Member creationDate
					Json_root["creationDate"] = creationDate;
					// Save Member modificationDate
					Json_root["modificationDate"] = modificationDate;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member key
					if (! Json_root["key"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["key"], key))	{	return false;	}
					}
					// Load Member description
					if (! Json_root["description"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["description"], description))	{	return false;	}
					}
					// Load Member contentType
					if (! Json_root["contentType"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["contentType"], temp))	{	return false;	}
						contentType = static_cast<Camel::Dar2::SS::ContentType>(temp);
					}
					// Load Member content
					if (! Json_root["content"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["content"], content))	{	return false;	}
					}
					// Load Member creationDate
					if (! Json_root["creationDate"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["creationDate"], creationDate))	{	return false;	}
					}
					// Load Member modificationDate
					if (! Json_root["modificationDate"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["modificationDate"], modificationDate))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_VCard2__MessageId_2615987886__

		#if !defined __DarMessageProtocol_SS_VCard2List__MessageId_423328740__
		#define __DarMessageProtocol_SS_VCard2List__MessageId_423328740__
		namespace SS
		{
			class VCard2List : public IDarMsg
			{
			public:
				static const unsigned int  msgid=423328740U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::VCard2List";	};

				// const member valiable declare

				// member valiable declare
				std::vector<Camel::Dar2::SS::VCard2* > vCards;

				// Optional member valiable declare

				VCard2List()
				{
					Init();
				}

				virtual ~VCard2List()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::vector<Camel::Dar2::SS::VCard2* >::iterator iter = vCards.begin(); iter != vCards.end(); ++iter)
					{
						Camel::Dar2::SS::VCard2* pItem = (*iter);
						delete pItem;
					}
					vCards.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::vector<Camel::Dar2::SS::VCard2* >::iterator iter = vCards.begin(); iter != vCards.end(); ++iter)
					{
						Camel::Dar2::SS::VCard2* pItem = (*iter);
						delete pItem;
					}
					vCards.clear();

					// Optional member variable clear
				}

				bool Copy(const VCard2List& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::vector<Camel::Dar2::SS::VCard2* >::const_iterator iter = rhs.vCards.begin(); iter != rhs.vCards.end(); ++iter)
					{
						Camel::Dar2::SS::VCard2* pItem = new Camel::Dar2::SS::VCard2;
						pItem->Copy(*(*iter));
						vCards.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member vCards
					nLength += sizeof(int);
					for ( std::vector<Camel::Dar2::SS::VCard2* >::iterator iter = vCards.begin(); iter != vCards.end(); ++iter)
					{
						Camel::Dar2::SS::VCard2* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_vCardsSize = vCards.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_vCardsSize ))      {      return false;     }
					for ( std::vector<Camel::Dar2::SS::VCard2* >::iterator iter = vCards.begin(); iter != vCards.end(); ++iter)
					{
						Camel::Dar2::SS::VCard2* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_vCardsSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_vCardsSize, bRequireChangeByteOrder))      {      return false;     }
					vCards.reserve( n_vCardsSize);
					for ( unsigned int nIndex = 0; nIndex < n_vCardsSize ; ++nIndex)
					{
						Camel::Dar2::SS::VCard2* pTemp = new Camel::Dar2::SS::VCard2;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						vCards.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member vCards
					{
						Json::Value JsonValue_vCards;
						for ( std::vector<Camel::Dar2::SS::VCard2* >::const_iterator iter = vCards.begin(); iter != vCards.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::VCard2* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_vCards.append( JsonValue_temp);
							}
						Json_root["vCards"] = JsonValue_vCards;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member vCards
					if ( (! Json_root["vCards"].isNull()) && (Json_root["vCards"].isArray()))
					{
						unsigned int n_vCardsSize = Json_root["vCards"].size();
						for ( unsigned int nIndex = 0; nIndex < n_vCardsSize ; ++nIndex)
						{
							Camel::Dar2::SS::VCard2* temp = new Camel::Dar2::SS::VCard2;
							if (false ==  temp->LoadJSON( Json_root["vCards"][nIndex]))		{    delete temp;	return false;     }
							vCards.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_VCard2List__MessageId_423328740__

		#if !defined __DarMessageProtocol_SS_LoginInfo__MessageId_1912514209__
		#define __DarMessageProtocol_SS_LoginInfo__MessageId_1912514209__
		namespace SS
		{
			class LoginInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1912514209U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::LoginInfo";	};

				// const member valiable declare

				// member valiable declare
				char authData[2048];
				int siteNo;
				std::string mainDomain;
				std::string subDomain;
				std::string serviceCode;
				bool forceDisconnect;
				std::string disconnectReason;

				// Optional member valiable declare

				LoginInfo()
				{
					Init();
				}

				virtual ~LoginInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( int nIndex = 0; nIndex < 2048 ; ++nIndex)
					{
						authData[nIndex] = static_cast<char>(0);
					}
					siteNo = static_cast<int>(0);
					mainDomain = "";
					subDomain = "";
					serviceCode = "";
					forceDisconnect = static_cast<bool>(0);
					disconnectReason = "";

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					::memset(authData, 0, sizeof(authData));
					::memset(&siteNo, 0, sizeof(siteNo));
					mainDomain = "";
					subDomain = "";
					serviceCode = "";
					::memset(&forceDisconnect, 0, sizeof(forceDisconnect));
					disconnectReason = "";

					// Optional member variable clear
				}

				bool Copy(const LoginInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( int i=0; i<2048; ++i)
					{
						authData[i] = rhs.authData[i];
					}
					siteNo = rhs.siteNo;
					mainDomain = rhs.mainDomain;
					subDomain = rhs.subDomain;
					serviceCode = rhs.serviceCode;
					forceDisconnect = rhs.forceDisconnect;
					disconnectReason = rhs.disconnectReason;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member authData
					nLength += sizeof(authData);
					// GetLength Member siteNo
					nLength += sizeof(siteNo);
					// GetLength Member mainDomain
					nLength += sizeof(int);
					nLength += mainDomain.length();
					// GetLength Member subDomain
					nLength += sizeof(int);
					nLength += subDomain.length();
					// GetLength Member serviceCode
					nLength += sizeof(int);
					nLength += serviceCode.length();
					// GetLength Member forceDisconnect
					nLength += sizeof(forceDisconnect);
					// GetLength Member disconnectReason
					nLength += sizeof(int);
					nLength += disconnectReason.length();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save<char>( stream, authData, 2048))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, siteNo))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, mainDomain))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, subDomain))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, serviceCode))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<bool>( stream, forceDisconnect))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, disconnectReason))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load<char>( clMessageBlock, authData, 2048, bRequireChangeByteOrder))
					{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, siteNo, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, mainDomain, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, subDomain, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, serviceCode, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<bool>( clMessageBlock, forceDisconnect, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, disconnectReason, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member authData
					{
						Json::Value JsonValue_authData;
						for ( int i=0; i<2048; ++i)
						{
							std::string temp;
							temp.insert((size_t)0, (size_t)1, authData[i]);
							JsonValue_authData.append(temp);
						}
						Json_root["authData"] = JsonValue_authData;
					}
					// Save Member siteNo
					Json_root["siteNo"] = siteNo;
					// Save Member mainDomain
					Json_root["mainDomain"] = mainDomain;
					// Save Member subDomain
					Json_root["subDomain"] = subDomain;
					// Save Member serviceCode
					Json_root["serviceCode"] = serviceCode;
					// Save Member forceDisconnect
					Json_root["forceDisconnect"] = forceDisconnect;
					// Save Member disconnectReason
					Json_root["disconnectReason"] = disconnectReason;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member authData
					if ( (! Json_root["authData"].isNull()) && (Json_root["authData"].isArray()))
					{
						unsigned int n_authData_Size = Json_root["authData"].size();
						if ( n_authData_Size > 2048)	{	return false;	}
						for ( unsigned int nIndex = 0; nIndex < n_authData_Size ; ++nIndex)
						{
							if (false == DAR2Rule_JSON::Load(Json_root["authData"][nIndex], authData[nIndex]))	{	return false;	}
						}
					}
					// Load Member siteNo
					if (! Json_root["siteNo"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["siteNo"], siteNo))	{	return false;	}
					}
					// Load Member mainDomain
					if (! Json_root["mainDomain"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["mainDomain"], mainDomain))	{	return false;	}
					}
					// Load Member subDomain
					if (! Json_root["subDomain"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["subDomain"], subDomain))	{	return false;	}
					}
					// Load Member serviceCode
					if (! Json_root["serviceCode"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["serviceCode"], serviceCode))	{	return false;	}
					}
					// Load Member forceDisconnect
					if (! Json_root["forceDisconnect"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["forceDisconnect"], forceDisconnect))	{	return false;	}
					}
					// Load Member disconnectReason
					if (! Json_root["disconnectReason"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["disconnectReason"], disconnectReason))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_LoginInfo__MessageId_1912514209__

		#if !defined __DarMessageProtocol_SS_ClanInfo__MessageId_404676564__
		#define __DarMessageProtocol_SS_ClanInfo__MessageId_404676564__
		namespace SS
		{
			class ClanInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=404676564U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::ClanInfo";	};

				// const member valiable declare

				// member valiable declare
				std::string serviceCode;
				std::string clanCode;
				std::string clanName;
				std::string generatorID;
				int publicType;
				std::string clanDescription;
				int memberCount;
				std::string clanInfo;
				std::string clanRoomID;
				std::string creationDate;
				std::string masterRevision;

				// Optional member valiable declare

				ClanInfo()
				{
					Init();
				}

				virtual ~ClanInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					serviceCode = "";
					clanCode = "";
					clanName = "";
					generatorID = "";
					publicType = static_cast<int>(0);
					clanDescription = "";
					memberCount = static_cast<int>(0);
					clanInfo = "";
					clanRoomID = "";
					creationDate = "";
					masterRevision = "";

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					serviceCode = "";
					clanCode = "";
					clanName = "";
					generatorID = "";
					::memset(&publicType, 0, sizeof(publicType));
					clanDescription = "";
					::memset(&memberCount, 0, sizeof(memberCount));
					clanInfo = "";
					clanRoomID = "";
					creationDate = "";
					masterRevision = "";

					// Optional member variable clear
				}

				bool Copy(const ClanInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					serviceCode = rhs.serviceCode;
					clanCode = rhs.clanCode;
					clanName = rhs.clanName;
					generatorID = rhs.generatorID;
					publicType = rhs.publicType;
					clanDescription = rhs.clanDescription;
					memberCount = rhs.memberCount;
					clanInfo = rhs.clanInfo;
					clanRoomID = rhs.clanRoomID;
					creationDate = rhs.creationDate;
					masterRevision = rhs.masterRevision;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member serviceCode
					nLength += sizeof(int);
					nLength += serviceCode.length();
					// GetLength Member clanCode
					nLength += sizeof(int);
					nLength += clanCode.length();
					// GetLength Member clanName
					nLength += sizeof(int);
					nLength += clanName.length();
					// GetLength Member generatorID
					nLength += sizeof(int);
					nLength += generatorID.length();
					// GetLength Member publicType
					nLength += sizeof(publicType);
					// GetLength Member clanDescription
					nLength += sizeof(int);
					nLength += clanDescription.length();
					// GetLength Member memberCount
					nLength += sizeof(memberCount);
					// GetLength Member clanInfo
					nLength += sizeof(int);
					nLength += clanInfo.length();
					// GetLength Member clanRoomID
					nLength += sizeof(int);
					nLength += clanRoomID.length();
					// GetLength Member creationDate
					nLength += sizeof(int);
					nLength += creationDate.length();
					// GetLength Member masterRevision
					nLength += sizeof(int);
					nLength += masterRevision.length();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, serviceCode))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, clanCode))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, clanName))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, generatorID))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, publicType))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, clanDescription))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, memberCount))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, clanInfo))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, clanRoomID))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, creationDate))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, masterRevision))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, serviceCode, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, clanCode, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, clanName, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, generatorID, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, publicType, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, clanDescription, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, memberCount, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, clanInfo, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, clanRoomID, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, creationDate, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, masterRevision, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member serviceCode
					Json_root["serviceCode"] = serviceCode;
					// Save Member clanCode
					Json_root["clanCode"] = clanCode;
					// Save Member clanName
					Json_root["clanName"] = clanName;
					// Save Member generatorID
					Json_root["generatorID"] = generatorID;
					// Save Member publicType
					Json_root["publicType"] = publicType;
					// Save Member clanDescription
					Json_root["clanDescription"] = clanDescription;
					// Save Member memberCount
					Json_root["memberCount"] = memberCount;
					// Save Member clanInfo
					Json_root["clanInfo"] = clanInfo;
					// Save Member clanRoomID
					Json_root["clanRoomID"] = clanRoomID;
					// Save Member creationDate
					Json_root["creationDate"] = creationDate;
					// Save Member masterRevision
					Json_root["masterRevision"] = masterRevision;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member serviceCode
					if (! Json_root["serviceCode"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["serviceCode"], serviceCode))	{	return false;	}
					}
					// Load Member clanCode
					if (! Json_root["clanCode"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["clanCode"], clanCode))	{	return false;	}
					}
					// Load Member clanName
					if (! Json_root["clanName"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["clanName"], clanName))	{	return false;	}
					}
					// Load Member generatorID
					if (! Json_root["generatorID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["generatorID"], generatorID))	{	return false;	}
					}
					// Load Member publicType
					if (! Json_root["publicType"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["publicType"], publicType))	{	return false;	}
					}
					// Load Member clanDescription
					if (! Json_root["clanDescription"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["clanDescription"], clanDescription))	{	return false;	}
					}
					// Load Member memberCount
					if (! Json_root["memberCount"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["memberCount"], memberCount))	{	return false;	}
					}
					// Load Member clanInfo
					if (! Json_root["clanInfo"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["clanInfo"], clanInfo))	{	return false;	}
					}
					// Load Member clanRoomID
					if (! Json_root["clanRoomID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["clanRoomID"], clanRoomID))	{	return false;	}
					}
					// Load Member creationDate
					if (! Json_root["creationDate"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["creationDate"], creationDate))	{	return false;	}
					}
					// Load Member masterRevision
					if (! Json_root["masterRevision"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["masterRevision"], masterRevision))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_ClanInfo__MessageId_404676564__

		#if !defined __DarMessageProtocol_SS_ClanInfoList__MessageId_3666510246__
		#define __DarMessageProtocol_SS_ClanInfoList__MessageId_3666510246__
		namespace SS
		{
			class ClanInfoList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=3666510246U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::ClanInfoList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::ClanInfo* > clanInfoList;

				// Optional member valiable declare

				ClanInfoList()
				{
					Init();
				}

				virtual ~ClanInfoList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::ClanInfo* >::iterator iter = clanInfoList.begin(); iter != clanInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ClanInfo* pItem = (*iter);
						delete pItem;
					}
					clanInfoList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::ClanInfo* >::iterator iter = clanInfoList.begin(); iter != clanInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ClanInfo* pItem = (*iter);
						delete pItem;
					}
					clanInfoList.clear();

					// Optional member variable clear
				}

				bool Copy(const ClanInfoList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::ClanInfo* >::const_iterator iter = rhs.clanInfoList.begin(); iter != rhs.clanInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ClanInfo* pItem = new Camel::Dar2::SS::ClanInfo;
						pItem->Copy(*(*iter));
						clanInfoList.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member clanInfoList
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::ClanInfo* >::iterator iter = clanInfoList.begin(); iter != clanInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ClanInfo* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_clanInfoListSize = clanInfoList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_clanInfoListSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::ClanInfo* >::iterator iter = clanInfoList.begin(); iter != clanInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ClanInfo* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_clanInfoListSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_clanInfoListSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_clanInfoListSize ; ++nIndex)
					{
						Camel::Dar2::SS::ClanInfo* pTemp = new Camel::Dar2::SS::ClanInfo;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						clanInfoList.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member clanInfoList
					{
						Json::Value JsonValue_clanInfoList;
						for ( std::list<Camel::Dar2::SS::ClanInfo* >::const_iterator iter = clanInfoList.begin(); iter != clanInfoList.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::ClanInfo* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_clanInfoList.append( JsonValue_temp);
							}
						Json_root["clanInfoList"] = JsonValue_clanInfoList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member clanInfoList
					if ( (! Json_root["clanInfoList"].isNull()) && (Json_root["clanInfoList"].isArray()))
					{
						unsigned int n_clanInfoListSize = Json_root["clanInfoList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_clanInfoListSize ; ++nIndex)
						{
							Camel::Dar2::SS::ClanInfo* temp = new Camel::Dar2::SS::ClanInfo;
							if (false ==  temp->LoadJSON( Json_root["clanInfoList"][nIndex]))		{    delete temp; return false;     }
							clanInfoList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_ClanInfoList__MessageId_3666510246__

		#if !defined __DarMessageProtocol_SS_ClanMemberProfileInfo__MessageId_1844224852__
		#define __DarMessageProtocol_SS_ClanMemberProfileInfo__MessageId_1844224852__
		namespace SS
		{
			class ClanMemberProfileInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1844224852U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::ClanMemberProfileInfo";	};

				// const member valiable declare

				// member valiable declare
				std::string nickname;
				std::string userCN;
				int avatarInfo[26];

				// Optional member valiable declare

				ClanMemberProfileInfo()
				{
					Init();
				}

				virtual ~ClanMemberProfileInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					nickname = "";
					userCN = "";
					for ( int nIndex = 0; nIndex < 26 ; ++nIndex)
					{
						avatarInfo[nIndex] = static_cast<int>(0);
					}

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					nickname = "";
					userCN = "";
					::memset(avatarInfo, 0, sizeof(avatarInfo));

					// Optional member variable clear
				}

				bool Copy(const ClanMemberProfileInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					nickname = rhs.nickname;
					userCN = rhs.userCN;
					for ( int i=0; i<26; ++i)
					{
						avatarInfo[i] = rhs.avatarInfo[i];
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member nickname
					nLength += sizeof(int);
					nLength += nickname.length();
					// GetLength Member userCN
					nLength += sizeof(int);
					nLength += userCN.length();
					// GetLength Member avatarInfo
					nLength += sizeof(avatarInfo);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, nickname))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, userCN))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, avatarInfo, 26))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, nickname, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, userCN, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load<int>( clMessageBlock, avatarInfo, 26, bRequireChangeByteOrder))
					{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member nickname
					Json_root["nickname"] = nickname;
					// Save Member userCN
					Json_root["userCN"] = userCN;
					// Save Member avatarInfo
					{
						Json::Value JsonValue_avatarInfo;
						for ( int i=0; i<26; ++i)
						{
							JsonValue_avatarInfo.append( avatarInfo[i]);
						}
						Json_root["avatarInfo"] = JsonValue_avatarInfo;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member nickname
					if (! Json_root["nickname"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["nickname"], nickname))	{	return false;	}
					}
					// Load Member userCN
					if (! Json_root["userCN"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["userCN"], userCN))	{	return false;	}
					}
					// Load Member avatarInfo
					if ( (! Json_root["avatarInfo"].isNull()) && (Json_root["avatarInfo"].isArray()))
					{
						unsigned int n_avatarInfo_Size = Json_root["avatarInfo"].size();
						if ( n_avatarInfo_Size > 26)	{	return false;	}
						for ( unsigned int nIndex = 0; nIndex < n_avatarInfo_Size ; ++nIndex)
						{
							if (false == DAR2Rule_JSON::Load(Json_root["avatarInfo"][nIndex], avatarInfo[nIndex]))	{	return false;	}
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_ClanMemberProfileInfo__MessageId_1844224852__

		#if !defined __DarMessageProtocol_SS_ClanMemberInfo__MessageId_2900406020__
		#define __DarMessageProtocol_SS_ClanMemberInfo__MessageId_2900406020__
		namespace SS
		{
			class ClanMemberInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2900406020U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::ClanMemberInfo";	};

				// const member valiable declare

				// member valiable declare
				std::string userID;
				Camel::Dar2::SS::PresenceInfo presence;
				Camel::Dar2::SS::ClanMemberProfileInfo profile;

				// Optional member valiable declare

				ClanMemberInfo()
				{
					Init();
				}

				virtual ~ClanMemberInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					userID = "";
					presence.Clear();
					profile.Clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					userID = "";
					presence.Clear();
					profile.Clear();

					// Optional member variable clear
				}

				bool Copy(const ClanMemberInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					userID = rhs.userID;
					presence.Copy(rhs.presence);
					profile.Copy(rhs.profile);

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member userID
					nLength += sizeof(int);
					nLength += userID.length();
					// GetLength Member presence
					nLength += presence.GetLength();
					// GetLength Member profile
					nLength += profile.GetLength();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, userID))		{    return false;     }
					if (false == presence.Save( stream))        {    return false;     }
					if (false == profile.Save( stream))        {    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, userID, bRequireChangeByteOrder))		{    return false;     }
					if ( false == presence.Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {      return false;     }
					if ( false == profile.Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {      return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member userID
					Json_root["userID"] = userID;
					// Save Member presence
					{
						Json::Value JsonValue_presence;
						if (false == presence.SaveJSON( JsonValue_presence))        {    return false;     }
						Json_root["presence"] = JsonValue_presence;
					}
					// Save Member profile
					{
						Json::Value JsonValue_profile;
						if (false == profile.SaveJSON( JsonValue_profile))        {    return false;     }
						Json_root["profile"] = JsonValue_profile;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member userID
					if (! Json_root["userID"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["userID"], userID))	{	return false;	}
					}
					// Load Member presence
					if (! Json_root["presence"].isNull())
					{
						if (false == presence.LoadJSON( Json_root["presence"]))	{	return false;	}
					}
					// Load Member profile
					if (! Json_root["profile"].isNull())
					{
						if (false == profile.LoadJSON( Json_root["profile"]))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_ClanMemberInfo__MessageId_2900406020__

		#if !defined __DarMessageProtocol_SS_ClanMemberInfoList__MessageId_2565911550__
		#define __DarMessageProtocol_SS_ClanMemberInfoList__MessageId_2565911550__
		namespace SS
		{
			class ClanMemberInfoList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2565911550U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::ClanMemberInfoList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::ClanMemberInfo* > clanMemberInfoList;

				// Optional member valiable declare

				ClanMemberInfoList()
				{
					Init();
				}

				virtual ~ClanMemberInfoList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::ClanMemberInfo* >::iterator iter = clanMemberInfoList.begin(); iter != clanMemberInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ClanMemberInfo* pItem = (*iter);
						delete pItem;
					}
					clanMemberInfoList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::ClanMemberInfo* >::iterator iter = clanMemberInfoList.begin(); iter != clanMemberInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ClanMemberInfo* pItem = (*iter);
						delete pItem;
					}
					clanMemberInfoList.clear();

					// Optional member variable clear
				}

				bool Copy(const ClanMemberInfoList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::ClanMemberInfo* >::const_iterator iter = rhs.clanMemberInfoList.begin(); iter != rhs.clanMemberInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ClanMemberInfo* pItem = new Camel::Dar2::SS::ClanMemberInfo;
						pItem->Copy(*(*iter));
						clanMemberInfoList.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member clanMemberInfoList
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::ClanMemberInfo* >::iterator iter = clanMemberInfoList.begin(); iter != clanMemberInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ClanMemberInfo* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_clanMemberInfoListSize = clanMemberInfoList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_clanMemberInfoListSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::ClanMemberInfo* >::iterator iter = clanMemberInfoList.begin(); iter != clanMemberInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ClanMemberInfo* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_clanMemberInfoListSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_clanMemberInfoListSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_clanMemberInfoListSize ; ++nIndex)
					{
						Camel::Dar2::SS::ClanMemberInfo* pTemp = new Camel::Dar2::SS::ClanMemberInfo;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						clanMemberInfoList.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member clanMemberInfoList
					{
						Json::Value JsonValue_clanMemberInfoList;
						for ( std::list<Camel::Dar2::SS::ClanMemberInfo* >::const_iterator iter = clanMemberInfoList.begin(); iter != clanMemberInfoList.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::ClanMemberInfo* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_clanMemberInfoList.append( JsonValue_temp);
							}
						Json_root["clanMemberInfoList"] = JsonValue_clanMemberInfoList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member clanMemberInfoList
					if ( (! Json_root["clanMemberInfoList"].isNull()) && (Json_root["clanMemberInfoList"].isArray()))
					{
						unsigned int n_clanMemberInfoListSize = Json_root["clanMemberInfoList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_clanMemberInfoListSize ; ++nIndex)
						{
							Camel::Dar2::SS::ClanMemberInfo* temp = new Camel::Dar2::SS::ClanMemberInfo;
							if (false ==  temp->LoadJSON( Json_root["clanMemberInfoList"][nIndex]))		{    delete temp; return false;     }
							clanMemberInfoList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_ClanMemberInfoList__MessageId_2565911550__

		#if !defined __DarMessageProtocol_SS_RevisionData__MessageId_2322136755__
		#define __DarMessageProtocol_SS_RevisionData__MessageId_2322136755__
		namespace SS
		{
			class RevisionData : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2322136755U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::RevisionData";	};

				// const member valiable declare

				// member valiable declare
				Camel::Dar2::SS::RevisionDataType type;
				std::string masterRevision;
				int pigeonRevision;

				// Optional member valiable declare

				RevisionData()
				{
					Init();
				}

				virtual ~RevisionData()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					type = static_cast<Camel::Dar2::SS::RevisionDataType>(0);
					masterRevision = "";
					pigeonRevision = static_cast<int>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					::memset(&type, 0, sizeof(type));
					masterRevision = "";
					::memset(&pigeonRevision, 0, sizeof(pigeonRevision));

					// Optional member variable clear
				}

				bool Copy(const RevisionData& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					type = rhs.type;
					masterRevision = rhs.masterRevision;
					pigeonRevision = rhs.pigeonRevision;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member type
					nLength += sizeof(type);
					// GetLength Member masterRevision
					nLength += sizeof(int);
					nLength += masterRevision.length();
					// GetLength Member pigeonRevision
					nLength += sizeof(pigeonRevision);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::RevisionDataType>( stream, type))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, masterRevision))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, pigeonRevision))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::RevisionDataType>( clMessageBlock, type, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, masterRevision, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, pigeonRevision, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member type
					Json_root["type"] = type;
					// Save Member masterRevision
					Json_root["masterRevision"] = masterRevision;
					// Save Member pigeonRevision
					Json_root["pigeonRevision"] = pigeonRevision;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member type
					if (! Json_root["type"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["type"], temp))	{	return false;	}
						type = static_cast<Camel::Dar2::SS::RevisionDataType>(temp);
					}
					// Load Member masterRevision
					if (! Json_root["masterRevision"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["masterRevision"], masterRevision))	{	return false;	}
					}
					// Load Member pigeonRevision
					if (! Json_root["pigeonRevision"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["pigeonRevision"], pigeonRevision))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_RevisionData__MessageId_2322136755__

		#if !defined __DarMessageProtocol_SS_RevisionDataList__MessageId_1177896671__
		#define __DarMessageProtocol_SS_RevisionDataList__MessageId_1177896671__
		namespace SS
		{
			class RevisionDataList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1177896671U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::RevisionDataList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::RevisionData* > revisions;

				// Optional member valiable declare

				RevisionDataList()
				{
					Init();
				}

				virtual ~RevisionDataList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::RevisionData* >::iterator iter = revisions.begin(); iter != revisions.end(); ++iter)
					{
						Camel::Dar2::SS::RevisionData* pItem = (*iter);
						delete pItem;
					}
					revisions.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::RevisionData* >::iterator iter = revisions.begin(); iter != revisions.end(); ++iter)
					{
						Camel::Dar2::SS::RevisionData* pItem = (*iter);
						delete pItem;
					}
					revisions.clear();

					// Optional member variable clear
				}

				bool Copy(const RevisionDataList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::RevisionData* >::const_iterator iter = rhs.revisions.begin(); iter != rhs.revisions.end(); ++iter)
					{
						Camel::Dar2::SS::RevisionData* pItem = new Camel::Dar2::SS::RevisionData;
						pItem->Copy(*(*iter));
						revisions.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member revisions
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::RevisionData* >::iterator iter = revisions.begin(); iter != revisions.end(); ++iter)
					{
						Camel::Dar2::SS::RevisionData* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_revisionsSize = revisions.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_revisionsSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::RevisionData* >::iterator iter = revisions.begin(); iter != revisions.end(); ++iter)
					{
						Camel::Dar2::SS::RevisionData* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_revisionsSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_revisionsSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_revisionsSize ; ++nIndex)
					{
						Camel::Dar2::SS::RevisionData* pTemp = new Camel::Dar2::SS::RevisionData;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						revisions.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member revisions
					{
						Json::Value JsonValue_revisions;
						for ( std::list<Camel::Dar2::SS::RevisionData* >::const_iterator iter = revisions.begin(); iter != revisions.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::RevisionData* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_revisions.append( JsonValue_temp);
							}
						Json_root["revisions"] = JsonValue_revisions;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member revisions
					if ( (! Json_root["revisions"].isNull()) && (Json_root["revisions"].isArray()))
					{
						unsigned int n_revisionsSize = Json_root["revisions"].size();
						for ( unsigned int nIndex = 0; nIndex < n_revisionsSize ; ++nIndex)
						{
							Camel::Dar2::SS::RevisionData* temp = new Camel::Dar2::SS::RevisionData;
							if (false ==  temp->LoadJSON( Json_root["revisions"][nIndex]))		{    delete temp; return false;     }
							revisions.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_RevisionDataList__MessageId_1177896671__

		#if !defined __DarMessageProtocol_SS_MobileDeviceInfo__MessageId_3917452939__
		#define __DarMessageProtocol_SS_MobileDeviceInfo__MessageId_3917452939__
		namespace SS
		{
			class MobileDeviceInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=3917452939U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::MobileDeviceInfo";	};

				// const member valiable declare

				// member valiable declare
				Camel::Dar2::SS::MobileDeviceType deviceType;
				std::string deviceModelName;
				std::string deviceFirmwareVersion;
				std::string deviceOSVersion;
				std::string phoneNumber;
				int phoneNumberAuthFlag;
				std::string macAddress;
				std::string simOperatorName;

				// Optional member valiable declare

				MobileDeviceInfo()
				{
					Init();
				}

				virtual ~MobileDeviceInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					deviceType = static_cast<Camel::Dar2::SS::MobileDeviceType>(0);
					deviceModelName = "";
					deviceFirmwareVersion = "";
					deviceOSVersion = "";
					phoneNumber = "";
					phoneNumberAuthFlag = static_cast<int>(0);
					macAddress = "";
					simOperatorName = "";

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					::memset(&deviceType, 0, sizeof(deviceType));
					deviceModelName = "";
					deviceFirmwareVersion = "";
					deviceOSVersion = "";
					phoneNumber = "";
					::memset(&phoneNumberAuthFlag, 0, sizeof(phoneNumberAuthFlag));
					macAddress = "";
					simOperatorName = "";

					// Optional member variable clear
				}

				bool Copy(const MobileDeviceInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					deviceType = rhs.deviceType;
					deviceModelName = rhs.deviceModelName;
					deviceFirmwareVersion = rhs.deviceFirmwareVersion;
					deviceOSVersion = rhs.deviceOSVersion;
					phoneNumber = rhs.phoneNumber;
					phoneNumberAuthFlag = rhs.phoneNumberAuthFlag;
					macAddress = rhs.macAddress;
					simOperatorName = rhs.simOperatorName;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member deviceType
					nLength += sizeof(deviceType);
					// GetLength Member deviceModelName
					nLength += sizeof(int);
					nLength += deviceModelName.length();
					// GetLength Member deviceFirmwareVersion
					nLength += sizeof(int);
					nLength += deviceFirmwareVersion.length();
					// GetLength Member deviceOSVersion
					nLength += sizeof(int);
					nLength += deviceOSVersion.length();
					// GetLength Member phoneNumber
					nLength += sizeof(int);
					nLength += phoneNumber.length();
					// GetLength Member phoneNumberAuthFlag
					nLength += sizeof(phoneNumberAuthFlag);
					// GetLength Member macAddress
					nLength += sizeof(int);
					nLength += macAddress.length();
					// GetLength Member simOperatorName
					nLength += sizeof(int);
					nLength += simOperatorName.length();

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save<Camel::Dar2::SS::MobileDeviceType>( stream, deviceType))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, deviceModelName))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, deviceFirmwareVersion))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, deviceOSVersion))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, phoneNumber))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, phoneNumberAuthFlag))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, macAddress))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, simOperatorName))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false ==  DAR2Rule_STREAM::Load<Camel::Dar2::SS::MobileDeviceType>( clMessageBlock, deviceType, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, deviceModelName, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, deviceFirmwareVersion, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, deviceOSVersion, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, phoneNumber, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, phoneNumberAuthFlag, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, macAddress, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, simOperatorName, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member deviceType
					Json_root["deviceType"] = deviceType;
					// Save Member deviceModelName
					Json_root["deviceModelName"] = deviceModelName;
					// Save Member deviceFirmwareVersion
					Json_root["deviceFirmwareVersion"] = deviceFirmwareVersion;
					// Save Member deviceOSVersion
					Json_root["deviceOSVersion"] = deviceOSVersion;
					// Save Member phoneNumber
					Json_root["phoneNumber"] = phoneNumber;
					// Save Member phoneNumberAuthFlag
					Json_root["phoneNumberAuthFlag"] = phoneNumberAuthFlag;
					// Save Member macAddress
					Json_root["macAddress"] = macAddress;
					// Save Member simOperatorName
					Json_root["simOperatorName"] = simOperatorName;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member deviceType
					if (! Json_root["deviceType"].isNull())
					{
						int temp;
						if (false == DAR2Rule_JSON::Load(Json_root["deviceType"], temp))	{	return false;	}
						deviceType = static_cast<Camel::Dar2::SS::MobileDeviceType>(temp);
					}
					// Load Member deviceModelName
					if (! Json_root["deviceModelName"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["deviceModelName"], deviceModelName))	{	return false;	}
					}
					// Load Member deviceFirmwareVersion
					if (! Json_root["deviceFirmwareVersion"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["deviceFirmwareVersion"], deviceFirmwareVersion))	{	return false;	}
					}
					// Load Member deviceOSVersion
					if (! Json_root["deviceOSVersion"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["deviceOSVersion"], deviceOSVersion))	{	return false;	}
					}
					// Load Member phoneNumber
					if (! Json_root["phoneNumber"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["phoneNumber"], phoneNumber))	{	return false;	}
					}
					// Load Member phoneNumberAuthFlag
					if (! Json_root["phoneNumberAuthFlag"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["phoneNumberAuthFlag"], phoneNumberAuthFlag))	{	return false;	}
					}
					// Load Member macAddress
					if (! Json_root["macAddress"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["macAddress"], macAddress))	{	return false;	}
					}
					// Load Member simOperatorName
					if (! Json_root["simOperatorName"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["simOperatorName"], simOperatorName))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_MobileDeviceInfo__MessageId_3917452939__

		#if !defined __DarMessageProtocol_SS_AuthTokenInfo__MessageId_1229074018__
		#define __DarMessageProtocol_SS_AuthTokenInfo__MessageId_1229074018__
		namespace SS
		{
			class AuthTokenInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1229074018U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::AuthTokenInfo";	};

				// const member valiable declare

				// member valiable declare
				std::string token;
				std::string parentToken;
				__int64 expireReservedDate;
				__int64 expireDate;
				int tokenState;

				// Optional member valiable declare

				AuthTokenInfo()
				{
					Init();
				}

				virtual ~AuthTokenInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					token = "";
					parentToken = "";
					expireReservedDate = static_cast<__int64>(0);
					expireDate = static_cast<__int64>(0);
					tokenState = static_cast<int>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					token = "";
					parentToken = "";
					::memset(&expireReservedDate, 0, sizeof(expireReservedDate));
					::memset(&expireDate, 0, sizeof(expireDate));
					::memset(&tokenState, 0, sizeof(tokenState));

					// Optional member variable clear
				}

				bool Copy(const AuthTokenInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					token = rhs.token;
					parentToken = rhs.parentToken;
					expireReservedDate = rhs.expireReservedDate;
					expireDate = rhs.expireDate;
					tokenState = rhs.tokenState;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member token
					nLength += sizeof(int);
					nLength += token.length();
					// GetLength Member parentToken
					nLength += sizeof(int);
					nLength += parentToken.length();
					// GetLength Member expireReservedDate
					nLength += sizeof(expireReservedDate);
					// GetLength Member expireDate
					nLength += sizeof(expireDate);
					// GetLength Member tokenState
					nLength += sizeof(tokenState);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, token))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, parentToken))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<__int64>( stream, expireReservedDate))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<__int64>( stream, expireDate))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, tokenState))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, token, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, parentToken, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<__int64>( clMessageBlock, expireReservedDate, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<__int64>( clMessageBlock, expireDate, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, tokenState, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member token
					Json_root["token"] = token;
					// Save Member parentToken
					Json_root["parentToken"] = parentToken;
					// Save Member expireReservedDate
					Json_root["expireReservedDate"] = expireReservedDate;
					// Save Member expireDate
					Json_root["expireDate"] = expireDate;
					// Save Member tokenState
					Json_root["tokenState"] = tokenState;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member token
					if (! Json_root["token"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["token"], token))	{	return false;	}
					}
					// Load Member parentToken
					if (! Json_root["parentToken"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["parentToken"], parentToken))	{	return false;	}
					}
					// Load Member expireReservedDate
					if (! Json_root["expireReservedDate"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["expireReservedDate"], expireReservedDate))	{	return false;	}
					}
					// Load Member expireDate
					if (! Json_root["expireDate"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["expireDate"], expireDate))	{	return false;	}
					}
					// Load Member tokenState
					if (! Json_root["tokenState"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["tokenState"], tokenState))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_AuthTokenInfo__MessageId_1229074018__

		#if !defined __DarMessageProtocol_SS_ServiceCodeInfo__MessageId_4158545590__
		#define __DarMessageProtocol_SS_ServiceCodeInfo__MessageId_4158545590__
		namespace SS
		{
			class ServiceCodeInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=4158545590U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::ServiceCodeInfo";	};

				// const member valiable declare

				// member valiable declare
				std::string serviceCode;
				std::string gameCode;
				std::string description;
				bool activeService;

				// Optional member valiable declare

				ServiceCodeInfo()
				{
					Init();
				}

				virtual ~ServiceCodeInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					serviceCode = "";
					gameCode = "";
					description = "";
					activeService = static_cast<bool>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					serviceCode = "";
					gameCode = "";
					description = "";
					::memset(&activeService, 0, sizeof(activeService));

					// Optional member variable clear
				}

				bool Copy(const ServiceCodeInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					serviceCode = rhs.serviceCode;
					gameCode = rhs.gameCode;
					description = rhs.description;
					activeService = rhs.activeService;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member serviceCode
					nLength += sizeof(int);
					nLength += serviceCode.length();
					// GetLength Member gameCode
					nLength += sizeof(int);
					nLength += gameCode.length();
					// GetLength Member description
					nLength += sizeof(int);
					nLength += description.length();
					// GetLength Member activeService
					nLength += sizeof(activeService);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save( stream, serviceCode))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, gameCode))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, description))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<bool>( stream, activeService))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, serviceCode, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, gameCode, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, description, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<bool>( clMessageBlock, activeService, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member serviceCode
					Json_root["serviceCode"] = serviceCode;
					// Save Member gameCode
					Json_root["gameCode"] = gameCode;
					// Save Member description
					Json_root["description"] = description;
					// Save Member activeService
					Json_root["activeService"] = activeService;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member serviceCode
					if (! Json_root["serviceCode"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["serviceCode"], serviceCode))	{	return false;	}
					}
					// Load Member gameCode
					if (! Json_root["gameCode"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["gameCode"], gameCode))	{	return false;	}
					}
					// Load Member description
					if (! Json_root["description"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["description"], description))	{	return false;	}
					}
					// Load Member activeService
					if (! Json_root["activeService"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["activeService"], activeService))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_ServiceCodeInfo__MessageId_4158545590__

		#if !defined __DarMessageProtocol_SS_ServiceCodeInfoList__MessageId_1760987570__
		#define __DarMessageProtocol_SS_ServiceCodeInfoList__MessageId_1760987570__
		namespace SS
		{
			class ServiceCodeInfoList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1760987570U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::ServiceCodeInfoList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::ServiceCodeInfo* > serviceCodeInfoList;

				// Optional member valiable declare

				ServiceCodeInfoList()
				{
					Init();
				}

				virtual ~ServiceCodeInfoList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::ServiceCodeInfo* >::iterator iter = serviceCodeInfoList.begin(); iter != serviceCodeInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ServiceCodeInfo* pItem = (*iter);
						delete pItem;
					}
					serviceCodeInfoList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::ServiceCodeInfo* >::iterator iter = serviceCodeInfoList.begin(); iter != serviceCodeInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ServiceCodeInfo* pItem = (*iter);
						delete pItem;
					}
					serviceCodeInfoList.clear();

					// Optional member variable clear
				}

				bool Copy(const ServiceCodeInfoList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::ServiceCodeInfo* >::const_iterator iter = rhs.serviceCodeInfoList.begin(); iter != rhs.serviceCodeInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ServiceCodeInfo* pItem = new Camel::Dar2::SS::ServiceCodeInfo;
						pItem->Copy(*(*iter));
						serviceCodeInfoList.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member serviceCodeInfoList
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::ServiceCodeInfo* >::iterator iter = serviceCodeInfoList.begin(); iter != serviceCodeInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ServiceCodeInfo* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_serviceCodeInfoListSize = serviceCodeInfoList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_serviceCodeInfoListSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::ServiceCodeInfo* >::iterator iter = serviceCodeInfoList.begin(); iter != serviceCodeInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::ServiceCodeInfo* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_serviceCodeInfoListSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_serviceCodeInfoListSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_serviceCodeInfoListSize ; ++nIndex)
					{
						Camel::Dar2::SS::ServiceCodeInfo* pTemp = new Camel::Dar2::SS::ServiceCodeInfo;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						serviceCodeInfoList.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member serviceCodeInfoList
					{
						Json::Value JsonValue_serviceCodeInfoList;
						for ( std::list<Camel::Dar2::SS::ServiceCodeInfo* >::const_iterator iter = serviceCodeInfoList.begin(); iter != serviceCodeInfoList.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::ServiceCodeInfo* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_serviceCodeInfoList.append( JsonValue_temp);
							}
						Json_root["serviceCodeInfoList"] = JsonValue_serviceCodeInfoList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member serviceCodeInfoList
					if ( (! Json_root["serviceCodeInfoList"].isNull()) && (Json_root["serviceCodeInfoList"].isArray()))
					{
						unsigned int n_serviceCodeInfoListSize = Json_root["serviceCodeInfoList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_serviceCodeInfoListSize ; ++nIndex)
						{
							Camel::Dar2::SS::ServiceCodeInfo* temp = new Camel::Dar2::SS::ServiceCodeInfo;
							if (false ==  temp->LoadJSON( Json_root["serviceCodeInfoList"][nIndex]))		{    delete temp; return false;     }
							serviceCodeInfoList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_ServiceCodeInfoList__MessageId_1760987570__

		#if !defined __DarMessageProtocol_SS_CategoryMasterInfo__MessageId_4267820706__
		#define __DarMessageProtocol_SS_CategoryMasterInfo__MessageId_4267820706__
		namespace SS
		{
			class CategoryMasterInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=4267820706U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::CategoryMasterInfo";	};

				// const member valiable declare

				// member valiable declare
				int categoryUid;
				int sortOrder;
				std::string serviceCode;
				std::string description;
				std::string serviceURL;
				std::string groupTypeName;
				bool useExternalSync;

				// Optional member valiable declare

				CategoryMasterInfo()
				{
					Init();
				}

				virtual ~CategoryMasterInfo()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					categoryUid = static_cast<int>(0);
					sortOrder = static_cast<int>(0);
					serviceCode = "";
					description = "";
					serviceURL = "";
					groupTypeName = "";
					useExternalSync = static_cast<bool>(0);

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					::memset(&categoryUid, 0, sizeof(categoryUid));
					::memset(&sortOrder, 0, sizeof(sortOrder));
					serviceCode = "";
					description = "";
					serviceURL = "";
					groupTypeName = "";
					::memset(&useExternalSync, 0, sizeof(useExternalSync));

					// Optional member variable clear
				}

				bool Copy(const CategoryMasterInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					categoryUid = rhs.categoryUid;
					sortOrder = rhs.sortOrder;
					serviceCode = rhs.serviceCode;
					description = rhs.description;
					serviceURL = rhs.serviceURL;
					groupTypeName = rhs.groupTypeName;
					useExternalSync = rhs.useExternalSync;

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member categoryUid
					nLength += sizeof(categoryUid);
					// GetLength Member sortOrder
					nLength += sizeof(sortOrder);
					// GetLength Member serviceCode
					nLength += sizeof(int);
					nLength += serviceCode.length();
					// GetLength Member description
					nLength += sizeof(int);
					nLength += description.length();
					// GetLength Member serviceURL
					nLength += sizeof(int);
					nLength += serviceURL.length();
					// GetLength Member groupTypeName
					nLength += sizeof(int);
					nLength += groupTypeName.length();
					// GetLength Member useExternalSync
					nLength += sizeof(useExternalSync);

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, categoryUid))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<int>( stream, sortOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, serviceCode))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, description))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, serviceURL))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save( stream, groupTypeName))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Save<bool>( stream, useExternalSync))		{    return false;     }

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, categoryUid, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<int>( clMessageBlock, sortOrder, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, serviceCode, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, description, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, serviceURL, bRequireChangeByteOrder))		{    return false;     }
					if (false == DAR2Rule_STREAM::Load( clMessageBlock, groupTypeName, bRequireChangeByteOrder))		{    return false;     }
					if (false ==  DAR2Rule_STREAM::Load<bool>( clMessageBlock, useExternalSync, bRequireChangeByteOrder))		{    return false;     }
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member categoryUid
					Json_root["categoryUid"] = categoryUid;
					// Save Member sortOrder
					Json_root["sortOrder"] = sortOrder;
					// Save Member serviceCode
					Json_root["serviceCode"] = serviceCode;
					// Save Member description
					Json_root["description"] = description;
					// Save Member serviceURL
					Json_root["serviceURL"] = serviceURL;
					// Save Member groupTypeName
					Json_root["groupTypeName"] = groupTypeName;
					// Save Member useExternalSync
					Json_root["useExternalSync"] = useExternalSync;
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member categoryUid
					if (! Json_root["categoryUid"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["categoryUid"], categoryUid))	{	return false;	}
					}
					// Load Member sortOrder
					if (! Json_root["sortOrder"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["sortOrder"], sortOrder))	{	return false;	}
					}
					// Load Member serviceCode
					if (! Json_root["serviceCode"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["serviceCode"], serviceCode))	{	return false;	}
					}
					// Load Member description
					if (! Json_root["description"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["description"], description))	{	return false;	}
					}
					// Load Member serviceURL
					if (! Json_root["serviceURL"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["serviceURL"], serviceURL))	{	return false;	}
					}
					// Load Member groupTypeName
					if (! Json_root["groupTypeName"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["groupTypeName"], groupTypeName))	{	return false;	}
					}
					// Load Member useExternalSync
					if (! Json_root["useExternalSync"].isNull())
					{
						if (false == DAR2Rule_JSON::Load(Json_root["useExternalSync"], useExternalSync))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_CategoryMasterInfo__MessageId_4267820706__

		#if !defined __DarMessageProtocol_SS_CategoryMasterInfoList__MessageId_2156326924__
		#define __DarMessageProtocol_SS_CategoryMasterInfoList__MessageId_2156326924__
		namespace SS
		{
			class CategoryMasterInfoList : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2156326924U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "SS::CategoryMasterInfoList";	};

				// const member valiable declare

				// member valiable declare
				std::list<Camel::Dar2::SS::CategoryMasterInfo* > categoryInfoList;

				// Optional member valiable declare

				CategoryMasterInfoList()
				{
					Init();
				}

				virtual ~CategoryMasterInfoList()
				{
					Clear();
				}

				virtual void Init()
				{
					// member variable init
					for ( std::list<Camel::Dar2::SS::CategoryMasterInfo* >::iterator iter = categoryInfoList.begin(); iter != categoryInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::CategoryMasterInfo* pItem = (*iter);
						delete pItem;
					}
					categoryInfoList.clear();

					// Optional member variable init
				}

				virtual void Clear()
				{
					// member variable clear
					for ( std::list<Camel::Dar2::SS::CategoryMasterInfo* >::iterator iter = categoryInfoList.begin(); iter != categoryInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::CategoryMasterInfo* pItem = (*iter);
						delete pItem;
					}
					categoryInfoList.clear();

					// Optional member variable clear
				}

				bool Copy(const CategoryMasterInfoList& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Init();
					// member variable copy
					for ( std::list<Camel::Dar2::SS::CategoryMasterInfo* >::const_iterator iter = rhs.categoryInfoList.begin(); iter != rhs.categoryInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::CategoryMasterInfo* pItem = new Camel::Dar2::SS::CategoryMasterInfo;
						pItem->Copy(*(*iter));
						categoryInfoList.push_back(pItem);
					}

					// Optional member variable copy
					return true;
				}

				// GetLength Function
				virtual int GetLength( IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _GetLengthSTREAM( );
					}
					return -1;
				}

				// GetLength STREAM Format
				int _GetLengthSTREAM( )
				{
					int nLength = 0;
					// GetLength Member categoryInfoList
					nLength += sizeof(int);
					for ( std::list<Camel::Dar2::SS::CategoryMasterInfo* >::iterator iter = categoryInfoList.begin(); iter != categoryInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::CategoryMasterInfo* pObject = (*iter);
						nLength += pObject->GetLength();
					}

					nLength += sizeof(unsigned char);

					return nLength;
				}

				// Save Function
				virtual bool Save(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM)
				{
					// Stream Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _SaveSTREAM( stream);
					}
					return false;
				}

				virtual bool Load(Base::System::Stream::Handle stream, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Format
					int nStreamSize = Camel::Base::System::Stream::GetLength(stream);
					MessageBlock clBlock;
					if ( false == clBlock.SetData(stream))
					{
						return false;
					}
					bool bRet = Load(clBlock, eRule, bRequireChangeByteOrder);
					Camel::Base::System::Stream::RemoveLeft(stream, nStreamSize - clBlock.GetLength());
					return bRet;
				}
				bool Load(MessageBlock& rMessageBlock, IDarMsg::ProtocolRule eRule = IDarMsg::STREAM, bool bRequireChangeByteOrder = false)
				{
					// STREAM Initial Format
					if ( IDarMsg::STREAM == eRule)
					{
						return _LoadSTREAM( rMessageBlock, bRequireChangeByteOrder);
					}
					return false;
				}

				// save STREAM Format
				bool _SaveSTREAM( Base::System::Stream::Handle stream)
				{
					// member variable save
					unsigned int n_categoryInfoListSize = categoryInfoList.size();
					if ( false == DAR2Rule_STREAM::Save( stream, n_categoryInfoListSize ))      {      return false;     }
					for ( std::list<Camel::Dar2::SS::CategoryMasterInfo* >::iterator iter = categoryInfoList.begin(); iter != categoryInfoList.end(); ++iter)
					{
						Camel::Dar2::SS::CategoryMasterInfo* pObject = (*iter);
						if (false ==  pObject->Save( stream))       {    return false;     }
					}

					// Optional member count save
					unsigned char cOptionalMemberCount = (unsigned char)(0);
					if (false ==  DAR2Rule_STREAM::Save<unsigned char>( stream, cOptionalMemberCount))		{    return false;     }

					// Optional member variable save
					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					unsigned int n_categoryInfoListSize = 0;
					if ( false == DAR2Rule_STREAM::Load( clMessageBlock, n_categoryInfoListSize, bRequireChangeByteOrder))      {      return false;     }
					for ( unsigned int nIndex = 0; nIndex < n_categoryInfoListSize ; ++nIndex)
					{
						Camel::Dar2::SS::CategoryMasterInfo* pTemp = new Camel::Dar2::SS::CategoryMasterInfo;
						if (false == pTemp->Load( clMessageBlock, IDarMsg::STREAM, bRequireChangeByteOrder))       {    delete pTemp;    return false;     }
						categoryInfoList.push_back( pTemp);
					}
					unsigned char cOptionalMemberCnt = 0;
					if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, cOptionalMemberCnt, bRequireChangeByteOrder))		{    return false;     }
					for ( unsigned int nOptionalIndex = 0; nOptionalIndex < cOptionalMemberCnt ; ++nOptionalIndex)
					{
						unsigned short nOptionalTypeHash = 0;
						if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nOptionalTypeHash, bRequireChangeByteOrder))		{    return false;     }
						unsigned short nOptionalType = nOptionalTypeHash & 7;

						if (0 == nOptionalType) // 1byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 1))		{    return false;     }
						}
						else if (1 == nOptionalType) // 2byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 2))		{    return false;     }
						}
						else if (2 == nOptionalType) // 4byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 4))		{    return false;     }
						}
						else if (3 == nOptionalType) // 8byte
						{
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, 8))		{    return false;     }
						}
						else if (4 == nOptionalType) // variable type
						{
							int nSkipSize = 0;
							if (false ==  DAR2Rule_STREAM::Load( clMessageBlock, nSkipSize, bRequireChangeByteOrder))		{    return false;     }
							if (false ==  DAR2Rule_STREAM::Skip( clMessageBlock, nSkipSize))		{    return false;     }
						}
					}
					return true;
				}

				#if defined(__JSON_LIB_SUPPORT__)
				// save JSON Format
				virtual bool SaveJSON(std::string& strJsonFormat, IDarMsg::JSONOutFormat eFormat = IDarMsg::FAST) const
				{
					Json::Value Json_root;
					if (false == SaveJSON(Json_root))	{	return false;	}
					if ( IDarMsg::FAST == eFormat )	{	Json::FastWriter writer;	strJsonFormat = writer.write(Json_root);	}
					else	{		Json::StyledWriter writer;		strJsonFormat = writer.write(Json_root);	}
					return true;
				}

				bool SaveJSON(Json::Value& Json_root) const
				{
					// Save Member categoryInfoList
					{
						Json::Value JsonValue_categoryInfoList;
						for ( std::list<Camel::Dar2::SS::CategoryMasterInfo* >::const_iterator iter = categoryInfoList.begin(); iter != categoryInfoList.end(); ++iter)
						{
							Json::Value JsonValue_temp;
							Camel::Dar2::SS::CategoryMasterInfo* pObject = (*iter);
							if (false == pObject->SaveJSON( JsonValue_temp))        {    return false;     }
							JsonValue_categoryInfoList.append( JsonValue_temp);
							}
						Json_root["categoryInfoList"] = JsonValue_categoryInfoList;
					}
					return true;
				}

				// Load json format
				virtual bool LoadJSON(const std::string& strJsonFormat)
				{
					Json::Value root;
					Json::Reader reader;
					if ( false == reader.parse( strJsonFormat, root ))		{		return false;		}
					return LoadJSON(root);
				}

				bool LoadJSON(const Json::Value& Json_root)
				{
					Init();
					// Load Member categoryInfoList
					if ( (! Json_root["categoryInfoList"].isNull()) && (Json_root["categoryInfoList"].isArray()))
					{
						unsigned int n_categoryInfoListSize = Json_root["categoryInfoList"].size();
						for ( unsigned int nIndex = 0; nIndex < n_categoryInfoListSize ; ++nIndex)
						{
							Camel::Dar2::SS::CategoryMasterInfo* temp = new Camel::Dar2::SS::CategoryMasterInfo;
							if (false ==  temp->LoadJSON( Json_root["categoryInfoList"][nIndex]))		{    delete temp; return false;     }
							categoryInfoList.push_back( temp);
						}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};
			// const member valiable


		};
		#endif     // !defined __DarMessageProtocol_SS_CategoryMasterInfoList__MessageId_2156326924__
	}
}
