// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameMode.h"
#include "SnakePlayer.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ASnakeGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASnakeGameMode::DelayedPlayerSpawn, 1.0f);
	
	GetWorldTimerManager().SetTimer(CountDownTimerHandle, this, &ASnakeGameMode::UpdateCountdown, 1.0f, true, 3.7f);
	
	ASnakePlayer::OnAnySnekDied.AddDynamic(this, &ASnakeGameMode::SomeoneDiedOhNo);
	ASnakePlayer::OnAnySnekGrew.AddDynamic(this, &ASnakeGameMode::SomeoneGrewOhYes);
	
}

void ASnakeGameMode::EndPlay(const EEndPlayReason::Type Param)
{
	Super::EndPlay(Param);
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	SpawnTimerHandle.Invalidate();
	ASnakePlayer::OnAnySnekDied.RemoveDynamic(this, &ASnakeGameMode::SomeoneDiedOhNo);
}

AActor* ASnakeGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> Starts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Starts);

	for (AActor* Start : Starts)
	{
		if (!UsedStarts.Contains(Start))
		{
			UsedStarts.Add(Start);
			return Start;
		}
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

void ASnakeGameMode::AddScore(bool bPlayer1)
{
	if (bPlayer1)
		Player1Score++;
	else
	{
		Player2Score++;
	}
}

void ASnakeGameMode::UpdateCountdown()
{
	GameStartCountdown = GameStartCountdown -1;
	OnCountdownChanged.Broadcast(GameStartCountdown);
	
	//GEngine->AddOnScreenDebugMessage(1, 0.6f, FColor::Emerald, FString("CONTDOWN!!! wowsa"));
	
	if (GameStartCountdown <= 0)
	{
		GetWorldTimerManager().ClearTimer(CountDownTimerHandle);
		CountDownTimerHandle.Invalidate();
		OnStart.Broadcast(true);
		//GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Magenta, FString("CONTDOWN finiiiiished"));
	}
}

void ASnakeGameMode::SomeoneGrewOhYes_Implementation(ASnakePlayer* Player)
{
	//OutToBP and INTO Hud wiiiie
	
	//bool bIsPlayer1 = Player->SnekPlayerIndex == 0;
	//AddScore(bIsPlayer1);
	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Emerald, FString
	//	("we added SCORE!  p1:  " + Player1Score + Player2Score));
}

void ASnakeGameMode::SomeoneDiedOhNo_Implementation(ASnakePlayer* Player)
{
	//Might spawn vfx in BPs instead? yeah.
}

void ASnakeGameMode::DelayedPlayerSpawn()
{
	APlayerController* PC = UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
	if (!PC)
	{
		PC = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	}
	
	if (!PC) return;
	
	APawn* Pawn = PC->GetPawn();
	
	if (ASnakePlayer* Snek = Cast<ASnakePlayer>(Pawn))
	{
		Snek->SnekPlayerIndex = 1;
		//Snek->bCanMove = false;
	}
	
	IPlatformInputDeviceMapper& Mapper = IPlatformInputDeviceMapper::Get();

	APlayerController* PC1 = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	if (!PC1 || !PC1->GetLocalPlayer()) return;

	FPlatformUserId Player1User = PC1->GetLocalPlayer()->GetPlatformUserId();

	TArray<FInputDeviceId> Devices;
	Mapper.GetAllConnectedInputDevices(Devices);

	for (const FInputDeviceId& Dev : Devices)
	{
		if (Dev == Mapper.GetDefaultInputDevice())
			continue;

		FPlatformUserId Current = Mapper.GetUserForInputDevice(Dev);
		if (Current != Player1User)
		{
			Mapper.Internal_ChangeInputDeviceUserMapping(Dev, Player1User, Current);
		}
	}
	
	
	
	TArray<FInputDeviceId> AllDevices;
	Mapper.GetAllConnectedInputDevices(AllDevices);

	UE_LOG(LogTemp, Warning, TEXT("=== Device report ==="));
	for (const FInputDeviceId& Dev : AllDevices)
	{
		FPlatformUserId OwnerCool = Mapper.GetUserForInputDevice(Dev);
		UE_LOG(LogTemp, Warning, TEXT("Device %d -> User %d (default=%d, unpaired=%d)"),
			Dev.GetId(),
			OwnerCool.GetInternalId(),
			Dev == Mapper.GetDefaultInputDevice(),
			Mapper.IsInputDeviceMappedToUnpairedUser(Dev));
	}

	for (int32 i = 0; i < 2; ++i)
	{
		if (APlayerController* Moar = UGameplayStatics::GetPlayerController(GetWorld(), i))
		{
			if (ULocalPlayer* LP = Moar->GetLocalPlayer())
			{
				UE_LOG(LogTemp, Warning, TEXT("Player %d -> User %d"),
					i, LP->GetPlatformUserId().GetInternalId());
			}
		}
	}
}
