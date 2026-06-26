// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakePhysBodyPart.generated.h"

UCLASS()
class SNAKEYCPP_API ASnakePhysBodyPart : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASnakePhysBodyPart();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPhysicsConstraintComponent* BodyMeshConstraint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TwistLimitAngle = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SwingLimitAngle = 30.0f;
	
	void SetupConstraint(float HalfSegmentLength);
	void LinkToPreviousBody(UPrimitiveComponent* PrevBody);
};
