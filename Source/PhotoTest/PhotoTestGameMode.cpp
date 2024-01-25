// Copyright Epic Games, Inc. All Rights Reserved.

#include "PhotoTestGameMode.h"
#include "PhotoTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

APhotoTestGameMode::APhotoTestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
