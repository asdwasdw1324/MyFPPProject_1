// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyFPPProjectGameMode.h"
#include "MyFPPProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyFPPProjectGameMode::AMyFPPProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

