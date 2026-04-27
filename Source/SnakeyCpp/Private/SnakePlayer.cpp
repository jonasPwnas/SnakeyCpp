// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SnakePlayerState.h"

// Sets default values
ASnakePawn::ASnakePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

TArray<FIntPoint> ASnakePawn::GetAllOccupiedGridCells() const
{
	TArray<FIntPoint> Occupied = BodyGridPositions;
	Occupied.Insert(CurrentGridPosition, 0);
	return Occupied;
}

// Called when the game starts or when spawned
void ASnakePawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASnakePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASnakePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ForwardAction)
		{
			EnhancedInputComponent->BindAction(ForwardAction, ETriggerEvent::Triggered, this, &ASnakePawn::Input_TryTurnForward);
		}
		if (BackwardAction)
		{
			EnhancedInputComponent->BindAction(BackwardAction, ETriggerEvent::Triggered, this, &ASnakePawn::Input_TryTurnBackward);
		}
		
		if (UpAction)
		{
			EnhancedInputComponent->BindAction(UpAction, ETriggerEvent::Triggered, this, &ASnakePawn::Input_TryTurnUp);
		}
		if (DownAction)
		{
			EnhancedInputComponent->BindAction(DownAction, ETriggerEvent::Triggered, this, &ASnakePawn::Input_TryTurnDown);
		}

		if (LeftAction)
		{
			EnhancedInputComponent->BindAction(LeftAction, ETriggerEvent::Triggered, this, &ASnakePawn::Input_TryTurnLeft);
		}
		if (RightAction)
		{
			EnhancedInputComponent->BindAction(RightAction, ETriggerEvent::Triggered, this, &ASnakePawn::Input_TryTurnRight);
		}
	}
}

void ASnakePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetupEnhancedInput();
}

void ASnakePawn::PawnClientRestart()
{
	Super::PawnClientRestart();
	SetupEnhancedInput();
}

void ASnakePawn::SetupEnhancedInput()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (SnakeInputMapping)
				{
					Subsystem->AddMappingContext(SnakeInputMapping, 0); // We add the Input Mapping Context to the player's Enhanced Input Subsystem, with a priority of 0 (higher priority contexts will override lower priority ones if they have overlapping bindings)
				}
			}
		}
	}
}

FVector ASnakePawn::GetVectorFromDirection(ESnakeDirection Direction) const
{
	switch (Direction)
	{
		case ESnakeDirection::Forward: return FVector::ForwardVector;
		case ESnakeDirection::Backward: return FVector::BackwardVector;
		case ESnakeDirection::Right: return FVector::RightVector;
		case ESnakeDirection::Left: return FVector::LeftVector;
		case ESnakeDirection::Up: return FVector::UpVector;
		case ESnakeDirection::Down: return FVector::DownVector;
		default: return FVector::ZeroVector;
	}
}

FIntPoint ASnakePawn::DirectionToGridOffset(ESnakeDirection Direction) const
{
	switch (Direction)
	{
	case ESnakeDirection::Forward: return FIntPoint(1, 0);
	case ESnakeDirection::Backward: return FIntPoint(-1, 0);
	case ESnakeDirection::Left: return FIntPoint(0, -1);
	case ESnakeDirection::Right: return FIntPoint(0, 1);
		/*How should I handle height? Probably a seperate value for gridlike gravity that allows for jumping
	*case ESnakeDirection::Up: return FIntPoint(1, 0);
	*case ESnakeDirection::Down: return FIntPoint(-1, 0); */
	default: return FIntPoint(0, 0);
	}
}

FVector ASnakePawn::GridToWorldLocation(const FIntPoint& GridPosition) const
{
	return FVector::ZeroVector;
}

//Input
void ASnakePawn::Input_TryTurnForward(const FInputActionValue& Value)
{
	bool bPressed = Value.Get<bool>();
	if (bPressed)
	{
		RequestedDirection = ESnakeDirection::Forward;
	}
}
void ASnakePawn::Input_TryTurnBackward(const FInputActionValue& Value)
{
	bool bPressed = Value.Get<bool>();
	if (bPressed)
	{
		RequestedDirection = ESnakeDirection::Backward;
	}
}

