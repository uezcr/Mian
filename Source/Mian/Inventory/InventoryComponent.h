#pragma once

#include "InventoryType.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventorySlot;
class UContainer;
class UPickupDisplay;
class APickupBase;
class UInventoryData;
class UInteractable;
class UInventoryScreen;

UENUM()
enum class EFindState:uint8
{
	E_Cancelled,
	E_Completed
};



//背包中的某一个容器中的Slot变化
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSlotUpdateDelegate,int32,ContainerUId,int32,InSlotId,FItemInfoDef,InItemToAdd);

UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIAN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
#pragma region Parameters

	//库存页的UI类,玩家有就行,其他为空
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventoryParameters")
	TSubclassOf<UInventoryScreen> InventoryScreenClass;;
	
	//这个库存的名字(仓库,商人,玩家)
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventoryParameters")
	FString InventoryName;

	//库存信息,动态的,初始化时也会携带
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventoryParameters")
	TArray<UInventoryData*> InventoryDatas;

	//这个组件的拥有者是什么类型的
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventoryParameters")
	EInventoryType InventoryType;

	//多少秒打一次射线
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventoryParameters")
	float TraceDelta = 0.05f;

	//射线长度
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventoryParameters")
	float TraceLength = 1000.f;

	//Debug
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventoryParameters")
	bool bDebugMode=false;

	//服务器校验射线数据
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventoryParameters")
	bool bServerVerifyTraces=false;

	//当容器中的Slot变化时
	UPROPERTY(Replicated,BlueprintReadWrite,Category="InventoryParameters")
	FOnSlotUpdateDelegate OnSlotUpdate;

	
#pragma endregion 

protected:
	//组件当前拥有的容器
	UPROPERTY(Replicated,BlueprintReadWrite,Category="InventoryParameters")
	TArray<FContainerInfo> Containers;

public:
	UInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UObject interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of UObject interface

	//~ActorComponent interface
	virtual void BeginPlay() override;
	//~End of ActorComponent interface

	//添加容器
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void AddContainer(const FContainerInfo& InContainerInfo);

	//设置容器
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void SetContainers(const TArray<FContainerInfo>& InContainers);

	//获取容器
	UFUNCTION(BlueprintPure,Category="Inventory")
	FORCEINLINE TArray<FContainerInfo> GetContainers(){return Containers;};

	//清空容器
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void ResetContainer();

	//加载子道具,这个道具会加载到容器里
	UFUNCTION(BlueprintCallable,Category="Inventory")
	bool LoadSubItem(const FSubItemInfo& InSubItemInfo);

	//请求交互当前射线扫到的Actor
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void Interactable(EInteractionType InInteractionType);

	//拾取道具
	UFUNCTION(BlueprintCallable,Category="Inventory")
	bool PickupItem(APickupBase* InPickupBase);

	/*Debug 打印信息*/
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void PrintDebug(FString InContent,FColor InColor=FColor::Blue,int32 InKey=1,float InPrintTime = 5.f);

//UI的公开接口	
#pragma region InventoryUI
	/*获取所有的容器UI*/
	UFUNCTION(BlueprintCallable,Category="Inventory")
	TArray<UContainer*> GetAllContainerWidgets();

	/*添加拾取UI到视口*/
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void AddDisplayTextToHUD(const FText&InDisplayText,const int32&InAmount);

	/*添加拾取UI到视口*/
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void RemoveDisplayText();

	/** 打开库存UI
	 * @param bOpenInventory	打开/关闭 
	 * @param bUseFade			平滑打开?
	 * @param bStopAnim			如果在播放动画,中断吗
	 */
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void ToggleInventory(bool bOpenInventory,bool bUseFade,bool bStopAnim);

	/*把所有的Slot恢复成他原本的颜色*/
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void ResetAllColorSlots();

	/** 通过容器Id获取对应的容器
	 * @param InContainerId		容器Id
	 * @return 容器
	 */
	UFUNCTION(BlueprintPure,Category="Inventory")
	FContainerInfo GetContainerByUId(const int32&InContainerId);

	/** 获取当前道具占据的所有Slot
	 * @param InItemContainerUId	容器Id 
	 * @param InItemSlotId			占用的左上角Id
	 * @return 返回占用的SlotId
	 */
	UFUNCTION(BlueprintPure,Category="Inventory")
	TArray<int32> GetItemSlotIndexes(const int32&InItemContainerUId,const int32&InItemSlotId);

	/** 通过容器ID,和插槽ID获取对应的Item
	 * @param InContainerId 容器ID
	 * @param InSlotId		插槽ID
	 * @return ItemInfo 道具信息
	 */
	UFUNCTION(BlueprintPure,Category="Inventory")
	FItemInfoDef GetItemByUIdAndSlotId(const int32&InContainerId,const int32&InSlotId);

	/*这个携带交互组件的Owner是商人吗*/
	UFUNCTION(BlueprintPure,Category="Inventory")
	bool IsVendorInventory() const;

	/**	添加修改的Slot
	 * @param InInventorySlot	修改的具体SlotWidget 
	 */
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void AddColorChangedSlot(UInventorySlot*InInventorySlot);
#pragma endregion

