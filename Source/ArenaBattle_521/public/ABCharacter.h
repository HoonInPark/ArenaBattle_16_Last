// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/*
* 다음과 같이 Character를 통해 PlayerPawn을 설정하면 Pawn 클래스를 통해 했을 때와 달리 
* 낭떠러지에서 떨어지는 것을 확인할 수 있다.
*/
#include "ArenaBattle_521.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

UCLASS()
class ARENABATTLE_521_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
};
