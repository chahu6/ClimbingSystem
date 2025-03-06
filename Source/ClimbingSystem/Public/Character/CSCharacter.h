// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CSCharacter.generated.h"

UCLASS()
class CLIMBINGSYSTEM_API ACSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACSCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	FORCEINLINE UCustomCharacterMovementComponent* GetCustomCharacterMovement() const { return MovementComponent; }

protected:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	UCustomCharacterMovementComponent* MovementComponent;
};
