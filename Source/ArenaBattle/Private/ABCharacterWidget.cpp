// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterWidget.h"
#include "ABCharacterStatComponent.h"
#include "Components/ProgressBar.h"

/*
UABCharacterWidget과 UABCharacterStatComponent를 연결시켜서
HP값이 변경될 때 Widget의 프로그레스바에 적용되도록 하는 함수
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
	/* 위젯 블루프린트에 있는 PB_HPBar라는 UI 컴포넌트를 찾아라
	* 찾았으면 HPProgressBar에 저장하라.
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
			/*최초에는 1.0을 설정하고
			* 이후 HP이 변경될 때 마다 UpdateHPWidget()가 호출되어서
			* 변경된 값에 따라 비율을 얻어서 프로그레스바에 설정한다.
			*/
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}

