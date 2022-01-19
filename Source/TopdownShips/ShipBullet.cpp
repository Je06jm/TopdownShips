// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipBullet.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ShipGameStateBase.h"
#include "BulletHitInterface.h"

// Sets default values
AShipBullet::AShipBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Setup our components
	RootComponent = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("RootComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	StaticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AShipBullet::BeginPlay()
{
	Super::BeginPlay();
	
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AShipBullet::OnOverlapBegin);
}

// Called every frame
void AShipBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AShipGameStateBase* GameState = GetWorld()->GetGameState<AShipGameStateBase>();
	// Move bullet
	if (GameState && GameState->IsPlayerAlive())
	{
		FVector NewLocation = GetActorLocation();
		NewLocation += GetActorForwardVector() * Speed * DeltaTime;
		SetActorLocation(NewLocation);
	}
}

void AShipBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Only process if the collider is not a bullet
	if (Cast<AShipBullet>(OtherActor) == nullptr)
	{
		// Call the collider's OnBulletHit if the function exists
		IBulletHitInterface* IBulletHit = Cast<IBulletHitInterface>(OtherActor);
		if (IBulletHit)
		{
			IBulletHit->Execute_OnBulletHit(OtherActor, OwningActor, 1.0f);
		}
		Destroy();
	}
}


// Sets our owning actor
bool AShipBullet::SetOwningActor(AActor* Actor)
{
	// Only set our owning actor if we don't already have one
	if ((OwningActor == nullptr) && Actor)
	{
		OwningActor = Actor;
		return true;
	}

	return false;
}

// Gets our owning actor
AActor* AShipBullet::GetOwningActor()
{
	return OwningActor;
}