//PRC
	/*服务器添加容器后,同步给本机的容器信息*/
	UFUNCTION(Client,Reliable,BlueprintCallable,Category="Inventory|RPC")
	void Client_InitWidgets(FClientInventoryData InClientInventoryData);
	
	//请求交互当前射线扫到的Actor
	UFUNCTION(Server,Reliable,BlueprintCallable,Category="Inventory|RPC")
	void Server_Interactable(AActor*InActor,EInteractionType InInteractionType);

	/** 请求移动道具位置
	 * @param InFromContainerUId	要移动的道具容器Id 
	 * @param InFormSlotId			要移动的道具SlotId
	 * @param ToContainerUId		要移动到的容器Id
	 * @param InToSlotId			要移动到的SlotId
	 * @param InAmount				要移动的数量
	 * @param InbRotated			旋转了吗
	 * @param OutbSuccess			返回值，成功了吗
	 */
	UFUNCTION(Server,Reliable,BlueprintCallable,Category="Inventory|RPC")
	void Server_RequestMoveItem(int32 InFromContainerUId,int32 InFormSlotId,int32 ToContainerUId,int32 InToSlotId,int32 InAmount,bool InbRotated);

	/** 更新指定容器中的道具
	 * @param InContainerUId		容器Id 
	 * @param InSlotId				SlotId
	 * @param InItemToAdd			要添加的道具
	 * @param InCurrentContainers	当前拥有的Containers
	 * @param InbIsLootWidget		更新的是战利品widget吗?
	 */
	UFUNCTION(Client,Reliable,BlueprintCallable,Category="Inventory|RPC")
	void Client_UpdateSlotWidget(int32 InContainerUId,int32 InSlotId,FItemInfoDef InItemToAdd,const TArray<FContainerInfo>& InCurrentContainers,bool InbIsLootWidget);

	/** 删除指定容器中的道具
	 * @param InContainerUId		容器Id 
	 * @param InSlotId				SlotId
	 * @param InCurrentContainers	当前拥有的Containers
	 * @param InbIsLootWidget		删除的是战利品widget吗?
	 */
	UFUNCTION(Client,Reliable,BlueprintCallable,Category="Inventory|RPC")
	void Client_RemoveItemWidget(int32 InContainerUId,int32 InSlotId,const TArray<FContainerInfo>& InCurrentContainers,bool InbIsLootWidget);

	/** 请求转移道具
	 * @param InSourceInventory 
	 * @param InSourceContainerUID 
	 * @param InSourceSlotId 
	 * @param InTargetInventory 
	 * @param InTargetContainerUID 
	 * @param InTargetSlotID 
	 * @param InAmount 
	 * @param InbRotated 
	 */
	UFUNCTION(Server,Reliable,BlueprintCallable,Category="Inventory|RPC")
	void Server_RequestTransferItem(UInventoryComponent*InSourceInventory,int32 InSourceContainerUID,int32 InSourceSlotId,UInventoryComponent*InTargetInventory,
		int32 InTargetContainerUID,int32 InTargetSlotID,int32 InAmount,bool InbRotated);
