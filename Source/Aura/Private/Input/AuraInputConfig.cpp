#include "Input/AuraInputConfig.h"
const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const {
	for(const FAuraInputAction& Action: AbilityInputActions) {
		if(Action.InputTag.MatchesTagExact(InputTag)) {
			return Action.InputAction;
		}
	}
	if(bLogNotFound) {
		UE_LOG(LogTemp, Error, TEXT("Can't find InputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
