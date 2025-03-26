// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"
#include "Net/UnrealNetwork.h"

AMyGameStateBase::AMyGameStateBase()
	: HostPlayerScore(0),
	GuestPlayerScore(0),
	HostAttemps(0),
	GuestAttemps(0)
{
	// 게임 상태 클래스에서 복제를 사용하도록 설정
	bReplicates = true;
}

// 점수 업데이트를 서버에서만 실행하도록 설정
void AMyGameStateBase::Server_SetHostScore_Implementation(int32 NewScore)
{
	HostPlayerScore = NewScore;
}

void AMyGameStateBase::Server_SetGuestScore_Implementation(int32 NewScore)
{
	GuestPlayerScore = NewScore;
}
void AMyGameStateBase::Server_SetHostAttemps_Implementation(int32 NewAttemps)
{
	HostAttemps = NewAttemps;
}

void AMyGameStateBase::Server_SetGuestAttemps_Implementation(int32 NewAttemps)
{
	GuestAttemps = NewAttemps;
}
// 클라이언트에서 점수가 변경되었을 때 실행될 함수
void AMyGameStateBase::OnRep_HostPlayerScore()
{
	UE_LOG(LogTemp, Warning, TEXT("Host 점수 변경됨: %d"), HostPlayerScore);
}

void AMyGameStateBase::OnRep_GuestPlayerScore()
{
	UE_LOG(LogTemp, Warning, TEXT("Guest 점수 변경됨: %d"), GuestPlayerScore);
}

// 복제할 변수를 Unreal의 네트워크 시스템에 등록
void AMyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyGameStateBase, HostPlayerScore);
	DOREPLIFETIME(AMyGameStateBase, GuestPlayerScore);
	DOREPLIFETIME(AMyGameStateBase, HostAttemps);
	DOREPLIFETIME(AMyGameStateBase, GuestAttemps);
}