void ASnakePawn::Input_TryTurnUp(const FInputActionValue& Value)
{
	bool bPressed = Value.Get<bool>();
	if (bPressed)
	{
		RequestedDirection = ESnakeDirection::Up;
	}
}
void ASnakePawn::Input_TryTurnDown(const FInputActionValue& Value)
{
	bool bPressed = Value.Get<bool>();
	if (bPressed)
	{
		RequestedDirection = ESnakeDirection::Down;
	}
}

void ASnakePawn::Input_TryTurnLeft(const FInputActionValue& Value)
{
	bool bPressed = Value.Get<bool>();
	if (bPressed)
	{
		RequestedDirection = ESnakeDirection::Left;
	}
}
void ASnakePawn::Input_TryTurnRight(const FInputActionValue& Value)
{
	bool bPressed = Value.Get<bool>();
	if (bPressed)
	{
		RequestedDirection = ESnakeDirection::Right;
	}
}

//Movement
void ASnakePawn::HandleDirectionChange()
{
	if (CurrentDirection != RequestedDirection && IsValidTurn(RequestedDirection))
	{
		CurrentDirection = RequestedDirection;
		UpdateDirection(CurrentDirection);
		UE_LOG(LogTemp, Type::Display, TEXT("Direction changed to: %s"), *UEnum::GetValueAsString(CurrentDirection));
	}
}

void ASnakePawn::UpdateDirection(ESnakeDirection NewDirection)
{
	switch (NewDirection)
	{
	case ESnakeDirection::Forward:	SetActorRotation(FRotator(0.f, 0.f, 0.f));
		break;
	case ESnakeDirection::Backward:	SetActorRotation(FRotator(0.f, 180.f, 0.f));
		break;
	case ESnakeDirection::Left:	SetActorRotation(FRotator(0.f, -90.f, 0.f));
		break;
	case ESnakeDirection::Right:SetActorRotation(FRotator(0.f, 90.f, 0.f));
		break;
		/* again whaaat to do. hmm
	case ESnakeDirection::Up:	SetActorRotation(FRotator(0.f, 0.f, 0.f));
		break;
	case ESnakeDirection::Down:	SetActorRotation(FRotator(0.f, 180.f, 0.f));
		break;
		*/
	}
}

void ASnakePawn::TickGridMovement(float DeltaTime)
{
	if (MoveStepTime <= 0.f)
	{
		return;
	}

	if (!bIsMovingToTarget)
	{
		// If at target, check for direction change and update target location
		StartNewMoveStep();
		if (!bIsMovingToTarget) //early out if not moving to target
		{
			return; 
		}
	}

	MoveInterpolationProgress += DeltaTime / MoveStepTime;
	const float Alpha = FMath::Clamp(MoveInterpolationProgress, 0.f, 1.f);

	// Use Lerp for constant speed across the cell
	const FVector NewHeadLocation = FMath::Lerp(StepStartWorldLocation, StepTargetWorldLocation, Alpha);
	SetActorLocation(NewHeadLocation, false);

	UpdateBodyVisuals(Alpha); 
	
	if (Alpha >= 1.f)
	{
		// We've reached the target grid location
		FinishMoveStep();
	}
}

void ASnakePawn::TickFreeMovement(float DeltaTime)
{
	FVector MovementVector = GetVectorFromDirection(CurrentDirection);
	FVector DesiredOffset = MovementVector * MoveSpeed * DeltaTime;
	AddActorWorldOffset(DesiredOffset, true);
}

bool ASnakePawn::IsValidTurn(ESnakeDirection NewDirection) const
{
	return false;
}

void ASnakePawn::DrawDebugInfo()
{
}

FIntPoint ASnakePawn::GetClampedStartGridPosition() const
{
	return FIntPoint(0, 0);
}

bool ASnakePawn::WouldHitWall(const FIntPoint& NextCell) const
{
	return false;
}

bool ASnakePawn::WouldHitSelf(const FIntPoint& NextCell) const
{
	return false;
}

//Snek Body
void ASnakePawn::StartNewMoveStep()
{
}

void ASnakePawn::FinishMoveStep()
{
}

void ASnakePawn::UpdateBodyVisuals(float Alpha)
{
}

void ASnakePawn::EnsureBodySegmentMeshCount()
{
}

void ASnakePawn::ClearBodyVisuals()
{
}

void ASnakePawn::GrowSnake(int32 Amount)
{
}

void ASnakePawn::AddInitialBodySegments(int32 NumSegments)
{
}

