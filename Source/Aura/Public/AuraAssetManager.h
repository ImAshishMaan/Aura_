#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * Set this in DefaultEngine.ini (google it how)
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager {
	GENERATED_BODY()

public:
	static UAuraAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
	
};
