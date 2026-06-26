// Fill out your copyright notice in the Description page of Project Settings.


#include "WallSpawner.h"
#include "SnakeWall.h"

AWallSpawner::AWallSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

FVector2D AWallSpawner::GetFieldSize()
{
	return FieldSize;
}

void AWallSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnWalls();
}

void AWallSpawner::SpawnWalls()
{
	if (!WallClass) return;
	UWorld* World = GetWorld();
	if (!World) return;

	const float HalfW = FieldSize.X * 0.5f;
	const float HalfD = FieldSize.Y * 0.5f;

	const FVector Center = GetActorLocation();

	auto SpawnWall = [&](FVector Location, FVector Size)
	{
		ASnakeWall* Wall = World->SpawnActor<ASnakeWall>(
			WallClass, Location, FRotator::ZeroRotator);
		if (Wall)
		{
			Wall->WallMesh->SetWorldScale3D(Size / 100.f);
		}
	};

	const float Z = Center.Z + WallHeight * 0.5f;

	SpawnWall(FVector(Center.X, Center.Y + HalfD, Z),
			  FVector(FieldSize.X + WallThickness * 2, WallThickness, WallHeight));
	SpawnWall(FVector(Center.X, Center.Y - HalfD, Z),
			  FVector(FieldSize.X + WallThickness * 2, WallThickness, WallHeight));
	SpawnWall(FVector(Center.X + HalfW, Center.Y, Z),
			  FVector(WallThickness, FieldSize.Y, WallHeight));
	SpawnWall(FVector(Center.X - HalfW, Center.Y, Z),
			  FVector(WallThickness, FieldSize.Y, WallHeight));
}

