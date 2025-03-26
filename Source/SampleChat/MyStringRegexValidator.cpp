// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStringRegexValidator.h"
#include <regex>

bool UMyStringRegexValidator::ValidateBaseballInput(const FString& InputString)
{
    // FString → std::string 변환
    std::string StdString = TCHAR_TO_UTF8(*InputString);
    std::regex Pattern(R"(^\/\d{3}$)");

    return std::regex_match(StdString, Pattern);
}
