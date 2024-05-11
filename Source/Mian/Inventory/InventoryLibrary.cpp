// // Copyright Epic Games, Inc. All Rights Reserved.


#include "InventoryLibrary.h"
#include "DataRegistrySubsystem.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "InventoryComponent.h"
#include "InventoryType.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/MGameMode.h"
#include "Kismet/GameplayStatics.h"


FName UInventoryLibrary::GetItemNameByGameplayTag(const FGameplayTag& InGameplayTag)
{	
	FString NameString = InGameplayTag.ToString();
	int32 NameIndex =  NameString.Find(TEXT("."),ESearchCase::IgnoreCase,ESearchDir::FromEnd);
	NameString.RemoveAt(0,NameIndex+1);
	return FName(NameString);
}

const bool UInventoryLibrary::GetDefaultIneventoryItemById(const FName& InItemId,FInventoryItem&OutInventoryItem)
{
	UDataRegistrySubsystem * DataRegistrySubsystem = GEngine->GetEngineSubsystem<UDataRegistrySubsystem>();
	if (DataRegistrySubsystem)
	{
		OutInventoryItem = *DataRegistrySubsystem->GetCachedItem<FInventoryItem>(FDataRegistryId(TEXT("Items"),InItemId));
		return true;
	}
	return false;
}

TArray<int32> UInventoryLibrary::AddUniqueContainers(TArray<FContainerInfo>& InContainers,UInventoryComponent* InTargetInventoryComp)
{
	if(!InTargetInventoryComp || InContainers.IsEmpty())
	{
		UE_LOG(LogTemp,Warning,TEXT("lzy UInventoryLibrary::AddUniqueContainers 传参不对"));
		return TArray<int32>(); 
	}
	AMGameMode*GameMode = Cast<AMGameMode>(InTargetInventoryComp->GetWorld()->GetAuthGameMode());
	if(!GameMode)return TArray<int32>(); 

	return GameMode->AddUniqueContainers(InContainers,InTargetInventoryComp);
}

FIntPoint UInventoryLibrary::SelectSize(const FGameplayTag& InSlotTag, const FIntPoint& InSize)
{
	if (IsInventorySlot(InSlotTag))
	{
		return InSize;
	}
	return FIntPoint(1,1);
}

bool UInventoryLibrary::IsInventorySlot(const FGameplayTag& InSlotTag)
{
	return InSlotTag.MatchesTagExact(UGameplayTagsManager::Get().RequestGameplayTag(TEXT("Inventory.Slot.InventorySlot")));
}

bool UInventoryLibrary::IsSlotSupported(const FName& InItemId, const FGameplayTag& InSlotTag)
{
	FInventoryItem InventoryItem;
	bool bIsSuccess = GetDefaultIneventoryItemById(InItemId,InventoryItem);
	if(!bIsSuccess) return false;
	
	return InventoryItem.DroppableSlotTypes.HasTag(InSlotTag);
}

bool UInventoryLibrary::IsValidTile(const FIntPoint& InTile, const FIntPoint& InContainerSize)
{
	return InTile.X<InContainerSize.X && InTile.Y<InContainerSize.Y && InTile.X>=0 && InTile.Y>=0;
}

FIntPoint UInventoryLibrary::IndexToTile(const int32& InSlotId, const FIntPoint& InContainerSize)
{
	return FIntPoint(InSlotId % InContainerSize.X,InSlotId / InContainerSize.Y);
}

int32 UInventoryLibrary::TileToIndex(const FIntPoint& InTile, const FIntPoint& InContainerSize)
{
	return InTile.X + InContainerSize.X * InTile.Y;
}

FIntPoint UInventoryLibrary::RotatedSize(FIntPoint InContainerSize, const bool& bRotated)
{
	if (bRotated)
	{
		return FIntPoint(InContainerSize.X,InContainerSize.Y);
	}
	return InContainerSize;
}

int32 UInventoryLibrary::SelectAmount(const FInventoryItem& InInventoryItem, const int32& InAmount)
{
	bool bIsStackable = IsStackable(InInventoryItem);
	if (bIsStackable)
	{
		return InAmount;
	}
	return 1;
}

bool UInventoryLibrary::IsStackable(const FInventoryItem& InInventoryItem)
{
	return InInventoryItem.OwnInventoryData==nullptr && InInventoryItem.bStackable;
}

UInventoryComponent* UInventoryLibrary::GetLocalPlayerInventoryComponent(UObject*InWorldContext)
{
	if (!InWorldContext->GetWorld()) return nullptr;
	
	if (InWorldContext->GetWorld()->GetFirstPlayerController() && InWorldContext->GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		APawn*LocalPawn = InWorldContext->GetWorld()->GetFirstPlayerController()->GetPawn();
		if (!LocalPawn) return nullptr;
		
		UInventoryComponent*InventoryComponent = Cast<UInventoryComponent>(LocalPawn->GetComponentByClass(UInventoryComponent::StaticClass()));
		if (InventoryComponent)
		{
			return InventoryComponent;
		}
	}
	return nullptr;
}

FLinearColor UInventoryLibrary::GetSupportedOutline()
{
	return FLinearColor(0.121138f,0.796875f,0.f,0.539f);
}

FLinearColor UInventoryLibrary::GetDefaultOutline()
{
	return FLinearColor(0.286275f,0.286275f,0.286275f,0.482353f);
}

bool UInventoryLibrary::IsSelfInventory(UWorld* InWorldContext, const UInventoryComponent* InInventoryComponent)
{
	if (GetLocalPlayerInventoryComponent(InWorldContext) == InInventoryComponent)
	{
		return true;
	}
	return false;
}


