// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DI/Impl/IsUInterface.h"
#include "DI/Impl/StaticClass.h"
#include "Containers/Map.h"

class UObject;
class UClass;
class UObjectContainer;

namespace UnrealDI_Impl
{
    class UNREALDI_API FObjectContainerIteratorBase
    {
    public:
        /* Whether iterator points to a valid object */
        bool HasValue() const { return ContainerIndex != INDEX_NONE; }

    protected:
        FObjectContainerIteratorBase(TConstArrayView<UObjectContainer*> InContainers, UClass* InClass);

        UObject* Get(bool bCreateIfNotExists) const;

        void Advance();

    private:
        static FScriptMapLayout MapLayout;
        TConstArrayView<UObjectContainer*> Containers;
        UClass* DesiredClass;
        int16 ContainerIndex;
        int16 MapIndex;
        int16 ArrayIndex;
    };
}

/*
 * Iterates over all objects of class T in the container
 */
template <typename T>
class TObjectContainerIterator : public UnrealDI_Impl::FObjectContainerIteratorBase
{
public:
    /* Whether iterator points to a valid object */
    operator bool() const
    {
        return HasValue();
    }

    /* Advances iterator to next object if possible */
    void operator++()
    {
        Advance();
    }

    /*
     * Returns current object. May return nullptr if bCreateIfNotExists is false
     * @param bCreateIfNotExists - whether we should create (Resolve) new object if it does not exist
     */
    auto Get(bool bCreateIfNotExists = false) const
    {
        return Cast(FObjectContainerIteratorBase::Get(bCreateIfNotExists));
    }

private:
    friend class UObjectContainer;

    TObjectContainerIterator(TConstArrayView<UObjectContainer*> InContainers)
        : UnrealDI_Impl::FObjectContainerIteratorBase(InContainers, UnrealDI_Impl::TStaticClass< T >::StaticClass())
    {
    }

    auto Cast(UObject* Object) const
    {
        if constexpr (TIsDerivedFrom< T, UObject >::Value)
        {
            return (T*)Object;
        }
        else if constexpr (UnrealDI_Impl::TIsUInterface< T >::Value)
        {
            return TScriptInterface< T >(Object);
        }
        else
        {
            return Object;
        }
    }
};
