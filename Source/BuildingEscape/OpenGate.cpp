// Copyright Gabriele Ficara 2021


#include "OpenGate.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenGate::UOpenGate()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenGate::BeginPlay()
{
	Super::BeginPlay();
	StartHeight = GetOwner()->GetActorLocation().Z;
	TargetHeight = StartHeight;
	FindAudioComponent();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s has an OpenDoor component but no PressurePlate selected"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenGate::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenDoor)
	{
		// Opening Door
		TargetHeight = StartHeight + DeltaHeight;

		MoveDoor(DeltaTime, DoorOpenSpeed, OpenDoorSound);
		DoorLastOpen = GetWorld()->GetTimeSeconds();
		OpenDoorSound = true;
		CloseDoorSound = false;
	}
	else if (DoorCloseDelay < GetWorld()->GetTimeSeconds() - DoorLastOpen)
	{
		// Closing Door
		TargetHeight = StartHeight;

		MoveDoor(DeltaTime, DoorCloseSpeed, CloseDoorSound);
		OpenDoorSound = false;
		CloseDoorSound = true;
	}

}

void UOpenGate::MoveDoor(const float& DeltaTime, const float& DoorSpeed, const bool& HasSoundPlayed)
{
	FVector OpenDoor = GetOwner()->GetActorLocation();

	// Uncomment for linear interpolation
	// OpenDoor.Yaw = FMath::FInterpConstantTo(OpenDoor.Yaw, TargetHeight, DeltaTime, DoorSpeed * 25);
	// Logaritmic interpolation
	OpenDoor.Z = FMath::FInterpTo(OpenDoor.Z, TargetHeight, DeltaTime, DoorSpeed);
	
	GetOwner()->SetActorLocation(OpenDoor);
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor %s is missin an audio component"), *GetOwner()->GetName())
	}
	if (!HasSoundPlayed)
	{
		AudioComponent->Play();
	}
}


float UOpenGate::TotalMassOfActors() const
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

void UOpenGate::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component!"), *GetOwner()->GetName());
	}
}
