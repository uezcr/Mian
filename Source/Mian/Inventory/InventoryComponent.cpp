// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryComponent.h"
#include "InventoryLibrary.h"
#include "DataAsset/InventoryData.h"
#include "Engine/World.h"
#include "DataAsset/ItemContainersData.h"
#include "Equipment/EquipmentDefinition.h"
#include "GameMode/MGameMode.h"
#include "Interface/Interactable.h"
#include "Net/UnrealNetwork.h"
#include "Physics/MCollisionChannel.h"
#include "Pickup/PickupBase.h"
#include "UI/MHUD.h"
#include "UI/Inventory/InventoryScreen.h"
#include "UI/Inventory/Pickup/PickupDisplay.h"

#pragma optimize("",off)
#include "UI/Inventory/Slot/InventorySlot.h"

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
	//就服务器执行,在内部判断了
	ServerInitInventory();

	if (GetOwner<APawn>() && GetOwner<APawn>()->IsLocallyControlled())
	{
		InventoryScreen = CreateWidget<UInventoryScreen>(GetOwner<APawn>()->GetController<APlayerController>(),InventoryScreenClass);
		GetWorld()->GetTimerManager().SetTimer(ClientTraceHandle,this,&UInventoryComponent::ClientTrace,TraceDelta,true);
	}
}

