// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjective.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSObjective::AFPSObjective()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void AFPSObjective::BeginPlay()
{
	Super::BeginPlay();
	PlayEffects();
}

void AFPSObjective::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

// Called every frame
void AFPSObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSObjective::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	PlayEffects();
}

