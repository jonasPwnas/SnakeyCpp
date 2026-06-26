// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeCamera.generated.h"

class UCameraComponent;

UCLASS()
class SNAKEYCPP_API ASnakeCamera : public AActor
{
    GENERATED_BODY()

public:
    ASnakeCamera();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraTracking")
    AActor* TrackTarget = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraTracking")
    float MaxLeanDegrees = 15.f;     // clamp, how far it can lean

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraTracking")
    float LeanInterpSpeed = 1.f;     // smoothing

    virtual void Tick(float DeltaTime) override;
    
protected:
    virtual void BeginPlay() override;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snake Camera")
    TObjectPtr<UCameraComponent> Camera;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snake Camera")
    TObjectPtr<USceneComponent> CameraPivot;
};