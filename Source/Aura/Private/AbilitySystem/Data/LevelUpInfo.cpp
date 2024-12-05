#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const {
	int32 Level = 1;
	bool bSearching = true;
	while(bSearching) {
		// LevelUpInformation[1] = Level 1 Information
		// LevelUpInformation[2] = Level 1 Information
		if(LevelUpInformation.Num() - 1 <= Level) return Level; // Check if the level list is valid
		
		if(XP >= LevelUpInformation[Level].LevelUpRequirement) { // using array as level up information 
			++Level;
		} else {
			bSearching = false;
		}
	}
	return Level;
}
