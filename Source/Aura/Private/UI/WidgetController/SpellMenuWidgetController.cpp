#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues() {
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies() {
	GetAuraASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel) {
		// update selected ability struct and broadcast buttons state doing this because button only update when we click on ability in UI so update the ui even if menu is already open
		if(SelectedAbility.Ability.MatchesTagExact(AbilityTag)) {
			SelectedAbility.Status = StatusTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
		}

		if(AbilityInfo) {
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag); // get ability info from data asset
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints) {
		SpellPointsChanged.Broadcast(SpellPoints);

		// update selected ability struct and broadcast buttons state when spell points change
		CurrentSpellPoints = SpellPoints;

		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
	});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag) {
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);

	const FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;

	if(!bTagValid || bTagNone || !bSpecValid) {
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	} else {
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}

	// setting struct when we select an ability in UI
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip); // Check if buttons should be enabled based on ability status and spell points and set them b4 broadcasting
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip); // Broadcast buttons state 
}

void USpellMenuWidgetController::SpendPointButtonPressed() {
	if(GetAuraASC()) {
		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton) {
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped)) {
		bShouldEnableEquipButton = true;
		if(SpellPoints > 0) {
			bShouldEnableSpellPointsButton = true;
		}
	} else if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible)) {
		if(SpellPoints > 0) {
			bShouldEnableSpellPointsButton = true;
		}
	} else if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked)) {
		bShouldEnableEquipButton = true;
		if(SpellPoints > 0) {
			bShouldEnableSpellPointsButton = true;
		}
	}
}
