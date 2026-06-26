// Fill out your copyright notice in the Description page of Project Settings.

#include "SnakeCamera.h"
#include "Camera/CameraComponent.h"

ASnakeCamera::ASnakeCamera()
{
    PrimaryActorTick.bCanEverTick = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    SetRootComponent(Camera);

    // Look straight down (-Z). Pitch -90 points the camera's forward at the floor.
    Camera->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
}

void ASnakeCamera::BeginPlay()
{
    Super::BeginPlay();

    // Start at the zoomed-in height so the first frame isn't a jarring jump.
    CurrentHeight = MinHeight;
}

void ASnakeCamera::SetTargets(AActor* InTargetA, AActor* InTargetB)
{
    TargetA = InTargetA;
    TargetB = InTargetB;
}

void ASnakeCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!TargetA || !TargetB)
    {
        return; // nothing to frame yet
    }

    const FVector LocA = TargetA->GetActorLocation();
    const FVector LocB = TargetB->GetActorLocation();

    // Midpoint on the play plane.
    const FVector Center = (LocA + LocB) * 0.5f;

    // How far apart the pawns are (horizontal distance is what matters top-down).
    const float Separation = FVector::Dist2D(LocA, LocB);

    // Desired height: base zoomed-in height plus an amount scaled by separation,
    // clamped so it never goes nearer than MinHeight or further than MaxHeight.
    const float DesiredHeight = FMath::Clamp(
        MinHeight + (Separation + Padding) * ZoomFactor,
        MinHeight,
        MaxHeight);

    // Ease the height toward the desired value for smooth zoom.
    CurrentHeight = FMath::FInterpTo(CurrentHeight, DesiredHeight, DeltaTime, ZoomInterpSpeed);

    // Target position: directly above the midpoint at the current height.
    // Center.Z is used as the ground reference the camera floats above.
    const FVector DesiredLocation(Center.X, Center.Y, Center.Z + CurrentHeight);

    // Ease position so the camera glides rather than snapping to the midpoint.
    const FVector NewLocation = FMath::VInterpTo(
        GetActorLocation(), DesiredLocation, DeltaTime, PositionInterpSpeed);

    SetActorLocation(NewLocation);

    // Keep looking straight down regardless of any drift.
    SetActorRotation(FRotator(-90.f, 0.f, 0.f));
}

