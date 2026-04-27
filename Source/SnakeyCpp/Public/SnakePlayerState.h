// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SnakePlayerState.generated.h"

/**
 * Per player tracking of:
 * - Current Score
 * - current death
 * - player index or other individual tracking for spawning and visuals
 */
UCLASS()
class SNAKEYCPP_API ASnakePlayerState : public APlayerState
{
	GENERATED_BODY()
	
};
