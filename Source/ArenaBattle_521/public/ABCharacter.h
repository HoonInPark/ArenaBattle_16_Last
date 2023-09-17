// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/*
* 다음과 같이 Character를 통해 PlayerPawn을 설정하면 Pawn 클래스를 통해 했을 때와 달리 
* 낭떠러지에서 떨어지는 것을 확인할 수 있다.
*/
#include "ArenaBattle_521.h"
#include "AB_Character_To_AnimInst.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

UCLASS()
class ARENABATTLE_521_API AABCharacter : public ACharacter, public IAB_Character_To_AnimInst
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode 
	{
		GTA, 
		DIABLO
	};

	void SetControlMode(EControlMode _NewControlMode);
	EControlMode CurrentControlMode;
	FVector DirectionToMove;

	float ArmLengthTo = 0.f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.f;
	float ArmRotationSpeed = 0.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

private:
	void UpDown(float _NewAxisValue);
	void LeftRight(float _NewAxisValue);
	void LookUp(float _NewAxisValue);
	void Turn(float _NewAxisValue);

	void ViewChange();
	void Attack();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* _Montage, bool _bInterrupted);

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;
};
