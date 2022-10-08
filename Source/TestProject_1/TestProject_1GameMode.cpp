// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestProject_1GameMode.h"
#include "TestProject_1Character.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/GameHUD.h"

ATestProject_1GameMode::ATestProject_1GameMode()
{

	HUDClass = AGameHUD::StaticClass();
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
