#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues() {
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies() {
	GetAuraASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag) {
		if(AbilityInfo) {
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag); // get ability info from data asset
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints) {
		SpellPointsChanged.Broadcast(SpellPoints);
	});
}
