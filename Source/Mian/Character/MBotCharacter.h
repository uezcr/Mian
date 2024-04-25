#pragma once

#include "MCharacter.h"
#include "MBotCharacter.generated.h"

class UMStateTreeComponent;

UCLASS()
class MIAN_API AMBotCharacter : public AMCharacter
{
	GENERATED_BODY()

public:
	AMBotCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mian|BotCharacter", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMStateTreeComponent> StateTree;
	
};
