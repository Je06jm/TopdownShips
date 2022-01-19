// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletHitInterface.h"
#include "EnemyShip.generated.h"

UCLASS()
class TOPDOWNSHIPS_API AEnemyShip : public AActor, public IBulletHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when we are hit by a bullet
	virtual void OnBulletHit_Implementation(AActor* BulletOwningActor, int Damage);

protected:
	// Components
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;
	
	// Reference to our bullet blueprint
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> BulletActor;

	// How much health we have
	UPROPERTY(EditAnywhere)
		int Health;

	// How many points to give the player when defeated
	UPROPERTY(EditAnywhere)
		int PointsWorth;

	// How fast we can move
	UPROPERTY(EditAnywhere)
		float Speed;

	// How close to the player must we be before we can fire
	UPROPERTY(EditAnywhere)
		float MinFireDistance;

	// How often we switch the state of firing
	UPROPERTY(EditAnywhere)
		float FireSwitchRate;

	// How much variance to the switching rate
	UPROPERTY(EditAnywhere)
		float FireSwitchRandom;

	FTimerHandle FireRandomHandle;

	void OnFireTimer();

	// How many bullets per second to fire
	UPROPERTY(EditAnywhere)
		float FireRate;

	FTimerHandle FireRateHandle;
	
	// Called when we fire a bullet
	void OnFireRate();

	// Reference to the player
	class APlayerShip* Player;

	// How far in front of us do we spawn bullets
	static const float SpawnBulletDistance;
};
