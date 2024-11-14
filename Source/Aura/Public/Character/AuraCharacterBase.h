#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

class USkeletalMeshComponent;

UCLASS()
class AURA_API AAuraCharacterBase : public ACharacter {
	GENERATED_BODY()

public:
	AAuraCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Component")
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
