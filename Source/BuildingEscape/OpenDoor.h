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
	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void MoveDoor(const float& DeltaTime, const float& DoorSpeed, const bool& HasSoundPlayed);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	float GetStartingPos();

private:
	UPROPERTY()
	float DoorLastOpen = 0.f;
	UPROPERTY()
	float StartYaw;
	UPROPERTY()
	float TargetYaw;
	UPROPERTY()
	bool OpenDoorSound = false;
	UPROPERTY()
	bool CloseDoorSound = true;

	/** Difference before closed and opened door(Rotation)*/
	UPROPERTY(EditAnywhere)
	float DeltaYaw = 90.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 6.f;
	
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.8f;
	
	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 2.f;
	
	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 50.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
};
