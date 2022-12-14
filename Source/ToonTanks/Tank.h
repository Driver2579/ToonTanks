// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	ATank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComponent;

	bool bAlive;

	// === Movement ===

	void Move(float Value);

	void EnableSprint();
	void DisableSprint();

	void Turn(float Value);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed;

	bool bSprintEnabled;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintCoefficient;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate;

	FVector DeltaLocation;
	FRotator DeltaRotation;

	// === Mouse events ===

	// Rotate turret
	APlayerController* TankPlayerController;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const;

	bool IsAlive();
};