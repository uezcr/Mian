// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryComponent.h"
#include "InventoryLibrary.h"
#include "Engine/World.h"
#include "NativeGameplayTags.h"
#include "DataAsset/ItemContainersData.h"
#include "Equipment/EquipmentDefinition.h"
#include "GameMode/MGameMode.h"
#include "Interface/Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Physics/MCollisionChannel.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryComponent)

UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, Containers);
	DOREPLIFETIME(ThisClass, OnSlotUpdate);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn && GetOwnerRole()==ROLE_AutonomousProxy)
	{
		GetWorld()->GetTimerManager().SetTimer(ClientTraceHandle,this,&UInventoryComponent::ClientTrace,TraceDelta,true);
	}
}


void UInventoryComponent::ClientTrace()
{
	APlayerController*PC = OwnerPawn->GetLocalViewingPlayerController();
	if(!PC)return;

	FHitResult HitResult;
	FVector WorldPosition;
	FRotator WorldDirection;
	PC->GetPlayerViewPoint(WorldPosition,WorldDirection);
	FVector TraceEndPosition = WorldPosition + (WorldDirection.Vector() * TraceLength);
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(InteractionTrace), /*bTraceComplex=*/ true, /*IgnoreActor=*/ GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult,WorldPosition,TraceEndPosition,M_TraceChannel_Interaction,TraceParams);

	if (HitResult.bBlockingHit && HitResult.GetActor() && HitResult.GetActor()!=TraceActor && HitResult.GetActor()->Implements<UInteractable>())
	{
		if(TraceActor)
		{
			IInteractable::Execute_OnTraceLeave(TraceActor);
		}
		
		TraceActor = HitResult.GetActor();
		IInteractable::Execute_OnTraceEnter(TraceActor);
	}else
	{
		if (TraceActor)
		{
			IInteractable::Execute_OnTraceLeave(TraceActor);
			TraceActor = nullptr;
		}
	}
	
	if (bDebugMode)
	{
		DrawDebugLine(GetWorld(),WorldPosition,TraceEndPosition,FColor::Red);
	}
}

void UInventoryComponent::AddContainer(const FContainerInfo& InContainerInfo)
{
	if (GetOwnerRole()==ROLE_Authority)
	{
		Containers.Add(InContainerInfo);
	}
}

void UInventoryComponent::SetContainers(const TArray<FContainerInfo>& InContainers)
{
	Containers = InContainers;
}

bool UInventoryComponent::LoadSubItem(const FSubItemInfo& InSubItemInfo)
{
	FInventoryItem InventoryItem;
	bool bIsSuccess = UInventoryLibrary::GetDefaultIneventoryItemById(InSubItemInfo.ItemID,InventoryItem);
	
	//跳过 - 项目无效成功，防止无限循环
	if (InventoryItem.Name.IsEmpty()) return true;
	int32 OutContainerUID,OutFoundSlotID,OutRemainder;
	bool bOutRotated;
	
	bool bFound = FindSpace(InSubItemInfo.ItemID,InSubItemInfo.ItemAmount,OutContainerUID,OutFoundSlotID,OutRemainder,bOutRotated);
	if (!bFound) return false;
	//查找空间放完道具的数量后,有可能容器满了,这个是余出来的数量
	int32 SurplusAmount = InSubItemInfo.ItemAmount - OutRemainder;

	UEquipmentDefinition* EquipmentDefinition = CreateItemData(InSubItemInfo.ItemID);
	TArray<int32> OutAssignedUIds;
	CreateItemContaines(InSubItemInfo.ItemID,OutAssignedUIds);
	
	//lzy TODO:未完成
	return true;
}

