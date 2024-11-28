#include "AbilitySystem/Abilities/AuraSummonAbility.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations() {
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions; // The angle between each minion spawn

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	TArray<FVector> SpawnLocations; // The locations that the minions will spawn at
	for(int32 i = 0; i < NumMinions; i++) {
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance); // The chosen spawn location of the minion
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility); // So it doesn't spawn under the ground if there is a stair or something
		if(Hit.bBlockingHit) {
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		SpawnLocations.Add(ChosenSpawnLocation);
	}

	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass() {
	return MinionClasses[FMath::RandRange(0, MinionClasses.Num() - 1)];
}
