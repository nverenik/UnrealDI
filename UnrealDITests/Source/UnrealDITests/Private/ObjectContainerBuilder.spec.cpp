// Copyright Andrei Sudarikov. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"

#include "DI/ObjectContainerBuilder.h"
#include "DI/ObjectContainer.h"

#include "MockClasses.h"
#include "LatentCommands.h"

BEGIN_DEFINE_SPEC(ObjectContainerBuilderSpec, "UnrealDI.ObjectContainerBuilder", EAutomationTestFlags::ClientContext | EAutomationTestFlags::EditorContext | EAutomationTestFlags::ServerContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(ObjectContainerBuilderSpec)

void ObjectContainerBuilderSpec::Define()
{
    It("Should Create Empty Container", [this]()
    {
        FObjectContainerBuilder Builder;
        
        UObjectContainer* Container = Builder.Build();
        
        TestNotNull("Container", Container);
    });

    Describe("Register Type", [this]()
    {
        It("Should Register Type", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterType<UMockReader>();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("UMockReader not registered", Container->IsRegistered<UMockReader>());
        });

        It("Should Register Type As Self", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterType<UMockReader>().AsSelf();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("UMockReader not registered", Container->IsRegistered<UMockReader>());
        });

        It("Should Register Type As Interface", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterType<UMockReader>().As<IReader>();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("IReader not registered", Container->IsRegistered<IReader>());
            TestFalse("UMockReader should not be registered", Container->IsRegistered<UMockReader>());
        });

        It("Should Register Type As Interface And Self", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterType<UMockReader>().As<IReader>().AsSelf();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("UMockReader not registered", Container->IsRegistered<UMockReader>());
            TestTrue("IReader not registered", Container->IsRegistered<IReader>());
        });

        It("Should Register Type By Interfaces", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterType<UMockReader>().ByInterfaces();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("IReader not registered", Container->IsRegistered<IReader>());
            TestFalse("UMockReader should not be registered", Container->IsRegistered<UMockReader>());
        });

        It("Should Register Type By Interfaces And Self", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterType<UMockReader>().ByInterfaces().AsSelf();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("UMockReader not registered", Container->IsRegistered<UMockReader>());
            TestTrue("IReader not registered", Container->IsRegistered<IReader>());
        });
    });

    Describe("Register Instance", [this]()
    {
        It("Should Register Instance", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterInstance(NewObject<UMockReader>());

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("UMockReader not registered", Container->IsRegistered<UMockReader>());
        });

        It("Should Register Instance As Self", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterInstance(NewObject<UMockReader>()).AsSelf();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("UMockReader not registered", Container->IsRegistered<UMockReader>());
        });

        It("Should Register Instance As Interface", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterInstance(NewObject<UMockReader>()).As<IReader>();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("IReader not registered", Container->IsRegistered<IReader>());
            TestFalse("UMockReader should not be registered", Container->IsRegistered<UMockReader>());
        });

        It("Should Register Instance As Interface And Self", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterInstance(NewObject<UMockReader>()).As<IReader>().AsSelf();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("IReader not registered", Container->IsRegistered<IReader>());
        });

        It("Should Register Instance By Interfaces", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterInstance(NewObject<UMockReader>()).ByInterfaces();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("IReader not registered", Container->IsRegistered<IReader>());
            TestFalse("UMockReader should not be registered", Container->IsRegistered<UMockReader>());
        });

        It("Should Register Instance By Interfaces And Self", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterInstance(NewObject<UMockReader>()).ByInterfaces().AsSelf();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("IReader not registered", Container->IsRegistered<IReader>());
        });
    });

    Describe("Register Factory", [this]()
    {
        It("Should Register Factory", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterFactory<UMockReader>([]() { return NewObject<UMockReader>(); });

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("UMockReader not registered", Container->IsRegistered<UMockReader>());
        });

        It("Should Register Factory As Self", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterFactory<UMockReader>([]() { return NewObject<UMockReader>(); }).AsSelf();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("UMockReader not registered", Container->IsRegistered<UMockReader>());
        });

        It("Should Register Factory As Interface", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterFactory<UMockReader>([]() { return NewObject<UMockReader>(); }).As<IReader>();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("IReader not registered", Container->IsRegistered<IReader>());
            TestFalse("UMockReader should not be registered", Container->IsRegistered<UMockReader>());
        });

        It("Should Register Factory As Interface And Self", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterFactory<UMockReader>([]() { return NewObject<UMockReader>(); }).As<IReader>().AsSelf();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("IReader not registered", Container->IsRegistered<IReader>());
        });

        It("Should Register Factory By Interfaces", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterFactory<UMockReader>([]() { return NewObject<UMockReader>(); }).ByInterfaces();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("IReader not registered", Container->IsRegistered<IReader>());
            TestFalse("UMockReader should not be registered", Container->IsRegistered<UMockReader>());
        });

        It("Should Register Factory By Interfaces And Self", [this]()
        {
            FObjectContainerBuilder Builder;
            Builder.RegisterFactory<UMockReader>([]() { return NewObject<UMockReader>(); }).ByInterfaces().AsSelf();

            UObjectContainer* Container = Builder.Build();

            TestNotNull("Container is nullptr", Container);
            TestTrue("IReader not registered", Container->IsRegistered<IReader>());
        });
    });
}