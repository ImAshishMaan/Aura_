#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"
class UAnimMontage;

// This class does not need to be modified.
// BlueprintType = Can be used in Blueprints by casting to this interface otherwise it will not work
UINTERFACE(MinimalAPI, BlueprintType) 
class UCombatInterface : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface {
	GENERATED_BODY()

public:
	virtual int32 GetPlayerLevel();
	virtual FVector GetCombatSocketLocation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();
};
