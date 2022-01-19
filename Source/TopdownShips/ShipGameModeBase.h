// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShipGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHIPS_API AShipGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AShipGameModeBase();

protected:
	// Default player pawn
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APawn> PlayerPawn;
};
