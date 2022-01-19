// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ShipGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHIPS_API AShipGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AShipGameStateBase();

	// Resets the GameState so it's ready for a new game
	UFUNCTION(BlueprintCallable)
		void NewGame();

	// Returns if the player is alive
	UFUNCTION(BlueprintCallable)
		bool IsPlayerAlive();

	// Sets the player's state to dead
	void KillPlayer();

protected:
	// Stores if the player is alive
	bool PlayerIsAlive;
};
