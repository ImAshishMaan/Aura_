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
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
	
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

void AAuraPlayerState::AddToAttributePoints(int32 InPoints) {
	AttributePoints += InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}
void AAuraPlayerState::AddToSpellPoints(int32 InPoints) {
	SpellPoints += InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
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
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP) {
	OnXPChangeDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints) {
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}
void AAuraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints) {
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
