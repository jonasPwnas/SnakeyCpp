// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SnakeGameInstance.generated.h"

/**
 * persistent between map load/reload
 * Tracks match wins per player
 * selected game settings
 * player configs (color, name, skin)
 * rematch logic, if any
 */
UCLASS()
class SNAKEYCPP_API USnakeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
};
