// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "SnakeGameMode.generated.h"

UCLASS()
class SNAKEYCPP_API ASnakeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
	
private:
	void DelayedPlayerSpawn();
};
