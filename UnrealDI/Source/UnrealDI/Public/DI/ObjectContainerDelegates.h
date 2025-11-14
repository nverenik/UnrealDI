#pragma once

#include "CoreMinimal.h"

class FObjectContainerDelegates
{
public:
    /** Callback right after NewObject, before dependencies are injected. */
    DECLARE_MULTICAST_DELEGATE_TwoParams(FOnObjectConstructed, UObject&, const class UObjectContainer&);
    static UNREALDI_API FOnObjectConstructed OnObjectConstructedDelegate;

    /** Callback after dependencies have been injected into the object. */
    DECLARE_MULTICAST_DELEGATE_TwoParams(FOnObjectInjected, UObject&, const class UObjectContainer&);
    static UNREALDI_API FOnObjectInjected OnObjectInjectedDelegate;

    /** Callback when the object is fully created and ready (construction + injection complete). */
    DECLARE_MULTICAST_DELEGATE_TwoParams(FOnObjectCreated, UObject&, const class UObjectContainer&);
    static UNREALDI_API FOnObjectCreated OnObjectCreatedDelegate;
};
