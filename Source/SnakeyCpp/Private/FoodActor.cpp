// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodActor.h"
#include "FoodSpawner.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AFoodActor::AFoodActor()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	SetRootComponent(OverlapSphere);
	OverlapSphere->SetSphereRadius(50.f);
	OverlapSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	OverlapSphere->SetGenerateOverlapEvents(true);

	FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoodMesh"));
	FoodMesh->SetupAttachment(RootComponent);
	FoodMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Tags.Add(FName("Food"));
}

void AFoodActor::Relocate()
{
	if (OwningSpawner)
	{
		SetActorLocation(OwningSpawner->GetValidFoodLocation());
	}
}

