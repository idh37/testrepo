#pragma once

#include <Camel/Dar/DarBaseMsg.h>
#include <Camel/Dar/DarRule_STREAM.h>
#if defined(__JSON_LIB_SUPPORT__)
#include <Camel/Dar/DarRule_JSON.h>
#endif // defined(__JSON_LIB_SUPPORT__)


namespace Camel
{
	namespace Dar
	{
		namespace TMS
		{
			namespace EliminateReason
			{

				#if !defined __DarEnum_TMS_EliminateReason_type__MessageId_1878723979__
				#define __DarEnum_TMS_EliminateReason_type__MessageId_1878723979__
				enum type
				{
					Allin = 0,
					LeaveRoom = 1,
					TimeOver = 2,
					RankingOver = 3,
					LeaveChannel = 4,
					DontEnterRoom = 5,
					Error = 6,
				};

				// Camel::Dar::TMS::EliminateReason::type enum => string
				inline const char* EnumToString(Camel::Dar::TMS::EliminateReason::type eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar::TMS::EliminateReason::Allin == eEnumValue)
					{
						return "Camel::Dar::TMS::EliminateReason::Allin";
					}
					if ( Camel::Dar::TMS::EliminateReason::LeaveRoom == eEnumValue)
					{
						return "Camel::Dar::TMS::EliminateReason::LeaveRoom";
					}
					if ( Camel::Dar::TMS::EliminateReason::TimeOver == eEnumValue)
					{
						return "Camel::Dar::TMS::EliminateReason::TimeOver";
					}
					if ( Camel::Dar::TMS::EliminateReason::RankingOver == eEnumValue)
					{
						return "Camel::Dar::TMS::EliminateReason::RankingOver";
					}
					if ( Camel::Dar::TMS::EliminateReason::LeaveChannel == eEnumValue)
					{
						return "Camel::Dar::TMS::EliminateReason::LeaveChannel";
					}
					if ( Camel::Dar::TMS::EliminateReason::DontEnterRoom == eEnumValue)
					{
						return "Camel::Dar::TMS::EliminateReason::DontEnterRoom";
					}
					if ( Camel::Dar::TMS::EliminateReason::Error == eEnumValue)
					{
						return "Camel::Dar::TMS::EliminateReason::Error";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar::TMS::EliminateReason::type string => enum
				inline Camel::Dar::TMS::EliminateReason::type StringToEnum(const char* szStringValue, Camel::Dar::TMS::EliminateReason::type eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar::TMS::EliminateReason::Allin", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::EliminateReason::Allin;
					}
					if ( strcmp("Camel::Dar::TMS::EliminateReason::LeaveRoom", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::EliminateReason::LeaveRoom;
					}
					if ( strcmp("Camel::Dar::TMS::EliminateReason::TimeOver", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::EliminateReason::TimeOver;
					}
					if ( strcmp("Camel::Dar::TMS::EliminateReason::RankingOver", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::EliminateReason::RankingOver;
					}
					if ( strcmp("Camel::Dar::TMS::EliminateReason::LeaveChannel", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::EliminateReason::LeaveChannel;
					}
					if ( strcmp("Camel::Dar::TMS::EliminateReason::DontEnterRoom", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::EliminateReason::DontEnterRoom;
					}
					if ( strcmp("Camel::Dar::TMS::EliminateReason::Error", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::EliminateReason::Error;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_TMS_EliminateReason_type__MessageId_1878723979__
			}
		}
		namespace TMS
		{
			namespace EnterRoomType
			{

				#if !defined __DarEnum_TMS_EnterRoomType_Type__MessageId_418762918__
				#define __DarEnum_TMS_EnterRoomType_Type__MessageId_418762918__
				enum Type
				{
					StartTournament = 0,
					Rematching = 1,
					EnterTournament = 2,
					Reserved = 3,
				};

				// Camel::Dar::TMS::EnterRoomType::Type enum => string
				inline const char* EnumToString(Camel::Dar::TMS::EnterRoomType::Type eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar::TMS::EnterRoomType::StartTournament == eEnumValue)
					{
						return "Camel::Dar::TMS::EnterRoomType::StartTournament";
					}
					if ( Camel::Dar::TMS::EnterRoomType::Rematching == eEnumValue)
					{
						return "Camel::Dar::TMS::EnterRoomType::Rematching";
					}
					if ( Camel::Dar::TMS::EnterRoomType::EnterTournament == eEnumValue)
					{
						return "Camel::Dar::TMS::EnterRoomType::EnterTournament";
					}
					if ( Camel::Dar::TMS::EnterRoomType::Reserved == eEnumValue)
					{
						return "Camel::Dar::TMS::EnterRoomType::Reserved";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar::TMS::EnterRoomType::Type string => enum
				inline Camel::Dar::TMS::EnterRoomType::Type StringToEnum(const char* szStringValue, Camel::Dar::TMS::EnterRoomType::Type eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar::TMS::EnterRoomType::StartTournament", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::EnterRoomType::StartTournament;
					}
					if ( strcmp("Camel::Dar::TMS::EnterRoomType::Rematching", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::EnterRoomType::Rematching;
					}
					if ( strcmp("Camel::Dar::TMS::EnterRoomType::EnterTournament", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::EnterRoomType::EnterTournament;
					}
					if ( strcmp("Camel::Dar::TMS::EnterRoomType::Reserved", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::EnterRoomType::Reserved;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_TMS_EnterRoomType_Type__MessageId_418762918__
			}
		}
		namespace TMS
		{
			namespace ErrorCode
			{

				#if !defined __DarEnum_TMS_ErrorCode_Type__MessageId_4245915489__
				#define __DarEnum_TMS_ErrorCode_Type__MessageId_4245915489__
				enum Type
				{
					Success = 0,
					Error = 1,
					CreateUserFail = 2,
					AlreadyRegistered = 3,
					NotEnterableState = 4,
					NotEnterableTime = 5,
					AlreadyFull = 6,
					BlockedUser = 7,
					NotFoundUser = 8,
					AlreadyStarted = 9,
					NotReserver = 10,
					InvalidPosition = 11,
					AlreadyEliminated = 12,
					InvalidId = 13,
					FailedReserveUser = 14,
					MaxPrizeCount = 15,
					NotUnearnedWin = 16,
					Disconnect = 17,
					Max = 9999,
				};

				// Camel::Dar::TMS::ErrorCode::Type enum => string
				inline const char* EnumToString(Camel::Dar::TMS::ErrorCode::Type eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar::TMS::ErrorCode::Success == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::Success";
					}
					if ( Camel::Dar::TMS::ErrorCode::Error == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::Error";
					}
					if ( Camel::Dar::TMS::ErrorCode::CreateUserFail == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::CreateUserFail";
					}
					if ( Camel::Dar::TMS::ErrorCode::AlreadyRegistered == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::AlreadyRegistered";
					}
					if ( Camel::Dar::TMS::ErrorCode::NotEnterableState == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::NotEnterableState";
					}
					if ( Camel::Dar::TMS::ErrorCode::NotEnterableTime == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::NotEnterableTime";
					}
					if ( Camel::Dar::TMS::ErrorCode::AlreadyFull == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::AlreadyFull";
					}
					if ( Camel::Dar::TMS::ErrorCode::BlockedUser == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::BlockedUser";
					}
					if ( Camel::Dar::TMS::ErrorCode::NotFoundUser == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::NotFoundUser";
					}
					if ( Camel::Dar::TMS::ErrorCode::AlreadyStarted == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::AlreadyStarted";
					}
					if ( Camel::Dar::TMS::ErrorCode::NotReserver == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::NotReserver";
					}
					if ( Camel::Dar::TMS::ErrorCode::InvalidPosition == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::InvalidPosition";
					}
					if ( Camel::Dar::TMS::ErrorCode::AlreadyEliminated == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::AlreadyEliminated";
					}
					if ( Camel::Dar::TMS::ErrorCode::InvalidId == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::InvalidId";
					}
					if ( Camel::Dar::TMS::ErrorCode::FailedReserveUser == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::FailedReserveUser";
					}
					if ( Camel::Dar::TMS::ErrorCode::MaxPrizeCount == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::MaxPrizeCount";
					}
					if ( Camel::Dar::TMS::ErrorCode::NotUnearnedWin == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::NotUnearnedWin";
					}
					if ( Camel::Dar::TMS::ErrorCode::Disconnect == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::Disconnect";
					}
					if ( Camel::Dar::TMS::ErrorCode::Max == eEnumValue)
					{
						return "Camel::Dar::TMS::ErrorCode::Max";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar::TMS::ErrorCode::Type string => enum
				inline Camel::Dar::TMS::ErrorCode::Type StringToEnum(const char* szStringValue, Camel::Dar::TMS::ErrorCode::Type eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::Success", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::Success;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::Error", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::Error;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::CreateUserFail", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::CreateUserFail;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::AlreadyRegistered", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::AlreadyRegistered;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::NotEnterableState", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::NotEnterableState;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::NotEnterableTime", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::NotEnterableTime;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::AlreadyFull", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::AlreadyFull;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::BlockedUser", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::BlockedUser;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::NotFoundUser", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::NotFoundUser;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::AlreadyStarted", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::AlreadyStarted;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::NotReserver", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::NotReserver;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::InvalidPosition", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::InvalidPosition;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::AlreadyEliminated", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::AlreadyEliminated;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::InvalidId", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::InvalidId;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::FailedReserveUser", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::FailedReserveUser;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::MaxPrizeCount", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::MaxPrizeCount;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::NotUnearnedWin", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::NotUnearnedWin;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::Disconnect", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::Disconnect;
					}
					if ( strcmp("Camel::Dar::TMS::ErrorCode::Max", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::ErrorCode::Max;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_TMS_ErrorCode_Type__MessageId_4245915489__
			}
		}
		namespace TMS
		{
			namespace GameOverState
			{

				#if !defined __DarEnum_TMS_GameOverState_Type__MessageId_1700531115__
				#define __DarEnum_TMS_GameOverState_Type__MessageId_1700531115__
				enum Type
				{
					Win = 0,
					Lose = 1,
					Fold = 2,
				};

				// Camel::Dar::TMS::GameOverState::Type enum => string
				inline const char* EnumToString(Camel::Dar::TMS::GameOverState::Type eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar::TMS::GameOverState::Win == eEnumValue)
					{
						return "Camel::Dar::TMS::GameOverState::Win";
					}
					if ( Camel::Dar::TMS::GameOverState::Lose == eEnumValue)
					{
						return "Camel::Dar::TMS::GameOverState::Lose";
					}
					if ( Camel::Dar::TMS::GameOverState::Fold == eEnumValue)
					{
						return "Camel::Dar::TMS::GameOverState::Fold";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar::TMS::GameOverState::Type string => enum
				inline Camel::Dar::TMS::GameOverState::Type StringToEnum(const char* szStringValue, Camel::Dar::TMS::GameOverState::Type eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar::TMS::GameOverState::Win", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::GameOverState::Win;
					}
					if ( strcmp("Camel::Dar::TMS::GameOverState::Lose", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::GameOverState::Lose;
					}
					if ( strcmp("Camel::Dar::TMS::GameOverState::Fold", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::GameOverState::Fold;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_TMS_GameOverState_Type__MessageId_1700531115__
			}
		}
		namespace TMS
		{
			namespace LeaveReason
			{

				#if !defined __DarEnum_TMS_LeaveReason_Type__MessageId_3247604440__
				#define __DarEnum_TMS_LeaveReason_Type__MessageId_3247604440__
				enum Type
				{
					Eliminated = 0,
					ExitRoom = 1,
					EndState = 2,
					MergeRoom = 3,
					UserExitRoom = 4,
					Disconnect = 5,
					AutoDie = 6,
				};

				// Camel::Dar::TMS::LeaveReason::Type enum => string
				inline const char* EnumToString(Camel::Dar::TMS::LeaveReason::Type eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar::TMS::LeaveReason::Eliminated == eEnumValue)
					{
						return "Camel::Dar::TMS::LeaveReason::Eliminated";
					}
					if ( Camel::Dar::TMS::LeaveReason::ExitRoom == eEnumValue)
					{
						return "Camel::Dar::TMS::LeaveReason::ExitRoom";
					}
					if ( Camel::Dar::TMS::LeaveReason::EndState == eEnumValue)
					{
						return "Camel::Dar::TMS::LeaveReason::EndState";
					}
					if ( Camel::Dar::TMS::LeaveReason::MergeRoom == eEnumValue)
					{
						return "Camel::Dar::TMS::LeaveReason::MergeRoom";
					}
					if ( Camel::Dar::TMS::LeaveReason::UserExitRoom == eEnumValue)
					{
						return "Camel::Dar::TMS::LeaveReason::UserExitRoom";
					}
					if ( Camel::Dar::TMS::LeaveReason::Disconnect == eEnumValue)
					{
						return "Camel::Dar::TMS::LeaveReason::Disconnect";
					}
					if ( Camel::Dar::TMS::LeaveReason::AutoDie == eEnumValue)
					{
						return "Camel::Dar::TMS::LeaveReason::AutoDie";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar::TMS::LeaveReason::Type string => enum
				inline Camel::Dar::TMS::LeaveReason::Type StringToEnum(const char* szStringValue, Camel::Dar::TMS::LeaveReason::Type eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar::TMS::LeaveReason::Eliminated", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::LeaveReason::Eliminated;
					}
					if ( strcmp("Camel::Dar::TMS::LeaveReason::ExitRoom", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::LeaveReason::ExitRoom;
					}
					if ( strcmp("Camel::Dar::TMS::LeaveReason::EndState", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::LeaveReason::EndState;
					}
					if ( strcmp("Camel::Dar::TMS::LeaveReason::MergeRoom", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::LeaveReason::MergeRoom;
					}
					if ( strcmp("Camel::Dar::TMS::LeaveReason::UserExitRoom", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::LeaveReason::UserExitRoom;
					}
					if ( strcmp("Camel::Dar::TMS::LeaveReason::Disconnect", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::LeaveReason::Disconnect;
					}
					if ( strcmp("Camel::Dar::TMS::LeaveReason::AutoDie", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::LeaveReason::AutoDie;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_TMS_LeaveReason_Type__MessageId_3247604440__
			}
		}
		namespace TMS
		{
			namespace State
			{

				#if !defined __DarEnum_TMS_State_Type__MessageId_193348664__
				#define __DarEnum_TMS_State_Type__MessageId_193348664__
				enum Type
				{
					None = 0,
					Enterable = 1,
					EnterRoom = 2,
					Pre = 3,
					EnterableEnd = 4,
					PreEnd = 5,
					SemiFinal = 6,
					SemiFinalEnd = 7,
					Final = 8,
					FinalEnd = 9,
					GameEnd = 10,
				};

				// Camel::Dar::TMS::State::Type enum => string
				inline const char* EnumToString(Camel::Dar::TMS::State::Type eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar::TMS::State::None == eEnumValue)
					{
						return "Camel::Dar::TMS::State::None";
					}
					if ( Camel::Dar::TMS::State::Enterable == eEnumValue)
					{
						return "Camel::Dar::TMS::State::Enterable";
					}
					if ( Camel::Dar::TMS::State::EnterRoom == eEnumValue)
					{
						return "Camel::Dar::TMS::State::EnterRoom";
					}
					if ( Camel::Dar::TMS::State::Pre == eEnumValue)
					{
						return "Camel::Dar::TMS::State::Pre";
					}
					if ( Camel::Dar::TMS::State::EnterableEnd == eEnumValue)
					{
						return "Camel::Dar::TMS::State::EnterableEnd";
					}
					if ( Camel::Dar::TMS::State::PreEnd == eEnumValue)
					{
						return "Camel::Dar::TMS::State::PreEnd";
					}
					if ( Camel::Dar::TMS::State::SemiFinal == eEnumValue)
					{
						return "Camel::Dar::TMS::State::SemiFinal";
					}
					if ( Camel::Dar::TMS::State::SemiFinalEnd == eEnumValue)
					{
						return "Camel::Dar::TMS::State::SemiFinalEnd";
					}
					if ( Camel::Dar::TMS::State::Final == eEnumValue)
					{
						return "Camel::Dar::TMS::State::Final";
					}
					if ( Camel::Dar::TMS::State::FinalEnd == eEnumValue)
					{
						return "Camel::Dar::TMS::State::FinalEnd";
					}
					if ( Camel::Dar::TMS::State::GameEnd == eEnumValue)
					{
						return "Camel::Dar::TMS::State::GameEnd";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar::TMS::State::Type string => enum
				inline Camel::Dar::TMS::State::Type StringToEnum(const char* szStringValue, Camel::Dar::TMS::State::Type eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar::TMS::State::None", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::State::None;
					}
					if ( strcmp("Camel::Dar::TMS::State::Enterable", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::State::Enterable;
					}
					if ( strcmp("Camel::Dar::TMS::State::EnterRoom", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::State::EnterRoom;
					}
					if ( strcmp("Camel::Dar::TMS::State::Pre", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::State::Pre;
					}
					if ( strcmp("Camel::Dar::TMS::State::EnterableEnd", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::State::EnterableEnd;
					}
					if ( strcmp("Camel::Dar::TMS::State::PreEnd", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::State::PreEnd;
					}
					if ( strcmp("Camel::Dar::TMS::State::SemiFinal", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::State::SemiFinal;
					}
					if ( strcmp("Camel::Dar::TMS::State::SemiFinalEnd", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::State::SemiFinalEnd;
					}
					if ( strcmp("Camel::Dar::TMS::State::Final", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::State::Final;
					}
					if ( strcmp("Camel::Dar::TMS::State::FinalEnd", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::State::FinalEnd;
					}
					if ( strcmp("Camel::Dar::TMS::State::GameEnd", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::State::GameEnd;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_TMS_State_Type__MessageId_193348664__
			}
		}
		namespace TMS
		{
			namespace UserPosition
			{

				#if !defined __DarEnum_TMS_UserPosition_Type__MessageId_48176342__
				#define __DarEnum_TMS_UserPosition_Type__MessageId_48176342__
				enum Type
				{
					None = 0,
					Lobby = 1,
					Room = 2,
					Eliminated = 3,
				};

				// Camel::Dar::TMS::UserPosition::Type enum => string
				inline const char* EnumToString(Camel::Dar::TMS::UserPosition::Type eEnumValue, const char* szDefaultReturnString)
				{
					if ( Camel::Dar::TMS::UserPosition::None == eEnumValue)
					{
						return "Camel::Dar::TMS::UserPosition::None";
					}
					if ( Camel::Dar::TMS::UserPosition::Lobby == eEnumValue)
					{
						return "Camel::Dar::TMS::UserPosition::Lobby";
					}
					if ( Camel::Dar::TMS::UserPosition::Room == eEnumValue)
					{
						return "Camel::Dar::TMS::UserPosition::Room";
					}
					if ( Camel::Dar::TMS::UserPosition::Eliminated == eEnumValue)
					{
						return "Camel::Dar::TMS::UserPosition::Eliminated";
					}
					return szDefaultReturnString;
				}

				// Camel::Dar::TMS::UserPosition::Type string => enum
				inline Camel::Dar::TMS::UserPosition::Type StringToEnum(const char* szStringValue, Camel::Dar::TMS::UserPosition::Type eDefaultReturnEnum)
				{
					if ( NULL == szStringValue)
					{
						return eDefaultReturnEnum;
					}
					if ( strcmp("Camel::Dar::TMS::UserPosition::None", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::UserPosition::None;
					}
					if ( strcmp("Camel::Dar::TMS::UserPosition::Lobby", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::UserPosition::Lobby;
					}
					if ( strcmp("Camel::Dar::TMS::UserPosition::Room", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::UserPosition::Room;
					}
					if ( strcmp("Camel::Dar::TMS::UserPosition::Eliminated", szStringValue) == 0) 
					{
						return Camel::Dar::TMS::UserPosition::Eliminated;
					}
					return eDefaultReturnEnum;
				}
				#endif     // !defined __DarEnum_TMS_UserPosition_Type__MessageId_48176342__
			}
		}

		#if !defined __DarMessageProtocol_TMS_Header__MessageId_4003000291__
		#define __DarMessageProtocol_TMS_Header__MessageId_4003000291__
		namespace TMS
		{
			class Header : public IDarMsg
			{
			public:
				static const unsigned int  msgid=4003000291U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "TMS::Header";	};

				// member valiable declare
				unsigned int nSeqKey;

				Header()
				{
					Clear();
				}

				virtual ~Header()
				{
					Clear();
				}

				virtual void Clear()
				{
					// Clear Member nSeqKey
					::memset(&nSeqKey, 0, sizeof(nSeqKey));
				}

				bool Copy(const Header& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Clear();
					// copy Member nSeqKey
					nSeqKey = rhs.nSeqKey;
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
					// GetLength Member nSeqKey
					nLength += sizeof(nSeqKey);

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
					// Save Member nSeqKey
					if (false ==  DARRule_STREAM::Save<unsigned int>( stream, nSeqKey))		{    return false;     }

					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					// Load Member nSeqKey
					if (false ==  DARRule_STREAM::Load<unsigned int>( clMessageBlock, nSeqKey, bRequireChangeByteOrder))		{    return false;     }
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
					// Save Member nSeqKey
					Json_root["nSeqKey"] = nSeqKey;
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
					Clear();
					// Load Member nSeqKey
					if (! Json_root["nSeqKey"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nSeqKey"], nSeqKey))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};

		};
		#endif     // !defined __DarMessageProtocol_TMS_Header__MessageId_4003000291__

		#if !defined __DarMessageProtocol_TMS_RoomInfo__MessageId_1351664208__
		#define __DarMessageProtocol_TMS_RoomInfo__MessageId_1351664208__
		namespace TMS
		{
			class RoomInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1351664208U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "TMS::RoomInfo";	};

				// member valiable declare
				int nGameCode;
				int nServerCode;
				int nRoomNum;
				__int64 nRoomKey;
				std::string strRoomCreator;
				std::string strTitle;

				RoomInfo()
				{
					Clear();
				}

				virtual ~RoomInfo()
				{
					Clear();
				}

				virtual void Clear()
				{
					// Clear Member nGameCode
					::memset(&nGameCode, 0, sizeof(nGameCode));
					// Clear Member nServerCode
					::memset(&nServerCode, 0, sizeof(nServerCode));
					// Clear Member nRoomNum
					::memset(&nRoomNum, 0, sizeof(nRoomNum));
					// Clear Member nRoomKey
					::memset(&nRoomKey, 0, sizeof(nRoomKey));
					// Clear Member strRoomCreator
					strRoomCreator = "";
					// Clear Member strTitle
					strTitle = "";
				}

				bool Copy(const RoomInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Clear();
					// copy Member nGameCode
					nGameCode = rhs.nGameCode;
					// copy Member nServerCode
					nServerCode = rhs.nServerCode;
					// copy Member nRoomNum
					nRoomNum = rhs.nRoomNum;
					// copy Member nRoomKey
					nRoomKey = rhs.nRoomKey;
					// copy Member strRoomCreator
					strRoomCreator = rhs.strRoomCreator;
					// copy Member strTitle
					strTitle = rhs.strTitle;
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
					// GetLength Member nGameCode
					nLength += sizeof(nGameCode);
					// GetLength Member nServerCode
					nLength += sizeof(nServerCode);
					// GetLength Member nRoomNum
					nLength += sizeof(nRoomNum);
					// GetLength Member nRoomKey
					nLength += sizeof(nRoomKey);
					// GetLength Member strRoomCreator
					nLength += sizeof(int);
					nLength += strRoomCreator.length();
					// GetLength Member strTitle
					nLength += sizeof(int);
					nLength += strTitle.length();

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
					// Save Member nGameCode
					if (false ==  DARRule_STREAM::Save<int>( stream, nGameCode))		{    return false;     }
					// Save Member nServerCode
					if (false ==  DARRule_STREAM::Save<int>( stream, nServerCode))		{    return false;     }
					// Save Member nRoomNum
					if (false ==  DARRule_STREAM::Save<int>( stream, nRoomNum))		{    return false;     }
					// Save Member nRoomKey
					if (false ==  DARRule_STREAM::Save<__int64>( stream, nRoomKey))		{    return false;     }
					// Save Member strRoomCreator
					if (false ==  DARRule_STREAM::Save( stream, strRoomCreator))		{    return false;     }
					// Save Member strTitle
					if (false ==  DARRule_STREAM::Save( stream, strTitle))		{    return false;     }

					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					// Load Member nGameCode
					if (false ==  DARRule_STREAM::Load<int>( clMessageBlock, nGameCode, bRequireChangeByteOrder))		{    return false;     }
					// Load Member nServerCode
					if (false ==  DARRule_STREAM::Load<int>( clMessageBlock, nServerCode, bRequireChangeByteOrder))		{    return false;     }
					// Load Member nRoomNum
					if (false ==  DARRule_STREAM::Load<int>( clMessageBlock, nRoomNum, bRequireChangeByteOrder))		{    return false;     }
					// Load Member nRoomKey
					if (false ==  DARRule_STREAM::Load<__int64>( clMessageBlock, nRoomKey, bRequireChangeByteOrder))		{    return false;     }
					// Load Member strRoomCreator
					if (false == DARRule_STREAM::Load( clMessageBlock, strRoomCreator, bRequireChangeByteOrder))		{    return false;     }
					// Load Member strTitle
					if (false == DARRule_STREAM::Load( clMessageBlock, strTitle, bRequireChangeByteOrder))		{    return false;     }
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
					// Save Member nGameCode
					Json_root["nGameCode"] = nGameCode;
					// Save Member nServerCode
					Json_root["nServerCode"] = nServerCode;
					// Save Member nRoomNum
					Json_root["nRoomNum"] = nRoomNum;
					// Save Member nRoomKey
					Json_root["nRoomKey"] = nRoomKey;
					// Save Member strRoomCreator
					Json_root["strRoomCreator"] = strRoomCreator;
					// Save Member strTitle
					Json_root["strTitle"] = strTitle;
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
					Clear();
					// Load Member nGameCode
					if (! Json_root["nGameCode"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nGameCode"], nGameCode))	{	return false;	}
					}
					// Load Member nServerCode
					if (! Json_root["nServerCode"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nServerCode"], nServerCode))	{	return false;	}
					}
					// Load Member nRoomNum
					if (! Json_root["nRoomNum"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nRoomNum"], nRoomNum))	{	return false;	}
					}
					// Load Member nRoomKey
					if (! Json_root["nRoomKey"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nRoomKey"], nRoomKey))	{	return false;	}
					}
					// Load Member strRoomCreator
					if (! Json_root["strRoomCreator"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["strRoomCreator"], strRoomCreator))	{	return false;	}
					}
					// Load Member strTitle
					if (! Json_root["strTitle"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["strTitle"], strTitle))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};

		};
		#endif     // !defined __DarMessageProtocol_TMS_RoomInfo__MessageId_1351664208__

		#if !defined __DarMessageProtocol_TMS_UserInfo__MessageId_2961065544__
		#define __DarMessageProtocol_TMS_UserInfo__MessageId_2961065544__
		namespace TMS
		{
			class UserInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=2961065544U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "TMS::UserInfo";	};

				// member valiable declare
				std::string strId;
				std::string strNickname;
				__int64 llChips;
				Camel::Dar::TMS::UserPosition::Type ePos;
				int nGameCount;
				int nRank;

				UserInfo()
				{
					Clear();
				}

				virtual ~UserInfo()
				{
					Clear();
				}

				virtual void Clear()
				{
					// Clear Member strId
					strId = "";
					// Clear Member strNickname
					strNickname = "";
					// Clear Member llChips
					::memset(&llChips, 0, sizeof(llChips));
					// Clear Member ePos
					::memset(&ePos, 0, sizeof(ePos));
					// Clear Member nGameCount
					::memset(&nGameCount, 0, sizeof(nGameCount));
					// Clear Member nRank
					::memset(&nRank, 0, sizeof(nRank));
				}

				bool Copy(const UserInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Clear();
					// copy Member strId
					strId = rhs.strId;
					// copy Member strNickname
					strNickname = rhs.strNickname;
					// copy Member llChips
					llChips = rhs.llChips;
					// copy Member ePos
					ePos = rhs.ePos;
					// copy Member nGameCount
					nGameCount = rhs.nGameCount;
					// copy Member nRank
					nRank = rhs.nRank;
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
					// GetLength Member strId
					nLength += sizeof(int);
					nLength += strId.length();
					// GetLength Member strNickname
					nLength += sizeof(int);
					nLength += strNickname.length();
					// GetLength Member llChips
					nLength += sizeof(llChips);
					// GetLength Member ePos
					nLength += sizeof(ePos);
					// GetLength Member nGameCount
					nLength += sizeof(nGameCount);
					// GetLength Member nRank
					nLength += sizeof(nRank);

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
					// Save Member strId
					if (false ==  DARRule_STREAM::Save( stream, strId))		{    return false;     }
					// Save Member strNickname
					if (false ==  DARRule_STREAM::Save( stream, strNickname))		{    return false;     }
					// Save Member llChips
					if (false ==  DARRule_STREAM::Save<__int64>( stream, llChips))		{    return false;     }
					// Save Member ePos
					if (false ==  DARRule_STREAM::Save<Camel::Dar::TMS::UserPosition::Type>( stream, ePos))		{    return false;     }
					// Save Member nGameCount
					if (false ==  DARRule_STREAM::Save<int>( stream, nGameCount))		{    return false;     }
					// Save Member nRank
					if (false ==  DARRule_STREAM::Save<int>( stream, nRank))		{    return false;     }

					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					// Load Member strId
					if (false == DARRule_STREAM::Load( clMessageBlock, strId, bRequireChangeByteOrder))		{    return false;     }
					// Load Member strNickname
					if (false == DARRule_STREAM::Load( clMessageBlock, strNickname, bRequireChangeByteOrder))		{    return false;     }
					// Load Member llChips
					if (false ==  DARRule_STREAM::Load<__int64>( clMessageBlock, llChips, bRequireChangeByteOrder))		{    return false;     }
					// Load Member ePos
					if (false ==  DARRule_STREAM::Load<Camel::Dar::TMS::UserPosition::Type>( clMessageBlock, ePos, bRequireChangeByteOrder))		{    return false;     }
					// Load Member nGameCount
					if (false ==  DARRule_STREAM::Load<int>( clMessageBlock, nGameCount, bRequireChangeByteOrder))		{    return false;     }
					// Load Member nRank
					if (false ==  DARRule_STREAM::Load<int>( clMessageBlock, nRank, bRequireChangeByteOrder))		{    return false;     }
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
					// Save Member strId
					Json_root["strId"] = strId;
					// Save Member strNickname
					Json_root["strNickname"] = strNickname;
					// Save Member llChips
					Json_root["llChips"] = llChips;
					// Save Member ePos
					Json_root["ePos"] = ePos;
					// Save Member nGameCount
					Json_root["nGameCount"] = nGameCount;
					// Save Member nRank
					Json_root["nRank"] = nRank;
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
					Clear();
					// Load Member strId
					if (! Json_root["strId"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["strId"], strId))	{	return false;	}
					}
					// Load Member strNickname
					if (! Json_root["strNickname"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["strNickname"], strNickname))	{	return false;	}
					}
					// Load Member llChips
					if (! Json_root["llChips"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["llChips"], llChips))	{	return false;	}
					}
					// Load Member ePos
					if (! Json_root["ePos"].isNull())
					{
						int temp;
						if (false == DARRule_JSON::Load(Json_root["ePos"], temp))	{	return false;	}
						ePos = static_cast<Camel::Dar::TMS::UserPosition::Type>(temp);
					}
					// Load Member nGameCount
					if (! Json_root["nGameCount"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nGameCount"], nGameCount))	{	return false;	}
					}
					// Load Member nRank
					if (! Json_root["nRank"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nRank"], nRank))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};

		};
		#endif     // !defined __DarMessageProtocol_TMS_UserInfo__MessageId_2961065544__

		#if !defined __DarMessageProtocol_TMS_UserRat__MessageId_3502944066__
		#define __DarMessageProtocol_TMS_UserRat__MessageId_3502944066__
		namespace TMS
		{
			class UserRat : public IDarMsg
			{
			public:
				static const unsigned int  msgid=3502944066U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "TMS::UserRat";	};

				// member valiable declare
				std::string strId;
				__int64 llTotalPrizeChips;
				int nFinalWinCount;
				int nEntryWinCount;
				int nFinalEntryCount;
				int nEntryCount;
				bool bReserved;

				UserRat()
				{
					Clear();
				}

				virtual ~UserRat()
				{
					Clear();
				}

				virtual void Clear()
				{
					// Clear Member strId
					strId = "";
					// Clear Member llTotalPrizeChips
					::memset(&llTotalPrizeChips, 0, sizeof(llTotalPrizeChips));
					// Clear Member nFinalWinCount
					::memset(&nFinalWinCount, 0, sizeof(nFinalWinCount));
					// Clear Member nEntryWinCount
					::memset(&nEntryWinCount, 0, sizeof(nEntryWinCount));
					// Clear Member nFinalEntryCount
					::memset(&nFinalEntryCount, 0, sizeof(nFinalEntryCount));
					// Clear Member nEntryCount
					::memset(&nEntryCount, 0, sizeof(nEntryCount));
					// Clear Member bReserved
					::memset(&bReserved, 0, sizeof(bReserved));
				}

				bool Copy(const UserRat& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Clear();
					// copy Member strId
					strId = rhs.strId;
					// copy Member llTotalPrizeChips
					llTotalPrizeChips = rhs.llTotalPrizeChips;
					// copy Member nFinalWinCount
					nFinalWinCount = rhs.nFinalWinCount;
					// copy Member nEntryWinCount
					nEntryWinCount = rhs.nEntryWinCount;
					// copy Member nFinalEntryCount
					nFinalEntryCount = rhs.nFinalEntryCount;
					// copy Member nEntryCount
					nEntryCount = rhs.nEntryCount;
					// copy Member bReserved
					bReserved = rhs.bReserved;
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
					// GetLength Member strId
					nLength += sizeof(int);
					nLength += strId.length();
					// GetLength Member llTotalPrizeChips
					nLength += sizeof(llTotalPrizeChips);
					// GetLength Member nFinalWinCount
					nLength += sizeof(nFinalWinCount);
					// GetLength Member nEntryWinCount
					nLength += sizeof(nEntryWinCount);
					// GetLength Member nFinalEntryCount
					nLength += sizeof(nFinalEntryCount);
					// GetLength Member nEntryCount
					nLength += sizeof(nEntryCount);
					// GetLength Member bReserved
					nLength += sizeof(bReserved);

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
					// Save Member strId
					if (false ==  DARRule_STREAM::Save( stream, strId))		{    return false;     }
					// Save Member llTotalPrizeChips
					if (false ==  DARRule_STREAM::Save<__int64>( stream, llTotalPrizeChips))		{    return false;     }
					// Save Member nFinalWinCount
					if (false ==  DARRule_STREAM::Save<int>( stream, nFinalWinCount))		{    return false;     }
					// Save Member nEntryWinCount
					if (false ==  DARRule_STREAM::Save<int>( stream, nEntryWinCount))		{    return false;     }
					// Save Member nFinalEntryCount
					if (false ==  DARRule_STREAM::Save<int>( stream, nFinalEntryCount))		{    return false;     }
					// Save Member nEntryCount
					if (false ==  DARRule_STREAM::Save<int>( stream, nEntryCount))		{    return false;     }
					// Save Member bReserved
					if (false ==  DARRule_STREAM::Save<bool>( stream, bReserved))		{    return false;     }

					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					// Load Member strId
					if (false == DARRule_STREAM::Load( clMessageBlock, strId, bRequireChangeByteOrder))		{    return false;     }
					// Load Member llTotalPrizeChips
					if (false ==  DARRule_STREAM::Load<__int64>( clMessageBlock, llTotalPrizeChips, bRequireChangeByteOrder))		{    return false;     }
					// Load Member nFinalWinCount
					if (false ==  DARRule_STREAM::Load<int>( clMessageBlock, nFinalWinCount, bRequireChangeByteOrder))		{    return false;     }
					// Load Member nEntryWinCount
					if (false ==  DARRule_STREAM::Load<int>( clMessageBlock, nEntryWinCount, bRequireChangeByteOrder))		{    return false;     }
					// Load Member nFinalEntryCount
					if (false ==  DARRule_STREAM::Load<int>( clMessageBlock, nFinalEntryCount, bRequireChangeByteOrder))		{    return false;     }
					// Load Member nEntryCount
					if (false ==  DARRule_STREAM::Load<int>( clMessageBlock, nEntryCount, bRequireChangeByteOrder))		{    return false;     }
					// Load Member bReserved
					if (false ==  DARRule_STREAM::Load<bool>( clMessageBlock, bReserved, bRequireChangeByteOrder))		{    return false;     }
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
					// Save Member strId
					Json_root["strId"] = strId;
					// Save Member llTotalPrizeChips
					Json_root["llTotalPrizeChips"] = llTotalPrizeChips;
					// Save Member nFinalWinCount
					Json_root["nFinalWinCount"] = nFinalWinCount;
					// Save Member nEntryWinCount
					Json_root["nEntryWinCount"] = nEntryWinCount;
					// Save Member nFinalEntryCount
					Json_root["nFinalEntryCount"] = nFinalEntryCount;
					// Save Member nEntryCount
					Json_root["nEntryCount"] = nEntryCount;
					// Save Member bReserved
					Json_root["bReserved"] = bReserved;
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
					Clear();
					// Load Member strId
					if (! Json_root["strId"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["strId"], strId))	{	return false;	}
					}
					// Load Member llTotalPrizeChips
					if (! Json_root["llTotalPrizeChips"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["llTotalPrizeChips"], llTotalPrizeChips))	{	return false;	}
					}
					// Load Member nFinalWinCount
					if (! Json_root["nFinalWinCount"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nFinalWinCount"], nFinalWinCount))	{	return false;	}
					}
					// Load Member nEntryWinCount
					if (! Json_root["nEntryWinCount"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nEntryWinCount"], nEntryWinCount))	{	return false;	}
					}
					// Load Member nFinalEntryCount
					if (! Json_root["nFinalEntryCount"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nFinalEntryCount"], nFinalEntryCount))	{	return false;	}
					}
					// Load Member nEntryCount
					if (! Json_root["nEntryCount"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nEntryCount"], nEntryCount))	{	return false;	}
					}
					// Load Member bReserved
					if (! Json_root["bReserved"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["bReserved"], bReserved))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};

		};
		#endif     // !defined __DarMessageProtocol_TMS_UserRat__MessageId_3502944066__

		#if !defined __DarMessageProtocol_TMS_TournamentInfo__MessageId_1156134904__
		#define __DarMessageProtocol_TMS_TournamentInfo__MessageId_1156134904__
		namespace TMS
		{
			class TournamentInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1156134904U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "TMS::TournamentInfo";	};

				// member valiable declare
				int nInning;
				Camel::Dar::TMS::State::Type eState;
				int nEntryCount;
				int nAliveCount;
				__int64 tStartTime;
				__int64 tPlayTime;

				TournamentInfo()
				{
					Clear();
				}

				virtual ~TournamentInfo()
				{
					Clear();
				}

				virtual void Clear()
				{
					// Clear Member nInning
					::memset(&nInning, 0, sizeof(nInning));
					// Clear Member eState
					::memset(&eState, 0, sizeof(eState));
					// Clear Member nEntryCount
					::memset(&nEntryCount, 0, sizeof(nEntryCount));
					// Clear Member nAliveCount
					::memset(&nAliveCount, 0, sizeof(nAliveCount));
					// Clear Member tStartTime
					::memset(&tStartTime, 0, sizeof(tStartTime));
					// Clear Member tPlayTime
					::memset(&tPlayTime, 0, sizeof(tPlayTime));
				}

				bool Copy(const TournamentInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Clear();
					// copy Member nInning
					nInning = rhs.nInning;
					// copy Member eState
					eState = rhs.eState;
					// copy Member nEntryCount
					nEntryCount = rhs.nEntryCount;
					// copy Member nAliveCount
					nAliveCount = rhs.nAliveCount;
					// copy Member tStartTime
					tStartTime = rhs.tStartTime;
					// copy Member tPlayTime
					tPlayTime = rhs.tPlayTime;
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
					// GetLength Member nInning
					nLength += sizeof(nInning);
					// GetLength Member eState
					nLength += sizeof(eState);
					// GetLength Member nEntryCount
					nLength += sizeof(nEntryCount);
					// GetLength Member nAliveCount
					nLength += sizeof(nAliveCount);
					// GetLength Member tStartTime
					nLength += sizeof(tStartTime);
					// GetLength Member tPlayTime
					nLength += sizeof(tPlayTime);

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
					// Save Member nInning
					if (false ==  DARRule_STREAM::Save<int>( stream, nInning))		{    return false;     }
					// Save Member eState
					if (false ==  DARRule_STREAM::Save<Camel::Dar::TMS::State::Type>( stream, eState))		{    return false;     }
					// Save Member nEntryCount
					if (false ==  DARRule_STREAM::Save<int>( stream, nEntryCount))		{    return false;     }
					// Save Member nAliveCount
					if (false ==  DARRule_STREAM::Save<int>( stream, nAliveCount))		{    return false;     }
					// Save Member tStartTime
					if (false ==  DARRule_STREAM::Save<__int64>( stream, tStartTime))		{    return false;     }
					// Save Member tPlayTime
					if (false ==  DARRule_STREAM::Save<__int64>( stream, tPlayTime))		{    return false;     }

					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					// Load Member nInning
					if (false ==  DARRule_STREAM::Load<int>( clMessageBlock, nInning, bRequireChangeByteOrder))		{    return false;     }
					// Load Member eState
					if (false ==  DARRule_STREAM::Load<Camel::Dar::TMS::State::Type>( clMessageBlock, eState, bRequireChangeByteOrder))		{    return false;     }
					// Load Member nEntryCount
					if (false ==  DARRule_STREAM::Load<int>( clMessageBlock, nEntryCount, bRequireChangeByteOrder))		{    return false;     }
					// Load Member nAliveCount
					if (false ==  DARRule_STREAM::Load<int>( clMessageBlock, nAliveCount, bRequireChangeByteOrder))		{    return false;     }
					// Load Member tStartTime
					if (false ==  DARRule_STREAM::Load<__int64>( clMessageBlock, tStartTime, bRequireChangeByteOrder))		{    return false;     }
					// Load Member tPlayTime
					if (false ==  DARRule_STREAM::Load<__int64>( clMessageBlock, tPlayTime, bRequireChangeByteOrder))		{    return false;     }
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
					// Save Member nInning
					Json_root["nInning"] = nInning;
					// Save Member eState
					Json_root["eState"] = eState;
					// Save Member nEntryCount
					Json_root["nEntryCount"] = nEntryCount;
					// Save Member nAliveCount
					Json_root["nAliveCount"] = nAliveCount;
					// Save Member tStartTime
					Json_root["tStartTime"] = tStartTime;
					// Save Member tPlayTime
					Json_root["tPlayTime"] = tPlayTime;
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
					Clear();
					// Load Member nInning
					if (! Json_root["nInning"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nInning"], nInning))	{	return false;	}
					}
					// Load Member eState
					if (! Json_root["eState"].isNull())
					{
						int temp;
						if (false == DARRule_JSON::Load(Json_root["eState"], temp))	{	return false;	}
						eState = static_cast<Camel::Dar::TMS::State::Type>(temp);
					}
					// Load Member nEntryCount
					if (! Json_root["nEntryCount"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nEntryCount"], nEntryCount))	{	return false;	}
					}
					// Load Member nAliveCount
					if (! Json_root["nAliveCount"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nAliveCount"], nAliveCount))	{	return false;	}
					}
					// Load Member tStartTime
					if (! Json_root["tStartTime"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["tStartTime"], tStartTime))	{	return false;	}
					}
					// Load Member tPlayTime
					if (! Json_root["tPlayTime"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["tPlayTime"], tPlayTime))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};

		};
		#endif     // !defined __DarMessageProtocol_TMS_TournamentInfo__MessageId_1156134904__

		#if !defined __DarMessageProtocol_TMS_ResultInfo__MessageId_1544121271__
		#define __DarMessageProtocol_TMS_ResultInfo__MessageId_1544121271__
		namespace TMS
		{
			class ResultInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=1544121271U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "TMS::ResultInfo";	};

				// member valiable declare
				int nRank;
				std::string strNickname;
				__int64 llPrizeMoney;

				ResultInfo()
				{
					Clear();
				}

				virtual ~ResultInfo()
				{
					Clear();
				}

				virtual void Clear()
				{
					// Clear Member nRank
					::memset(&nRank, 0, sizeof(nRank));
					// Clear Member strNickname
					strNickname = "";
					// Clear Member llPrizeMoney
					::memset(&llPrizeMoney, 0, sizeof(llPrizeMoney));
				}

				bool Copy(const ResultInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Clear();
					// copy Member nRank
					nRank = rhs.nRank;
					// copy Member strNickname
					strNickname = rhs.strNickname;
					// copy Member llPrizeMoney
					llPrizeMoney = rhs.llPrizeMoney;
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
					// GetLength Member nRank
					nLength += sizeof(nRank);
					// GetLength Member strNickname
					nLength += sizeof(int);
					nLength += strNickname.length();
					// GetLength Member llPrizeMoney
					nLength += sizeof(llPrizeMoney);

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
					// Save Member nRank
					if (false ==  DARRule_STREAM::Save<int>( stream, nRank))		{    return false;     }
					// Save Member strNickname
					if (false ==  DARRule_STREAM::Save( stream, strNickname))		{    return false;     }
					// Save Member llPrizeMoney
					if (false ==  DARRule_STREAM::Save<__int64>( stream, llPrizeMoney))		{    return false;     }

					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					// Load Member nRank
					if (false ==  DARRule_STREAM::Load<int>( clMessageBlock, nRank, bRequireChangeByteOrder))		{    return false;     }
					// Load Member strNickname
					if (false == DARRule_STREAM::Load( clMessageBlock, strNickname, bRequireChangeByteOrder))		{    return false;     }
					// Load Member llPrizeMoney
					if (false ==  DARRule_STREAM::Load<__int64>( clMessageBlock, llPrizeMoney, bRequireChangeByteOrder))		{    return false;     }
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
					// Save Member nRank
					Json_root["nRank"] = nRank;
					// Save Member strNickname
					Json_root["strNickname"] = strNickname;
					// Save Member llPrizeMoney
					Json_root["llPrizeMoney"] = llPrizeMoney;
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
					Clear();
					// Load Member nRank
					if (! Json_root["nRank"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["nRank"], nRank))	{	return false;	}
					}
					// Load Member strNickname
					if (! Json_root["strNickname"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["strNickname"], strNickname))	{	return false;	}
					}
					// Load Member llPrizeMoney
					if (! Json_root["llPrizeMoney"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["llPrizeMoney"], llPrizeMoney))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};

		};
		#endif     // !defined __DarMessageProtocol_TMS_ResultInfo__MessageId_1544121271__

		#if !defined __DarMessageProtocol_TMS_SetMoneyInfo__MessageId_4024226605__
		#define __DarMessageProtocol_TMS_SetMoneyInfo__MessageId_4024226605__
		namespace TMS
		{
			class SetMoneyInfo : public IDarMsg
			{
			public:
				static const unsigned int  msgid=4024226605U;

				virtual unsigned int GetMessageId()		{	return msgid;	};
				virtual  const char* GetMessageName()	{	return "TMS::SetMoneyInfo";	};

				// member valiable declare
				std::string strId;
				__int64 llPrizeMoney;

				SetMoneyInfo()
				{
					Clear();
				}

				virtual ~SetMoneyInfo()
				{
					Clear();
				}

				virtual void Clear()
				{
					// Clear Member strId
					strId = "";
					// Clear Member llPrizeMoney
					::memset(&llPrizeMoney, 0, sizeof(llPrizeMoney));
				}

				bool Copy(const SetMoneyInfo& rhs)
				{
					if (this == &rhs)
					{
						return false;
					}
					Clear();
					// copy Member strId
					strId = rhs.strId;
					// copy Member llPrizeMoney
					llPrizeMoney = rhs.llPrizeMoney;
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
					// GetLength Member strId
					nLength += sizeof(int);
					nLength += strId.length();
					// GetLength Member llPrizeMoney
					nLength += sizeof(llPrizeMoney);

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
					// Save Member strId
					if (false ==  DARRule_STREAM::Save( stream, strId))		{    return false;     }
					// Save Member llPrizeMoney
					if (false ==  DARRule_STREAM::Save<__int64>( stream, llPrizeMoney))		{    return false;     }

					return true;
				}

				bool _LoadSTREAM( MessageBlock& clMessageBlock, bool bRequireChangeByteOrder = false)
				{
					// Load Member strId
					if (false == DARRule_STREAM::Load( clMessageBlock, strId, bRequireChangeByteOrder))		{    return false;     }
					// Load Member llPrizeMoney
					if (false ==  DARRule_STREAM::Load<__int64>( clMessageBlock, llPrizeMoney, bRequireChangeByteOrder))		{    return false;     }
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
					// Save Member strId
					Json_root["strId"] = strId;
					// Save Member llPrizeMoney
					Json_root["llPrizeMoney"] = llPrizeMoney;
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
					Clear();
					// Load Member strId
					if (! Json_root["strId"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["strId"], strId))	{	return false;	}
					}
					// Load Member llPrizeMoney
					if (! Json_root["llPrizeMoney"].isNull())
					{
						if (false == DARRule_JSON::Load(Json_root["llPrizeMoney"], llPrizeMoney))	{	return false;	}
					}
					return true;
				}

				#endif // defined(__JSON_LIB_SUPPORT__)
			};

		};
		#endif     // !defined __DarMessageProtocol_TMS_SetMoneyInfo__MessageId_4024226605__
	}
}
