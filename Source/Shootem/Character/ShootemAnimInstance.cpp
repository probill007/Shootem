// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootemAnimInstance.h"

#include "ShootemCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UShootemAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ShootemCharacter = Cast<AShootemCharacter>(TryGetPawnOwner());
}

void UShootemAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (ShootemCharacter == nullptr)
	{
		ShootemCharacter = Cast<AShootemCharacter>(TryGetPawnOwner());
	}
	if (ShootemCharacter == nullptr) return;

	FVector Velocity = ShootemCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = ShootemCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = ShootemCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bWeaponEquipped = ShootemCharacter->IsWeaponEquipped();
	bIsCrouched = ShootemCharacter->bIsCrouched;
	bAiming = ShootemCharacter->IsAiming();
}