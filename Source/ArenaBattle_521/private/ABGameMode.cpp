// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
//#include "ABPawn.h"
#include "ABCharacter.h"
#include "ABPlayerController.h"

AABGameMode::AABGameMode()
{
	// 다음은 폰을 월드상에 생성하고 이를 DefaultPawn로 지정하는 게 아니라, 클래스의 정보를 미리 저장하는 행우이다.
	DefaultPawnClass = AABCharacter::StaticClass();
	PlayerControllerClass = AABPlayerController::StaticClass();
}

void AABGameMode::PostLogin(APlayerController* _NewPlayer)
{
	ABLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(_NewPlayer);
	ABLOG(Warning, TEXT("PostLogin End"));
}
