// Fill out your copyright notice in the Description page of Project Settings.

#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance(): PawnMovement_AnimInst() // �̷��� �ϸ� FPawnMovement�� �⺻ ������ �ʱ�ȭ�ȴ�. �׳� ���϶�� �����ϴ� �����.
{
	// ABLOG(Warning, TEXT(" PawnMovement_AnimInst : %f, %hhd"), PawnMovement_AnimInst._Velocity, PawnMovement_AnimInst._IsFalling);
	CurrentPawnSpeed = 0.f;
	IsInAir = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(
		TEXT("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
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

void UABAnimInstance::SendMovement_Implementation(FPawnMovement _PawnMovement)
{
	PawnMovement_AnimInst = _PawnMovement;
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
