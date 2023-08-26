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

	// 이 ABAnimInstance를 사용하는 폰을 얻어온다.
	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;

	// 해당 폰이 유효하면 실행
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

//AttackHitCheck의 이벤트 호출 처리
void UABAnimInstance::AnimNotify_AttackHitCheck()
{
	ABLOG_S(Warning);
	OnAttackHitCheck.Broadcast();
}

//NextAttackCheck의 이벤트 호출 처리
void UABAnimInstance::AnimNotify_NextAttackCheck()
{
	ABLOG_S(Warning);
	// 이 델리게이트에 등록한 모든 함수들을 호출해라
	OnNextAttackCheck.Broadcast();
}


FName UABAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	ABLOG(Warning, TEXT("Attack%d"), Section);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
