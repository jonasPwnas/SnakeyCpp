// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ASnakePlayer::ASnakePlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASnakePlayer::BeginPlay()
{
	Super::BeginPlay();
	MovementSpeed = BaseMovementSpeed;
	TurnSpeed = BaseTurnSpeed;
}

// Called every frame
void ASnakePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveSnekForward(DeltaTime);
	
}

// Called to bind functionality to input
void ASnakePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASnakePlayer::MoveSnekForward(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * MovementSpeed * DeltaTime);
}

void ASnakePlayer::SetMovementSpeed(float Speed, bool bReset)
{
	if (bReset)
	{
		MovementSpeed = BaseMovementSpeed;
		return;
	}
	MovementSpeed = MovementSpeed + Speed;
}

void ASnakePlayer::SetTurnSpeed(float Speed, bool bReset)
{
	if (bReset)
	{
		TurnSpeed = BaseTurnSpeed;
		return;
	}
	TurnSpeed = TurnSpeed + Speed;
}

