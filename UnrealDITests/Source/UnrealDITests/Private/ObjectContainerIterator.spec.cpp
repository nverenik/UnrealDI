// Copyright Andrei Sudarikov. All Rights Reserved.

#include "Misc/AutomationTest.h"

#include "DI/ObjectContainer.h"
#include "DI/ObjectContainerBuilder.h"
#include "DI/ObjectContainerIterator.h"

#include "MockReader.h" 

BEGIN_DEFINE_SPEC(FObjectContainerIteratorSpec, "UnrealDI.ObjectContainerIterator", EAutomationTestFlags::ClientContext | EAutomationTestFlags::EditorContext | EAutomationTestFlags::ServerContext | EAutomationTestFlags::EngineFilter)
template <typename T>
static int Count(TObjectContainerIterator<T>& It);
END_DEFINE_SPEC(FObjectContainerIteratorSpec)

void FObjectContainerIteratorSpec::Define()
{
    Describe("UObject derived", [this]
    {
        It("Should Iterate over registrations in single container", [this]
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterType<UMockReader>();
            Builder.RegisterType<UMockReader>();

            UObjectContainer* Container = Builder.Build();
            TObjectContainerIterator<UMockReader> It = Container->CreateIterator<UMockReader>();

            TestEqual("Counter", Count(It), 2);
        });

        It("Should Iterate over registrations in parent container", [this]
        {
            FObjectContainerBuilder ParentBuilder;
            ParentBuilder.RegisterType<UMockReader>();
            ParentBuilder.RegisterType<UMockReader>();

            FObjectContainerBuilder Builder;

            UObjectContainer* Container = Builder.BuildNested(*ParentBuilder.Build());
            TObjectContainerIterator<UMockReader> It = Container->CreateIterator<UMockReader>(EObjectContainerIteratorFlags::IncludeParent);

            TestEqual("Counter", Count(It), 2);
        });

        It("Should not Iterate over registrations in parent container", [this]
        {
            FObjectContainerBuilder ParentBuilder;
            ParentBuilder.RegisterType<UMockReader>();
            ParentBuilder.RegisterType<UMockReader>();

            FObjectContainerBuilder Builder;

            UObjectContainer* Container = Builder.BuildNested(*ParentBuilder.Build());
            TObjectContainerIterator<UMockReader> It = Container->CreateIterator<UMockReader>();

            TestEqual("Counter", Count(It), 0);
        });

        It("Should Iterate over registrations in parent and child containers", [this]
        {
            FObjectContainerBuilder ParentBuilder;
            ParentBuilder.RegisterType<UMockReader>();
            ParentBuilder.RegisterType<UMockReader>();

            FObjectContainerBuilder Builder;
            Builder.RegisterType<UMockReader>();
            Builder.RegisterType<UMockReader>();

            UObjectContainer* Container = Builder.BuildNested(*ParentBuilder.Build());
            TObjectContainerIterator<UMockReader> It = Container->CreateIterator<UMockReader>(EObjectContainerIteratorFlags::IncludeParent);

            TestEqual("Counter", Count(It), 4);
        });

        It("Should return specific UObject pointer", [this]
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterType<UMockReader>();

            UObjectContainer* Container = Builder.Build();
            TObjectContainerIterator<UMockReader> It = Container->CreateIterator<UMockReader>();

            static_assert(std::is_same_v<UMockReader*, decltype(It.Get())>, "Incorrect return type");
        });
    });

    Describe("IInterface", [this]
    {
        It("Should Iterate over registrations in single container", [this]
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterType<UMockReader>().As<IReader>();
            Builder.RegisterType<UMockReader>().As<IReader>();

            UObjectContainer* Container = Builder.Build();
            TObjectContainerIterator<IReader> It = Container->CreateIterator<IReader>();

            TestEqual("Counter", Count(It), 2);
        });

        It("Should Iterate over registrations in parent container", [this]
        {
            FObjectContainerBuilder ParentBuilder;
            ParentBuilder.RegisterType<UMockReader>().As<IReader>();
            ParentBuilder.RegisterType<UMockReader>().As<IReader>();

            FObjectContainerBuilder Builder;

            UObjectContainer* Container = Builder.BuildNested(*ParentBuilder.Build());
            TObjectContainerIterator<IReader> It = Container->CreateIterator<IReader>(EObjectContainerIteratorFlags::IncludeParent);

            TestEqual("Counter", Count(It), 2);
        });

        It("Should not Iterate over registrations in parent container", [this]
        {
            FObjectContainerBuilder ParentBuilder;
            ParentBuilder.RegisterType<UMockReader>().As<IReader>();
            ParentBuilder.RegisterType<UMockReader>().As<IReader>();

            FObjectContainerBuilder Builder;

            UObjectContainer* Container = Builder.BuildNested(*ParentBuilder.Build());
            TObjectContainerIterator<IReader> It = Container->CreateIterator<IReader>();

            TestEqual("Counter", Count(It), 0);
        });

        It("Should Iterate over registrations in parent and child containers", [this]
        {
            FObjectContainerBuilder ParentBuilder;
            ParentBuilder.RegisterType<UMockReader>().As<IReader>();
            ParentBuilder.RegisterType<UMockReader>().As<IReader>();

            FObjectContainerBuilder Builder;
            Builder.RegisterType<UMockReader>().As<IReader>();
            Builder.RegisterType<UMockReader>().As<IReader>();

            UObjectContainer* Container = Builder.BuildNested(*ParentBuilder.Build());
            TObjectContainerIterator<IReader> It = Container->CreateIterator<IReader>(EObjectContainerIteratorFlags::IncludeParent);

            TestEqual("Counter", Count(It), 4);
        });

        It("Should return TSciptInterface", [this]
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterType<UMockReader>().As<IReader>();

            UObjectContainer* Container = Builder.Build();
            TObjectContainerIterator<IReader> It = Container->CreateIterator<IReader>();

            static_assert(std::is_same_v<TScriptInterface<IReader>, decltype(It.Get())>, "Incorrect return type");
        });
    });

    It("Should Iterate over default registrations", [this]
    {
        FObjectContainerBuilder Builder;

        UObjectContainer* Container = Builder.Build();
        TObjectContainerIterator<UObject> It = Container->CreateIterator<UObject>();

        TestEqual("Counter", Count(It), 3);
    });

    It("Should Iterate over all registrations in single container", [this]
    {
        FObjectContainerBuilder Builder;
        Builder.RegisterType<UMockReader>();
        Builder.RegisterType<UMockReader>();

        UObjectContainer* Container = Builder.Build();
        TObjectContainerIterator<UObject> It = Container->CreateIterator<UObject>();

        TestEqual("Counter", Count(It), 5);
    });

    It("Should Iterate over all registrations in parent and child containers", [this]
    {
        FObjectContainerBuilder ParentBuilder;
        ParentBuilder.RegisterType<UMockReader>();
        ParentBuilder.RegisterType<UMockReader>();

        FObjectContainerBuilder Builder;
        Builder.RegisterType<UMockReader>();
        Builder.RegisterType<UMockReader>();

        UObjectContainer* Container = Builder.BuildNested(*ParentBuilder.Build());
        TObjectContainerIterator<UObject> It = Container->CreateIterator<UObject>(EObjectContainerIteratorFlags::IncludeParent);

        TestEqual("Counter", Count(It), (3 /*builtin*/ + 2 /*manual*/) * 2 /*containers*/);
    });

    It("Should return existing instance", [this]
    {
        FObjectContainerBuilder Builder;
        Builder.RegisterType<UMockReader>().SingleInstance(true);

        UObjectContainer* Container = Builder.Build();
        TObjectContainerIterator<UMockReader> It = Container->CreateIterator<UMockReader>();

        TestNotNull("Existing instance", It.Get());
    });

    It("Should not return instance if not yet created", [this]
    {
        FObjectContainerBuilder Builder;
        Builder.RegisterType<UMockReader>();

        UObjectContainer* Container = Builder.Build();
        TObjectContainerIterator<UMockReader> It = Container->CreateIterator<UMockReader>();

        TestNull("Existing instance", It.Get());
    });

    It("Should create instance if not yet created", [this]
    {
        FObjectContainerBuilder Builder;
        Builder.RegisterType<UMockReader>();

        UObjectContainer* Container = Builder.Build();
        TObjectContainerIterator<UMockReader> It = Container->CreateIterator<UMockReader>();

        TestNotNull("Existing instance", It.Get(true));
    });
}

template <typename T>
int FObjectContainerIteratorSpec::Count(TObjectContainerIterator<T>& It)
{
    int Counter = 0;
    for (; It; ++It)
    {
        ++Counter;
    }

    return Counter;
}
