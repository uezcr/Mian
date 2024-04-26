#pragma once

#include "InventoryType.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

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

	/*Debug 打印信息*/
	UFUNCTION(BlueprintCallable,Category="Inventory")
	void PrintDebug(FString InContent,FColor InColor=FColor::Blue,int32 InKey=1,float InPrintTime = 5.f);

//PRC	
	//请求交互当前射线扫到的Actor
	UFUNCTION(Server,Reliable,BlueprintCallable,Category="Inventory|RPC")
	void Server_Interactable(AActor*InActor,EInteractionType InInteractionType);

	/*服务器添加容器后,同步给本机的容器信息*/
	UFUNCTION(Client,Reliable,BlueprintCallable,Category="Inventory|RPC")
	void Client_InitWidgets(FClientInventoryData InClientInventoryData);
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

	/*通过插槽ID,获取容器中对应的Items中的下标,通过下标去找ItemDef*/
	int32 GetItemIndexBySlotId(const FContainerInfo&InContainerInfo,const int32&InSlotId);

	/** 通过容器ID,和插槽ID获取对应的Item
	 * @param InContainerId 容器ID
	 * @param InSlotId		插槽ID
	 * @return ItemInfo 道具信息
	 */
	FItemInfoDef GetItemByUIdAndSlotId(const int32&InContainerId,const int32&InSlotId);

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
	void SetItemOnContainer(const FItemInfoDef&InItemInfo,const int32&InContainerId,const int32&InSlotId);

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
#pragma endregion 
};
