// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipGameModeBase.h"
#include "PlayerShip.h"
#include "ShipGameStateBase.h"

AShipGameModeBase::AShipGameModeBase() : Super()
{
	// Sets default classes
	if (PlayerPawn)
	{
		DefaultPawnClass = PlayerPawn;
	}
	GameStateClass = AShipGameStateBase::StaticClass();
}