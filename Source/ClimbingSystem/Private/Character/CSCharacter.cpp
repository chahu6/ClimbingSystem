// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CSCharacter.h"
#include "Components/CustomCharacterMovementComponent.h"

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

void ACSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

