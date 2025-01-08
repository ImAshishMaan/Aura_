#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot() {
	// TODO: Check slot status based on loaded data
	SetWidgetSwitcherIndex.Broadcast(2);
}

void UMVVM_LoadSlot::SetPlayerName(FString NewPlayerName) {
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, NewPlayerName);
}

void UMVVM_LoadSlot::SetLoadSlotName(FString NewLoadSlotName) {
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, NewLoadSlotName);
}
