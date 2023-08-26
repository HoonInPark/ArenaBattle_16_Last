// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterWidget.h"
#include "ABCharacterStatComponent.h"
#include "Components/ProgressBar.h"

/*
UABCharacterWidget�� UABCharacterStatComponent�� ������Ѽ�
HP���� ����� �� Widget�� ���α׷����ٿ� ����ǵ��� �ϴ� �Լ�
*/
void UABCharacterWidget::BindCharacterStat(UABCharacterStatComponent* NewCharacterStat)
{
	ABCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	//NewCharacterStat->OnHPChanged.AddLambda([this]() -> void {
	//	if (CurrentCharacterStat.IsValid())
	//	{
	//		ABLOG(Warning, TEXT("HPRatio : %f"), CurrentCharacterStat->GetHPRatio());
	//	}
	//});

	NewCharacterStat->OnHPChanged.AddUObject(this, &UABCharacterWidget::UpdateHPWidget);
}

void UABCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	/* ���� �������Ʈ�� �ִ� PB_HPBar��� UI ������Ʈ�� ã�ƶ�
	* ã������ HPProgressBar�� �����϶�.
	*/
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	ABCHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void UABCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			/*���ʿ��� 1.0�� �����ϰ�
			* ���� HP�� ����� �� ���� UpdateHPWidget()�� ȣ��Ǿ
			* ����� ���� ���� ������ �� ���α׷����ٿ� �����Ѵ�.
			*/
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}

