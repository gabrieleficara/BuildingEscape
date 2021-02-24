// Copyright Gabriele Ficara 2021


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	StartYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw = StartYaw;
	FindAudioComponent();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s has an OpenDoor component but no PressurePlate selected"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenDoor)
	{
		// Opening Door
		TargetYaw = StartYaw + DeltaYaw;

		MoveDoor(DeltaTime, DoorOpenSpeed, OpenDoorSound);
		DoorLastOpen = GetWorld()->GetTimeSeconds();
		OpenDoorSound = true;
		CloseDoorSound = false;
	}
	else if (DoorCloseDelay < GetWorld()->GetTimeSeconds() - DoorLastOpen)
	{
		// Closing Door
		TargetYaw = StartYaw;

		MoveDoor(DeltaTime, DoorCloseSpeed, CloseDoorSound);
		OpenDoorSound = false;
		CloseDoorSound = true;
	}

}

void UOpenDoor::MoveDoor(const float& DeltaTime, const float& DoorSpeed, const bool& HasSoundPlayed)
{
	FRotator OpenDoor = GetOwner()->GetActorRotation();

	// Uncomment for linear interpolation
	// OpenDoor.Yaw = FMath::FInterpConstantTo(OpenDoor.Yaw, TargetYaw, DeltaTime, DoorSpeed * 25);
	// Logaritmic interpolation
	OpenDoor.Yaw = FMath::FInterpTo(OpenDoor.Yaw, TargetYaw, DeltaTime, DoorSpeed);
	
	GetOwner()->SetActorRotation(OpenDoor);
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor %s is missin an audio component"), *GetOwner()->GetName())
	}
	if (!HasSoundPlayed)
	{
		AudioComponent->Play();
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	if (!PressurePlate) {return (0.f);}
	// Find all overlapping Actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor: OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component!"), *GetOwner()->GetName());
	}
}