void UInventoryComponent::ClientTrace()
{
	APlayerController*PC = GetOwner<APawn>()->GetLocalViewingPlayerController();
	if(!PC)return;

	FHitResult HitResult;
	FVector WorldPosition;
	FRotator WorldDirection;
	PC->GetPlayerViewPoint(WorldPosition,WorldDirection);
	FVector TraceEndPosition = WorldPosition + (WorldDirection.Vector() * TraceLength);
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(InteractionTrace), /*bTraceComplex=*/ true, /*IgnoreActor=*/ GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult,WorldPosition,TraceEndPosition,M_TraceChannel_Interaction,TraceParams);

	if (HitResult.bBlockingHit && HitResult.GetActor())
	{
		if(TraceActor && HitResult.GetActor()!=TraceActor)
		{
			IInteractable::Execute_OnTraceLeave(TraceActor);
			TraceActor=nullptr;
		}
		if(HitResult.GetActor()->Implements<UInteractable>())
		{
			TraceActor = HitResult.GetActor();
			IInteractable::Execute_OnTraceEnter(TraceActor);
		}
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

void UInventoryComponent::ResetContainer()
{
	Containers.Empty();
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
	FItemInfoDef ItemInfo = FItemInfoDef(InSubItemInfo.ItemID,bOutRotated,SurplusAmount,OutFoundSlotID,OutAssignedUIds,EquipmentDefinition);
	SetItemOnContainer(ItemInfo,OutContainerUID,OutFoundSlotID);

	return true;
}

void UInventoryComponent::Interactable(EInteractionType InInteractionType)
{
	if (TraceActor && GetOwner<APawn>()->IsLocallyControlled())
	{
		Server_Interactable(TraceActor,InInteractionType);
	}
}

void UInventoryComponent::Server_Interactable_Implementation(AActor* InActor, EInteractionType InInteractionType)
{
	if (InActor)
	{
		IInteractable::Execute_OnInteract(InActor,GetOwner(),InInteractionType);
	}
}

bool UInventoryComponent::PickupItem(APickupBase* InPickupBase)
{
	FItemInfoDef ItemInfoDef = InPickupBase->GetItemInfo();
	
	TFunction<void(FFindResult FindResult)> FindFoundSpace = [&](FFindResult FindResult)
	{
		ItemInfoDef.bRotated = FindResult.bRotated;
		ItemInfoDef.ItemAmount = FindResult.Amount;
		bool bPickupSuccess = SetItemOnContainer(ItemInfoDef,FindResult.ContainerUID,FindResult.SlotID);

		if (bPickupSuccess)
		{
			InPickupBase->ReduceAmount(FindResult.Amount);
		}
		
		if(bDebugMode)
		{
			FString StrLog = FString::Printf(TEXT("Pickedup Pickid=%s Amount=%d"),*ItemInfoDef.ItemID.ToString(),FindResult.Amount);
			PrintDebug(StrLog,FColor::Blue,1);
		}
	};
	Containers.Append(InPickupBase->GetInventoryComponent()->GetContainers());
	EFindState FindState = ForEachFoundSpace(ItemInfoDef.ItemID,ItemInfoDef.ItemAmount,FindFoundSpace);
	
	return FindState == EFindState::E_Completed?true:false;
}

TArray<UContainer*> UInventoryComponent::GetAllContainerWidgets()
{
	TArray<UContainer*>ReturnValue = LootContainers;
	ReturnValue.Append(ContainerWidgets);
	return ReturnValue;
}

void UInventoryComponent::AddDisplayTextToHUD(const FText& InDisplayText, const int32& InAmount)
{
	if (GetOwner<APawn>()->IsLocallyControlled())
	{
		AMHUD*HUD = GetOwner<APawn>()->GetController<APlayerController>()->GetHUD<AMHUD>();
		UPickupDisplay*PickupDisplay = HUD->GetPickupDisplay();
		PickupDisplay->SetPickupText(InDisplayText,InAmount);
		if(!PickupDisplay->IsInViewport())
		{
			PickupDisplay->AddToViewport(1);
		}
		else if (!PickupDisplay->IsVisible())
		{
			PickupDisplay->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UInventoryComponent::RemoveDisplayText()
{
	if (GetOwner<APawn>()->IsLocallyControlled())
	{
		AMHUD*HUD = GetOwner<APawn>()->GetController<APlayerController>()->GetHUD<AMHUD>();
		UPickupDisplay*PickupDisplay = HUD->GetPickupDisplay();
		if(PickupDisplay->IsVisible())
		{
			PickupDisplay->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UInventoryComponent::ToggleInventory(bool bOpenInventory, bool bUseFade, bool bStopAnim)
{
	//lzy TODO:背包UI未完成
}

void UInventoryComponent::ResetAllColorSlots()
{
	for (UInventorySlot*&InventorySlot:ColorChangedSlots)
	{
		InventorySlot->ResetSlotColor();
	}
	ColorChangedSlots.Empty();
}

FContainerInfo UInventoryComponent::GetContainerByUId(const int32& InContainerId)
{
	int32 ContainerIndex = GetContainerIndexByUId(InContainerId);
	return Containers[ContainerIndex];
}

TArray<int32> UInventoryComponent::GetItemSlotIndexes(const int32& InItemContainerUId, const int32& InItemSlotId)
{
	const FContainerInfo& ContainerInfo = GetContainerByUId(InItemContainerUId);
	const int32& ItemIndex = GetItemIndexBySlotId(ContainerInfo,InItemSlotId);
	if (ItemIndex == -1) return TArray<int32>();
	const FItemInfoDef& ItemInfo = ContainerInfo.Items[ItemIndex];

	//起始位置和容器大小
	const FIntPoint& ContainerSize = ContainerInfo.ContainerSize;
	const FIntPoint& StartPosition = UInventoryLibrary::IndexToTile(InItemSlotId,ContainerSize);

	FInventoryItem InventoryItem;
	UInventoryLibrary::GetDefaultIneventoryItemById(ItemInfo.ItemID,InventoryItem);
	FIntPoint ItemSize = UInventoryLibrary::SelectSize(ContainerInfo.ContainerTag,InventoryItem.ItemSize);
	if (ItemInfo.bRotated)
	{
		ItemSize = UInventoryLibrary::RotatedSize(ContainerInfo.ContainerSize,ItemInfo.bRotated);
	}

	TArray<int32> ReturnValue;
	for (int Raw = StartPosition.Y;Raw<StartPosition.Y+ItemSize.Y; Raw++)
	{
		for (int Cloumn = StartPosition.X;Cloumn<StartPosition.X+ItemSize.X;Cloumn++)
		{
			FIntPoint CurrentPosition(Cloumn,Raw);
			bool bIsValid = UInventoryLibrary::IsValidTile(CurrentPosition,ContainerInfo.ContainerSize);
			if(!bIsValid) return TArray<int32>();
			int32 SlotIndex= UInventoryLibrary::TileToIndex(CurrentPosition,ContainerInfo.ContainerSize);
			ReturnValue.Add(SlotIndex);
		}
	}
	return ReturnValue;
}

void UInventoryComponent::PrintDebug(FString InContent, FColor InColor,int32 InKey,float InPrintTime)
{
	if(bDebugMode && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(InKey,InPrintTime,InColor,InContent);
	}
}

EFindState UInventoryComponent::ForEachFoundSpace(const FName& InItemId,const int32&InItemAmout,TFunction<void(FFindResult FindResult)> InLoopFuntion)
{
	int32 ItemAmount = InItemAmout;
	int32 OutContainerUID, OutFoundSlotID,OutRemainder;
	bool bOutRotated;
	bool bFound = FindSpace(InItemId,ItemAmount,OutContainerUID,OutFoundSlotID,OutRemainder,bOutRotated);

	FFindResult FindResult(OutContainerUID,OutFoundSlotID,ItemAmount-OutRemainder,bOutRotated);
	if(bFound)
	{
		ItemAmount = OutRemainder;
		if(ItemAmount>0)
		{
			//递归 TFunction LoopBody
			InLoopFuntion(FindResult);
			return ForEachFoundSpace(InItemId,ItemAmount,InLoopFuntion);
		}
		else
		{
			InLoopFuntion(FindResult);
			return EFindState::E_Completed;
		}
	}else
	{
		return EFindState::E_Cancelled;
	}
}

void UInventoryComponent::Client_InitWidgets_Implementation(FClientInventoryData InClientInventoryData)
{
	//lzy TODO:背包UI未完成
}

void UInventoryComponent::ServerInitInventory()
{
	if (GetOwner<APawn>() == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("lzy Pawn == nullptr"));
		return;
	}
	
	if (!GetOwner<APawn>()->HasAuthority()) return;

	if (InventoryType == EInventoryType::E_Player)
	{
		OnSlotUpdate.AddDynamic(this,&ThisClass::HandleSelfOnSlotUpdate);
		TArray<FClientInventoryData>ClientInventoryDatas = SetupInventory();
		
		for (FClientInventoryData&ClientInventoryData:ClientInventoryDatas)
		{
			Client_InitWidgets(ClientInventoryData);
		}
	}
	else if (InventoryType == EInventoryType::E_Car)
	{
		SetupInventory();
	}
	else if (InventoryType == EInventoryType::E_AI)
	{
		SetupInventory();
	}
}

TArray<FClientInventoryData> UInventoryComponent::SetupInventory()
{
	TArray<FClientInventoryData> ClientInventoryDatas;
	if (InventoryDatas.IsEmpty()) return ClientInventoryDatas;

	for (UInventoryData*&InventoryData:InventoryDatas)
	{
		FClientInventoryData ClientInventoryData;
		ClientInventoryData.UIDs = UInventoryLibrary::AddUniqueContainers(InventoryData->Containers,this);
		ClientInventoryData.InventoryData = InventoryData;
		
		ClientInventoryDatas.Add(ClientInventoryData);
	}
	return ClientInventoryDatas;
}

void UInventoryComponent::HandleSelfOnSlotUpdate(int32 ContainerUId, int32 InSlotId, FItemInfoDef InItemToAdd)
{
	//lzy TODO:未完成
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

int32 UInventoryComponent::GetItemIndexBySlotId(const FContainerInfo& InContainerInfo, const int32& InSlotId)
{
	for (int32 i = 0;i<InContainerInfo.Items.Num();i++)
	{
		if (InContainerInfo.Items[i].CurrentSlotID == InSlotId)
		{
			return i;
		}
	}
	return -1;
}

FItemInfoDef UInventoryComponent::GetItemByUIdAndSlotId(const int32& InContainerId, const int32& InSlotId)
{
	const int32& ContainerIndex = GetContainerIndexByUId(InContainerId);
	const FContainerInfo& ContainerInfo = Containers[ContainerIndex];
	const int32& ItemIndex = GetItemIndexBySlotId(ContainerInfo,InSlotId);
	return ContainerInfo.Items[ItemIndex];
}

void UInventoryComponent::AddColorChangedSlot(UInventorySlot* InInventorySlot)
{
	if (InInventorySlot)
	{
		ColorChangedSlots.AddUnique(InInventorySlot);
	}
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

bool UInventoryComponent::SetItemOnContainer(const FItemInfoDef& InItemInfo, const int32& InContainerId,const int32& InSlotId)
{
	if(InItemInfo.ItemAmount == 0) return false;

	//具体的容器
	int32 ContainerIndex = GetContainerIndexByUId(InContainerId);
	FContainerInfo& ContainerInfo = Containers[ContainerIndex];
	
	//在容器中,这个Slot上的东西
	int32 ContainerItemIndex = GetItemIndexBySlotId(ContainerInfo,InSlotId);

	//这个道具的详细信息
	FInventoryItem InventoryItem;
	UInventoryLibrary::GetDefaultIneventoryItemById(InItemInfo.ItemID,InventoryItem);
	
	//这slot上没东西
	if(ContainerItemIndex == -1)
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
			int32 SubContainerIndex = GetContainerIndexByUId(SubContainerUId);
			FContainerInfo& SubContainerInfo = Containers[SubContainerIndex];
			//设置他显不显示库存
			SubContainerInfo.bCheckForSpace = ContainerInfo.bShowInventory;
		}

		TArray<int32> ItemOccupySlotIds = GetItemSlotIndexs(InContainerId,ItemInfo.CurrentSlotID);
		for (int i =0;i<ItemOccupySlotIds.Num();i++)
		{
			
			ContainerInfo.Slots[ItemOccupySlotIds[i]].ItemSlotID = ItemOccupySlotIds[0];
			ContainerInfo.Slots[ItemOccupySlotIds[i]].bIsEmpty = false;
		}

		OnSlotUpdate.Broadcast(InContainerId,InSlotId,InItemInfo);
		return true;
	}

	//如果不为空说明这有东西,看看是不是相同的尝试去叠加
	bool bIsStackable = UInventoryLibrary::IsStackable(InventoryItem);
	FItemInfoDef OriginalItemInfo =ContainerInfo.Items[ContainerItemIndex];
	
	if (OriginalItemInfo.ItemID == InItemInfo.ItemID && bIsStackable)
	{
		AddItemAmount(InContainerId,InSlotId,InItemInfo.ItemAmount);
		return true;
	}
	return false;
}

TArray<int32> UInventoryComponent::GetItemSlotIndexs(const int32& InItemContainerUId,const int32& InItemSlotId)
{
	//找到容器里的具体Item
	const int32& ContainerIndex = GetContainerIndexByUId(InItemContainerUId);
	const int32& ItemIndex=GetItemIndexBySlotId(Containers[ContainerIndex],InItemSlotId);
	if(ItemIndex==-1)return TArray<int32>();
	
	FItemInfoDef ItemInfo =Containers[ContainerIndex].Items[ItemIndex];
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
			//lzy TODO:跳过还是返回?
			if(!bIsValid) return TArray<int32>();
			int32 OccupySlotIndex = UInventoryLibrary::TileToIndex(CurrentTile,Containers[ContainerIndex].ContainerSize);
			OccupySlotIds.Add(OccupySlotIndex);
		}
	}

	return OccupySlotIds;
}

void UInventoryComponent::AddItemAmount(const int32& InContainerUId, const int32 InSlotId,const int32& InAddAmount)
{
	int32 ContainerIndex = GetContainerIndexByUId(InContainerUId);
	int32 ItemIndex = GetItemIndexBySlotId(Containers[ContainerIndex],InSlotId);
	FItemInfoDef&ItemInfoDef = Containers[ContainerIndex].Items[ItemIndex];
	if (ItemInfoDef.ItemAmount + InAddAmount <= 0)
	{
		RemoveItem(InContainerUId,InSlotId);
		OnSlotUpdate.Broadcast(InContainerUId,InSlotId,FItemInfoDef());
		return;
	}

	FInventoryItem InventoryItem;
	UInventoryLibrary::GetDefaultIneventoryItemById(ItemInfoDef.ItemID,InventoryItem);
	if (InventoryItem.bStackable)
	{
		ItemInfoDef.ItemAmount+=InAddAmount;
		OnSlotUpdate.Broadcast(InContainerUId,InSlotId,ItemInfoDef);
	}
}

void UInventoryComponent::RemoveItem(const int32& InContainerId, const int32& InSlotId)
{
	ClearItemOnSlots(InContainerId,InSlotId);
	int32 ContainerIndex = GetContainerIndexByUId(InContainerId);
	int32 ItemIndex = GetItemIndexBySlotId(Containers[ContainerIndex],InSlotId);
	Containers[ContainerIndex].Items.RemoveAt(ItemIndex);
}

void UInventoryComponent::ClearItemOnSlots(const int32& InContainerId, const int32& InSlotId)
{
	TArray<int32>ItemIndexs = GetItemSlotIndexs(InContainerId,InSlotId);
	SetSlotsEmpty(InContainerId,ItemIndexs);
}

void UInventoryComponent::SetSlotsEmpty(const int32& InContainerId, const TArray<int32>& InSlotIndexs,const bool& InbSetEmpty)
{
	int32 ContainerIndex = GetContainerIndexByUId(InContainerId);
	for (const int32&SlotIndex : InSlotIndexs)
	{
		Containers[ContainerIndex].Slots[SlotIndex].ItemSlotID = -1;
		Containers[ContainerIndex].Slots[SlotIndex].bIsEmpty = InbSetEmpty;
	}
}

#pragma optimize("",on)