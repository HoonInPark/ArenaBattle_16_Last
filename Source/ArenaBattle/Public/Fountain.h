// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Fountain.generated.h"

UCLASS()
class ARENABATTLE_API AFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*C++���� �����Ҵ����� ������ ��ü�� �����ͺ����� �����Ѵ�.
	��ü�� �� �̻� ������� ���� ���� �ݵ�� delete�� ��ü�� �����ؾ� �ϴµ�
	�𸮾��� UPROPERTY()��ũ�θ� ����ؼ� �ڵ����� ��ü�� �������ش�.*/

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Water;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* Light;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* Splash;

	/*��üŸ���� VisibleAnywhere�� �����ϰ�
	��Ÿ��(������Ƽ�� Ÿ��(int, float...), ���ڿ�(FString, FName), ����ü(FVector, FRotator, FTransform...)
	�� EditAnywhere�� ����Ѵ�.
	*/
	UPROPERTY(EditAnywhere, Category=ID)
	int32 ID;

	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* Movement;

private:
	/* AllowPrivateAccess = true�� Unity�� [SerializeField]�� ������ �Ӽ�
	* ���α׷��� �󿡼��� private�������� ���� ������ �� ������ private�� �����ϵ�
	* �����Ϳ����� ���������ϵ��� ������ �� �ִ� ����
	*/
	UPROPERTY(EditAnywhere, Category=Stat, Meta=(AllowPrivateAccess = true))
	float RotateSpeed;
};
