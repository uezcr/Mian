#include "Character/MCharacter.h"
#include "Camera/MCameraComponent.h"

AMCharacter::AMCharacter(const FObjectInitializer& ObjectInitializer)
{
	CameraComponent = CreateDefaultSubobject<UMCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
}
