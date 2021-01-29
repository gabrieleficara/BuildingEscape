// Copyright Gabriele Ficara 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void MoveDoor(const float& DeltaTime, const float& DoorSpeed);
	float TotalMassOfActors() const;
	void FindAudioComponent();

private:
	UPROPERTY()
	float DoorLastOpen = 0.f;
	UPROPERTY()
	float StartYaw;
	UPROPERTY()
	float TargetYaw;

	UPROPERTY(EditAnywhere)
	float DeltaYaw = 90.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 6.f;
	
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.8f;
	
	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 2.f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 50.f;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	UPROPERTY()
	bool BHasSoundPlayed = false;
};
