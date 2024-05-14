#pragma once

#include "NativeGameplayTags.h"

namespace MGameplayTags
{
	MIAN_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// Declare all of the custom native tags that Game will use
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Strength);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Dexterity);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Constitution);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Intelligence);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Wisdom);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Spirit);

	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);

	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Inventory);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Pickup);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_NormalAttack);
	
	// These are mappings from MovementMode enums to GameplayTags associated with those enums (below)
	MIAN_API	extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	MIAN_API	extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;

	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_NavWalking);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Swimming);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Flying);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Custom);
	
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_AutoRunning);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);
	
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Slot_InventorySlot);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Slot_Headphone);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Slot_Head);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Slot_Body);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Slot_Pants);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Slot_Weapon);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Slot_Backpack);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Slot_Vest);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Slot_Hand);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Slot_Shoes);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Slot_CarFront);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Slot_CarTop);
	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Slot_CarLight);

	MIAN_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTree_AIState_Patrol);
	
};
