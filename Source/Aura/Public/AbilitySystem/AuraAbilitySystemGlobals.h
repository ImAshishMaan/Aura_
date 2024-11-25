#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals {
	GENERATED_BODY()

	// So we can use our custom gameplay effect context in the ability system (FAuraGameplayEffectContext)
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
