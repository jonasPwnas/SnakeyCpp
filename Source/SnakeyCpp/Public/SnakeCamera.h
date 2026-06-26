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

    virtual void Tick(float DeltaTime) override;

    /** Register the two pawns this camera should frame. */
    UFUNCTION(BlueprintCallable, Category = "Snake Camera")
    void SetTargets(AActor* InTargetA, AActor* InTargetB);

protected:
    virtual void BeginPlay() override;

    /** The camera that players view through. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snake Camera")
    TObjectPtr<UCameraComponent> Camera;

    /** The two pawns to keep in frame. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snake Camera|Targets")
    TObjectPtr<AActor> TargetA;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snake Camera|Targets")
    TObjectPtr<AActor> TargetB;

    /** Closest the camera is allowed to get (the "zoomed in" point). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snake Camera|Zoom", meta = (ClampMin = "0.0"))
    float MinHeight = 1200.f;

    /** Furthest the camera is allowed to pull back. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snake Camera|Zoom", meta = (ClampMin = "0.0"))
    float MaxHeight = 6000.f;

    /**
     * Multiplier converting the distance between the two pawns into height.
     * Higher = pulls back sooner / harder as players separate.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snake Camera|Zoom", meta = (ClampMin = "0.0"))
    float ZoomFactor = 1.0f;

    /** Extra padding (uu) added to the separation so pawns aren't at the screen edge. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snake Camera|Zoom", meta = (ClampMin = "0.0"))
    float Padding = 500.f;

    /** How quickly the camera moves toward its target position. Higher = snappier. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snake Camera|Smoothing", meta = (ClampMin = "0.0"))
    float PositionInterpSpeed = 5.f;

    /** How quickly the camera zooms toward its target height. Higher = snappier. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snake Camera|Smoothing", meta = (ClampMin = "0.0"))
    float ZoomInterpSpeed = 4.f;

private:
    /** Current smoothed height, so zoom eases rather than snaps. */
    float CurrentHeight = 0.f;
};