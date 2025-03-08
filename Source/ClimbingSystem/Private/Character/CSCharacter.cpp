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

void ACSCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	FVector ForwardDirection;
	FVector RightDirection;
	if (MovementComponent->IsClimbing())
	{
		ForwardDirection = FVector::CrossProduct(MovementComponent->GetClimbSurfaceNormal(), -GetActorRightVector());
		RightDirection = FVector::CrossProduct(MovementComponent->GetClimbSurfaceNormal(), GetActorUpVector());
	}
	else
	{
		if (Controller != nullptr)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		}
	}
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ACSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Started, this, &ACSCharacter::Climb);
		EnhancedInputComponent->BindAction(CancelClimbAction, ETriggerEvent::Started, this, &ACSCharacter::CancelClimb);
		EnhancedInputComponent->BindAction(MovebAction, ETriggerEvent::Triggered, this, &ACSCharacter::Move);
	}
}
