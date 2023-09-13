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
	ArmLengthSpeed = 3.f;
	ArmRotationSpeed = 10.f;
}

#pragma region ForUE5
void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UAnimInstance* CurrentAnimInstance = GetMesh()->GetAnimInstance();

	if (!CurrentAnimInstance)
	{
		const UAnimInstance* pAnimInstance = NewObject<UABAnimInstance>(GetMesh(), UABAnimInstance::StaticClass());
		GetMesh()->SetAnimInstanceClass(pAnimInstance->GetClass());
		ABLOG(Warning, TEXT(" AnimInstance Allocate Succeeded : %s"), *GetMesh()->GetAnimInstance()->GetName());
	}
	else // 이 Character에 할당된 AnimInstance가 있을 때~
	{
		const UAnimInstance* pAnimInstance = Cast<UABAnimInstance>(CurrentAnimInstance);
		ABLOG(Warning, TEXT(" AnimInstance Already Allocated : %s"), *pAnimInstance->GetName());
	}
}
#pragma endregion ForUE5

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
		SpringArm->bUsePawnControlRotation = true; // WASD를 누름에 따라 Pawn의 정면 방향이 회전하는지 여부.
		SpringArm->bInheritPitch = true; // 이것을 false로 설정하면 Pawn을 중심으로 공전하는 SpringArm의 Pitch 방향이 잠긴다.
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true; // 카메라가 다른 물체와 충돌되는지 여부를 결정.

		/*
		 * bUseControllerRotationYaw 옵션에서 관건은, Pawn을 중심으로 SpringArm이 회전할 때
		 * Pawn 자체도 회전하는지 여부이다. 지구의 공전, 자전을 생각하면 개념을 이해하기 쉬울 것이다.
		 * 여기 GTA 케이스 같은 경우는 카메라가 공전을 하지만 Character는 자전하지 않는다.
		 * 이는 bUseControllerRotationYaw의 값을 변경해 보면서 그 이유를 확인해 볼 수 있다.
		 * 마우스 입력 값을 폰의 정면방향을 돌리는 행위와 일치시킬지 여부를 결정하는 것이 이 변수값이다.
		 */
		bUseControllerRotationYaw = false;
		// bUseControllerRotationYaw와 달리 여기선 WASD를 누름에 따라 Pawn의 정면방향을 회전할 것인지.
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
		// 책에서는 SpringArm->RelativeRotation라는 멤버에 접근하지만, 현 UE5 기준 이런건 없다...
		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
		break;
	}

	switch (CurrentControlMode)
	{
	case AABCharacter::EControlMode::DIABLO:
		/*
		* SizeSquared()는, DirectionToMove와 같은 벡터 데이터형식의 각 요소를 제곱하여 더한 다음 
		* 그것의 제곱근을 구하는 것이다. 
		* 간단히 말하면, 벡터의 크기를 구할 때 쓴다.
		* FRotationMatrix::MakeFromX(DirectionToMove).Rotator()를 통해서는 WASD 값에서 Pawn의 회전 값을 도출해낸다.
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
