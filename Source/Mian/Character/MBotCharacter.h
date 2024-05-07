#pragma once

#include "MCharacter.h"
#include "MBotCharacter.generated.h"

class AMPatrolRoute;
class UMStateTreeComponent;
class AMAICoordinator;

UCLASS(Abstract)
class MIAN_API AMBotCharacter : public AMCharacter
{
	GENERATED_BODY()

public:
	AMBotCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	AMAICoordinator* GetAICoordinator() const {return AICoordinator;}

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mian|BotCharacter", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMStateTreeComponent> StateTree;
	UPROPERTY(EditAnywhere,Category = "AI")
	TObjectPtr<AMAICoordinator> AICoordinator;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<AMPatrolRoute> PatrolRoute;
};
