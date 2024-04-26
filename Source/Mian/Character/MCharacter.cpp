#include "Character/MCharacter.h"
#include "MCharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MCharacter)

AMCharacter::AMCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UMCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
}
