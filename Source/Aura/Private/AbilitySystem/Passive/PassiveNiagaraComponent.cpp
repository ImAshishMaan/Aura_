#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent() {
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay() {
	Super::BeginPlay();

	if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))) {
		AuraASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate); // Binding the delegate

		ActivateIfEquipped(AuraASC);
	} else if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner())) {
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC) {
			if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))) {
				AuraASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate); // Binding the delegate

				ActivateIfEquipped(AuraASC);
			}
		});
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate) {
	if(AbilityTag.MatchesTagExact(PassiveSpellTag)) {
		if(bActivate && !IsActive()) {
			Activate();
		} else {
			Deactivate();
		}
	}
}

// incase if our component is not active when we start the game we need to activate it
// may be all delegated got triggered before the component is active. etc reasons
void UPassiveNiagaraComponent::ActivateIfEquipped(UAuraAbilitySystemComponent* AuraASC) {
	const bool bStartupAbilitiesGiven = AuraASC->bStartupAbilitiesGiven;
	if(bStartupAbilitiesGiven) {
		if(AuraASC->GetStatusFromAbilityTag(PassiveSpellTag) == FAuraGameplayTags::Get().Abilities_Status_Equipped) {
			Activate();
		}
	}
}
