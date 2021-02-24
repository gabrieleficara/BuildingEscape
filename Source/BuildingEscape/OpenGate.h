// Copyright Gabriele Ficara 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "OpenGate.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenGate : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenGate();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

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
	float StartHeight;
	UPROPERTY()
	float TargetHeight;
	UPROPERTY()
	bool OpenDoorSound = false;
	UPROPERTY()
	bool CloseDoorSound = true;
	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	/** Difference before closed and opened gate(Height)*/
	UPROPERTY(EditAnywhere)
	float DeltaHeight = 200.f;

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