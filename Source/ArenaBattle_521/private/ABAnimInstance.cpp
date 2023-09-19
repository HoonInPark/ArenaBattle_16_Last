// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance(): PawnMovement_AnimInst() // 이렇게 하면 FPawnMovement가 기본 값으로 초기화된다. 그냥 편하라고 제공하는 기능임.
{
	// ABLOG(Warning, TEXT(" PawnMovement_AnimInst : %f, %hhd"), PawnMovement_AnimInst._Velocity, PawnMovement_AnimInst._IsFalling);
	CurrentPawnSpeed = 0.f;
	bIsInAir = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(
		TEXT("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
		AttackMontage = ATTACK_MONTAGE.Object;
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UABAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
		Montage_Play(AttackMontage, 1.f);
}

void UABAnimInstance::SendMovement_Implementation(FPawnMovement _PawnMovement)
{
	CurrentPawnSpeed = _PawnMovement._Velocity;
	bIsInAir = _PawnMovement._bIsInAir;
	ABLOG(Warning, TEXT(" _PawnMovement : %f, %hhd"), _PawnMovement._Velocity, _PawnMovement._bIsInAir);
}

void UABAnimInstance::SendEvent_Implementation(EEventType _EventType)
{
	switch (_EventType)
	{
	case EEventType::ATTACK:
		PlayAttackMontage();
		ABLOG_S(Warning);
		break;
	case EEventType::JUMP:
		
		break;
	}
}
