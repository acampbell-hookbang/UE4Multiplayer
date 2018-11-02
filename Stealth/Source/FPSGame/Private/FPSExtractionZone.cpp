// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetBoxExtent(FVector(ZoneSize));
	OverlapComp->SetHiddenInGame(false);
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = OverlapComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(ZoneSize);
	DecalComp->SetupAttachment(RootComponent);
}

void AFPSExtractionZone::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);


	AFPSCharacter* myCharacter = Cast<AFPSCharacter>(OtherActor);
	if (myCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap with Extraction Zone: No Character"));
	}
	if (myCharacter != nullptr)
	{
		if (myCharacter->bIsCarryingObjective)
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlap with Extraction Zone: Character has Objective"));
			AFPSGameMode* gameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
			if (gameMode != nullptr)
			{
				gameMode->CompleteMission(Cast<APawn>(OtherActor), true);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlap with Extraction Zone: Character does not have Objective"));
			UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
		}
	}
}
