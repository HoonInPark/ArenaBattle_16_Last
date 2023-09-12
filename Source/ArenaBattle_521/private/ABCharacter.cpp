// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"

// Sets default values
AABCharacter::AABCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);
}

void AABCharacter::UpDown(float _NewAxisValue)
{
	AddMovementInput(GetActorForwardVector(), _NewAxisValue);

}

void AABCharacter::LeftRight(float _NewAxisValue)
{
	AddMovementInput(GetActorRightVector(), _NewAxisValue);
}

/*
* ���� AddController~Input�� ���콺�Է� ��ȣ���� PlayerController�� ��Ʈ�� ȸ�� ������ �����ϴ� �Լ��̴�.
* ���� �� ���콺�� Ż����� ABPlayerController�� ������ ����, �̰��� Transform ���� 
* displayall PlayerController ControlRotation���� ������ ���� ��ġ�ϴ� ���� Ȯ���� �� �ִ�.
*/
void AABCharacter::LookUp(float _NewAxisValue)
{
	AddControllerPitchInput(_NewAxisValue);
}

void AABCharacter::Turn(float _NewAxisValue)
{
	AddControllerYawInput(_NewAxisValue);
}

