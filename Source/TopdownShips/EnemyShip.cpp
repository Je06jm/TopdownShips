// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyShip.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "ShipGameStateBase.h"
#include "PlayerShip.h"
#include "ShipBullet.h"

const float AEnemyShip::SpawnBulletDistance = 110.0f;

// Sets default values
AEnemyShip::AEnemyShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));

	// Setup static mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyShip::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* Pawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
	Player = Cast<APlayerShip>(Pawn);
}

// Called every frame
void AEnemyShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AShipGameStateBase* GameState = GetWorld()->GetGameState<AShipGameStateBase>();

	if (GameState && GameState->IsPlayerAlive())
	{
		FVector PlayerDirection = {};
		FVector PlayerLocation = {};
		FVector Location = GetActorLocation();

		// Get direction to the player
		if (Player)
		{
			PlayerLocation = Player->GetActorLocation();
			PlayerDirection = PlayerLocation - Location;
		}

		// Move our actor
		{
			FVector Movement = PlayerDirection.GetSafeNormal();
			SetActorLocation(Location + Movement * Speed * DeltaTime);
		}

		// Look at player
		{
			float Angle = atan(PlayerDirection.Y / PlayerDirection.X);
			FRotator Rotation = GetActorRotation();
			Rotation.Yaw = FMath::RadiansToDegrees(Angle);
			if (PlayerDirection.X < 0.0)
			{
				// The rotation points in the opposite direction when X < 0
				Rotation.Yaw += 180.0f;
			}
			SetActorRotation(Rotation);
		}

		// Turn on/off timers based on distance
		{
			float Distance = FVector::Distance(PlayerLocation, Location);
			if ((Distance > MinFireDistance) && GetWorld()->GetTimerManager().IsTimerActive(FireRandomHandle))
			{
				GetWorld()->GetTimerManager().ClearTimer(FireRandomHandle);
			}
			else if ((Distance <= MinFireDistance) && !GetWorld()->GetTimerManager().IsTimerActive(FireRandomHandle)) {
				OnFireTimer();
			}
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(FireRandomHandle);
		GetWorld()->GetTimerManager().ClearTimer(FireRateHandle);
	}
}

// Called when a bullet hits us
void AEnemyShip::OnBulletHit_Implementation(AActor* BulletOwningActor, int Damage)
{
	// Only process damage if the bullet was fired from the player
	if (Cast<APlayerShip>(BulletOwningActor))
	{
		Health -= Damage;
		if (Health <= 0)
		{
			// We be dead
			if (Player)
			{
				Player->AddScore(PointsWorth);
			}

			Destroy();
		}
	}
}

// Handles the enabling/disabling of our firing
void AEnemyShip::OnFireTimer()
{
	// Switches the firing state to on/off
	if (GetWorld()->GetTimerManager().IsTimerActive(FireRateHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(FireRateHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(FireRateHandle, this, &AEnemyShip::OnFireRate, 1.0f / FireRate, true, 0.0f);
	}

	{
		// Sets a random timer of when to run this function again
		float NewTime = FireSwitchRate + FMath::FRand() * FireSwitchRandom;
		GetWorld()->GetTimerManager().SetTimer(FireRandomHandle, this, &AEnemyShip::OnFireTimer, NewTime, false);
	}
}

// Handles to firing of bullets
void AEnemyShip::OnFireRate()
{
	// Calculate the spawn point to be just in front of us
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation += GetActorForwardVector() * SpawnBulletDistance;

	FRotator SpawnRotation = GetActorRotation();

	if (BulletActor)
	{
		// Spawn the bullet and set us as it's owner
		AActor* AActorBullet = GetWorld()->SpawnActor<AActor>(BulletActor, SpawnLocation, SpawnRotation);
		AShipBullet* Bullet = Cast<AShipBullet>(AActorBullet);
		if (Bullet)
		{
			Bullet->SetOwningActor(Cast<AActor>(this));
		}
	}
}