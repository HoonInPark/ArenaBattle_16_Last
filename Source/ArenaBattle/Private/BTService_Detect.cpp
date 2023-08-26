// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "ABAIController.h"
#include "ABCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

/*Task는 단위 업무를 말한다.
Service는 컴포짓에 결합시키는 노드로써
컴포짓(시퀀스, 셀렉터, 패러랠..) 하부 트리의 Task가 실행되는 동안
Service는 지속적으로 실행되면서, Blackboard에 값을 저장하거나
Background업무를 처리하기 위한 노드이다.
*/


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

// Interval 주기로 계속 호출한다.
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 비헤이비어 트리와 연결된 AIController -> 폰을 찾는다.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();	// 중심위치
	float DetectRadius = 600.0f;							// 반경

	if (nullptr == World)
		return;

	// 현재 위치를 중심으로 반경 6미터의 원안에 Attack(Trace Preset)으로 검색한다.
	// Attack Trace로 검출되는 대상은 Only ABCharacter로 설정해놓았다.
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

	// 검출되었다.
	if (bResult)
	{
		// 일단 블랙보드의 Target에 null을 저장
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, nullptr);

		// OverlapMultiByChannel 로 검출했으므로 TArray의 객체, 여러 개가 검출될 수 있다.
		/* auto는 전달되는 값에 따라 자료형이 결정된다.
		* const는 전달된 값을 쓰기를 방지하고 읽기전용으로 처리하고 싶을 때
		*/
		for (auto const& OverlapResult : OverlapResults)
		{
			AABCharacter* ABCharacter = Cast<AABCharacter>(OverlapResult.GetActor());
			// ABCharacter가 검출 And 사용자의 캐릭터냐?
			if (ABCharacter && ABCharacter->GetController()->IsPlayerController())
			{
				// 검출되었으니 TargetKey에 ABCharacter를 저장.
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, ABCharacter);

				// 범위 등을 눈에 보이게 디버깅용
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, ABCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), ABCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}
	}

	// 범위 등을 눈에 보이게 디버깅용
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}