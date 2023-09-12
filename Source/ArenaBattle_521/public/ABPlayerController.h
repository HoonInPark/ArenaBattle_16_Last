// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle_521.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * ABPlayerController 생성 -> ABPawn 생성 -> ABPlayerController가 ABPawn에 빙의 -> BeginPlay() 호출
 */
UCLASS()
class ARENABATTLE_521_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* _aPawn) override;
};