bool UInventoryComponent::FindSpace(const FName& InItemId, const int32& InAmount, int32& OutContainerUID,
	int32& OutFoundSlotID, int32& OutRemainder,bool& bOutRotated)
{
	FInventoryItem InventoryItem;
	UInventoryLibrary::GetDefaultIneventoryItemById(InItemId,InventoryItem);

	//搜索到的库存容器
	TArray<FContainerInfo> SecondCheck;
	
	for (FContainerInfo&ContainerInfo:Containers)
	{
		//是否有空间
		if (!ContainerInfo.bCheckForSpace) continue;

		//容器大小
		int32 ContainerSize = ContainerInfo.ContainerSize.X * ContainerInfo.ContainerSize.Y;
		//道具要占用的格子
		FIntPoint ItemTile = UInventoryLibrary::SelectSize(ContainerInfo.ContainerTag,InventoryItem.ItemSize);
		int32 ItemSize = ItemTile.X * ItemTile.Y;
		//容量够吗
		if (ContainerSize < ItemSize) continue;

		//当前容器,不能装这个类型的道具
		if(ContainerInfo.BannedItemType.Contains(InventoryItem.ItemType)) continue;

		//这个道具可以放到这个插槽吗
		if(!UInventoryLibrary::IsSlotSupported(InItemId,ContainerInfo.ContainerTag)) continue;

		//是不是库存槽,如果不是那就是装备槽了
		if(UInventoryLibrary::IsInventorySlot(ContainerInfo.ContainerTag))
		{
			SecondCheck.Add(ContainerInfo);
			continue;
		}

		//这找的是装备槽
		if (EnoughSpaceOnContainer(ContainerInfo,InItemId,InAmount,false,true,OutFoundSlotID,bOutRotated,OutRemainder))
		{
			OutContainerUID = ContainerInfo.ContainerUID;
			return true;
		}
	}

	for (FContainerInfo&ContainerInfo:SecondCheck)
	{
		//查找容器中可以存放这个物体,并且装入
		if (EnoughSpaceOnContainer(ContainerInfo,InItemId,InAmount,false,true,OutFoundSlotID,bOutRotated,OutRemainder))
		{
			OutContainerUID = ContainerInfo.ContainerUID;
			return true;
		}
	}
	
	return false;
}

bool UInventoryComponent::EnoughSpaceOnContainer(const FContainerInfo& InContainer, const FName& InItemId,const int32& InAmount,
	const bool& InbRotated, const bool& InbAnyRotation, int32& OutFoundSlotId, bool& OutbRotated, int32& OutRemainder)
{
	FInventoryItem InventoryItem;
	UInventoryLibrary::GetDefaultIneventoryItemById(InItemId,InventoryItem);

	for (const FItemInfoDef&ItemInfo:InContainer.Items)
	{
		bool bStacked = TryToStack(InContainer.ContainerUID,ItemInfo.CurrentSlotID,InItemId,InAmount,OutRemainder);
		if(bStacked)
		{
			OutFoundSlotId = ItemInfo.CurrentSlotID;
			OutbRotated=false;
			return true;
		}
	}

	for (const FSlotInfo&SlotInfo:InContainer.Slots)
	{
		int32 SlotId = SlotInfo.SlotID;
		FIntPoint SlotTile = UInventoryLibrary::IndexToTile(SlotId,InContainer.ContainerSize);
		FIntPoint ItemSize = InventoryItem.ItemSize;
		//有可能是往往装备槽放的
		ItemSize = UInventoryLibrary::SelectSize(InContainer.ContainerTag,ItemSize);
		ItemSize = UInventoryLibrary::RotatedSize(ItemSize,InbRotated);
		
		if (MountableOrNot(SlotTile,ItemSize,InContainer))
		{
			OutFoundSlotId = SlotId;
			OutbRotated = InbRotated;
			OutRemainder = 0;
			return true;
		}
		else if (InbAnyRotation) //放不下,尝试旋转一下在放
		{
			ItemSize = UInventoryLibrary::RotatedSize(ItemSize,!InbRotated);
			if(MountableOrNot(SlotTile,ItemSize,InContainer))
			{
				OutFoundSlotId = SlotId;
				OutbRotated = !InbRotated;
				OutRemainder = 0;
				return true;
			}
		}
	}

	OutFoundSlotId = -1;
	OutbRotated = false;
	OutRemainder = 0;
	return false;
}

