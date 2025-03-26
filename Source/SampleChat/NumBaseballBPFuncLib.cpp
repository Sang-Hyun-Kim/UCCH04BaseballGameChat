// Fill out your copyright notice in the Description page of Project Settings.


#include "NumBaseballBPFuncLib.h"

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


