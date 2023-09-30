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

// ABCharacter에서 Attack 함수에 의해 호출되는 함수이다.
void UABAnimInstance::PlayAttackMontage()
{
	// if (!Montage_IsPlaying(AttackMontage))
	// 	Montage_Play(AttackMontage, 1.f);

	Montage_Play(AttackMontage, 1.f);
}

/*
* Montage_JumpToSection()는 언리얼에서 기본으로 제공하는 키워드이다.
* 첫번째 인수로는 몽타주 섹션 이름이 들어가고, 두번째 인수로는 그 섹션이 속한 몽차주 인스턴스 변수가 들어간다.
*/
void UABAnimInstance::JumpToAttackMontageSection(int32 _NewSection)
{
	// Montage_IsPlaying()는 델리게이트는 아니고 그때마다 Montage가 실행되고 있는지 확인하는 함수이다.
	ABCHECK(Montage_IsPlaying(AttackMontage));
	ABLOG(Warning, TEXT(" Montage_IsPlaying(AttackMontage) : %d"), Montage_IsPlaying(AttackMontage));
	ABLOG(Warning, TEXT("Current Section is %s"), *(Montage_GetCurrentSection(AttackMontage).ToString()));

	Montage_JumpToSection(GetAttackMontageSectionName(_NewSection), AttackMontage);
}

void UABAnimInstance::AnimNotify_AttackHitCheck() // 공격이 가장 멀리 뻗었을 때 호출.
{
	OnAttackHitCheck.Broadcast();
	//ABLOG_S(Warning);
}

void UABAnimInstance::AnimNotify_NextAttackCheck() // 공격을 휘두르는 것이 끝날 때 호출.
{
	OnNextAttackCheck.Broadcast();
}

FName UABAnimInstance::GetAttackMontageSectionName(int32 _Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(_Section, 1, 4), NAME_None);
	ABLOG(Warning, TEXT("Attack%d"), _Section);
	return FName(*FString::Printf(TEXT("Attack%d"), _Section));
}
