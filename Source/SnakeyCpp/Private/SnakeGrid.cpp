// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGrid.h"


// Sets default values
ASnakeGrid::ASnakeGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASnakeGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnakeGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

