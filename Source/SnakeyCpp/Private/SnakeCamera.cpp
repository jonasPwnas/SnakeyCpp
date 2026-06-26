// Fill out your copyright notice in the Description page of Project Settings.

#include "SnakeCamera.h"
#include "Camera/CameraComponent.h"

ASnakeCamera::ASnakeCamera()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraPivot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraPivot"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    SetRootComponent(CameraPivot);
    Camera->SetupAttachment(RootComponent);
}

void ASnakeCamera::BeginPlay()
{
    Super::BeginPlay();
}


void ASnakeCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!Camera) return;

    FRotator TargetRel = FRotator::ZeroRotator;

    if (TrackTarget)
    {
        // Direction from camera to target, in world space
        const FVector CamLoc = Camera->GetComponentLocation();
        const FVector ToTarget = TrackTarget->GetActorLocation() - CamLoc;

        // Convert to the camera's local space
        const FVector LocalDir =
            Camera->GetComponentTransform().InverseTransformVectorNoScale(ToTarget);

        float LeanYaw   = FMath::RadiansToDegrees(FMath::Atan2(LocalDir.Y, LocalDir.X));
        float LeanPitch = FMath::RadiansToDegrees(FMath::Atan2(LocalDir.Z, LocalDir.X));

        LeanYaw   = FMath::Clamp(LeanYaw,   -MaxLeanDegrees, MaxLeanDegrees);
        LeanPitch = FMath::Clamp(LeanPitch, -MaxLeanDegrees, MaxLeanDegrees);

        TargetRel = FRotator(LeanPitch, LeanYaw, 0.f);
    }
    // else: TargetRel stays ZeroRotator and eases back to rest

    // Smoothly interpolate toward the target (lean, or rest if no target)
    const FRotator CurrentRel = Camera->GetRelativeRotation();
    const FRotator NewRel = FMath::RInterpTo(CurrentRel, TargetRel, DeltaTime, LeanInterpSpeed);

    Camera->SetRelativeRotation(NewRel);
}