private:

	/*服务器要初始化容器的一些操作*/
	void ServerInitInventory();
	
	/*初始化当前组件的库存*/
	TArray<FClientInventoryData> SetupInventory();

	/*当前组件的Slot有变化时,只有服务器接受*/
	UFUNCTION()
	void HandleSelfOnSlotUpdate(int32 ContainerUId,int32 InSlotId,FItemInfoDef InItemToAdd);
	
	/** 查找组件中的容器里 有没有空间存放这个东西
	 * @param InItemId			道具ID
	 * @param InAmount			道具数量
	 * @param OutContainerUID	可以存放的容器ID
	 * @param OutFoundSlotID	找到的容器中的SlotID
	 * @param OutRemainder		剩下的数量
	 * @param bOutRotated		旋转了吗
	 * @return bFound 有没有找到空闲的空间
	 */
	bool FindSpace(const FName&InItemId,const int32&InAmount,int32& OutContainerUID,int32&OutFoundSlotID,int32& OutRemainder,bool& bOutRotated);

	/** 容器里有没有足够的Slot空间存放这个东西，并且装入
	 * @param InContainer		容器
	 * @param InItemId			道具ID
	 * @param InAmount			道具当前的数量
	 * @param InbRotated		当前物体旋转了吗
	 * @param InbAnyRotation		这个物体可以旋转吗
	 * @param OutFoundSlotId	存放物体的插槽左上角ID
	 * @param OutbRotated		如果当前容器的数量不够存放这个道具,内部会进行尝试旋转,然后在存放一次,这是返回这个物体在背包内被旋转了没
	 * @param OutRemainder		当前容器存放完物体后,这个物体还剩余的数量,比如子弹
	 * @return bEnough			存放成功
	 */
	bool EnoughSpaceOnContainer(const FContainerInfo&InContainer,const FName&InItemId,const int32 &InAmount,const bool &InbRotated,const bool &InbAnyRotation,
	                            int32 &OutFoundSlotId,bool &OutbRotated,int32&OutRemainder);

	/** 尝试在已有道具上叠层(比如钱)
	 * @param InContainerUId	容器ID
	 * @param InSlotId			插槽左上脚的那个ID
	 * @param InItemId			道具ID
	 * @param InItemAmount		这个道具的数量
	 * @param OutRemainder		剩余多少没叠
	 * @return bStacked			叠加成功了吗
	 */
	bool TryToStack(const int32 &InContainerUId,const int32&InSlotId,const FName&InItemId,const int32& InItemAmount,int32 &OutRemainder);

	/** 当前容器可以存放这个尺寸的道具吗
	 * @param InSlotTile	从容器中的那个格子开始 
	 * @param InItemSize	道具的大小
	 * @param InContainer	要搜索的容器
	 * @return 是否可以装入这个尺寸的道具
	 */
	bool MountableOrNot(const FIntPoint&InSlotTile,const FIntPoint&InItemSize,const FContainerInfo&InContainer);

	/*通过容器ID,获取容器数组中管理的容器下标*/
	int32 GetContainerIndexByUId(const int32&InContainerId);

	/*通过容器ID,获取输入容器数组中管理的容器下标*/
	int32 GetContainerIndexByUId(const TArray<FContainerInfo>& InContainers,const int32&InContainerId) const;

	/*通过插槽ID,获取容器中对应的Items中的下标,通过下标去找ItemDef*/
	int32 GetItemIndexBySlotId(const FContainerInfo&InContainerInfo,const int32&InSlotId);

	/*	TODO:lzy 这个根据道具情况进行更改
	 *	创建对应的装备信息,如果这个道具没有装备信息返回空
	 */
	UEquipmentDefinition*CreateItemData(const FName&InItemId);

	/** 创建Item的容器,比如背包背心什么的
	 * @param InItemId			道具ID
	 * @param OutAssignedUIds	创建的那些容器ID
	 * @return 创建成功了吗
	 */
	bool CreateItemContaines(const FName&InItemId,TArray<int32>&OutAssignedUIds);

	/**	设置Item到容器
	 * @param InItemInfo		道具信息
	 * @param InContainerId		要往那个容器里设置
	 * @param InSlotId			容器的具体插槽
	 */
	bool SetItemOnContainer(const FItemInfoDef&InItemInfo,const int32&InContainerId,const int32&InSlotId);

	/** 通过Item在容器中的Id,获取他占据的每个格子的Index
	 * @param InItemContainerUId	容器Id
	 * @param InItemSlotId			在容器中这个道具的Id
	 * @return 这个道具占据的每个插槽
	 */
	TArray<int32> GetItemSlotIndexs(const int32& InItemContainerUId,const int32&InItemSlotId);

	/** 给某一个容器中的Item添加指定的数量
	 * @param InContainerUId	容器ID
	 * @param InSlotId			这个道具的SlotId
	 * @param InAddAmount		要添加的数量
	 */
	void AddItemAmount(const int32&InContainerUId,const int32 InSlotId,const int32&InAddAmount);

	/** 从容器中删除这个Slot下的道具
	 * @param InContainerId		容器ID
	 * @param InSlotId			SlotId
	 */
	void RemoveItem(const int32&InContainerId,const int32&InSlotId);

	/** 清理容器中这个Slot下的道具
	 * @param InContainerId		容器ID
	 * @param InSlotId			SlotId
	 */
	void ClearItemOnSlots(const int32&InContainerId,const int32&InSlotId);

	/** 设置容器中的这些插槽为空
	 * @param InContainerId		容器ID
	 * @param InSlotIndexs		要置为空的slotIndex
	 */
	void SetSlotsEmpty(const int32&InContainerId,const TArray<int32>&InSlotIndexs,const bool&InbSetEmpty = true);

	/** 递归查询当前库存中是否存在空间,并且尝试存放
	 * @param InItemId		要存放的道具ID
	 * @param InItemAmout	要存放的数量
	 * @param InLoopFuntion 每次存放完会要执行什么,会返回一个FFindResult
	 * @return 查找成功或者失败
	 */
	EFindState ForEachFoundSpace(const FName& InItemId,const int32&InItemAmout,TFunction<void(FFindResult FindResult)> InLoopFuntion);

	/** 移动道具
	 * @param InSourceContainerUId	这个道具归属的容器 
	 * @param InSourceSlotId		这个道具的所在的SlotId
	 * @param InTargetContainerUId	这个道具要到的容器
	 * @param InTargetSlotId		这个道具要到的SlotId
	 * @param InAmount				道具的数量
	 * @param InbRotated			这个道具旋转了吗
	 * @return 移动成功了吗
	 */
	bool MoveItem(const int32& InSourceContainerUId,const int32& InSourceSlotId,const int32& InTargetContainerUId,const int32& InTargetSlotId,const int32& InAmount,const bool& InbRotated);

	/** 获取嵌套的所有容器
	 * @param InUIds	要检查的容器
	 * @return 所有的容器
	 */
	TArray<int32> GetNestedContainerUIds(const TArray<int32>& InUIds);

	/** 当拖拽到一个插槽上进行检测，是否可以放入，可以就直接装入
	 * @param InItemToDrop			在拖拽的道具
	 * @param InSourceContainerUId	这个道具原来在的容器Id
	 * @param InTargetContainerUId	拖拽到的目标容器Id
	 * @param InTargetSlotId		拖拽到的目标SlotId
	 * @param OutDropContainerUId	返回实际这个物品装到那个容器Id
	 * @param OutDropSlotId			返回实际这个物品装到那个SlotId
	 * @param OutDropAmount			返回拖拽的数量
	 * @param OutbDropRotated		拖拽时他旋转了吗
	 * @return 成功拖拽到别的插槽了
	 */
	bool CanDropToSlot(const FItemInfoDef&InItemToDrop,const int32&InSourceContainerUId,const int32& InTargetContainerUId,const int32&InTargetSlotId,
	                   int32&OutDropContainerUId,int32&OutDropSlotId,int32&OutDropAmount,bool&OutbDropRotated);

	/** 尝试吧道具装入库存
	 * @param InItemToAdd				要装的道具 
	 * @param InParentItemContainerUID	要装在那(这个一般是背包/背心)的容器Id
	 * @param InParentItemSlotID		要装在那个具体的Slot
	 * @param OutContainerUID			实际装在那个容器里了
	 * @param OutSlotId					实际装入的Slot
	 * @param OutRemainder				剩余数量
	 * @param OutbRotated				旋转了吗
	 * @return 装成功了？
	 */
	bool CanAddInside(const FItemInfoDef&InItemToAdd,const int32&InParentItemContainerUID,const int32&InParentItemSlotID,int32 &OutContainerUID,
	                  int32 &OutSlotId,int32& OutRemainder,bool&OutbRotated);
	
	/** 这组容器是否全部为空
	 * @param InContainerUIDs	要检查的所有容器Id
	 * @return 全部为空 = true
	 */
	bool AreContainersEmpty(const TArray<int32>&InContainerUIDs);

	/** 更新容器中的信息
	 * @param InContainerUId	要更新的容器Id 
	 * @param InSlotId			要更新的SlotId
	 * @param InItemToAdd		要添加的道具
	 * @param InbIsLootWidget	战利品的Widget?
	 */
	void UpdateSlot(const int32& InContainerUId,const int32& InSlotId,const FItemInfoDef& InItemToAdd,const bool& InbIsLootWidget);
	
	/** 更新容器Widget里的信息
	 * @param InContainerUId		要更新的容器Id 
	 * @param InSlotId				具体的SlotId
	 * @param InItemToAdd			要更新成的道具
	 * @param InCurrentContainers	当前的容器
	 * @param InbIsLootWidget		是战利品容器吗
	 */
	void UpdateSlotWidget(const int32& InContainerUId,const int32& InSlotId,const FItemInfoDef& InItemToAdd,const TArray<FContainerInfo>& InCurrentContainers,const bool& InbIsLootWidget);

	/** 删除指定容器中的道具
	 * @param InContainerUId		容器Id 
	 * @param InSlotId				SlotId
	 * @param InCurrentContainers	当前拥有的Containers
	 * @param InbIsLootWidget		删除的是战利品widget吗?
	 */
	void RemoveItemWidget(int32 InContainerUId,int32 InSlotId, const TArray<FContainerInfo>& InCurrentContainers,bool InbIsLootWidget);
	
	/** 根据容器Id获取战利品容器(LootContainers)里的Widget
	 * @param InContainerUID	容器Id
	 * @return 容器Widget
	 */
	UContainer*GetLootContainerWidgetByUID(const int32&InContainerUID);

	/** 根据容器Id获取容器(ContainerWidgets)里的Widget
	 * @param InContainerUID	容器Id
	 * @return 容器Widget
	 */
	UContainer*GetContainerWidgetByUID(const int32&InContainerUID);

	/** 转移道具
	 * @param InSourceInventory 
	 * @param InSourceContainerUID 
	 * @param InSourceSlotId 
	 * @param InTargetInventory 
	 * @param InTargetContainerUID 
	 * @param InTargetSlotID 
	 * @param InAmount 
	 * @param InbRotated 
	 */
	void TransferItem(UInventoryComponent*InSourceInventory,const int32& InSourceContainerUID,const int32& InSourceSlotId,UInventoryComponent*InTargetInventory,
	                  const int32& InTargetContainerUID,const int32& InTargetSlotID,const int32& InAmount,const bool& InbRotated);
	
//射线检测
#pragma region InteractionTrace
protected:	
	UPROPERTY()
	FTimerHandle ClientTraceHandle;

	void ClientTrace();

	//扫到的对象
	UPROPERTY(BlueprintReadOnly,Category="Inventory|Trace")
	AActor*TraceActor;
#pragma endregion 

//InventoryUI相关
#pragma region InventoryUI

	//库存的UI指针
	UPROPERTY()
	UInventoryScreen*InventoryScreen;

	//战利品容器UI
	UPROPERTY()
	TArray<UContainer*> LootContainers;

	//当前拥有的容器UI
	UPROPERTY()
	TArray<UContainer*> ContainerWidgets;

	//改变颜色的所有Slot
	UPROPERTY()
	TArray<UInventorySlot*> ColorChangedSlots;
#pragma endregion 
};
