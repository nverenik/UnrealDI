// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

namespace UnrealDI_Impl
{
    /*
     * Invokes IResolverImpl with correct argument
     * Main purpose of this class is to provide conversion operator to be called with type of argument that InitDependencies requested
     */
    template <typename TDependency>
    struct TDependencyResolverInvoker
    {
        using FResultType = typename TDecay<TDependency>::Type;

        TDependencyResolverInvoker(const IResolver& Resolver)
            : Resolver(Resolver)
        {
        }

        // this conversion operator is called when this object is casted to a parameter of InitDependencies method
        operator FResultType();

        const IResolver& Resolver;
    };
}

#include "DI/Impl/CommonDependencyResolvers.h"

namespace UnrealDI_Impl
{
    template <typename TDependency>
    TDependencyResolverInvoker<TDependency>::operator TDependencyResolverInvoker<TDependency>::FResultType()
    {
        using FResolverType = ::TDependencyResolver< FResultType >;
        return FResolverType::Resolve(Resolver);
    }
}
