// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShip.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "ShipBullet.h"
#include "ShipGameStateBase.h"

// Sets static const values
const float APlayerShip::SpawnBulletDistance = 110.0f;

// Sets default values
APlayerShip::APlayerShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	MeshRotator = CreateDefaultSubobject<USceneComponent>(TEXT("MeshRotator"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	// Attach our components
	MeshRotator->SetupAttachment(RootComponent);
	StaticMesh->SetupAttachment(MeshRotator);
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	// Setup SpringArm
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-90.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 1600.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 10.0f;

	// Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerShip::BeginPlay()
{
	Super::BeginPlay();
	
	// Sets up collision
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &APlayerShip::OnOverlapBegin);
	
	// Sets current health
	CurrentHealth = Health;

	// Update UI
	OnUpdateScore(0);
	OnUpdateHealth(1.0f);
	
}

// Called every frame
void APlayerShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AShipGameStateBase* GameState = GetWorld()->GetGameState<AShipGameStateBase>();

	// Don't do anything if the GameState is not our GameState class or if the player is dead
	if (GameState && GameState->IsPlayerAlive())
	{
		MovementInput = MovementInput.GetSafeNormal();

			// Rotate our actor
		{
			FRotator NewRotation = MeshRotator->GetRelativeRotation();
				NewRotation.Yaw += RotationSpeed * RotationInput * DeltaTime;
				MeshRotator->SetRelativeRotation(NewRotation);
		}

		// Move our actor
		{
			FVector NewLocation = GetActorLocation();
			NewLocation.X += MaxSpeed * MovementInput.Y * DeltaTime;
			NewLocation.Y += MaxSpeed * MovementInput.X * DeltaTime;
			SetActorLocation(NewLocation);

			// Update UI's player location
			{
				const APlayerController* PlayerController = Cast<const APlayerController>(GetController());

				// Project our position onto the screen
				int32 ScreenWidth, ScreenHeight;
				PlayerController->GetViewportSize(ScreenWidth, ScreenHeight);

				FVector2D ScreenLocation;
				PlayerController->ProjectWorldLocationToScreen(NewLocation, ScreenLocation);

				int32 ScreenX = int32(ScreenLocation.X);
				int32 ScreenY = int32(ScreenLocation.Y);

				ScreenLocation.X = float(ScreenX);
				ScreenLocation.Y = float(ScreenY);

				OnUpdatePlayerLocation(ScreenLocation);
			}
		}
	}
	else if (GameState)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}
}

// Called to bind functionality to input
void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Hook up input events
	InputComponent->BindAxis("MoveForward", this, &APlayerShip::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerShip::MoveRight);
	InputComponent->BindAxis("RotateRight", this, &APlayerShip::RotateRight);

	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerShip::FirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &APlayerShip::FireReleased);
}

// Called when a bullet hits us
void APlayerShip::OnBulletHit_Implementation(AActor* BulletOwningActor, int Damage)
{
	// Apply damage and update UI
	CurrentHealth -= Damage;

	if (CurrentHealth <= 0)
	{
		KillPlayer();
	}
	OnUpdateHealth(float(CurrentHealth) / float(Health));
}


void APlayerShip::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Kill player if the collider has the tag "DestroyOnHit"
	if (OtherActor->ActorHasTag("DestroyOnHit"))
	{
		KillPlayer();
	}
}

// Adds score to the player
void APlayerShip::AddScore(int AddedScore)
{
	Score += AddedScore;
	OnUpdateScore(Score);
}

// Returns our current score
int APlayerShip::GetScore()
{
	return Score;
}

// Called when the player is killed
void APlayerShip::KillPlayer()
{
	// Updates our GameState to reflect that the player is dead
	AShipGameStateBase* GameState = GetWorld()->GetGameState<AShipGameStateBase>();
	if (GameState)
	{
		GameState->KillPlayer();
	}
	OnUpdateHealth(0.0f);
}

// Called when we fire
void APlayerShip::OnFire()
{
	// Calculate the bullet spawn location to be in front of us
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation += MeshRotator->GetForwardVector() * SpawnBulletDistance;

	FRotator SpawnRotation = GetActorRotation();
	SpawnRotation += MeshRotator->GetRelativeRotation();

	if (BulletActor)
	{
		// Spawn the bullet and set us as the owning actor
		AActor* AActorBullet = GetWorld()->SpawnActor<AActor>(BulletActor, SpawnLocation, SpawnRotation);
		AShipBullet* Bullet = Cast<AShipBullet>(AActorBullet);
		if (Bullet)
		{
			Bullet->SetOwningActor(Cast<AActor>(this));
		}
	}
}

// Handles 'MoveForward' axis
void APlayerShip::MoveForward(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

// Handles 'MoveRight' axis
void APlayerShip::MoveRight(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

// Handles 'RotateRight' axis
void APlayerShip::RotateRight(float AxisValue)
{
	RotationInput = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

// Handles 'Fire' action pressed
void APlayerShip::FirePressed()
{
	// Enables the timer that calls OnFire
	AShipGameStateBase* GameState = GetWorld()->GetGameState<AShipGameStateBase>();
	if (GameState && GameState->IsPlayerAlive())
	{
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &APlayerShip::OnFire, 1.0f / FireRate, true, 0.0f);
	}
}

// Handles 'Fire' action released
void APlayerShip::FireReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}