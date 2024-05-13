#pragma once

#include "Templates/SubclassOf.h"
#include "EquipmentDefinition.generated.h"

class AActor;
class UMAbilitySet;
class UEquipmentInstance;

USTRUCT()
struct FEquipmentActorToSpawn
{
	GENERATED_BODY()

	FEquipmentActorToSpawn()
	{}

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};

UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class UEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<UEquipmentInstance> InstanceType;
	
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<TObjectPtr<const UMAbilitySet>> AbilitySetsToGrant;
	
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<FEquipmentActorToSpawn> ActorsToSpawn;
};
