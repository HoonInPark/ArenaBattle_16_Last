// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
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
