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

	/*C++에서 동적할당으로 생성한 객체를 포인터변수에 저장한다.
	객체를 더 이상 사용하지 않을 때는 반드시 delete로 객체를 해제해야 하는데
	언리얼은 UPROPERTY()매크로를 사용해서 자동으로 객체를 해제해준다.*/

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Water;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* Light;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* Splash;

	/*객체타입은 VisibleAnywhere로 설정하고
	값타입(프리미티브 타입(int, float...), 문자열(FString, FName), 구조체(FVector, FRotator, FTransform...)
	은 EditAnywhere를 사용한다.
	*/
	UPROPERTY(EditAnywhere, Category=ID)
	int32 ID;

	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* Movement;

private:
	/* AllowPrivateAccess = true는 Unity의 [SerializeField]와 동일한 속성
	* 프로그래밍 상에서는 private설정으로 직접 접근할 수 없도록 private을 유지하되
	* 에디터에서는 편집가능하도록 접근할 수 있는 설정
	*/
	UPROPERTY(EditAnywhere, Category=Stat, Meta=(AllowPrivateAccess = true))
	float RotateSpeed;
};
