// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

class APawn;
class USoundBase;

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AFPSGameMode();

	void CompleteMission(APawn* InstigatorPawn, bool bSuccess);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionCompleted(APawn* InstigatorPawn, bool bSuccess);

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* GameCompleteSound;

	UPROPERTY(VisibleAnywhere, Category = "GameMode")
	bool bIsMissionComplete;
};



