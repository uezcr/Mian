#include "MGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"
#include "MLogChannels.h"

namespace MGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Strength, "Attribute.Primary.Strength", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Dexterity, "Attribute.Primary.Dexterity", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Constitution, "Attribute.Primary.Constitution", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Intelligence, "Attribute.Primary.Intelligence", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Wisdom, "Attribute.Primary.Wisdom", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Spirit, "Attribute.Primary.Spirit", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_IsDead, "Ability.ActivateFail.IsDead", "Ability failed to activate because its owner is dead.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cooldown, "Ability.ActivateFail.Cooldown", "Ability failed to activate because it is on cool down.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cost, "Ability.ActivateFail.Cost", "Ability failed to activate because it did not pass the cost checks.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_TagsBlocked, "Ability.ActivateFail.TagsBlocked", "Ability failed to activate because tags are blocking it.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_TagsMissing, "Ability.ActivateFail.TagsMissing", "Ability failed to activate because tags are missing.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Networking, "Ability.ActivateFail.Networking", "Ability failed to activate because it did not pass the network checks.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_ActivationGroup, "Ability.ActivateFail.ActivationGroup", "Ability failed to activate because of its activation group.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Behavior_SurvivesDeath, "Ability.Behavior.SurvivesDeath", "An ability with this type tag should not be canceled due to death.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Stick, "InputTag.Look.Stick", "Look (stick) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch, "InputTag.Crouch", "Crouch input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_AutoRun, "InputTag.AutoRun", "Auto-run input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Inventory, "InputTag.Inventory", "Inventory input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Pickup, "InputTag.Pickup", "Pickup input.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Crouching, "Status.Crouching", "Target is crouching.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_AutoRunning, "Status.AutoRunning", "Target is auto-running.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death, "Status.Death", "Target has the death status.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Dying, "Status.Death.Dying", "Target has begun the death process.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Dead, "Status.Death.Dead", "Target has finished the death process.");
						  
	// These are mapped to the movement modes inside GetMovementModeTagMap()
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Walking, "Movement.Mode.Walking", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_NavWalking, "Movement.Mode.NavWalking", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Falling, "Movement.Mode.Falling", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Swimming, "Movement.Mode.Swimming", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Flying, "Movement.Mode.Flying", "Default Character movement tag");

	// When extending Lyra, you can create your own movement modes but you need to update GetCustomMovementModeTagMap()
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Custom, "Movement.Mode.Custom", "This is invalid and should be replaced with custom tags.  See LyraGameplayTags::CustomMovementModeTagMap.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Death, "GameplayEvent.Death", "Event that fires on death. This event only fires on the server.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Reset, "GameplayEvent.Reset", "Event that fires once a player reset is executed.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_RequestReset, "GameplayEvent.RequestReset", "Event to request a player's pawn to be instantly replaced with a new one at a valid spawn location.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Inventory_Slot_InventorySlot, "Inventory.Slot.InventorySlot", "库存插槽");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Inventory_Slot_Headphone, "Inventory.Slot.Headphone", "耳机");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Inventory_Slot_Head, "Inventory.Slot.Head", "头");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Inventory_Slot_Body, "Inventory.Slot.Body", "身体");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Inventory_Slot_Pants, "Inventory.Slot.Pants", "裤子");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Inventory_Slot_Weapon, "Inventory.Slot.Weapon", "武器");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Inventory_Slot_Backpack, "Inventory.Slot.Backpack", "背包");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Inventory_Slot_Vest, "Inventory.Slot.Vest", "防弹背心");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Inventory_Slot_Hand, "Inventory.Slot.Hand", "手");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Inventory_Slot_Shoes, "Inventory.Slot.Shoes", "鞋子");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Inventory_Slot_CarFront, "Inventory.Slot.CarFront", "汽车（前盖）");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Inventory_Slot_CarTop, "Inventory.Slot.CarTop", "汽车（车尾）");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Inventory_Slot_CarLight, "Inventory.Slot.CarLight", "汽车（灯）");
	// Unreal Movement Modes
	const TMap<uint8, FGameplayTag> MovementModeTagMap =
	{
		{ MOVE_Walking, Movement_Mode_Walking },
		{ MOVE_NavWalking, Movement_Mode_NavWalking },
		{ MOVE_Falling, Movement_Mode_Falling },
		{ MOVE_Swimming, Movement_Mode_Swimming },
		{ MOVE_Flying, Movement_Mode_Flying },
		{ MOVE_Custom, Movement_Mode_Custom }
	};

	// Custom Movement Modes
	const TMap<uint8, FGameplayTag> CustomMovementModeTagMap =
	{
		// Fill these in with your custom modes
	};

	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
		FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

		if (!Tag.IsValid() && bMatchPartialString)
		{
			FGameplayTagContainer AllTags;
			Manager.RequestAllGameplayTags(AllTags, true);

			for (const FGameplayTag& TestTag : AllTags)
			{
				if (TestTag.ToString().Contains(TagString))
				{
					UE_LOG(LogMian, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
					Tag = TestTag;
					break;
				}
			}
		}

		return Tag;
	}
}
