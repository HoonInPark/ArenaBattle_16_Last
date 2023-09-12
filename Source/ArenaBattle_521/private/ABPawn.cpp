// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPawn.h"


// Sets default values
AABPawn::AABPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AABPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region LoggingGamePlayFramework
/*
* ������ ���� PossessedBy�� PostInitializeComponents�� ȣ���ϸ� �α׸� Ȯ���� �� �ִ�.
* ArenaBattle_521: Warning: AABPlayerController::PostInitializeComponents(9)
* ArenaBattle_521: Warning: AABGameMode::PostLogin(17)PostLogin Begin
* ArenaBattle_521: Warning: AABPawn::PostInitializeComponents(31)
* ArenaBattle_521: Warning: AABPlayerController::OnPossess(14)
* ArenaBattle_521: Warning: AABPawn::PossessedBy(36)
* ArenaBattle_521: Warning: AABGameMode::PostLogin(19)PostLogin End
*/
void AABPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

void AABPawn::PossessedBy(AController* _NewController)
{
	ABLOG_S(Warning);
	Super::PossessedBy(_NewController);
}
#pragma endregion LoggingGamePlayFramework

// Called to bind functionality to input
void AABPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