bool UInventoryComponent::TryToStack(const int32& InContainerUId, const int32& InSlotId,const FName& InItemId,
	const int32& InItemAmount, int32& OutRemainder)
{
	//获取容器里的道具
	FItemInfoDef ItemInfoInstance= GetItemByUIdAndSlotId(InContainerUId,InSlotId);
	if (InItemId != ItemInfoInstance.ItemID) return false;

	//道具有效吗
	FInventoryItem InventoryItem;
	UInventoryLibrary::GetDefaultIneventoryItemById(InItemId,InventoryItem);
	if (InventoryItem.Name.IsEmpty()) return false;

	//道具能叠层吗
	if(!InventoryItem.bStackable) return false;

	//是商人吗
	if (InventoryType == EInventoryType::E_Vendor)
	{
		OutRemainder = 0;
		return true;
	}

	//当前物品的数量
	int32 AddItemAmount = ItemInfoInstance.ItemAmount + InItemAmount;
	int32 Remainder = AddItemAmount - FMath::Clamp(AddItemAmount,0,InventoryItem.MaxStackSize);
	//只要小于,那肯定就叠加了,返回剩余数量且成功
	if (Remainder < InItemAmount)
	{
		OutRemainder = Remainder;
		return true;
	}
	return false;
}

bool UInventoryComponent::MountableOrNot(const FIntPoint& InSlotTile, const FIntPoint& InItemSize,const FContainerInfo& InContainer)
{
	//列
	for (int Row = InSlotTile.Y; Row < InSlotTile.Y+InItemSize.Y; Row++)
	{
		//行
		for (int Column = InSlotTile.X; Column<InSlotTile.X+InItemSize.X; Column++)
		{
			//格子越界了
			bool bIsValid = UInventoryLibrary::IsValidTile(FIntPoint(Column,Row),InContainer.ContainerSize);
			if(!bIsValid) return false;
			//这个插槽放东西了吗
			int32 SlotIndex = UInventoryLibrary::TileToIndex(FIntPoint(Column,Row),InContainer.ContainerSize);
			if(!InContainer.Slots[SlotIndex].bIsEmpty) return false;
		}
	}
	
	return true;
}

int32 UInventoryComponent::GetContainerIndexByUId(const int32& InContainerId)
{
	for (int i = 0;i<Containers.Num();i++)
	{
		if (Containers[i].ContainerUID == InContainerId)
		{
			return i;
		}
	}
	return -1;
}

FItemInfoDef UInventoryComponent::GetItemIndexBySlotId(const FContainerInfo& InContainerInfo, const int32& InSlotId)
{
	for (int32 i = 0;i<InContainerInfo.Items.Num();i++)
	{
		if (InContainerInfo.Items[i].CurrentSlotID == InSlotId)
		{
			return InContainerInfo.Items[i];
		}
	}
	return FItemInfoDef();
}

FItemInfoDef UInventoryComponent::GetItemByUIdAndSlotId(const int32& InContainerId, const int32& InSlotId)
{
	const int32& ContainerIndex = GetContainerIndexByUId(InContainerId);
	const FContainerInfo& ContainerInfo = Containers[ContainerIndex];
	return GetItemIndexBySlotId(ContainerInfo,InSlotId);
}

UEquipmentDefinition* UInventoryComponent::CreateItemData(const FName& InItemId)
{
	
	FInventoryItem InventoryItem;
	UInventoryLibrary::GetDefaultIneventoryItemById(InItemId,InventoryItem);
	if(InventoryItem.EquipmentDefinition)
	{
		return NewObject<UEquipmentDefinition>(GetWorld(),InventoryItem.EquipmentDefinition);
	}
	return nullptr;
}

bool UInventoryComponent::CreateItemContaines(const FName& InItemId, TArray<int32>& OutAssignedUIds)
{
	AMGameMode*GM = Cast<AMGameMode>(GetWorld()->GetAuthGameMode());
	if(!GM) return false;
	
	FInventoryItem InventoryItem;
	UInventoryLibrary::GetDefaultIneventoryItemById(InItemId,InventoryItem);
	if (InventoryItem.OwnInventoryData)
	{
		TArray<FContainerInfo> ContainerInfos = InventoryItem.OwnInventoryData->GetContainers();
		
		OutAssignedUIds = GM->AddUniqueContainers(ContainerInfos,this);
		return true;
	}
	return false;
}

