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
	/* ABCharacter�� ��� ������Ʈ�� 
	* UABCharacterStatComponent�� UABCharacterWidget�� ������ �ִ�.
	* �Ʒ�ó�� ���� �ʰ� UABCharacterStatComponent* �� ����ص� ������
	* �Ʒ�ó�� �ϸ� ��ü�� ������ �ϵ�
	* �𸮾� ���ο��� ����ī��Ʈ�� �����ϴ� �Ϳ��� ������ �� �����Ƿ�
	* ������ �ϰ� �޸� ������ �����ο�����.
	*/
	TWeakObjectPtr<class UABCharacterStatComponent> CurrentCharacterStat;

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
