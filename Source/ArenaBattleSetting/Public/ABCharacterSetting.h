// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABCharacterSetting.generated.h"

/**
 * 
 */

/*config폴더의 DefaultArenaBattle.ini을 읽어라*/
UCLASS(config=ArenaBattle)
class ARENABATTLESETTING_API UABCharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UABCharacterSetting();

	/*DefaultArenaBattle.ini을 읽어서 CharacterAssets에 경로를 저장*/
	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets;
};
