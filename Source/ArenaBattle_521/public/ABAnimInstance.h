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
	 * VisibleDefaultsOnly�� VisibleInstanceOnly�� ����
	 * ���ڴ� ���� ���� �󿡼��� �� �� �ִ�.
	 * ���ڴ� �̰��� ���� �� ��üȭ���� �� ������ �󿡼��� �� �� �ִ�.
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	UAnimMontage* AttackMontage;
};
