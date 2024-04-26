// // Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryLibrary.generated.h"

class UInventoryComponent;
struct FContainerInfo;
struct FGameplayTag;
struct FInventoryItem;
/**
 * 
 */
UCLASS()
class MIAN_API UInventoryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	//通过GameplayTag获取这个道具的名字
	//主要用来在DR中查找道具项
	UFUNCTION(BlueprintPure,Category="InventoryLibrary",meta = (Categories = "Inventory.Item"))
	static FName GetItemNameByGameplayTag(const FGameplayTag& InGameplayTag);

	//通过表行名获取道具信息
	UFUNCTION(BlueprintPure,Category="InventoryLibrary")
	static const bool GetDefaultIneventoryItemById(const FName& InItemId,FInventoryItem&OutInventoryItem);

	/* 在服务器调，客户端没反应,添加的核心在GameMode
	 * 添加在世界中唯一存在的容器
	 * @param InContainers				要添加的那些容器
	 * @param InTargetInventoryComp		这些容器归属于那个组件
	 */
	UFUNCTION(BlueprintCallable,Category="InventoryLibrary")
	static TArray<int32> AddUniqueContainers(TArray<FContainerInfo>&InContainers,UInventoryComponent*InTargetInventoryComp);

	/* 根据容器Slot的类型，和要装备收纳的道具占用的格子大小,返回具体大小
	 * 除了InventorySlot标签的要占据多个格子，目前其他都是只占用一个(武器槽，背心槽等)
	 * @param InSlotTag		Slot类型
	 * @param InSize		道具的要占用的格子数量
	 * @return 实际要占用的格子
	 */
	UFUNCTION(BlueprintPure,Category="InventoryLibrary")
	static FIntPoint SelectSize(const FGameplayTag& InSlotTag,const FIntPoint& InSize);

	/* 当前的SlotTag是不是InventorySlot
	 * @param InSlotTag		要检查的Slot
	 * @return	是不是InventorySlot
	 */
	UFUNCTION(BlueprintPure,Category="InventoryLibrary")
	static bool IsInventorySlot(const FGameplayTag& InSlotTag);

	/* 这个道具是否可以下放到这个类型的插槽
	 * @param InItemId		要下放的道具ID
	 * @param InSlotTag		要下放的插槽
	 * @return 可以下放吗
	 */
	UFUNCTION(BlueprintPure,Category="InventoryLibrary")
	static bool IsSlotSupported(const FName& InItemId,const FGameplayTag& InSlotTag);

	/** 这个格子有效吗
	 * @param InTile			具体的格子位置 
	 * @param InContainerSize	容器的大小
	 * @return 是否有效
	 */
	UFUNCTION(BlueprintPure,Category="InventoryLibrary")
	static bool IsValidTile(const FIntPoint&InTile,const FIntPoint&InContainerSize);

	/** 通过SlotID(容器中Slots中的Index)获取当前Slot在具体的那个格子上
	 * @param InSlotId			SlotID
	 * @param InContainerSize	具体容器的大小
	 * @return 在容器中的位置
	 */
	UFUNCTION(BlueprintPure,Category="InventoryLibrary")
	static FIntPoint IndexToTile(const int32& InSlotId,const FIntPoint&InContainerSize);

	/** 通过具体的格子位置,获取他在容器储存的Slots中的下标(SlotId)
	 * @param InTile			具体格子位置
	 * @param InContainerSize	容器的大小
	 * @return 容器储存的Slots中的下标(SlotId)
	 */
	UFUNCTION(BlueprintPure,Category="InventoryLibrary")
	static int32 TileToIndex(const FIntPoint&InTile,const FIntPoint&InContainerSize);

	/** 旋转大小
	 * @param InContainerSize	当前容器大小 
	 * @return 旋转后的大小
	 */
	UFUNCTION(BlueprintPure,Category="InventoryLibrary")
	static FIntPoint RotatedSize(FIntPoint InContainerSize,const bool&bRotated);

	/** 筛选他的数量,有可能他是不能叠加的
	 * @param InInventoryItem	道具信息
	 * @param InAmount			
	 * @return 
	 */
	UFUNCTION(BlueprintPure,Category="InventoryLibrary")
	static int32 SelectAmount(const FInventoryItem&InInventoryItem,const int32&InAmount);

	/** 这个Item可以叠层吗
	 * @param InInventoryItem 
	 * @return 可以?
	 */
	UFUNCTION(BlueprintPure,Category="InventoryLibrary")
	static bool IsStackable(const FInventoryItem&InInventoryItem);

	/*获取本地玩家身上的库存组件*/
	UFUNCTION(BlueprintCallable,Category="InventoryLibrary")
	static UInventoryComponent*GetLocalPlayerInventoryComponent(UObject*InWorldContext);
};
