#pragma once

#include "MCharacter.h"
#include "MPlayerCharacter.generated.h"

UCLASS()
class MIAN_API AMPlayerCharacter : public AMCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
