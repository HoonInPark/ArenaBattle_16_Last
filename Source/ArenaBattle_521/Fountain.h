// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle_521.h"
#include "GameFramework/Actor.h"
#include "Fountain.generated.h"

UCLASS()
class ARENABATTLE_521_API AFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/*
	* ���� �Ǽ��� �θ�Ŭ�������� virtual�� ���� ���� �ذ� �� �Լ��� �ڽ� Ŭ�������� ������� ��, 
	* �ڽ� Ŭ�������� �� �Լ��� override�� �� �ָ� �����Ϸ��� �θ� Ŭ������ �� �Լ��� �ڽĿ��� �����ִ� �Լ���� ���� �� �� �ִ�.
	* ���⼭ �߿��� ����, ��Ģ�� ������ �Լ��� �θ� Ŭ�������� virtual�� �� �ִ� �� �´ٴ� ���̴�.
	*/
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Water;
	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* Light;
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* Splash;
	/*
	* Component���� ���� ���� ������ �� �����ͻ󿡼� �޽� ���� ������ ������ �� �� �־���.
	* �ٵ� �Ʒ��� ���� int32������ ����� ID ������ 	UPROPERTY(VisibleAnywhere)��� ���൵ 
	* �����ͻ󿡼� �� ���� ������ �� ����.
	* ���⼭ �� �� �ִ� ����, Component���� �����ϴ� ��(�̸��׸� Mesh�� ParticleSystem����)�� 
	* int32 ������ ���� �����ϴ� ���� ������ ��ȣ���� �󿡼� �Ͼ�ٴ� ���̴�. 
	*/
	UPROPERTY(EditAnywhere, Category=ID)
	int32 ID;
};
