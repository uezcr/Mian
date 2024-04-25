#pragma once

#include "ModularAIController.h"
#include "MBotController.generated.h"

UCLASS(Blueprintable)
class MIAN_API AMBotController : public AModularAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMBotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
