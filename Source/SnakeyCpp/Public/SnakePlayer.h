// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SnakePlayer.generated.h"

class UInputAction;
class UCameraComponent;
class UInputMappingContext;
class UStaticMeshComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class SNAKEYCPP_API ASnakePlayer : public APawn
{
	GENERATED_BODY()

public:
	ASnakePlayer();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UEnhancedInputComponent> SnakeInputComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> UpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> DownAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LeftAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> RightAction;

	///Value at start of game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	float BaseMovementSpeed = 100.0f;
	///Value at start of game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	float BaseTurnSpeed = 100.0f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void MoveSnekForward(float DeltaTime);
	void SetMovementSpeed(float Speed, bool bReset);
	void SetTurnSpeed(float Speed, bool bReset);
	
private:
	///Value we use and can modify
	float MovementSpeed = 100.0f;
	///Value we use and can modify
	float TurnSpeed = 100.0f;
};
