// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeWall.h"

// Sets default values
ASnakeWall::ASnakeWall()
{
	PrimaryActorTick.bCanEverTick = false;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	SetRootComponent(WallMesh);
	
	WallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WallMesh->SetCollisionProfileName(TEXT("BlockAll"));
	WallMesh->SetMobility(EComponentMobility::Static);
}


