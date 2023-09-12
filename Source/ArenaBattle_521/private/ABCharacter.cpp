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

	SetControlMode(EControlMode::DIABLO);
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AABCharacter::SetControlMode(EControlMode _NewControlMode)
{
	CurrentControlMode = _NewControlMode;

	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::GTA:
		SpringArm->TargetArmLength = 450.f;
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
		break;
	case AABCharacter::EControlMode::DIABLO:
		SpringArm->TargetArmLength = 800.f;
		SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = true;
		break;
	}

}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::DIABLO:
		if (DirectionToMove.SizeSquared() > 0.f)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}
		break;
	}
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
	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), _NewAxisValue);
		break;
	case AABCharacter::EControlMode::DIABLO:
		DirectionToMove.X = _NewAxisValue;
		break;
	}
}

void AABCharacter::LeftRight(float _NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), _NewAxisValue);
		break;
	case AABCharacter::EControlMode::DIABLO:
		DirectionToMove.Y = _NewAxisValue;
		break;
	}
}

#pragma region PlayerControllerControlRotation
/*
* 다음 AddController~Input은 마우스입력 신호값을 PlayerController의 컨트롤 회전 값으로 변경하는 함수이다.
* 실행 중 마우스를 탈출시켜 ABPlayerController를 선택해 보면, 이것의 Transform 값이 
* displayall PlayerController ControlRotation으로 나오는 값과 일치하는 것을 확인할 수 있다.
*/
void AABCharacter::LookUp(float _NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::GTA:
		AddControllerPitchInput(_NewAxisValue);
		break;
	}
}

void AABCharacter::Turn(float _NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::GTA:
		AddControllerYawInput(_NewAxisValue);
		break;
	}
}
#pragma endregion PlayerControllerControlRotation

