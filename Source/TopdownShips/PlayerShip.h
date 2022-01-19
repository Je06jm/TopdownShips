// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BulletHitInterface.h"
#include "PlayerShip.generated.h"

UCLASS()
class TOPDOWNSHIPS_API APlayerShip : public APawn, public IBulletHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Called when we are hit by a bullet
	virtual void OnBulletHit_Implementation(AActor* BulletOwningActor, int Damage);

	// Called to update out score in the blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Player")
		void OnUpdateScore(int NewScore);
	
	// Called to update our health in the blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Player")
		void OnUpdateHealth(float Percentage);

	// Called to update our on screen position in the blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Player")
		void OnUpdatePlayerLocation(FVector2D Location);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when we gained score
	void AddScore(int AddedScore);

	// Returns our current score
	int GetScore();

protected:
	// Current score
	int Score;
	
	// Our initial health value
	UPROPERTY(EditAnywhere)
		int Health;

	// Our current health value
	int CurrentHealth;

	// Movement variables
	UPROPERTY(EditAnywhere)
		float RotationSpeed;

	// How fast we can move
	UPROPERTY(EditAnywhere)
		float MaxSpeed;

	// Components
	UPROPERTY(EditAnywhere)
		class USceneComponent* MeshRotator;

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	// Reference to the bullet blueprint
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> BulletActor;
	
	// Called when we die
	void KillPlayer();

	// Fire variables
	FTimerHandle FireTimerHandle;

	// How many bullets do we fire
	UPROPERTY(EditAnywhere)
		float FireRate;

	// Fire functions
	void OnFire();

	// Input variables
	FVector2D MovementInput;
	float RotationInput;

	// Input functions
	// Handles 'MoveForward' axis
	void MoveForward(float AxisValue);
	// Handles 'MoveRight' axis
	void MoveRight(float AxisValue);
	// Handles 'RotateRight' axis
	void RotateRight(float AxisValue);
	// Handles 'Fire' action pressed
	void FirePressed();
	// Handles 'Fire' action released
	void FireReleased();

	// Constant variables
	static const float SpawnBulletDistance;
};
