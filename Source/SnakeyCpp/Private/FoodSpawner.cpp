// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodSpawner.h"
#include "FoodActor.h"
#include "SnakePhysBodyPart.h"
#include "WallSpawner.h"
#include "SnakePlayer.h"
#include "SnakeWall.h"
#include "Kismet/GameplayStatics.h"

AFoodSpawner::AFoodSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AFoodSpawner::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), AWallSpawner::StaticClass()))
    {
        if (AWallSpawner* Walls = Cast<AWallSpawner>(Found))
        {
            FVector2D FullSize = Walls->GetFieldSize();
            
            FieldSize.X = FullSize.X - FieldWallOffset;
            FieldSize.Y = FullSize.Y - FieldWallOffset;
        }
    }
    
    if (!FoodClass) return;
    UWorld* World = GetWorld();
    if (!World) return;

    for (int32 i = 0; i < 2; i++)
    {
        const FVector Loc = GetValidFoodLocation();
        AFoodActor* Food = World->SpawnActor<AFoodActor>(
            FoodClass, Loc, FRotator::ZeroRotator);
        if (Food)
        {
            Food->OwningSpawner = this;
        }
    }
}

FVector AFoodSpawner::GetValidFoodLocation() const
{
    const UWorld* World = GetWorld();
    const FVector Center = GetActorLocation();
    const float HalfW = FieldSize.X * 0.5f;
    const float HalfD = FieldSize.Y * 0.5f;

    TArray<AActor*> Snakes;
    TArray<AActor*> Walls;
    //TArray<AActor*> Foods;
    if (World)
    {
        UGameplayStatics::GetAllActorsOfClass(World, ASnakePlayer::StaticClass(), Snakes);
        UGameplayStatics::GetAllActorsOfClass(World, ASnakeWall::StaticClass(), Walls);
        //UGameplayStatics::GetAllActorsOfClass(World, AFoodActor::StaticClass(), Foods);
    }

    FVector Candidate = FVector(Center.X, Center.Y, Center.Z + SpawnHeight);

    for (int32 Attempt = 0; Attempt < MaxPlacementAttempts; Attempt++)
    {
        Candidate = FVector(
            Center.X + FMath::FRandRange(-HalfW, HalfW),
            Center.Y + FMath::FRandRange(-HalfD, HalfD),
            Center.Z + SpawnHeight);

        bool bTooClose = false;


        for (AActor* Snake : Snakes)
        {
            if (!Snake) continue;

            if (FVector::Dist2D(Candidate, Snake->GetActorLocation()) < SnakeClearRadius)
            {
                bTooClose = true;
                break;
            }

            if (const ASnakePlayer* SP = Cast<ASnakePlayer>(Snake))
            {
                for (const AActor* Body : SP->BodyParts)
                {
                    if (Body &&
                        FVector::Dist2D(Candidate, Body->GetActorLocation()) < SnakeClearRadius)
                    {
                        bTooClose = true;
                        break;
                    }
                }
            }
            if (bTooClose) break;
        }
        
        if (!bTooClose)
        {
            for (AActor* Wall : Walls)
            {
                if (!Wall) continue;
                if (FVector::Dist2D(Candidate, Wall->GetActorLocation()) < WallClearRadius)
                {
                    bTooClose = true;
                    break;
                }
            }
        }

        if (!bTooClose)
        {
            return Candidate;
        }
    }

    return Candidate;
}