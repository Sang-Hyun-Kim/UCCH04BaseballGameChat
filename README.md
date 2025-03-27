# 강의 자료 기반 리슨 서버를 활용한 숫자 야구 게임 만들기


## 목차
- 개요
- 주요 구현 기능
- 시연 영상 테스트
- 평가 체크 리스트

### 개요
해당 레포지토리는 내일배움캠프 언리얼 2기의 리슨 서버 과제 제출 및 설명을 것입니다.
---
### 주요 구현 기능
- 필수 기능
  - GameMode 서버 로직
  - 3자리 난수 생성 로직
  - 판정 로직
  - 시도 횟수 및 상태 관리
  - 승리 무승부 게임 리셋
- 도전 기능
  - 턴 제어 기능
  - 타이머 기능
---  
#### GameMode 서버 로직

---
#### 3자리 난수 생성 로직

난수 생성 로직은 BlueprintFunctionLibrary 을 상속받은 UNumBaseballBPFuncLib 클래스에서 수행한다.  

```C++
// 
#pragma once
// NumBaseballBPFuncLib.h
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NumBaseballBPFuncLib.generated.h"

UCLASS()
class SAMPLECHAT_API UNumBaseballBPFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Number Baseball")
	static TArray<int32> RandomNumberGenerator();

	UFUNCTION(BlueprintCallable, Category = "Number Baseball")
	static TArray<int32> ScoreCheck(const TArray<int32>& Answer,int32 Guess);
};
```
```C++
// 
#pragma once
// NumBaseballBPFuncLib.cpp
TArray<int32> UNumBaseballBPFuncLib::RandomNumberGenerator()
{
	TArray<int32> Digits;
	int32 Number = 0;

	// 3개의 서로 다른 숫자를 선택
	while (Digits.Num() < 3)
	{
		int32 Digit = FMath::RandRange(0, 9);

		// 첫 번째 숫자는 0이 되면 안됨
		if (Digits.Num() == 0 && Digit == 0)
		{
			continue;
		}

		// 중복 방지
		if (!Digits.Contains(Digit))
		{
			Digits.Add(Digit);
		}
	}
	return Digits;
}

```
---
#### 판정 로직

판정 로직은 BlueprintFunctionLibrary 을 상속받은 UNumBaseballBPFuncLib 클래스에서  static TArray<int32> ScoreCheck(const TArray<int32>& Answer,int32 Guess) 함수를 통해 이루어진 반환값을 GameMode BP의 이벤트 그래프 내의 ProcessNumAnswer 함수 BP 안에서 로직통해 결과 출력, 승리 및 무승부 여부를 판단한다.

```C++
// 
#pragma once
// NumBaseballBPFuncLib.cpp
TArray<int32> UNumBaseballBPFuncLib::ScoreCheck(const TArray<int32>& Answer, int32 Guess)
{
	TArray<int32> GuessDigits;

	// Guess 값을 한 자리씩 분해
	GuessDigits.Add(Guess / 100);        // 100의 자리
	GuessDigits.Add((Guess / 10) % 10);  // 10의 자리
	GuessDigits.Add(Guess % 10);         // 1의 자리

	int32 Strikes = 0;
	int32 Balls = 0;

	for (int32 i = 0; i < 3; i++)
	{
		if (Answer[i] == GuessDigits[i])
		{
			Strikes++; // 같은 숫자가 같은 위치
		}
		else if (Answer.Contains(GuessDigits[i]))
		{
			Balls++; // 같은 숫자가 있지만 위치 다름
		}
	}
	int32 Outs = 3 - Strikes - Balls;
	TArray<int32> Result;
	Result.Add(Strikes);
	Result.Add(Balls);
	Result.Add(Outs);
	return Result;
}
```
ScoreCheck 함수를 통해 인자로 받은 정답과 플레이어의 답안을 비교, Strikes, Balls, Outs의 갯수를 담은 배열을 반환하면, GameMode는 해당 반환을 사용한다.
먼저 ProcessNumAnswer 함수 BP 내에서는 턴 및 입력 검증 후 옳바른 경우에 결과 검사 수행을, 아닌 경우에는 안내 메세지를 BroadCast 이벤트로 넘긴다.

