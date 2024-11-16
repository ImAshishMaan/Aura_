#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface {
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual void BeginPlay() override;

	//~ Begin IEnemyInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End IEnemyInterface

	//~ Begin ICombatInterface
	virtual int32 GetPlayerLevel() override;
	//~ End ICombatInterface

protected:

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defailts")
	int32 Level = 1;
	

};
