// // Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory/InventoryType.h"
#include "Inventory/Interface/Interactable.h"
#include "PickupBase.generated.h"


class UInventoryComponent;
enum class EInteractionType : uint8;

UCLASS()
class MIAN_API APickupBase : public AActor,public IInteractable
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Pickup")
	UStaticMeshComponent*ItemMeshComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Pickup")
	UInventoryComponent*InventoryComponent;

	//这个道具有没有容器
	UPROPERTY(BlueprintReadWrite,Category="Pickup",EditInstanceOnly,meta=(ExposeOnSpawn))
	TArray<FContainerInfo> SpawnContainers;

	//道具的数据结构
	UPROPERTY(Replicated,BlueprintReadWrite,Category="Pickup",EditInstanceOnly,meta=(ExposeOnSpawn))
	FItemInfoDef ItemInfo;

	//这个道具要有一些什么道具(比如这个道具是个仓库)
	UPROPERTY(Replicated,BlueprintReadWrite,Category="Pickup",EditInstanceOnly,meta=(ExposeOnSpawn))
	TArray<FSubItemInfo> SubItems;

	
public:
	// Sets default values for this actor's properties
	APickupBase();
	
	//~IInterface interface
	virtual void OnInteract_Implementation(AActor*InSourceInteractable,EInteractionType InInteractionType)override;
	virtual void OnTraceEnter_Implementation()override;
	virtual void OnTraceLeave_Implementation()override;
	//~End of IInterface interface

	/*清空容器中的数据*/
	UFUNCTION(BlueprintCallable,Category="Pickup")
	void ResetContainerInfo();

	/*减少数量*/
	UFUNCTION(BlueprintCallable,Category="Pickup")
	void ReduceAmount(const int32&InAmount);
	
	FORCEINLINE FItemInfoDef GetItemInfo(){return ItemInfo;};
	FORCEINLINE UInventoryComponent*GetInventoryComponent(){return InventoryComponent;};
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
