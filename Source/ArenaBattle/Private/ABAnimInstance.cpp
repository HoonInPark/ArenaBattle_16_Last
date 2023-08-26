// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("AnimMontage'/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// �� ABAnimInstance�� ����ϴ� ���� ���´�.
	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;

	// �ش� ���� ��ȿ�ϸ� ����
	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UABAnimInstance::PlayAttackMontage()
{
	ABCHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);
}


void UABAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(!IsDead);
	ABCHECK(Montage_IsPlaying(AttackMontage));
	//if (!Montage_IsPlaying(AttackMontage))
	//	Montage_Play(AttackMontage);
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

//AttackHitCheck�� �̺�Ʈ ȣ�� ó��
void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	ABLOG_S(Warning);
	OnAttackHitCheck.Broadcast();
}

//NextAttackCheck�� �̺�Ʈ ȣ�� ó��
void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	ABLOG_S(Warning);
	// �� ��������Ʈ�� ����� ��� �Լ����� ȣ���ض�
	OnNextAttackCheck.Broadcast();
}


FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	ABLOG(Warning, TEXT("Attack%d"), Section);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
