// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SnakeGameMode.generated.h"

/**
 * this class should track:
 * - rules
 * - spawning one ore more pawns
 * - waiting for death(s) or for a timer to run out and then setting a winner
 * - Handles resetting of pawns for a rematch
 */
UCLASS()
class SNAKEYCPP_API ASnakeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
};
