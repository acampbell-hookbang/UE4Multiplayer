// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Section1_ExplosionGameMode.h"
#include "Section1_ExplosionHUD.h"
#include "Section1_ExplosionCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASection1_ExplosionGameMode::ASection1_ExplosionGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASection1_ExplosionHUD::StaticClass();
}
