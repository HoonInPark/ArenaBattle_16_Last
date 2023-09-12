// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle_521.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * ABPlayerController ���� -> ABPawn ���� -> ABPlayerController�� ABPawn�� ���� -> BeginPlay() ȣ��
 */
UCLASS()
class ARENABATTLE_521_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* _aPawn) override;
};
