#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
class UMVVM_LoadSlot;
class USaveGame;
class ULoadScreenSaveGame;
/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase {
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Information")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UFUNCTION (BlueprintCallable, Category="SaveGame")  
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;
};
