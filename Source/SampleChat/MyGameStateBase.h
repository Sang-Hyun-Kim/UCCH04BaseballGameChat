// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLECHAT_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMyGameStateBase();

	UPROPERTY(Replicated,BlueprintReadWrite)
	int32 HostPlayerScore;


	UPROPERTY(Replicated,BlueprintReadWrite)
	int32 GuestPlayerScore;


	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 HostAttemps;


	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 GuestAttemps;

	/** 점수를 설정하는 서버 전용 함수 */
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetHostScore(int32 NewScore);

	UFUNCTION(Server, Reliable,BlueprintCallable)
	void Server_SetGuestScore(int32 NewScore);

	/** 횟수 변경 */
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetHostAttemps(int32 NewAttemps);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetGuestAttemps(int32 NewAttemps);

protected:
	/** Replicated 변수 동기화 함수 */
	UFUNCTION()
	void OnRep_HostPlayerScore();

	UFUNCTION()
	void OnRep_GuestPlayerScore();

	/** 네트워크 복제를 위한 함수 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};