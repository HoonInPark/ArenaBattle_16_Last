// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UABCharacterStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	/* ABCharacter는 멤버 컴포넌트로 
	* UABCharacterStatComponent과 UABCharacterWidget을 가지고 있다.
	* 아래처럼 하지 않고 UABCharacterStatComponent* 를 사용해도 되지만
	* 아래처럼 하면 객체를 참조는 하되
	* 언리얼 내부에서 참조카운트를 관리하는 것에는 배제할 수 있으므로
	* 참조만 하고 메모리 해제는 자유로워진다.
	*/
	TWeakObjectPtr<class UABCharacterStatComponent> CurrentCharacterStat;

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
