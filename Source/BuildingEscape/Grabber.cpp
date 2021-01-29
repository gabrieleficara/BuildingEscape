// Copyright Gabriele Ficara 2021

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

// Out is an empty macro we use to mark a function's parameters as an out parameter
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("GRABBER IN!"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get the players view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Get the players controller location and orientation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	// Draw a line from player showing the reach

	// The following is a vectorial sum
	// The Rotation has Norm (size) 1 so we need to multiply it by the lenght we need to reach
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	// DrawDebugLine Allows us to draw a line going between two points in our wold.
	// In our case from our location to one meter in the direction we are looking at
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0, 0, 255),
		false,
		0.f,
		0,
		5.f
	);

	FHitResult Hit;
	// Define Parameters to pass into the collision function
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	// Check if a Actor with physic properties is reachable by us
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Display, TEXT("Actor %s Ready is within reach to be grabbed"), *(ActorHit->GetName()));
	}
	// To find the physicshandle component the same pown as the grabber we will use GetOwner 
	// FindComponentByClass is a function template, meaning we need to use angle brackets to specify the class we are looking for.
	// It will find the first object in the class
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("")
		);
	}
	else
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("No physic handle component found in %s"),
			*GetOwner()->GetName()
		);
	}
}