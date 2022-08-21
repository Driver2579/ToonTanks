// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATower::ATower() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireRange = 1000;
	FireRate = 2;

	Damage = 10;

	bInFireRange = false;
}

// Called when the game starts or when spawned
void ATower::BeginPlay() {
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	bCanFire = false;
}

// Called every frame
void ATower::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (InFireRange()) {
		RotateTurret(Tank->GetActorLocation());

		if (!bInFireRange) {
			GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, false);

			bInFireRange = true;
		}

		Fire();
	}

	else {
		bInFireRange = false;
		bCanFire = false;
	}
}

bool ATower::InFireRange() {
	if (Tank) {
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

		if (Distance <= FireRange) {
			return true;
		}
	}

	return false;
}

void ATower::CheckFireCondition() {
	if (Tank && Tank->IsAlive()) {
		bCanFire = true;
	}
}

void ATower::HandleDestruction() {
	Super::HandleDestruction();

	Destroy();
}