#include "MBotCharacter.h"

#include "AI/MStateTreeComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MBotCharacter)

AMBotCharacter::AMBotCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	StateTree = CreateDefaultSubobject<UMStateTreeComponent>(TEXT("StateTree"));
}
