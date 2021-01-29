// Copyright Gabriele Ficara 2021

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Out is an empty macro we use to mark a function's parameters as an out parameter
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	// To find the physicshandle component the same pown as the grabber we will use GetOwner 
	// FindComponentByClass is a function template, meaning we need to use angle brackets to specify the class we are looking for.
	// It will find the first object in the class
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("No physic handle component found in %s"),
			*GetOwner()->GetName()
		);
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	// Input component is present on every actor so this should always be true
	if (InputComponent)
	{
		// instance of the class that is executing this code
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhisicBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	if (ActorHit)
	{	
		if (!PhysicsHandle) {return;}
		// If we hit something then attach the physics handle.
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			GetPlayersReach()
		);	
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) {return;}
	UE_LOG(LogTemp, Warning, TEXT("Grab button released"));
	PhysicsHandle->ReleaseComponent();
}

// Called every frame (Hot Loop)
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) {return;}
	// if the physics handle is attach
	if (PhysicsHandle->GrabbedComponent)
	{
		// move the object we are holding
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}


FHitResult UGrabber::GetFirstPhisicBodyInReach() const
{
	FHitResult Hit;
	// Raycast parameter to a certain distance
	// Define Parameters to pass into the collision function
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	// Check if a Actor with physic properties is reachable by us
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Display, TEXT("Actor %s Ready is within reach to be grabbed"), *(ActorHit->GetName()));
	}

	return Hit;
}

FVector UGrabber::GetPlayersReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Get the players controller location and orientation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	// The following is a vectorial sum
	// The Rotation has Norm (size) 1 so we need to multiply it by the lenght we need to reach
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Get the players controller location and orientation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}