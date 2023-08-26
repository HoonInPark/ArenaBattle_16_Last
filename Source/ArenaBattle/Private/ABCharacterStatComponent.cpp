// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterStatComponent.h"
#include "ABGameInstance.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// 일반 Actor는 PostInitializeComponent()로 초기화하는데
	// ActorComponent는 InitializeComponent()를 사용한다.
	// 사용하려면 true로 설정해야 한다.
	bWantsInitializeComponent = true;

	// ...

	Level = 1;
}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UABCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetNewLevel(Level);
}

void UABCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ABCHECK(nullptr != ABGameInstance);
	// 레벨에 해당하는 행의 정보를 모두 가져온다.
	CurrentStatData = ABGameInstance->GetABCharacterData(NewLevel);
	// 정상적으로 가져왔다면
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		//CurrentHP = CurrentStatData->MaxHP;
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		ABLOG(Error, TEXT("Level (%d) data does't exist"), NewLevel);
	}
}

void UABCharacterStatComponent::SetDamage(float NewDamage)
{
	ABCHECK(nullptr != CurrentStatData);
	/*현재HP에서 Damage를 감소하는데 범위는 0~MaxHP이다.
	만약 0미만이면 0의 값을, MaxHP값을 넘으면 MaxHP값을 리턴한다.*/
	//CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);
	//// HP가 모두 소진되었을 때
	//if (CurrentHP <= 0.0f)
	//{
	//	// 이 델리게이트에 등록한 모든 함수들을 실행하라.
	//	OnHPIsZero.Broadcast();
	//}

	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void UABCharacterStatComponent::SetHP(float NewHP)
{
	ABLOG(Warning, TEXT("SetHP(%f)"), NewHP);
	CurrentHP = NewHP;
	// HP가 변할 때마다 연결되는 함수를 호출(프로그레스 바 Percent변경)
	OnHPChanged.Broadcast();
	/*float나 double같은 실수값은 컴퓨터에서 근사치로 계산된다.
	그러므로 우리는 실수를 사칙연산할 때 정수처럼 딱 떨어지는 값이 아니라
	작은 소수점값이 남아 있을 수 있다.

	그래서 매운 작은 값인 0.0001보다 작으면 0이라고 판단한다.
	*/
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		// Dead애니메이션을 실행하기 위한 델리게이트 연결 함수 호출
		ABLOG(Warning, TEXT("OnHPIsZero.Broadcast()"));
		OnHPIsZero.Broadcast();
	}
}

// 스탯에 있는 공격값을 가져와라
// 상대를 공격할 때 전달할 Damage가 된다.
float UABCharacterStatComponent::GetAttack() const
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

/*프로그레스 바는 0 ~ 1.0사이로 값을 채운다.
1.0은 100%채우는 것이고 0은 비우는 것이다.
그러므로 CurrentHP값을 MaxHP값으로 나눠서
비율로 전달해야 한다.
*/
float UABCharacterStatComponent::GetHPRatio() const
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);

	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}

int32 UABCharacterStatComponent::GetDropExp() const
{
	return CurrentStatData->DropExp;
}