// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "AIController.h"
#include "ABAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AABAIController();
	virtual void OnPossess(APawn* InPawn) override;

	/*static 클래스 소속으로 단 1개만 생성, 객체별로 생성되지 않음
	const 상수 : 1번 값을 정하면 바꿀 수 없다
	초기화는 클래스 밖에다 한다.
	*/
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

	void RunAI();
	void StopAI();

	/* OnPossess일 때 타이머를 실행시키고
	* OnUnPossess일 때 타이머를 종료시킨다.
	* 타이머에서는 계속 랜덤한 위치를 얻고 해당 위치로 이동시킨다.
	virtual void OnUnPossess() override;

private:
	void OnRepeatTimer();			// 타이머 함수

	FTimerHandle RepeatTimerHandle; // 타이머를 생성하고 가리키는 핸들
	float RepeatInterval;			// 타이머 시간간격
	*/

	// 타이머 대신 BehaviorTree를 사용한다

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;		// 비헤이비어 트리 객체 포인터

	UPROPERTY()
	class UBlackboardData* BBAsset;		// 블랙보드 객체 포인터
};
