// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameMode.h"
#include "ABPawn.h"

AABGameMode::AABGameMode()
{
	// ������ ���� ����� �����ϰ� �̸� DefaultPawn�� �����ϴ� �� �ƴ϶�, Ŭ������ ������ �̸� �����ϴ� ����̴�.
	DefaultPawnClass = AABPawn::StaticClass();
}
