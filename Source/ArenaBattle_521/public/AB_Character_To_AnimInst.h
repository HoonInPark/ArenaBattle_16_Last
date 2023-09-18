// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle_521.h"
#include "UObject/Interface.h"
#include "AB_Character_To_AnimInst.generated.h"

USTRUCT(BlueprintType)
struct FPawnMovement
{
	GENERATED_BODY()

	UPROPERTY()
	float _Velocity;

};

UENUM(BlueprintType)
enum class EEventType : uint8
{
	ATTACK,
	JUMP
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAB_Character_To_AnimInst : public UInterface
{
	GENERATED_BODY()
};

/**
 * Character에서 AnimInstance로 보내는 정보는 다음과 같다.
 * Pawn->GetVelocity().Size() => float
 * Character->GetMovementComponent()->IsFalling() => bool
 */
class ARENABATTLE_521_API IAB_Character_To_AnimInst
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void SendMovement(FPawnMovement _PawnMovement);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void SendEvent(EEventType _EventType);
};
