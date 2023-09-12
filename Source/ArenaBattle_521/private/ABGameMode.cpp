// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
#include "ABPawn.h"
#include "ABPlayerController.h"

AABGameMode::AABGameMode()
{
	// ������ ���� ����� �����ϰ� �̸� DefaultPawn�� �����ϴ� �� �ƴ϶�, Ŭ������ ������ �̸� �����ϴ� ����̴�.
	DefaultPawnClass = AABPawn::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
}

void AABGameMode::PostLogin(APlayerController* _NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(_NewPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));
}
