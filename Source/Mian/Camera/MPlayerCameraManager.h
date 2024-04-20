#pragma once

#include "Camera/PlayerCameraManager.h"

#include "MPlayerCameraManager.generated.h"

class FDebugDisplayInfo;
class UCanvas;
class UObject;


#define M_CAMERA_DEFAULT_FOV			(80.0f)
#define M_CAMERA_DEFAULT_PITCH_MIN	(-89.0f)
#define M_CAMERA_DEFAULT_PITCH_MAX	(89.0f)

class UMUICameraManagerComponent;

/**
 * AMPlayerCameraManager
 *
 *	The base player camera manager class used by this project.
 */
UCLASS(notplaceable, MinimalAPI)
class AMPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	AMPlayerCameraManager(const FObjectInitializer& ObjectInitializer);

	UMUICameraManagerComponent* GetUICameraComponent() const;

protected:

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos) override;

private:
	/** The UI Camera Component, controls the camera when UI is doing something important that gameplay doesn't get priority over. */
	UPROPERTY(Transient)
	TObjectPtr<UMUICameraManagerComponent> UICamera;
};
