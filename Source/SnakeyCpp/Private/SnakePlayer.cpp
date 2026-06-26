// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FoodActor.h"
#include "SnakePhysBodyPart.h"
#include "Components/SphereComponent.h"
#include "InputActionValue.h"
#include "SnakeGameMode.h"
#include "ViewportInteractionTypes.h"
#include "Kismet/GameplayStatics.h"

FOnSnekDied ASnakePlayer::OnAnySnekDied;
FOnSnekGrow ASnakePlayer::OnAnySnekGrew;

// Sets default values
ASnakePlayer::ASnakePlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	SetRootComponent(BodyMesh);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(BodyMesh);
	CollisionSphere->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ASnakePlayer::BeginPlay()
{
	SetActorTickEnabled(false);
	Super::BeginPlay();
	
	//Bind events
	AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), ASnakeGameMode::StaticClass());
	if (ASnakeGameMode* Mode = Cast<ASnakeGameMode>(Found))
	{
		Mode->OnStart.AddDynamic(this, &ASnakePlayer::AllowMovement);
		StarterBodyAmount = Mode->SnakeStartLength;
	}
	
	//Movement setup
	MovementSpeed = BaseMovementSpeed;
	TurnSpeed = BaseTurnSpeed;
	
	//Bind overlap
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASnakePlayer::OnHeadOverlap);
	
	//Spawn initial bodies
	UWorld* World = GetWorld();
	UPrimitiveComponent* AttachTo = BodyMesh;
	FVector TailLocation = GetActorLocation();
	const FRotator SnakeRotation = GetActorRotation();
	FVector SpawnLocation = TailLocation - GetActorForwardVector() * BodySpacing;
	ASnakePhysBodyPart* NewBody = World->SpawnActor<ASnakePhysBodyPart>
	(
		BodyClass, SpawnLocation, SnakeRotation
	);
	if (!NewBody) return;
	NewBody->SetupConstraint(100.f);
	NewBody->LinkToPreviousBody(AttachTo);
	BodyParts.Add(NewBody);
	if (StarterBodyAmount > 1)
		SpawnBodyParts(StarterBodyAmount - 1);
	
}

void ASnakePlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AActor* camera = UGameplayStatics::GetActorOfClass(GetWorld(), ASnakeCamera::StaticClass());
	
	SharedCamera = Cast<ASnakeCamera>(camera);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->bAutoManageActiveCameraTarget = false;
		PlayerController->SetViewTargetWithBlend(SharedCamera, 0.1f);
	}
}

// Called every frame
void ASnakePlayer::Tick(float DeltaTime)
{
	if (!bCanMove) return;
	
	Super::Tick(DeltaTime);
	SteerSnek(DeltaTime);
	MoveSnek(DeltaTime);
}

void ASnakePlayer::AllowMovement(bool bAllow)
{
	SetActorTickEnabled(bAllow);
}

void ASnakePlayer::SetDesiredDirection(const FInputActionValue& InputValue)
{
	if (!InputValue.Get<FVector2D>().IsNearlyZero())
		DesiredDirection = InputValue.Get<FVector2D>();
	
}

void ASnakePlayer::SpawnBodyParts(int32 BodiesToSpawn)
{
	if (!BodyClass) return;
	UWorld* World = GetWorld();
	
	const FRotator SnakeRotation = GetActorRotation();
	const FVector  BackDir       = -GetActorForwardVector();

	for (int32 i = 0; i < BodiesToSpawn; i++)
	{
		ASnakePhysBodyPart* LastBody = BodyParts.Last();
		UPrimitiveComponent* AttachTo = LastBody->BodyMesh;

		FVector SpawnLocation = LastBody->GetActorLocation() + BackDir * BodySpacing;

		ASnakePhysBodyPart* NewBody = World->SpawnActor<ASnakePhysBodyPart>(
			BodyClass, SpawnLocation, SnakeRotation);

		if (!NewBody) return;

		NewBody->SetupConstraint(BodySpacing * 0.5f);
		NewBody->LinkToPreviousBody(AttachTo);
		BodyParts.Add(NewBody);
		SetMovementSpeed(FoodSpeedIncrease, false);
	}
}

// Called to bind functionality to input
void ASnakePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//Input setup
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ASnakePlayer::SetDesiredDirection);
		EIC->BindAction(SteerAction, ETriggerEvent::Completed, this, &ASnakePlayer::SetDesiredDirection);
	}
	
}

void ASnakePlayer::OnHeadOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep)
{
	if (!OtherActor || OtherActor == this) return;
	
	if (OtherActor->ActorHasTag("Food"))
	{
		if (AFoodActor* Food = Cast<AFoodActor>(OtherActor))
		{
			Food->Relocate();
			SpawnBodyParts(1);
			OnAnySnekGrew.Broadcast(this);
			return;
		}
	}
	
	//Would have liked to do something cool when eating anothers body,
	//like just clipping that snake there, buuuut I have no time left :')
	if (OtherActor->ActorHasTag("Wall"))
	{
		Die();
		return;
	}
	
	if (OtherActor->ActorHasTag("Body"))
	{
		Die();
	}
	
}

void ASnakePlayer::Die()
{
	OnAnySnekDied.Broadcast(this);
	GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Red, FString("Oooof biiiig death"));
}

void ASnakePlayer::ApplyPlayer2HeadMaterial()
{
	if (Player2HeadMaterial)
		BodyMesh->SetMaterial(0, Player2HeadMaterial);
}

void ASnakePlayer::SteerSnek(float DeltaTime)
{
	if (!DesiredDirection.IsNearlyZero())
	{
		float TargetYaw = FMath::RadiansToDegrees(
			FMath::Atan2(DesiredDirection.Y, -DesiredDirection.X));

		FRotator CurrentRot = GetActorRotation();
		FRotator TargetRot(0.f, TargetYaw, 0.f);

		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, TurnSpeed);
		SetActorRotation(NewRot);
	}
}

void ASnakePlayer::MoveSnek(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * MovementSpeed * DeltaTime);
}

void ASnakePlayer::SetMovementSpeed(float SpeedToAdd, bool bReset)
{
	if (bReset)
	{
		MovementSpeed = BaseMovementSpeed;
		return;
	}
	MovementSpeed = MovementSpeed + SpeedToAdd;
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
