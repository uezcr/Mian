#pragma once

#include "Logging/LogMacros.h"

class UObject;

MIAN_API DECLARE_LOG_CATEGORY_EXTERN(LogMian, Log, All);
MIAN_API DECLARE_LOG_CATEGORY_EXTERN(LogMianCombat, Log, All);
MIAN_API DECLARE_LOG_CATEGORY_EXTERN(LogMianExperience, Log, All);
MIAN_API DECLARE_LOG_CATEGORY_EXTERN(LogMianAbilitySystem, Log, All);
MIAN_API DECLARE_LOG_CATEGORY_EXTERN(LogMianTeams, Log, All);

MIAN_API FString GetClientServerContextString(UObject* ContextObject = nullptr);

DEFINE_LOG_CATEGORY_STATIC( MIANLogger, Display, All );
#define MIAN_Log( FormatString, ... )                                                                                                                           \
UE_LOG( MIANLogger, Display, TEXT( "[%s line:%d,thread:%d,this:%p]: %s" ), *FString( __PRETTY_FUNCTION__ ), __LINE__, this, FPlatformTLS::GetCurrentThreadId(), \
*FString::Printf( TEXT( FormatString ), ##__VA_ARGS__ ) )
#define MIAN_Warning( FormatString, ... )                                                                                                                           \
UE_LOG( MIANLogger, Warning, TEXT( "[%s line:%d,thread:%d,this:%p]: %s" ), *FString( __PRETTY_FUNCTION__ ), __LINE__, this, FPlatformTLS::GetCurrentThreadId(), \
*FString::Printf( TEXT( FormatString ), ##__VA_ARGS__ ) )
#define MIAN_Error( FormatString, ... )                                                                                                                           \
UE_LOG( MIANLogger, Error, TEXT( "[%s line:%d,thread:%d,this:%p]: %s" ), *FString( __PRETTY_FUNCTION__ ), __LINE__, this, FPlatformTLS::GetCurrentThreadId(), \
*FString::Printf( TEXT( FormatString ), ##__VA_ARGS__ ) )