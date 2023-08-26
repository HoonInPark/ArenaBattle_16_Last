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

	/*static Ŭ���� �Ҽ����� �� 1���� ����, ��ü���� �������� ����
	const ��� : 1�� ���� ���ϸ� �ٲ� �� ����
	�ʱ�ȭ�� Ŭ���� �ۿ��� �Ѵ�.
	*/
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

	void RunAI();
	void StopAI();

	/* OnPossess�� �� Ÿ�̸Ӹ� �����Ű��
	* OnUnPossess�� �� Ÿ�̸Ӹ� �����Ų��.
	* Ÿ�̸ӿ����� ��� ������ ��ġ�� ��� �ش� ��ġ�� �̵���Ų��.
	virtual void OnUnPossess() override;

private:
	void OnRepeatTimer();			// Ÿ�̸� �Լ�

	FTimerHandle RepeatTimerHandle; // Ÿ�̸Ӹ� �����ϰ� ����Ű�� �ڵ�
	float RepeatInterval;			// Ÿ�̸� �ð�����
	*/

	// Ÿ�̸� ��� BehaviorTree�� ����Ѵ�

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;		// �����̺�� Ʈ�� ��ü ������

	UPROPERTY()
	class UBlackboardData* BBAsset;		// ������ ��ü ������
};
