#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

// Creates an instance of the TargetDataUnderMouse ability task
UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility) {
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

// Activates the ability task and handles input from mouse cursor or waits for data on the server
void UTargetDataUnderMouse::Activate() {
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if(bIsLocallyControlled) {
		// If locally controlled, send mouse cursor data to the server
		SendMouseCursorData();
	} else {
		// We are on the server, so set up to listen for target data from the client
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey(); // Used for synchronizing actions between the client and the server

		// Add a delegate to be called when target data is received
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		// Check if target data has already been set and call the delegate if so
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if(!bCalledDelegate) {
			// If target data is not available, set up to wait for remote player data
			SetWaitingOnRemotePlayerData();
		}
	}
}

// Sends mouse cursor hit data to the server and broadcasts it to clients
void UTargetDataUnderMouse::SendMouseCursorData() {
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get()); // Ensures correct prediction key usage on the server

	// Retrieve the player controller and perform a hit test under the mouse cursor
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	// Create target data from the hit result
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	// Sending cursor data to the server for replication
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
	);

	// Broadcasting cursor data to other clients
	if(ShouldBroadcastAbilityTaskDelegates()) {
		ValidData.Broadcast(DataHandle);
	}
}

// Called on the server when target data is replicated from the client
void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag) const {
	// Consumes the replicated target data to ensure it is not used again on the server and release it from cached data
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	// Broadcasts the valid target data to listeners if necessary
	if(ShouldBroadcastAbilityTaskDelegates()) {
		ValidData.Broadcast(DataHandle);
	}
}
