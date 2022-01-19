// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipGameStateBase.h"

AShipGameStateBase::AShipGameStateBase()
{
	NewGame();
}

// Resets the GameState so it's ready for a new game
void AShipGameStateBase::NewGame()
{
	PlayerIsAlive = true;
}

// Returns if the player is alive
bool AShipGameStateBase::IsPlayerAlive()
{
	return PlayerIsAlive;
}

// Sets the player's state to dead
void AShipGameStateBase::KillPlayer()
{
	PlayerIsAlive = false;
}