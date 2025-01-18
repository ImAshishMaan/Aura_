#pragma once

#include "CoreMinimal.h"
#include "CheckPoint/Checkpoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AMapEntrance : public ACheckpoint {
	GENERATED_BODY()

public:
	AMapEntrance(const FObjectInitializer& ObjectInitializer);
	
	/* Highlight Interface */
	virtual void HighlightActor_Implementation() override; // Highlight the Map Entrance regardless of whether it has been reached
	/* Highlight Interface */
	
	/* Save Interface */
	virtual void LoadActor_Implementation() override; // Do nothing when loading a Map Entrance (dont glow)
	/* end Save Interface */
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;
	
	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;

protected:
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult) override;
};
