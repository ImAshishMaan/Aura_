#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 *  Aura Gameplay Tags
 *
 *  Singleton containing native Gameplay Tags
 */
struct FAuraGameplayTags {
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializedNativeGameplayTags();

	FGameplayTag Attributes_Secondary_Armor;
protected:
private:
	static FAuraGameplayTags GameplayTags;
 
};
