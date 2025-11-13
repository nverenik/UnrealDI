// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Delegates/Delegate.h"

class UObjectContainer;

class FObjectContainerDelegates
{
public:
    DECLARE_MULTICAST_DELEGATE_TwoParams(FOnObjectCreated, UObject&, const UObjectContainer&);

    /** Callback right after NewObject, before dependencies are injected. */
    static UNREALDI_API FOnObjectCreated OnObjectConstructedDelegate;

    /** Callback after dependencies have been injected into the object, but before finalization (e.g. AActor::FinishSpawning). */
    static UNREALDI_API FOnObjectCreated OnObjectInjectedDelegate;

    /** Callback when the object is fully created and ready (construction + injection + finalization completed). */
    static UNREALDI_API FOnObjectCreated OnObjectCreatedDelegate;
};
