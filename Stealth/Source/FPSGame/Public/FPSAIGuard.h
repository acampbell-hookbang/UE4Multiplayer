// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted
};

UENUM(BlueprintType)
enum class EPatrolState : uint8
{
	NotStarted,
	InProgress,
	Paused,
	Stopped
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "Orientation")
	FRotator OriginalRotation;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* HeardPawn, const FVector & Location, float Volume);

	UFUNCTION()
	void ResetOrientation();

	FTimerHandle ResetOrientationTimer;
	
	// Guard Alertness
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

	void SetGuardState(EAIState NewState);

	EAIState GuardState;

	// Guard Patrol
	UPROPERTY(VisibleAnywhere, Category = "AI")
	float PatrolLerpValue;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	int PatrolLerpSegment;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	float PatrolLerpIncrement;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetPatrolState(EPatrolState NewState);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void AddPatrolTarget(FVector TargetPos);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetPatrolSpeed(float Speed);

	void Patrol();

	EPatrolState PreviousPatrolState;
	EPatrolState PatrolState;
	TArray<FVector> PatrolTargets;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
