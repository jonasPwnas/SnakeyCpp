// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "SnakeGrid.h"
#include "GameFramework/Pawn.h"
#include "SnakePlayer.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UStaticMeshComponent;
class USpringArmComponent;
class USphereComponent;


UENUM(Blueprintable)
enum class ESnakeDirection : uint8
{
	Forward, Backward, Left, Right, Up, Down
};

UCLASS()
class SNAKEYCPP_API ASnakePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnakePawn();
	
	UFUNCTION(BlueprintPure, Category = "Snake")
	TArray<FIntPoint> GetAllOccupiedGridCells() const;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void PawnClientRestart() override;
	void SetupEnhancedInput();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float MoveStepTime = 0.2f; // Time it takes to move from one grid cell to the next when using grid movement.
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snake", meta = (AllowPrivateAccess = true))
	TArray<FIntPoint> BodyGridPositions;
	
	UPROPERTY()
	TObjectPtr<ASnakeGrid> SnakeGrid;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snake", meta = (AllowPrivateAccess = true))
	int32 PendingGrowth = 0;
	
	ESnakeDirection CurrentDirection = ESnakeDirection::Forward;
	ESnakeDirection RequestedDirection = ESnakeDirection::Forward;
	FIntPoint CurrentGridPosition = FIntPoint(0, 0);
	FIntPoint PendingNextGridPosition = FIntPoint(0, 0);
	
	FVector StepStartWorldLocation = FVector::ZeroVector;
	FVector StepTargetWorldLocation = FVector::ZeroVector;
	
	float MoveInterpolationProgress = 0.f;
	bool bIsMovingToTarget = false;
	bool bIsDead = false;
	FTimerHandle ResetTimerHandle;
	
	TArray<FIntPoint> PreviousBodyGridPositions;
	TArray<FIntPoint> TargetBodyGridPositions;
	
	UPROPERTY(Transient)
	TArray<TObjectPtr<UStaticMeshComponent>> BodyMeshes;
	
	//Helpers
	FVector GetVectorFromDirection(ESnakeDirection Direction) const;
	FIntPoint DirectionToGridOffset(ESnakeDirection Direction) const;
	FVector GridToWorldLocation(const FIntPoint& GridPosition) const; //Move this to grid actor

	// Input callback functions
	void Input_TryTurnForward(const FInputActionValue& Value);
	void Input_TryTurnBackward(const FInputActionValue& Value);
	void Input_TryTurnUp(const FInputActionValue& Value);
	void Input_TryTurnDown(const FInputActionValue& Value);
	void Input_TryTurnLeft(const FInputActionValue& Value);
	void Input_TryTurnRight(const FInputActionValue& Value);
	
	//Movement
	void HandleDirectionChange();
	void UpdateDirection(ESnakeDirection NewDirection);
	void TickGridMovement(float DeltaTime);
	void TickFreeMovement(float DeltaTime);
	bool IsValidTurn(ESnakeDirection NewDirection) const;
	void DrawDebugInfo();
	FIntPoint GetClampedStartGridPosition() const;
	bool WouldHitWall(const FIntPoint& NextCell) const;
	bool WouldHitSelf(const FIntPoint& NextCell) const;

	// Body system
	void StartNewMoveStep();
	void FinishMoveStep();
	void UpdateBodyVisuals(float Alpha);			
	void EnsureBodySegmentMeshCount();
	void ClearBodyVisuals();
	void GrowSnake(int32 Amount = 1);
	void AddInitialBodySegments(int32 NumSegments);
	
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> HeadMesh;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;
	
	//Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> SnakeInputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ForwardAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> BackwardAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> UpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DownAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LeftAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RightAction;
	
	// Movement / Grid
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bUseGridMovement = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	FIntPoint StartGridPosition = FIntPoint(10, 10);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	FVector GridOrigin = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true", ClampMin = "1.0", ClampMax = "100.0"))
	float CollisionSphereRadius = 32.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	bool bShowDebugCollision = true;

	// Body visuals settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Snake|Body", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMesh> BodySegmentMeshAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Snake|Body", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInterface> BodySegmentMaterial = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Snake", meta = (AllowPrivateAccess = "true"))
	int32 InitialBodyLength = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Snake|Body", meta = (AllowPrivateAccess = "true"))
	FVector BodySegmentMeshScale = FVector(0.9f, 0.9f, 0.9f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Snake|Body", meta = (AllowPrivateAccess = "true"))
	float BodySegmentMeshZOffset = 0.01f;
	
};
