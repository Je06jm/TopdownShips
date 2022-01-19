// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShipBullet.generated.h"

UCLASS()
class TOPDOWNSHIPS_API AShipBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShipBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Sets our owning actor
	bool SetOwningActor(AActor* Actor);

	// Gets our owning actor
	AActor* GetOwningActor();
protected:
	// Components
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
		class USphereComponent* Collision;

	// How fast we move
	UPROPERTY(EditAnywhere)
		float Speed = 100.0f;
	
	AActor* OwningActor;
};
