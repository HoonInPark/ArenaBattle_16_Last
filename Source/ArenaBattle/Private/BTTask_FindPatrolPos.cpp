// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "ABAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// �� Task�� �Ҽӵ� BehaviorTree -> BehaviorTree�� ����ϴ� AIController
	// AIController�� ���ǵ� Pawn(Character)
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	// Pawn�� �����ϴ� ����(Level)�� �׺���̼� �ý����� �� �۵��ϴ���
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
		return EBTNodeResult::Failed;

	// BehaviorTree -> Blackboard -> HomePosKey ���� �����Ͷ�
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AABAIController::HomePosKey);
	FNavLocation NextPatrol;

	// Origin ��ġ���� 5���� �ݰ����� ������ ��ġ�� ���ؼ� NextPatrol�� �����ض�
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		// Blackboard�� PatrolPos�� �̵��� NextPatrol���� �����ض�
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AABAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}