// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle_521.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class ARENABATTLE_521_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	UPROPERTY(VisibleAnywhere, Category = Box)
	UBoxComponent* Trigger;
	UPROPERTY(VisibleAnywhere, Category = Box)
	UStaticMeshComponent* Box;
	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* Effect;
	UPROPERTY(EditInstanceOnly, Category = Box)
	TSubclassOf<class AABWeapon> WeaponItemClass;

private:
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* _OverlapComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* _PSystem);
};
