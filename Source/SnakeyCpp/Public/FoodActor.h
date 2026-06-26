// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoodActor.generated.h"

class USphereComponent;
class AFoodSpawner;

UCLASS()
class AFoodActor : public AActor
{
	GENERATED_BODY()

public:
	AFoodActor();
	
	UPROPERTY()
	AFoodSpawner* OwningSpawner = nullptr;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FoodMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* OverlapSphere;
	
	void Relocate();
};