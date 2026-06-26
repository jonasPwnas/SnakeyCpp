// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallSpawner.generated.h"

class ASnakeWall;

UCLASS()
class SNAKEYCPP_API AWallSpawner : public AActor
{
	GENERATED_BODY()
public:
	AWallSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Arena")
	TSubclassOf<ASnakeWall> WallClass;

	UPROPERTY(EditAnywhere, Category="Arena")
	FVector2D FieldSize = FVector2D(2000.f, 2000.f);

	UPROPERTY(EditAnywhere, Category="Arena")
	float WallThickness = 50.f;

	UPROPERTY(EditAnywhere, Category="Arena")
	float WallHeight = 200.f;

	void SpawnWalls();
};
