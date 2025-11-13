// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

namespace UnrealDI_Impl
{

    template <typename... TArgs>
    struct TFunctionWithDependenciesInvoker
    {
        /* Invokes given function with args injected from Container */
        template <typename TFunction>
        static void Invoke(const IResolver& Resolver, TFunction&& Function);
    };

    /* Provides type of TFunctionWithDependenciesInvoker based on TFunction arguments */
    template <typename TFunction>
    struct TFunctionWithDependenciesInvokerProvider : public TFunctionWithDependenciesInvokerProvider< decltype(&TFunction::operator()) >
    {
    };

    /* Specialization for member function */
    template <typename TClass, typename TResult, typename... TArgs>
    struct TFunctionWithDependenciesInvokerProvider<TResult(TClass::*)(TArgs...) const>
    {
        using Invoker = TFunctionWithDependenciesInvoker<TArgs...>;
    };

}

#include "DI/Impl/DependencyResolverInvoker.h"
#include "DI/Impl/CommonDependencyResolvers.h"

class IResolver;

namespace UnrealDI_Impl
{

    /* Invokes given function with args injected from Container */
    template <typename... TArgs>
    template <typename TFunction>
    void TFunctionWithDependenciesInvoker<TArgs...>::Invoke(const IResolver& Resolver, TFunction&& Function)
    {
        Function(UnrealDI_Impl::TDependencyResolverInvoker<TArgs>(Resolver)...);
    }

}
