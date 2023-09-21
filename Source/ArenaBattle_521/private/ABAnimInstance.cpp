// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
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
	// if (!Montage_IsPlaying(AttackMontage))
	// 	Montage_Play(AttackMontage, 1.f);

	Montage_Play(AttackMontage, 1.f);
}

// ABCharacter���� ȣ��Ǵ� �Լ��̴�.
void UABAnimInstance::JumpToAttackMontageSection(int32 _NewSection)
{
	// Montage_IsPlaying()�� ��������Ʈ�� �ƴϰ� �׶����� Montage�� ����ǰ� �ִ��� Ȯ���ϴ� �Լ��̴�.
	ABCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(_NewSection), AttackMontage);
}

void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
	// ABLOG_S(Warning);
}

void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 _Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(_Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), _Section));
}
