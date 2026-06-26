// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoodSpawner.generated.h"

class AFoodActor;

UCLASS()
class AFoodSpawner : public AActor
{
	GENERATED_BODY()

public:
	AFoodSpawner();

	// Random point inside the field, not on a snake or other food.
	FVector GetValidFoodLocation() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Food")
	TSubclassOf<AFoodActor> FoodClass;

	UPROPERTY(EditAnywhere, Category = "Food")
	FVector2D FieldSize = FVector2D(2000.f, 2000.f);

	UPROPERTY(EditAnywhere, Category = "Food")
	float SpawnHeight = 50.f;

	UPROPERTY(EditAnywhere, Category = "Food")
	float SnakeClearRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = "Food")
	float FoodClearRadius = 100.f;

	UPROPERTY(EditAnywhere, Category = "Food")
	int32 MaxPlacementAttempts = 30;
	
	UPROPERTY(EditAnywhere, Category = "Food")
	float FieldWallOffset = 350.f;
};
