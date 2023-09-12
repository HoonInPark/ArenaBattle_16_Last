// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPawn.h"


// Sets default values
AABPawn::AABPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	Capsule->SetCapsuleHalfHeight(88.f);
	Capsule->SetCapsuleRadius(34.f);
	Mesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_CARDBOARD.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_CARDBOARD.Object);
	}
}

// Called when the game starts or when spawned
void AABPawn::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Script/Engine.AnimSequence'/Game/Book/Animations/WarriorRun.WarriorRun'"));
	if (AnimAsset != nullptr)
	{
		Mesh->PlayAnimation(AnimAsset, true);
	}
}

// Called every frame
void AABPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region LoggingGamePlayFramework
/*
* 다음과 같이 PossessedBy와 PostInitializeComponents를 호출하면 로그를 확인할 수 있다.
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

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABPawn::LeftRight);
}

void AABPawn::UpDown(float _NewAxisValue)
{
	//ABLOG(Warning, TEXT("%f"), _NewAxisValue);
	AddMovementInput(GetActorForwardVector(), _NewAxisValue);
}

void AABPawn::LeftRight(float _NewAxisValue)
{
	//ABLOG(Warning, TEXT("%f"), _NewAxisValue);
	AddMovementInput(GetActorRightVector(), _NewAxisValue);
}

