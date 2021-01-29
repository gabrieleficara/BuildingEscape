// Copyright Gabriele Ficara 2021


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	StartYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw = StartYaw + DeltaYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s has an OpenDoor component but no PressurePlate selected"), *GetOwner()->GetName());
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		// Opening Door
		TargetYaw = StartYaw + DeltaYaw;

		MoveDoor(DeltaTime, DoorOpenSpeed);
		DoorLastOpen = GetWorld()->GetTimeSeconds();
	}
	else if (DoorCloseDelay < GetWorld()->GetTimeSeconds() - DoorLastOpen)
	{
		// Closing Door
		TargetYaw = StartYaw;

		MoveDoor(DeltaTime, DoorCloseSpeed);
	}

}

void UOpenDoor::MoveDoor(const float& DeltaTime, const float& DoorSpeed)
{
	FRotator OpenDoor = GetOwner()->GetActorRotation();

	// UE_LOG(LogTemp, Display, TEXT("Actor %s starting Rotation: %s"), *GetOwner()->GetName(), *OpenDoor.ToString());
	// UE_LOG(LogTemp, Display, TEXT("Target yaw: %f"), *GetOwner()->GetName(), TargetYaw);

	// Uncomment for linear interpolation
	// OpenDoor.Yaw = FMath::FInterpConstantTo(OpenDoor.Yaw, TargetYaw, DeltaTime, DoorSpeed * 25);
	// Logaritmic interpolation
	OpenDoor.Yaw = FMath::FInterpTo(OpenDoor.Yaw, TargetYaw, DeltaTime, DoorSpeed);
	
	GetOwner()->SetActorRotation(OpenDoor);
}
