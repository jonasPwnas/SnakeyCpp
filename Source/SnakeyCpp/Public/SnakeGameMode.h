// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "SnakeGameMode.generated.h"


class ASnakePlayer;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartSession);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReachedMaxScore, ASnakePlayer*, WinnerPlayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownChanged, int32, NewCount);

UCLASS()
class SNAKEYCPP_API ASnakeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:	
	virtual void BeginPlay() override;

public:
	//Player start
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	UPROPERTY()
	TArray<AActor*> UsedStarts;
	
	//Event instances
	UPROPERTY(Blueprintable)
	FOnStartSession OnStart;
	UPROPERTY(Blueprintable)
	FOnReachedMaxScore OnReachedMaxScore;
	UPROPERTY(Blueprintable)
	FOnCountdownChanged OnCountdownChanged;
	
	UPROPERTY(EditAnywhere)
	int32 GameStartCountdown = 3;

	UPROPERTY(EditAnywhere)
	int32 WinningScore = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Player1Score;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Player2Score;
	
	UFUNCTION()
	void AddScore(bool bPlayer1);
	UFUNCTION()
	void UpdateCountdown();
	UFUNCTION(BlueprintNativeEvent)
	void SomeoneDiedOhNo(ASnakePlayer* Player);
	
	FTimerHandle CountDownTimerHandle;
	
private:
	void DelayedPlayerSpawn();
};
