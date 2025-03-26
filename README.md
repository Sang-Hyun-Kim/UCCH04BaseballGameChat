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

반환된 배열은 ResultCheck BP 함수 노드를 통해 

---
#### 시도 횟수 및 상태 관리

---
#### 승리 무승부 게임 리셋

---
#### 턴 제어 기능

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
