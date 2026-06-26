// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameMode.h"

#include "Kismet/GameplayStatics.h"

void ASnakeGameMode::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASnakeGameMode::DelayedPlayerSpawn, 1.0f);
	
}

void ASnakeGameMode::DelayedPlayerSpawn()
{
	APlayerController* PC = UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
	
	IPlatformInputDeviceMapper& Mapper = IPlatformInputDeviceMapper::Get();

	APlayerController* PC1 = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	if (!PC1 || !PC1->GetLocalPlayer()) return;

	FPlatformUserId Player1User = PC1->GetLocalPlayer()->GetPlatformUserId();

	TArray<FInputDeviceId> Devices;
	Mapper.GetAllConnectedInputDevices(Devices);

	for (const FInputDeviceId& Dev : Devices)
	{
		if (Dev == Mapper.GetDefaultInputDevice())
			continue;   // skip keyboard/mouse — leave on player 0

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

	// And what user each player controller actually has:
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
