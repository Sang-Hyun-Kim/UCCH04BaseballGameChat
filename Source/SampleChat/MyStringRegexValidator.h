// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyStringRegexValidator.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLECHAT_API UMyStringRegexValidator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "MyString Regex Valid")
	static bool ValidateBaseballInput(const FString& InputString);
};
