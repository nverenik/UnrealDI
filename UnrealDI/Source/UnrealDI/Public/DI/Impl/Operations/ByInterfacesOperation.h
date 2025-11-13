// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UObject/Class.h"
#include "Templates/UnrealTypeTraits.h"

enum class EInterfaceSearchOptions
{
    /* Search interfaces only in Implementer class */
    CurrentClass,

    /* Search interfaces in Implementer class and all its Super classes */
    CurrentAndSuper,
};

namespace UnrealDI_Impl
{
namespace RegistrationOperations
{
    template<typename TConfigurator>
    class TByInterfacesOperation
    {
    public:
        /* Registers type to be resolvable as all intefaces that it implements */
        TConfigurator& ByInterfaces(EInterfaceSearchOptions Options = EInterfaceSearchOptions::CurrentClass)
        {
            TConfigurator& This = StaticCast<TConfigurator&>(*this);
            UClass* ImplClass = This.ImplClass;

            do
            {
                for (auto& Interface : ImplClass->Interfaces)
                {
                    This.InterfaceTypes.AddUnique(Interface.Class);
                }

                ImplClass = ImplClass->GetSuperClass();
            } while (ImplClass != nullptr && Options != EInterfaceSearchOptions::CurrentClass);

            return This;
        }
    };
}
}
