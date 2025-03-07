// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CustomCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/ECustomMovement.h"

void UCustomCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	ClimbQueryParams.AddIgnoredActor(GetOwner());
}

void UCustomCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SweepAndStoreWallHits();
}

void UCustomCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	if (bWantsToClimb)
	{
		SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovementMode::CMOVE_Climbing);
	}

	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
}

void UCustomCharacterMovementComponent::TryClimbing()
{
	if (CanStartClimbing())
	{
		bWantsToClimb = true;
	}
}

void UCustomCharacterMovementComponent::CancelClimbing()
{
	bWantsToClimb = false;
}

bool UCustomCharacterMovementComponent::IsClimbing() const
{
	return MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == ECustomMovementMode::CMOVE_Climbing;
}

FVector UCustomCharacterMovementComponent::GetClimbSurfaceNormal() const
{
	return CurrentWallHits.Num() > 0 ? CurrentWallHits[0].Normal : FVector::Zero();
}

void UCustomCharacterMovementComponent::SweepAndStoreWallHits()
{
	const FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CollisionCapsuleRadius, CollisionCapsuleHalfHeight);
	const FVector StartOffset = UpdatedComponent->GetForwardVector() * 20;

	//避免使用相同的开始/结束位置进行Sweep，因为它不会触发对风景的Hit。
	const FVector Start = UpdatedComponent->GetComponentLocation() + StartOffset;
	const FVector End = Start + UpdatedComponent->GetForwardVector();

	TArray<FHitResult> Hits;
	const bool bHitWall = GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_WorldStatic, CollisionShape, ClimbQueryParams);

	bHitWall ? CurrentWallHits = Hits : CurrentWallHits.Reset();

#if ENABLE_DRAW_DEBUG
	DrawDebugCapsule(GetWorld(), Start, CollisionCapsuleHalfHeight, CollisionCapsuleRadius, FQuat::Identity, FColor::Red, false, 0.f);
	DrawDebugCapsule(GetWorld(), End, CollisionCapsuleHalfHeight, CollisionCapsuleRadius, FQuat::Identity, FColor::Green, false, 0.f);
#endif
	EyeHeightTrace(100.f);
}

bool UCustomCharacterMovementComponent::CanStartClimbing()
{
	for (FHitResult& Hit : CurrentWallHits)
	{
		const FVector HorizontalNormal = Hit.Normal.GetSafeNormal2D();

		const float HorizontalDot = FVector::DotProduct(UpdatedComponent->GetForwardVector(), -HorizontalNormal);
		const float VerticalDot = FVector::DotProduct(Hit.Normal, HorizontalNormal);

		const float HorizontalDegress = FMath::RadiansToDegrees(FMath::Acos(HorizontalDot));

		const bool bIsCeiling = FMath::IsNearlyZero(VerticalDot);

		if (HorizontalDegress <= MinHorizontalDegreesToStartClimbing 
			&& !bIsCeiling && IsFacingSurface(VerticalDot))
		{
			return true;
		}
	}
	return false;
}

bool UCustomCharacterMovementComponent::EyeHeightTrace(const float TraceDistance) const
{
	FHitResult UpperEdgeHit;

	const FVector Start = UpdatedComponent->GetComponentLocation() +
		(UpdatedComponent->GetUpVector() * GetCharacterOwner()->BaseEyeHeight);
	const FVector End = Start + (UpdatedComponent->GetForwardVector() * TraceDistance);

#if ENABLE_DRAW_DEBUG
	DrawDebugLine(GetWorld(), Start, End, FColor::Yellow);
#endif

	return GetWorld()->LineTraceSingleByChannel(UpperEdgeHit, Start, End, ECC_WorldStatic, ClimbQueryParams);
}

bool UCustomCharacterMovementComponent::IsFacingSurface(float Steepness) const
{
	constexpr float BaseLength = 80.f;
	const float SteepnessMultiplier = 1 + (1 - Steepness) * 5;

	return EyeHeightTrace(BaseLength * SteepnessMultiplier);
}
