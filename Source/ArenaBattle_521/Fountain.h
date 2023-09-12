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
	* 만약 실수로 부모클래스에서 virtual을 쓰는 것을 잊고 그 함수를 자식 클래스에서 상속했을 때, 
	* 자식 클래스에서 그 함수에 override를 써 주면 컴파일러는 부모 클래스의 그 함수가 자식에게 물려주는 함수라는 것을 알 수 있다.
	* 여기서 중요한 것은, 원칙상 물려줄 함수면 부모 클래스에서 virtual을 써 주는 게 맞다는 것이다.
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
	* Component들은 위와 같이 선언한 뒤 에디터상에서 메시 같은 에셋을 배정해 줄 수 있었다.
	* 근데 아래와 같이 int32형으로 선언된 ID 변수는 	UPROPERTY(VisibleAnywhere)라고 헤줘도 
	* 에디터상에서 그 값을 변경할 수 없다.
	* 여기서 알 수 있는 것은, Component형을 변경하는 것(이를테면 Mesh를 ParticleSystem으로)과 
	* int32 변수의 값을 변경하는 것이 동일한 보호수준 상에서 일어난다는 것이다. 
	*/
	UPROPERTY(EditAnywhere, Category=ID)
	int32 ID;
};