![Image](https://github.com/user-attachments/assets/99a800f3-216a-4547-a24d-c79fd6f60c1d)
![Image](https://github.com/user-attachments/assets/05ab0ecd-dae1-4f21-a983-94a3dd84c7b9)
![Image](https://github.com/user-attachments/assets/9c12e2d2-c464-4d82-bac9-d4a285a7f48c)
![Image](https://github.com/user-attachments/assets/8df25d47-39b8-454f-bcf4-b50ef2fdecbd)
![Image](https://github.com/user-attachments/assets/5827c546-77be-4fbd-8b67-c85473df1ae5)

반환된 배열을 이용하는 ResultCheck BP 함수 노드는 결과 메세지를 BroadCast 하도록 생성 및 반환하고 승리시 IsFinished 배열의 값을 True로 변환한다. 이는 승리 및 시도 체크에 사용된다.

---
#### 시도 횟수 및 상태 관리

플레이어의 시도 횟수 및 상태 관리는 GameStateBase 클래스를 상속받은 AMyGameStateBase 에서 담당한다.

```C++
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
```
```C++
void AMyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyGameStateBase, HostPlayerScore);
	DOREPLIFETIME(AMyGameStateBase, GuestPlayerScore);
	DOREPLIFETIME(AMyGameStateBase, HostAttemps);
	DOREPLIFETIME(AMyGameStateBase, GuestAttemps);
}
```
각 플레이어의 시도 횟수와 점수를 관리하는 AMyGameStateBase 내에서 추가 확장을 위해 OnRep_ 함수들과 _Impletation 함수들을 같이 구현 해놓은 다음 시도와 점수 변화가 생길 때 GameMode에서 해당 함수를 호출시키는 방식으로 구현하였다.
각 레플리케이트 될 변수들을 레플레키이션 시스템에 등록한 다음 서버에서 값을 조작하면 각 Client로 반영되도록 구현하였다.
이를 이용해, GameMode에서는 UpdateAttempts, CheckAttempts 라는 BP 함수를 구현해 사용한다.
먼저 플레이어의 턴의 답안 제출 후 정답이 아니라면 UpdateAttemps 함수를 통해 답안을 제출한 플레이어의 시도 횟수를 업데이트하고 이후 CheckAttempts 함수가 양 플레이어의 시도 횟수가 최대 시도 횟수(현재 3회)에 도달한 경우 무승부 안내 메세지를 전송하고 게임을 초기화하는 작업을 시작한다.
![Image](https://github.com/user-attachments/assets/fec6ab34-4d80-4b61-bebc-d6baf34a88de)
![Image](https://github.com/user-attachments/assets/7d6def65-7c3f-4bf8-b5d8-1cc145dbd5c8)
![Image](https://github.com/user-attachments/assets/a95db7f8-b975-427f-882a-baea4179d56d)

---
#### 승리 무승부 게임 리셋
- 승리
	- 승리 시(IsFinished 변수가 참인 경우)시 맞춘 플레이어의 점수를 올린다. 정답을 맞췄다는 안내 메시지와, 현재 각 플레이어의 점수를 BroadCast 하여 각 클라이언트에게 콘솔출력되도록 구현했다.
	- 그 후 게임 초기화 작업이 시작된다.

```C++
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
```
![Image](https://github.com/user-attachments/assets/c26286de-5058-4b09-bd6c-fc90e156703f)

- 무승부
	- 무승부 시 게임 초기화 작업시 시작된다.

- 게임 초기화
	- 게임 승리 및 무승부 시 초기화 작업은 다음과 같다
		- 호스트의 턴으로 변경
  		- IsFinished 변수 false
    		- 새로운 숫자 야구 답안 생성
      		- 승리 또는 무승부 관련 안내 출력
        	- 승리의 경우 현재 양 플레이어의 점수를 출력
         	- 턴 시스템의 제한시간을 초기화
          	- ClearAttempts BP 함수 구현, 해당 함수는 각 플레이어의 시도 횟수 0으로 초기화

![Image](https://github.com/user-attachments/assets/e52f9e02-bee5-49bc-8836-756ddbe3a864)

![Image](https://github.com/user-attachments/assets/504a7203-5b40-4bf3-ab8f-28524c4d12a9)

![Image](https://github.com/user-attachments/assets/ea2287ad-efe7-4d46-af00-e45f60358e3c)

---
#### 턴 제어 기능
- 턴 시스템은, 턴 제한시간(TurnTime float 변수, 최대 8초)안에 답안 채팅이 서버로 전달되지 않는다면 다른플레이어의 턴으로 전환되는 로직을 구현하였다.
- 게임 초기화시 시작되는 InitTurnTimer와 OnTurnEnd BP 함수를 GameMode 내에서 구현했고 이를 사용했다.
- InitTurnTimer 함수 BP는 OnTurnEnd 함수를 제한시간이 지나면 호출하도록 TimerHandle안에 저장하는 SetTimerbyFunctionName BP 노드 실행하고 안내 메세지를 BroadCast하는 방식이다.
![Image](https://github.com/user-attachments/assets/f2994b6f-ec2a-4ed2-b538-1541b5623fb9)
- OnTurnEnd 함수는 제한 시간 도달 또는 입력 시 실행되는 함수 이며, 턴 전환시 실행되는 함수다. 다시 OnTurnEnd 함수를 타이머에 등록하는 것이 특징이다.
![Image](https://github.com/user-attachments/assets/4d982827-698a-43cd-9e60-8ec5eeeb67b0)

![Image](https://github.com/user-attachments/assets/da3135e4-cb63-4440-ac1a-8bae2b084599)

---

#### 타이머 기능

---
### 시연 영상 테스트

---
### 평가 체크 리스트
과제 발제 문서의 평가 체크 리스트 구현 여부를 표로 정리했습니다.
|기능|평가 종류|여부|
|:---:|:---:|:---:|
|GameMode 서버 로직 구현|완성도(필수)|🟢|
|난수 생성 로직 구현|완성도(필수)|🟢|
|판정 로직|완성도(필수)|🟢|
|시도 횟수 및 상태 관리|완성도(필수)|🟢|
|승리, 무승부, 게임 리셋|완성도(필수)|🟢|
|C++ 라이브러리 작성|완성도(필수)|🟢|
|턴 제어 기능|완성도(도전)|🟢|
|게임 고도화|완성도(도전)|🟢|
|구현과정 Readme.md 정리|이해도(필수)|🟢|
|이해하기 쉬운 코드 정리|이해도(도전)|🟢|
|언리올 제공 자료구조 사용|이해도(도전)|🟢|
|TArray 사용에 따른 최적화 고민|이해도(도전)|🟢|
|Readme 작성을 통한 과제 소개|우수성(필수)|🟢|
|타이머 구현|우수성(도전)|🟢|
