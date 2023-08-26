// Fill out your copyright notice in the Description page of Project Settings.


#include "ABAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

// static const변수의 초기화
// 블랙보드에서 아래 해당 이름으로 찾아서 값을 저장/읽기를 하기 위해서
const FName AABAIController::HomePosKey(TEXT("HomePos"));
const FName AABAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AABAIController::TargetKey(TEXT("Target"));

AABAIController::AABAIController()
{
	//RepeatInterval = 3.0f;	// 타이머 간격 3초 설정

	// 생성된 블랙보드 애셋의 주소를 저장한다.
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/Book/AI/BB_ABCharacter.BB_ABCharacter'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	// 생성된 비헤이비어 트리 애셋의 주소를 저장한다.
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Book/AI/BT_ABCharacter.BT_ABCharacter'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}
void AABAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 타이머를 RepeatInterval 간격으로 계속 반복 호출
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AABAIController::OnRepeatTimer, RepeatInterval, true);

	/*AIController에서 블랙보드를 사용하기 위해 아래 함수를 호출해주소
	블랙보드를 Blackboard 변수를 통해 접근한다.
	*/
	/*
	if (UseBlackboard(BBAsset, Blackboard))
	{
		// HomePosKey에 설정한 이름으로 블랙보드 변수에 Pawn의 위치를 저장한다.
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		
		// AIController에서 비헤이비어 트리를 동작시킨다.
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

	// 반복되는 타이머를 종료시켜라
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

// 타이머가 호출될 때 실행하는 함수
void AABAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	ABCHECK(nullptr != CurrentPawn);

	// 네이게이션 시스템 사용하기 위한 객체 정보 얻기
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem)
		return;

	FNavLocation NextLocation;
	// 현재위치로부터 반경 5미터 이내의 임의의 위치 NextLocation에 저장
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		// 얻은 위치로 이동
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
