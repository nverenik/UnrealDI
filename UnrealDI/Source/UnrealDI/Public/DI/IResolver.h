// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "DI/Impl/StaticClass.h"
#include "UObject/Interface.h"
#include "Templates/EnableIf.h"
#include "IResolver.generated.h"

template<typename T>
class TObjectsCollection;

template <typename T>
class TFactory;

class UClass;

UINTERFACE()
class UNREALDI_API UResolver : public UInterface { GENERATED_BODY() };

/*
 * Provides methods for resolving objects by Type and UClass
 */
class UNREALDI_API IResolver
{
    GENERATED_BODY()

public:
    /* Returns instance of given Type. Asserts if Type is not registered */
    virtual UObject* Resolve(UClass* Type) const = 0;

    /* Returns instance of given Type */
    template <typename T>
    typename TEnableIf<TIsDerivedFrom<T, UObject>::Value, T*>::Type
        Resolve() const
    {
        return (T*)Resolve(UnrealDI_Impl::TStaticClass< T >::StaticClass());
    }

    /* Returns instance of given Interface. Asserts if Interface is not registered */
    template <typename T>
    typename TEnableIf<UnrealDI_Impl::TIsUInterface< T >::Value, TScriptInterface< T >>::Type
        Resolve() const
    {
        return Resolve(UnrealDI_Impl::TStaticClass< T >::StaticClass());
    }


    /* Returns all instances of given Type. Asserts if Type is not registered */
    virtual TObjectsCollection<UObject> ResolveAll(UClass* Type) const = 0;

    /* Returns all instances of given Type. Asserts if Type is not registered */
    template <typename T>
    TObjectsCollection<T> ResolveAll() const
    {
        return TObjectsCollection<T>(ResolveAll(UnrealDI_Impl::TStaticClass< T >::StaticClass()));
    }


    /* Returns Factory that can be used to resolve given Type. Asserts if Type is not registered */
    virtual TFactory<UObject> ResolveFactory(UClass* Type) const = 0;

    /* Returns Factory that can be used to resolve given Type. Asserts if Type is not registered */
    template <typename T>
    TFactory<T> ResolveFactory() const
    {
        return ResolveFactory(UnrealDI_Impl::TStaticClass< T >::StaticClass());
    }


    /* Returns instance of given Type if it is registered, otherwise returns nullptr */
    virtual UObject* TryResolve(UClass* Type) const = 0;

    /* Returns instance of given Type if it is registered, otherwise returns nullptr */
    template <typename T>
    typename TEnableIf<TIsDerivedFrom<T, UObject>::Value, T*>::Type
        TryResolve() const
    {
        return (T*)TryResolve(UnrealDI_Impl::TStaticClass< T >::StaticClass());
    }

    /* Returns instance of given Interface if it is registered, otherwise returns nullptr */
    template <typename T>
    typename TEnableIf<UnrealDI_Impl::TIsUInterface< T >::Value, TScriptInterface< T >>::Type
        TryResolve() const
    {
        return TryResolve(UnrealDI_Impl::TStaticClass< T >::StaticClass());
    }


    /* Returns all instances of given Type if it is registered, otherwise returns empty collection */
    virtual TObjectsCollection<UObject> TryResolveAll(UClass* Type) const = 0;

    /* Returns all instances of given Type if it is registered, otherwise returns empty collection */
    template <typename T>
    TObjectsCollection<T> TryResolveAll() const
    {
        return TObjectsCollection<T>(TryResolveAll(UnrealDI_Impl::TStaticClass< T >::StaticClass()));
    }


    /* Returns Factory that can be used to resolve given Type if it is registered, otherwise returns invalid TFactory */
    virtual TFactory<UObject> TryResolveFactory(UClass* Type) const = 0;

    /* Returns Factory that can be used to resolve given Type if it is registered, otherwise returns invalid TFactory */
    template <typename T>
    TFactory<T> TryResolveFactory() const
    {
        return TryResolveFactory(UnrealDI_Impl::TStaticClass< T >::StaticClass());
    }


    /* Returns true if given type is Registered and can be resolved */
    virtual bool IsRegistered(UClass* Type) const = 0;

    /* Returns true if given type is Registered and can be resolved */
    template <typename T>
    bool IsRegistered() const
    {
        return IsRegistered(UnrealDI_Impl::TStaticClass<T>::StaticClass());
    }


    /*
     * Invokes provided function injecting dependencies into its arguments the same way InitDependencies are usually invoked
     * Example:
     *    Resolver->InvokeWithDependencies([](UMyService* Service, TScriptInterface<IMyOtherService>&& OtherService)
     *    {  // Do Something with dependencies  });
     * @param Function - lambda with arguments that should be resolved
     */
    template <typename TFunction>
    void InvokeWithDependencies(TFunction&& Function);
};

#if CPP // without this #if UHT complains about includes after "IResolver.generated.h"
// this include must be after IResolver is fully declared
#include "DI/Impl/InvokeWithDependencies.h"
#endif

template<typename TFunction>
inline void IResolver::InvokeWithDependencies(TFunction&& Function)
{
    UnrealDI_Impl::TFunctionWithDependenciesInvokerProvider<TFunction>::Invoker::Invoke(*this, Forward<TFunction>(Function));
}
