// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "SnakeGameMode.generated.h"


class ASnakePlayer;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartSession, bool, bStartSession);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReachedMaxScore, ASnakePlayer*, WinnerPlayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountdownChanged, int32, NewCount);

UCLASS()
class SNAKEYCPP_API ASnakeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type);

public:
	//Player start
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	UPROPERTY()
	TArray<AActor*> UsedStarts;
	
	UPROPERTY()
	FTimerHandle SpawnTimerHandle;
	
	//Event instances
	UPROPERTY(Blueprintable, BlueprintAssignable, BlueprintCallable)
	FOnStartSession OnStart;
	UPROPERTY(Blueprintable, BlueprintAssignable)
	FOnReachedMaxScore OnReachedMaxScore;
	UPROPERTY(Blueprintable, BlueprintAssignable)
	FOnCountdownChanged OnCountdownChanged;
	
	UPROPERTY(EditAnywhere)
	int32 GameStartCountdown = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Blueprintable)
	int32 SnakeStartLength = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Blueprintable)
	int32 WinningScore = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Blueprintable)
	int32 Player1Score;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Blueprintable)
	int32 Player2Score;
	
	UFUNCTION()
	void AddScore(bool bPlayer1);
	UFUNCTION()
	void UpdateCountdown();
	UFUNCTION(BlueprintNativeEvent)
	void SomeoneDiedOhNo(ASnakePlayer* Player);
	UFUNCTION(BlueprintNativeEvent)
	void SomeoneGrewOhYes(ASnakePlayer* Player);
	
	FTimerHandle CountDownTimerHandle;
	
private:
	void DelayedPlayerSpawn();
};
