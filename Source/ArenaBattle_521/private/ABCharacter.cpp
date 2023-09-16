// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "ABAnimInstance.h"

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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/Book/SkeletalMesh/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	SetControlMode(EControlMode::DIABLO);
	ArmLengthSpeed = 3.f;
	ArmRotationSpeed = 10.f;

	IsAttacking = false;
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != AnimInstance);

	AnimInstance->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);
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
		//SpringArm->TargetArmLength = 450.f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		ArmLengthTo = 450.f;
		SpringArm->bUsePawnControlRotation = true; // WASD�� ������ ���� Pawn�� ���� ������ ȸ���ϴ��� ����.
		SpringArm->bInheritPitch = true; // �̰��� false�� �����ϸ� Pawn�� �߽����� �����ϴ� SpringArm�� Pitch ������ ����.
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true; // ī�޶� �ٸ� ��ü�� �浹�Ǵ��� ���θ� ����.

		/*
		 * bUseControllerRotationYaw �ɼǿ��� ������, Pawn�� �߽����� SpringArm�� ȸ���� ��
		 * Pawn ��ü�� ȸ���ϴ��� �����̴�. ������ ����, ������ �����ϸ� ������ �����ϱ� ���� ���̴�.
		 * ���� GTA ���̽� ���� ���� ī�޶� ������ ������ Character�� �������� �ʴ´�.
		 * �̴� bUseControllerRotationYaw�� ���� ������ ���鼭 �� ������ Ȯ���� �� �� �ִ�.
		 * ���콺 �Է� ���� ���� ��������� ������ ������ ��ġ��ų�� ���θ� �����ϴ� ���� �� �������̴�.
		 */
		bUseControllerRotationYaw = false;
		// bUseControllerRotationYaw�� �޸� ���⼱ WASD�� ������ ���� Pawn�� ��������� ȸ���� ������.
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
		break;
	case AABCharacter::EControlMode::DIABLO:
		//SpringArm->TargetArmLength = 800.f;
		//SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
		ArmLengthTo = 800.f;
		ArmRotationTo = FRotator(-45.f, 0.f, 0.f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
		break;
	}
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::DIABLO:
		// å������ SpringArm->RelativeRotation��� ����� ����������, �� UE5 ���� �̷��� ����...
		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
		break;
	}

	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::DIABLO:
		/*
		* SizeSquared()��, DirectionToMove�� ���� ���� ������������ �� ��Ҹ� �����Ͽ� ���� ����
		* �װ��� �������� ���ϴ� ���̴�.
		* ������ ���ϸ�, ������ ũ�⸦ ���� �� ����.
		* FRotationMatrix::MakeFromX(DirectionToMove).Rotator()�� ���ؼ��� WASD ������ Pawn�� ȸ�� ���� �����س���.
		*/
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

	PlayerInputComponent->BindAction(TEXT("ViewChange"), IE_Pressed, this, &AABCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AABCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AABCharacter::Attack);

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
* ���� AddController~Input�� ���콺�Է� ��ȣ���� PlayerController�� ��Ʈ�� ȸ�� ������ �����ϴ� �Լ��̴�.
* ���� �� ���콺�� Ż����� ABPlayerController�� ������ ����, �̰��� Transform ����
* displayall PlayerController ControlRotation���� ������ ���� ��ġ�ϴ� ���� Ȯ���� �� �ִ�.
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

void AABCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->GetRelativeRotation());
		SetControlMode(EControlMode::GTA);
		break;
	}
}

void AABCharacter::Attack()
{
	//ABLOG_S(Warning);

	// ���� ���� ������ �����߿� �ִٸ� Attack() �Լ��� ������ �ٵ� �������� �ʰ� �����带 ����������.
	if (IsAttacking) return;

	// �� Ŭ������ �޽ÿ� �Ҵ�� �ִ� AnimInstance ��ü�� �����´�.
	const auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	// �Ҵ�� AnimInstance�� ���� ��쿡�� ����������.
	if (nullptr == AnimInstance) return;

	AnimInstance->PlayAttackMontage();
	ABLOG(Warning, TEXT(" AnimInstance->PlayAttackMontage() : %b"), AnimInstance->IsAnyMontagePlaying());

	IsAttacking = true;
}

void AABCharacter::OnAttackMontageEnded(UAnimMontage* _Montage, bool _bInterrupted)
{
	ABCHECK(IsAttacking);
	IsAttacking = false;
}
