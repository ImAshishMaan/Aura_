#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState() {

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); // Mixed because its a player we need all players to have the same attribute set

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");                      
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

void AAuraPlayerState::AddToXP(int32 InXP) {
	XP += InXP;
	OnXPChangeDelegate.Broadcast(XP);
}

void AAuraPlayerState::AddToLevel(int32 InLevel) {
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::SetXP(int32 NewXP) {
	XP = NewXP;
	OnXPChangeDelegate.Broadcast(XP);
}

void AAuraPlayerState::SetLevel(int32 NewLevel) {
	Level = NewLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel) {
	
}

void AAuraPlayerState::OnRep_XP(int32 OldXP) {
	
}
