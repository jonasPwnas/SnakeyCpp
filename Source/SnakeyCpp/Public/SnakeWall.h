// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeWall.generated.h"

UCLASS()
class SNAKEYCPP_API ASnakeWall : public AActor
{
	GENERATED_BODY()
public:
	ASnakeWall();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WallMesh;
};