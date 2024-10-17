// Copyright Ola Magnusson


#include "Combat/LockonComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Enemy.h"

// Sets default values for this component's properties
ULockonComponent::ULockonComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = GetOwner<ACharacter>();
	// you always get controllers from the world, since it's not MP, getting the first ensures that it's the player.
	Controller = GetWorld()->GetFirstPlayerController();
	MovementComponent = OwnerRef->GetCharacterMovement();
	SpringArmComponent = OwnerRef->FindComponentByClass<USpringArmComponent>();
}

void ULockonComponent::StartLockon(float Radius)
{
	FHitResult OutResult;
	FVector CurrentLocation = OwnerRef->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	FCollisionQueryParams IgnoreParams = FCollisionQueryParams(FName(TEXT("Ignore Collision Params")), false, OwnerRef);
		
	bool bHasFoundTarget = GetWorld()->SweepSingleByChannel(
		OutResult,
		CurrentLocation,
		CurrentLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		Sphere,
		IgnoreParams
		);

	// check, otherwise crash if no target in range
	if (!bHasFoundTarget) { return; }

	if (!OutResult.GetActor()->Implements<UEnemy>()) { return;}
	CurrentTargetActor = OutResult.GetActor();

	Controller->SetIgnoreLookInput(true);
	MovementComponent->bOrientRotationToMovement = false;
	MovementComponent->bUseControllerDesiredRotation = true;

	SpringArmComponent->TargetOffset = FVector(0, 0, 100);

	IEnemy::Execute_OnSelect(CurrentTargetActor);
	
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Actor Detected: %s"),
		*OutResult.GetActor()->GetName()
		);
}

void ULockonComponent::EndLockon()
{
	IEnemy::Execute_OnDeselect(CurrentTargetActor);
	CurrentTargetActor = nullptr;
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->bUseControllerDesiredRotation = false;
	SpringArmComponent->TargetOffset = FVector(0, 0, 0);

	Controller->ResetIgnoreLookInput();
}

void ULockonComponent::ToggleLockon(float Radius)
{
	if (IsValid(CurrentTargetActor))
	{
		EndLockon();
	}
	else
	{
		StartLockon(Radius);
	}
}

void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(CurrentTargetActor)) { return; }

	FVector CurrentLocation = OwnerRef->GetActorLocation();
	FVector TargetLocation = CurrentTargetActor->GetActorLocation();

	double Distance = FVector::Distance(CurrentLocation, TargetLocation);
	if (Distance >= BreakDistance)
	{
		EndLockon();
		return;
	}

	TargetLocation.Z -= 125;
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(
		CurrentLocation,
		TargetLocation
		);

	Controller->SetControlRotation(NewRotation);

}

