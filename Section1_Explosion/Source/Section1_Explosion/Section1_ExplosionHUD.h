// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Section1_ExplosionHUD.generated.h"

UCLASS()
class ASection1_ExplosionHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASection1_ExplosionHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

