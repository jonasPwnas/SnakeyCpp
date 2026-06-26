// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnakeCamera.h"
#include "GameFramework/Pawn.h"
#include "SnakePlayer.generated.h"

class ASnakePhysBodyPart;
class UInputAction;
class UInputMappingContext;
class UStaticMeshComponent;
class USpringArmComponent;
class USphereComponent;
struct FInputActionValue;

UCLASS()
class SNAKEYCPP_API ASnakePlayer : public APawn
{
	GENERATED_BODY()

public:
	ASnakePlayer();
	
	//Body
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BodyMesh;
	
	//Collider/overlap
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> SteerAction;
	
	//Controls
	//Mapping context
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InputMapping;
	///Value at start of game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	float BaseMovementSpeed = 150.0f;
	///Value at start of game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	float BaseTurnSpeed = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	float FoodSpeedIncrease = 20.f;
	
	//BODDDY!!
	UPROPERTY(EditAnywhere, Category = "SNEK BODY")
	TSubclassOf<ASnakePhysBodyPart> BodyClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SNEK BODY")
	int32 StarterBodyAmount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SNEK BODY")
	float BodySpacing = 120.0f;
	UPROPERTY()
	TArray<ASnakePhysBodyPart*> BodyParts;
	
	UFUNCTION()
	void SpawnBodyParts(int32 BodiesToSpawn);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SetDesiredDirection(const FInputActionValue& InputValue);
	void SteerSnek(float DeltaTime);
	void MoveSnek(float DeltaTime);
	void SetMovementSpeed(float Speed, bool bReset);
	void SetTurnSpeed(float Speed, bool bReset);
	
private:
	//Input direction
	FVector2D DesiredDirection = FVector2D::ZeroVector;
	
	///Value we use and can modify
	float MovementSpeed = 0.0f;
	///Value we use and can modify
	float TurnSpeed = 0.0f;
	
	//Shared camera
	UPROPERTY()
	ASnakeCamera* SharedCamera;
	
};