void UInventoryComponent::SetItemOnContainer(const FItemInfoDef& InItemInfo, const int32& InContainerId,const int32& InSlotId)
{
	if(InItemInfo.ItemAmount == 0) return;

	//具体的容器
	int32 ContainerIndex = GetContainerIndexByUId(InContainerId);
	FContainerInfo& ContainerInfo = Containers[ContainerIndex];
	
	//在容器中,这个Slot上的东西
	FItemInfoDef OriginalItemInfo = GetItemIndexBySlotId(ContainerInfo,InSlotId);

	//这个道具的详细信息
	FInventoryItem InventoryItem;
	UInventoryLibrary::GetDefaultIneventoryItemById(InItemInfo.ItemID,InventoryItem);
	
	//这slot上没东西
	if(OriginalItemInfo.ItemID.IsNone())
	{
		//给容器中添加Item
		FItemInfoDef ItemInfo = InItemInfo;
		ItemInfo.ItemAmount = UInventoryLibrary::SelectAmount(InventoryItem,InItemInfo.ItemAmount);
		ItemInfo.bRotated = InItemInfo.bRotated && UInventoryLibrary::IsInventorySlot(ContainerInfo.ContainerTag);
		ItemInfo.CurrentSlotID = InSlotId;
		ContainerInfo.Items.Add(ItemInfo);

		// 容器里装的这个道具,有可能也是有容器的道具,比如背包装了个背包
		for (const int32&SubContainerUId:InItemInfo.OwnContainerUIDs)
		{
			if(SubContainerUId == -1) continue;
			//找到这个子容器
			int32 SubContainerIndex = GetContainerIndexByUId(InContainerId);
			FContainerInfo& SubContainerInfo = Containers[SubContainerIndex];
			//设置他显不显示库存
			SubContainerInfo.bCheckForSpace = ContainerInfo.bShowInventory;
		}

		TArray<int32> ItemOccupySlotIds = GetItemSlotIndexs(InContainerId,InItemInfo.CurrentSlotID);
		for (int i =0;i<ItemOccupySlotIds.Num();i++)
		{
			ContainerInfo.Slots[i].ItemSlotID = ItemOccupySlotIds[0];
			ContainerInfo.Slots[i].bIsEmpty = false;
		}

		OnSlotUpdate.Broadcast(InContainerId,InSlotId,InItemInfo);
		return;
	}

	//如果不为空说明这有相同东西,尝试去叠加
	bool bIsStackable = UInventoryLibrary::IsStackable(InventoryItem);
	if (OriginalItemInfo.ItemID == InItemInfo.ItemID && bIsStackable)
	{
		AddItemAmount(InContainerId,InSlotId,InItemInfo.ItemAmount);
	}
}

TArray<int32> UInventoryComponent::GetItemSlotIndexs(const int32& InItemContainerUId,const int32& InItemSlotId)
{
	//找到容器里的具体Item
	const int32& ContainerIndex = GetContainerIndexByUId(InItemContainerUId);
	FItemInfoDef ItemInfo = GetItemIndexBySlotId(Containers[ContainerIndex],InItemSlotId);
	if(ItemInfo.ItemID.IsNone()) return TArray<int32>();

	//查找这个道具在容器中的具体起始位置
	FInventoryItem InventoryItem;
	UInventoryLibrary::GetDefaultIneventoryItemById(ItemInfo.ItemID,InventoryItem);
	FIntPoint ActualSize = UInventoryLibrary::SelectSize(Containers[ContainerIndex].ContainerTag,InventoryItem.ItemSize);
	ActualSize = UInventoryLibrary::RotatedSize(ActualSize,ItemInfo.bRotated);
	FIntPoint TopLifeTile = UInventoryLibrary::IndexToTile(InItemSlotId,Containers[ContainerIndex].ContainerSize);

	//记录道具具体占据的格子
	TArray<int32> OccupySlotIds;
	for (int Row = TopLifeTile.Y;Row<TopLifeTile.Y+ActualSize.Y; Row++)
	{
		for (int Column = TopLifeTile.X;Column<TopLifeTile.X + ActualSize.X; Column++)
		{
			FIntPoint CurrentTile = FIntPoint(Column,Row);
			bool bIsValid = UInventoryLibrary::IsValidTile(CurrentTile,Containers[ContainerIndex].ContainerSize);
			if(!bIsValid) continue;
			int32 OccupySlotIndex = UInventoryLibrary::TileToIndex(CurrentTile,Containers[ContainerIndex].ContainerSize);
			OccupySlotIds.Add(OccupySlotIndex);
		}
	}

	return OccupySlotIds;
}

void UInventoryComponent::AddItemAmount(const int32& InContainerUId, const int32 InSlotId,const int32& InAddAmount)
{
	//TODO:lzy 未完成
}



