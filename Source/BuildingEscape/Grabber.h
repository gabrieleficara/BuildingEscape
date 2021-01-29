// Copyright Gabriele Ficara 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame (Hot Loop)
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();
	void GrabberDebugLine() const;
	// Return the first actor with a physic body
	FHitResult GetFirstPhisicBodyInReach() const;

	//nullptr -> Null pointer in c++
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// UPROPERTY allows to edit the Parameter from the editor
	// EditAnywere indicates that this property can be edited by property windows, on archetypes and instances. 
	UPROPERTY(EditAnywhere)
	bool DebugLine = false;

	UPROPERTY(EditAnywhere)
	float Reach = 100;
};
