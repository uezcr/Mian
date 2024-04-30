// // Copyright Epic Games, Inc. All Rights Reserved.


#include "PickupBase.h"

#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryLibrary.h"
#include "Inventory/DataAsset/ItemContainersData.h"
#include "Net/UnrealNetwork.h"

APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);
	
	ItemMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComp"));
	SetRootComponent(ItemMeshComp);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	
}

void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	FInventoryItem InventoryItem;
	bool bIsSuccess = UInventoryLibrary::GetDefaultIneventoryItemById(ItemInfo.ItemID,InventoryItem);
	if (bIsSuccess && InventoryItem.PickupMesh)
	{
		ItemMeshComp->SetStaticMesh(InventoryItem.PickupMesh);
	}
	
	if (!HasAuthority()) return;

	//没有要生成的容器
	if (SpawnContainers.IsEmpty())
	{
		//这个道具是不是自己有容器(比如背包 背心)
		if(!InventoryItem.OwnInventoryData) return;
		
		TArray<FContainerInfo> OutContainerInfo;
		TArray<FIntPoint> OutContainerPosition;
		InventoryItem.OwnInventoryData->GetContainersAndPosition(OutContainerInfo,OutContainerPosition);
		
		ItemInfo.OwnContainerUIDs = UInventoryLibrary::AddUniqueContainers(OutContainerInfo,InventoryComponent);
	}
	else
	{
		InventoryComponent->SetContainers(SpawnContainers);
	}

	for (FSubItemInfo&SubItemInfo:SubItems)
	{
		InventoryComponent->LoadSubItem(SubItemInfo);
	}
	
}

void APickupBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APickupBase,ItemInfo);
	DOREPLIFETIME(APickupBase,SubItems);
}

void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickupBase::OnInteract_Implementation(AActor* InSourceInteractable, EInteractionType InInteractionType)
{
	UInventoryComponent*SourceInventoryComponent=Cast<UInventoryComponent>(InSourceInteractable->GetComponentByClass(UInventoryComponent::StaticClass()));
	switch (InInteractionType)
	{
		case EInteractionType::E_Default:
			break;
		case EInteractionType::E_Pickup:
			if (SourceInventoryComponent)
			{
				SourceInventoryComponent->PickupItem(this);
			}
			break;
		case EInteractionType::E_CheckItem:
			break;
		case EInteractionType::E_GetInCar:
			break;
		case EInteractionType::E_ShowCarStats:
			break;
	}
}

void APickupBase::OnTraceEnter_Implementation()
{
	ItemMeshComp->SetRenderCustomDepth(true);
	UInventoryComponent*LocalPlayerInventoryComponent = UInventoryLibrary::GetLocalPlayerInventoryComponent(GetWorld());
	if(LocalPlayerInventoryComponent)
	{
		FInventoryItem InventoryItem;
		UInventoryLibrary::GetDefaultIneventoryItemById(ItemInfo.ItemID,InventoryItem);
		LocalPlayerInventoryComponent->AddDisplayTextToHUD(InventoryItem.Name,ItemInfo.ItemAmount);
	}
}

void APickupBase::OnTraceLeave_Implementation()
{
	ItemMeshComp->SetRenderCustomDepth(false);
	UInventoryComponent*LocalPlayerInventoryComponent = UInventoryLibrary::GetLocalPlayerInventoryComponent(GetWorld());
	if(LocalPlayerInventoryComponent)
	{
		LocalPlayerInventoryComponent->RemoveDisplayText();
	}
}

void APickupBase::ResetContainerInfo()
{
	if (!HasAuthority())return;
	
	InventoryComponent->ResetContainer();
	ItemInfo.OwnContainerUIDs.Empty();
}

void APickupBase::ReduceAmount(const int32& InAmount)
{
	ItemInfo.ItemAmount -= InAmount;
	if (ItemInfo.ItemAmount<=0)
	{
		FInventoryItem InventoryItem;
		UInventoryLibrary::GetDefaultIneventoryItemById(ItemInfo.ItemID,InventoryItem);
		if(InventoryItem.bDestoryOnPickup)
		{
			UInventoryComponent*LocalPlayerInventoryComponent = UInventoryLibrary::GetLocalPlayerInventoryComponent(GetWorld());
			if(LocalPlayerInventoryComponent)
			{
				LocalPlayerInventoryComponent->RemoveDisplayText();
			}
			
			Destroy();
		}
		else
		{
			ResetContainerInfo();
		}
	}
	
}



