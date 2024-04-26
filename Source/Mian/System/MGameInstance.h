#pragma once

#include "CommonGameInstance.h"
#include "MGameInstance.generated.h"

class AMPlayerController;
class UObject;

UCLASS(Config = Game)
class MIAN_API UMGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()

public:

	UMGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	AMPlayerController* GetPrimaryPlayerController() const;
	
	virtual bool CanJoinRequestedSession() const override;
	virtual void HandlerUserInitialized(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext) override;

	virtual void ReceivedNetworkEncryptionToken(const FString& EncryptionToken, const FOnEncryptionKeyResponse& Delegate) override;
	virtual void ReceivedNetworkEncryptionAck(const FOnEncryptionKeyResponse& Delegate) override;

protected:

	virtual void Init() override;
	virtual void Shutdown() override;

	void OnPreClientTravelToSession(FString& URL);

	/** A hard-coded encryption key used to try out the encryption code. This is NOT SECURE, do not use this technique in production! */
	TArray<uint8> DebugTestEncryptionKey;
	
};
