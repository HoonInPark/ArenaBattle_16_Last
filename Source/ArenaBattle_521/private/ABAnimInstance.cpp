// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAnimInstance.h"

UABAnimInstance::UABAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	IsInAir = false;
	IsDead = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(
		TEXT("/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
		AttackMontage = ATTACK_MONTAGE.Object;
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;

	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		if (const auto Character = Cast<ACharacter>(Pawn))
			IsInAir = Character->GetMovementComponent()->IsFalling();
	}
}

// ABCharacter���� Attack �Լ��� ���� ȣ��Ǵ� �Լ��̴�.
void UABAnimInstance::PlayAttackMontage()
{
	// if (!Montage_IsPlaying(AttackMontage))
	// 	Montage_Play(AttackMontage, 1.f);

	ABCHECK(!IsDead);

	Montage_Play(AttackMontage, 1.f);
}

/*
* Montage_JumpToSection()�� �𸮾󿡼� �⺻���� �����ϴ� Ű�����̴�.
* ù��° �μ��δ� ��Ÿ�� ���� �̸��� ����, �ι�° �μ��δ� �� ������ ���� ������ �ν��Ͻ� ������ ����.
*/
void UABAnimInstance::JumpToAttackMontageSection(int32 _NewSection)
{
	ABCHECK(!IsDead);

	// Montage_IsPlaying()�� ��������Ʈ�� �ƴϰ� �׶����� Montage�� ����ǰ� �ִ��� Ȯ���ϴ� �Լ��̴�.
	ABCHECK(Montage_IsPlaying(AttackMontage));
	ABLOG(Warning, TEXT(" Montage_IsPlaying(AttackMontage) : %d"), Montage_IsPlaying(AttackMontage));
	ABLOG(Warning, TEXT("Current Section is %s"), *(Montage_GetCurrentSection(AttackMontage).ToString()));

	Montage_JumpToSection(GetAttackMontageSectionName(_NewSection), AttackMontage);
}

void UABAnimInstance::AnimNotify_AttackHitCheck() // ������ ���� �ָ� ������ �� ȣ��.
{
	OnAttackHitCheck.Broadcast();
	//ABLOG_S(Warning);
}

void UABAnimInstance::AnimNotify_NextAttackCheck() // ������ �ֵθ��� ���� ���� �� ȣ��.
{
	OnNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 _Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(_Section, 1, 4), NAME_None);
	ABLOG(Warning, TEXT("Attack%d"), _Section);
	return FName(*FString::Printf(TEXT("Attack%d"), _Section));
}
