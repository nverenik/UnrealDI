// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "IReader.generated.h"

UINTERFACE(MinimalApi)
class UReader : public UInterface
{
    GENERATED_BODY()
};

class UNREALDITESTS_API IReader
{
    GENERATED_BODY()

public:
    virtual FString Read() = 0;
};

UINTERFACE(MinimalApi)
class UBetterReader : public UInterface
{
    GENERATED_BODY()
};

class UNREALDITESTS_API IBetterReader
{
    GENERATED_BODY()
};
