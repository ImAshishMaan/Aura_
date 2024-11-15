#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor() {
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay() {
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass) {
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(TargetASC == nullptr) return;
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext(); // Context for the effect i.e. data for the effect like owner or instigator etc
	EffectContextHandle.AddSourceObject(this); // Adding what caused the effect
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle); // Spec for the effect
	
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get()); // * to get the actual spec data by dereferencing

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite; // checking the policy at runtime ... not changing it.
	if(bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor) {
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)	ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor) {
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	

	// Removing infinite effects on end overlap by looking at ActiveEffectHandles
	if(InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if(!IsValid(TargetASC)) return;
		
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for(TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair: ActiveEffectHandles) {
			if(TargetASC == HandlePair.Value) {
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for(FActiveGameplayEffectHandle& Handle: HandlesToRemove) {
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}
