// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacterStatComponent.h"
#include "ABGameInstance.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// �Ϲ� Actor�� PostInitializeComponent()�� �ʱ�ȭ�ϴµ�
	// ActorComponent�� InitializeComponent()�� ����Ѵ�.
	// ����Ϸ��� true�� �����ؾ� �Ѵ�.
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
	// ������ �ش��ϴ� ���� ������ ��� �����´�.
	CurrentStatData = ABGameInstance->GetABCharacterData(NewLevel);
	// ���������� �����Դٸ�
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
	/*����HP���� Damage�� �����ϴµ� ������ 0~MaxHP�̴�.
	���� 0�̸��̸� 0�� ����, MaxHP���� ������ MaxHP���� �����Ѵ�.*/
	//CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);
	//// HP�� ��� �����Ǿ��� ��
	//if (CurrentHP <= 0.0f)
	//{
	//	// �� ��������Ʈ�� ����� ��� �Լ����� �����϶�.
	//	OnHPIsZero.Broadcast();
	//}

	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void UABCharacterStatComponent::SetHP(float NewHP)
{
	ABLOG(Warning, TEXT("SetHP(%f)"), NewHP);
	CurrentHP = NewHP;
	// HP�� ���� ������ ����Ǵ� �Լ��� ȣ��(���α׷��� �� Percent����)
	OnHPChanged.Broadcast();
	/*float�� double���� �Ǽ����� ��ǻ�Ϳ��� �ٻ�ġ�� ���ȴ�.
	�׷��Ƿ� �츮�� �Ǽ��� ��Ģ������ �� ����ó�� �� �������� ���� �ƴ϶�
	���� �Ҽ������� ���� ���� �� �ִ�.

	�׷��� �ſ� ���� ���� 0.0001���� ������ 0�̶�� �Ǵ��Ѵ�.
	*/
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		// Dead�ִϸ��̼��� �����ϱ� ���� ��������Ʈ ���� �Լ� ȣ��
		ABLOG(Warning, TEXT("OnHPIsZero.Broadcast()"));
		OnHPIsZero.Broadcast();
	}
}

// ���ȿ� �ִ� ���ݰ��� �����Ͷ�
// ��븦 ������ �� ������ Damage�� �ȴ�.
float UABCharacterStatComponent::GetAttack() const
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

/*���α׷��� �ٴ� 0 ~ 1.0���̷� ���� ä���.
1.0�� 100%ä��� ���̰� 0�� ���� ���̴�.
�׷��Ƿ� CurrentHP���� MaxHP������ ������
������ �����ؾ� �Ѵ�.
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