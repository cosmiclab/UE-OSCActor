// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OSCActor.h"
#include "Subsystems/EngineSubsystem.h"
#include "OSCServer.h"
#include "OSCBundle.h"
#include "OSCCineCameraActor.h"
#include "OSCManagerSubsystem.h"
#include "OSCActorSubsystem.generated.h"

UCLASS(config=Project, defaultconfig)
class UOSCActorSettings : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, config, Category = OSCActor, meta = (GetOptions = "GetServerNames"))
	TArray<FString> OSCServerNames;

	UPROPERTY(EditAnywhere, config, Category = OSCActor)
	float SensorAspectRatio = 16.0 / 9.0;

	UFUNCTION()
	TArray<FString> GetServerNames() const
	{
		UOSCManagerSubsystem* OscManager = GEngine->GetEngineSubsystem<UOSCManagerSubsystem>();
		int32 ServerCount = OscManager->GetServerCount();
		TArray<FString> ServerNames;

		for (int ServerId = 0; ServerId < ServerCount; ++ServerId)
		{
			auto ServerInfo = OscManager->GetServerInfo(ServerId);
			ServerNames.Add(ServerInfo.ToString());
		}

		return ServerNames;
	}
};

UCLASS()
class OSCACTOR_API UOSCActorSubsystem : public UEngineSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Tickable impelmentation
	virtual TStatId GetStatId() const override;

	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(Category = "OSCActor", EditAnywhere, BlueprintReadOnly)
	int32 FrameNumber = 0;

	void UpdateActorReference(UActorComponent* Component_);
	void RemoveActorReference(UActorComponent* Component_);

protected:
	bool bInitialized = false;
	TArray<int32> ServerIds;
	TArray<UOSCServer*> OscServers;
	TMap<FString, UOSCActorComponent*> OSCActorComponentMap;
	TMap<FString, UOSCCineCameraComponent*> OSCCameraComponentMap;

	UFUNCTION()
	void OnOscBundleReceived(const FOSCBundle& Bundle, const FString& IPAddress, int32 Port);
};
