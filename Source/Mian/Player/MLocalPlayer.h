#pragma once

#include "CommonLocalPlayer.h"
#include "MLocalPlayer.generated.h"

class UMSettingsShared;

UCLASS()
class MIAN_API UMLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()

public:
	/** Gets the shared setting for this player, this is read using the save game system so may not be correct until after user login */
	UFUNCTION()
	UMSettingsShared* GetSharedSettings() const;

private:
	UPROPERTY(Transient)
	mutable TObjectPtr<UMSettingsShared> SharedSettings;
	
};
