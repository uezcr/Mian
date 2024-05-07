#include "MPlayerCharacter.h"
#include "Camera/MCameraComponent.h"

AMPlayerCharacter::AMPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CameraComponent = CreateDefaultSubobject<UMCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
}
