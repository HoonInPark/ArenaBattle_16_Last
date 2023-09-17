// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle_521.h"
#include "AB_Character_To_AnimInst.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_521_API UABAnimInstance : public UAnimInstance, public IAB_Character_To_AnimInst
{
	GENERATED_BODY()

public:
	UABAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	bool IsInAir;

	/*
	 * VisibleDefaultsOnly와 VisibleInstanceOnly의 차이
	 * 전자는 블프 편집 상에서만 볼 수 있다.
	 * 후자는 이것이 레벨 상에 객체화됐을 때 에디터 상에서만 볼 수 있다.
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	UAnimMontage* AttackMontage;

private:
	FPawnMovement PawnMovement_AnimInst;
	virtual void SendMovement_Implementation(FPawnMovement _PawnMovement) override;
	virtual void SendEvent_Implementation(EEventType _EventType) override;
};
