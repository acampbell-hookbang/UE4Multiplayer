// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "FPSCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

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

	UE_LOG(LogTemp, Warning, TEXT("AFPSExtractionZone: Overlap with Extraction Zone"));
	AFPSCharacter* myCharacter = Cast<AFPSCharacter>(OtherActor);
	if (myCharacter != nullptr)
	{
	}
}
