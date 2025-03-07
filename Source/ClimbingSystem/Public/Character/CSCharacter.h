// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CSCharacter.generated.h"

UCLASS()
class CLIMBINGSYSTEM_API ACSCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ClimbAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CancelClimbAction;

public:
	ACSCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	void Climb();

	void CancelClimb();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	FORCEINLINE UCustomCharacterMovementComponent* GetCustomCharacterMovement() const { return MovementComponent; }

protected:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	UCustomCharacterMovementComponent* MovementComponent;
};
