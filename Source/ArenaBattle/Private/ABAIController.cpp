// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

// static const������ �ʱ�ȭ
// �����忡�� �Ʒ� �ش� �̸����� ã�Ƽ� ���� ����/�б⸦ �ϱ� ���ؼ�
const FName AABAIController::HomePosKey(TEXT("HomePos"));
const FName AABAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AABAIController::TargetKey(TEXT("Target"));

AABAIController::AABAIController()
{
	//RepeatInterval = 3.0f;	// Ÿ�̸� ���� 3�� ����

	// ������ ������ �ּ��� �ּҸ� �����Ѵ�.
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/Book/AI/BB_ABCharacter.BB_ABCharacter'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	// ������ �����̺�� Ʈ�� �ּ��� �ּҸ� �����Ѵ�.
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Book/AI/BT_ABCharacter.BT_ABCharacter'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}
void AABAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Ÿ�̸Ӹ� RepeatInterval �������� ��� �ݺ� ȣ��
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AABAIController::OnRepeatTimer, RepeatInterval, true);

	/*AIController���� �����带 ����ϱ� ���� �Ʒ� �Լ��� ȣ�����ּ�
	�����带 Blackboard ������ ���� �����Ѵ�.
	*/
	/*
	if (UseBlackboard(BBAsset, Blackboard))
	{
		// HomePosKey�� ������ �̸����� ������ ������ Pawn�� ��ġ�� �����Ѵ�.
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		
		// AIController���� �����̺�� Ʈ���� ���۽�Ų��.
		if (!RunBehaviorTree(BTAsset))
		{
			ABLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
	*/
}
/*
void AABAIController::OnUnPossess()
{
	Super::OnUnPossess();

	// �ݺ��Ǵ� Ÿ�̸Ӹ� ������Ѷ�
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

// Ÿ�̸Ӱ� ȣ��� �� �����ϴ� �Լ�
void AABAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	ABCHECK(nullptr != CurrentPawn);

	// ���̰��̼� �ý��� ����ϱ� ���� ��ü ���� ���
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem)
		return;

	FNavLocation NextLocation;
	// ������ġ�κ��� �ݰ� 5���� �̳��� ������ ��ġ NextLocation�� ����
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		// ���� ��ġ�� �̵�
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		ABLOG(Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
	}
}
*/

void AABAIController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			ABLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void AABAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
