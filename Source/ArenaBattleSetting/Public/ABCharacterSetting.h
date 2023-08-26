// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABCharacterSetting.generated.h"

/**
 * 
 */

/*config������ DefaultArenaBattle.ini�� �о��*/
UCLASS(config=ArenaBattle)
class ARENABATTLESETTING_API UABCharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UABCharacterSetting();

	/*DefaultArenaBattle.ini�� �о CharacterAssets�� ��θ� ����*/
	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets;
};
