// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NumBaseballBPFuncLib.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLECHAT_API UNumBaseballBPFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Number Baseball")
	static TArray<int32> RandomNumberGenerator();

	UFUNCTION(BlueprintCallable, Category = "Number Baseball")
	static TArray<int32> ScoreCheck(const TArray<int32>& Answer,int32 Guess);

	//UFUNCTION(BlueprintCallable, Category = "Number Baseball")
	//static void ProcessChatMessage(const FString& Msg);
};
