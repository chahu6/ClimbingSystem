// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CSCharacter.h"
#include "Components/CustomCharacterMovementComponent.h"
#include "EnhancedInputComponent.h"

ACSCharacter::ACSCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = Cast<UCustomCharacterMovementComponent>(GetCharacterMovement());
}

void ACSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACSCharacter::Climb()
{
	MovementComponent->TryClimbing();
}

void ACSCharacter::CancelClimb()
{
	MovementComponent->CancelClimbing();
}

void ACSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Started, this, &ACSCharacter::Climb);
		EnhancedInputComponent->BindAction(CancelClimbAction, ETriggerEvent::Started, this, &ACSCharacter::CancelClimb);
	}
}
