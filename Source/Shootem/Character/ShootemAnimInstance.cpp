// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootemAnimInstance.h"
#include "ShootemCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Shootem/Weapon/Weapon.h"


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
	EquippedWeapon = ShootemCharacter->GetEquippedWeapon();
	bIsCrouched = ShootemCharacter->bIsCrouched;
	bAiming = ShootemCharacter->IsAiming();
	TurningInPlace = ShootemCharacter->GetTurningInPlace();

	FRotator AimRotation = ShootemCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShootemCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 15.f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = ShootemCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	AO_Yaw = ShootemCharacter->GetAO_Yaw();
	AO_Pitch = ShootemCharacter->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && ShootemCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		ShootemCharacter->GetMesh()->TransformToBoneSpace(FName("Hand_R"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));
	}
}