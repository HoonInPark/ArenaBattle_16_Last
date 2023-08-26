// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

/*Task�� ���� ������ ���Ѵ�.
Service�� �������� ���ս�Ű�� ���ν�
������(������, ������, �з���..) �Ϻ� Ʈ���� Task�� ����Ǵ� ����
Service�� ���������� ����Ǹ鼭, Blackboard�� ���� �����ϰų�
Background������ ó���ϱ� ���� ����̴�.
*/


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

// Interval �ֱ�� ��� ȣ���Ѵ�.
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// �����̺�� Ʈ���� ����� AIController -> ���� ã�´�.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();	// �߽���ġ
	float DetectRadius = 600.0f;							// �ݰ�

	if (nullptr == World)
		return;

	// ���� ��ġ�� �߽����� �ݰ� 6������ ���ȿ� Attack(Trace Preset)���� �˻��Ѵ�.
	// Attack Trace�� ����Ǵ� ����� Only ABCharacter�� �����س��Ҵ�.
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,			// Attack
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);	

	// ����Ǿ���.
	if (bResult)
	{
		// �ϴ� �������� Target�� null�� ����
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, nullptr);

		// OverlapMultiByChannel �� ���������Ƿ� TArray�� ��ü, ���� ���� ����� �� �ִ�.
		/* auto�� ���޵Ǵ� ���� ���� �ڷ����� �����ȴ�.
		* const�� ���޵� ���� ���⸦ �����ϰ� �б��������� ó���ϰ� ���� ��
		*/
		for (auto const& OverlapResult : OverlapResults)
		{
			AABCharacter* ABCharacter = Cast<AABCharacter>(OverlapResult.GetActor());
			// ABCharacter�� ���� And ������� ĳ���ͳ�?
			if (ABCharacter && ABCharacter->GetController()->IsPlayerController())
			{
				// ����Ǿ����� TargetKey�� ABCharacter�� ����.
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, ABCharacter);

				// ���� ���� ���� ���̰� ������
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, ABCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), ABCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}
	}

	// ���� ���� ���� ���̰� ������
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}