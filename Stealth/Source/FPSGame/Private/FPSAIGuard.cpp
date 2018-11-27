// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "FPSGameMode.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);
	GuardState = EAIState::Idle;
	PatrolLerpValue = 0;
	PatrolLerpSegment = 0;
	PatrolLerpIncrement = 0.005;
	PreviousPatrolState = EPatrolState::NotStarted;
	PatrolState = EPatrolState::NotStarted;
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 16, FColor::Red, false, 10.0f);
	AFPSGameMode* gameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode != nullptr)
	{
		gameMode->CompleteMission(Cast<APawn>(SeenPawn), false);
	}
	SetGuardState(EAIState::Alerted);
	SetPatrolState(EPatrolState::Stopped);
}

void AFPSAIGuard::OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), Location, 32.0f, 16, FColor::Green, false, 10.0f);
	FVector toNoiseMaker = Location - GetActorLocation();
	toNoiseMaker.Z = 0.0f; // Ignore pitch/roll
	FRotator rot = UKismetMathLibrary::MakeRotFromX(toNoiseMaker);
	rot.Normalize();
	SetActorRotation(rot);

	GetWorldTimerManager().ClearTimer(ResetOrientationTimer);
	GetWorldTimerManager().SetTimer(ResetOrientationTimer, this, &AFPSAIGuard::ResetOrientation, 3);

	SetGuardState(EAIState::Suspicious);
	SetPatrolState(EPatrolState::Paused);
}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);
	SetPatrolState(EPatrolState::InProgress);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}
	GuardState = NewState;
	OnStateChanged(NewState);
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();
}

void AFPSAIGuard::SetPatrolState(EPatrolState NewState)
{
	if (PatrolState == NewState || PatrolState == EPatrolState::Stopped)
	{
		return;
	}
	EPatrolState OldPatrolState = PatrolState;
	PatrolState = NewState;
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Patrol();
}

void AFPSAIGuard::AddPatrolTarget(FVector TargetPos)
{
	PatrolTargets.Add(TargetPos);
}

void AFPSAIGuard::SetPatrolSpeed(float Speed)
{
	PatrolLerpIncrement = Speed;
}

void AFPSAIGuard::Patrol()
{
	if (PatrolState != EPatrolState::InProgress)
	{
		return;
	}
	if (PatrolTargets.Num() < 0)
	{
		return;
	}

	if (PreviousPatrolState == EPatrolState::NotStarted)
	{
		// Initialize
		PatrolLerpValue = 0;
		PatrolLerpSegment = 0;
		FVector NewLocation = PatrolTargets[PatrolLerpSegment];
		NewLocation.Z = GetActorLocation().Z;
		SetActorLocation(NewLocation);
	}
	else if (PatrolTargets.Num() > 1)
	{
		// Increment
		PatrolLerpValue += PatrolLerpIncrement;
		if (PatrolLerpValue > 1.0f)
		{
			PatrolLerpValue = 0;
			PatrolLerpSegment = (PatrolLerpSegment + 1) % PatrolTargets.Num();
			PatrolLerpValue = 0.0f;
		}
		int IndexStart = PatrolLerpSegment;
		int IndexEnd = (PatrolLerpSegment + 1) % PatrolTargets.Num();
		FVector NewLocation = FMath::Lerp(PatrolTargets[IndexStart], PatrolTargets[IndexEnd], PatrolLerpValue);
		NewLocation.Z = GetActorLocation().Z;
		SetActorLocation(NewLocation);
	}
	PreviousPatrolState = PatrolState;
}

