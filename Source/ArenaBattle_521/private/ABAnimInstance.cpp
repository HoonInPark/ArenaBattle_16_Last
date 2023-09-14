// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	IsInAir = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
		AttackMontage = ATTACK_MONTAGE.Object;
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		if (const auto Character = Cast<ACharacter>(Pawn))
			IsInAir = Character->GetMovementComponent()->IsFalling();
	}
}

void UABAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
		Montage_Play(AttackMontage, 1.f);
}
