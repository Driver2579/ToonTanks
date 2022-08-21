// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksPlayerController.h"

// Sets default values
ATank::ATank() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);

	Speed = 250;

	bSprintEnabled = false;
	SprintCoefficient = 1.5;

	TurnRate = 30;

	FireRate = 0.5;
	Damage = 25;

	DeltaLocation = FVector::ZeroVector;
	DeltaRotation = FRotator::ZeroRotator;

	bAlive = true;
}

// Called when the game starts or when spawned
void ATank::BeginPlay() {
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());
}

// Called every frame
void ATank::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// Movement
	if (DeltaLocation.X != 0) {
		AddActorLocalOffset(DeltaLocation, true);

		if (DeltaRotation.Yaw != 0) {
			if (DeltaLocation.X < 0) {
				DeltaRotation.Yaw *= -1;
			}

			AddActorLocalRotation(DeltaRotation, true);
		}
	}

	// Mouse movement
	if (TankPlayerController) {
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
	
		RotateTurret(HitResult.ImpactPoint);
	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATank::Move);
	PlayerInputComponent->BindAxis("Turn", this, &ATank::Turn);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATank::Fire);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATank::EnableSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATank::DisableSprint);
}

void ATank::Move(float Value) {
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	DeltaLocation.X = Value * Speed * DeltaTime;

	if (bSprintEnabled) {
		DeltaLocation.X *= SprintCoefficient;
	}
}

void ATank::EnableSprint() {
	bSprintEnabled = true;
}


void ATank::DisableSprint() {
	bSprintEnabled = false;
}

void ATank::Turn(float Value) {
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	DeltaRotation.Yaw = Value * TurnRate * DeltaTime;
}

void ATank::HandleDestruction() {
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);

	bAlive = false;
}

APlayerController* ATank::GetTankPlayerController() const {
	return TankPlayerController;
}

bool ATank::IsAlive() {
	return bAlive;
}