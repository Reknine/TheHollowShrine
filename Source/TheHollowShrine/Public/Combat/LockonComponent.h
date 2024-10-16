// Copyright Ola Magnusson

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockonComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEHOLLOWSHRINE_API ULockonComponent : public UActorComponent
{
	GENERATED_BODY()

	ACharacter* OwnerRef;
	APlayerController* Controller;
	class UCharacterMovementComponent* MovementComponent;
	class USpringArmComponent* SpringArmComponent;

public:	
	// Sets default values for this component's properties
	ULockonComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartLockon(float Radius = 750.0f);
	
	void EndLockon();

	UFUNCTION(BlueprintCallable)
	void ToggleLockon(float Radius = 750.0f);

public:
	AActor* CurrentTargetActor;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
