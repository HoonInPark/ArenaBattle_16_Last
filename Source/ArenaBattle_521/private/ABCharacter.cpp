// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "ABAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "ABWeapon.h"

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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(
		TEXT("/Game/Book/SkeletalMesh/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_CARDBOARD.Succeeded())
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter")); // �� Character�� Collision �̸��� Custom���� ���?
	ABLOG(Warning, TEXT(" CollisionProfileName : %s"), *GetCapsuleComponent()->GetCollisionProfileName().ToString());

	/*
	FName WeaponSocket(TEXT("hand_rSocket"));
	
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Silly_Weapons/Blade_Finger/SK_Bade_Finger.SK_Bade_Finger"));
		if (SK_WEAPON.Succeeded())
			Weapon->SetSkeletalMesh(SK_WEAPON.Object);

		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}
	*/

	SetControlMode(EControlMode::DIABLO);
	ArmLengthSpeed = 3.f;
	ArmRotationSpeed = 10.f;

	IsAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();

	AttackRange = 200.f;
	AttackRadius = 50.f;
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != ABAnim);
	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);

	/*
	* ABCharacter�� AnimInstance�� ��������Ʈ ��Ű��ó �����ϱ�
	* 
	* DECLARE_MULTICAST_DELEGATE() Ű���带 ���� �μ��� ��������Ʈ�� ���.
	* ���⼭�� FOnNextAttackCheckDelegate�� �����.
	* �׸��� ������� ������ ���� ���� ����.
	* FOnNextAttackCheckDelegate OnNextAttackCheck;
	* �� OnNextAttackCheck ������ ����Լ��� ������ ���� ȣ���ϸ� �ٸ� Ŭ������ �Լ���, �� ��Ƽĳ��Ʈ ��������Ʈ�� ��ϵ� �Լ����� ��� ȣ��Ǵ� ����.
	* OnNextAttackCheck.BroadCast();
	* 
	* ���⼭ ȣ�� ��ü�� ABAnimInstance�̰�, ȣ��Ǵ� �Լ����� ABCharacter�� �ִ�.
	* ABAnimInstance���� DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate) ��ɾ�� ��������Ʈ ��θ� �� �״�.
	* �� ȣ���� �ߵ���Ű�� ��ɾ�� OnNextAttackCheck.BroadCast()�ε�, �̴� �ִ� ��Ƽ���� ������ ȣ��Ǵ� AnimNotify_AttackHitCheck() �Լ� ���� �ִ�.
	* 
	* �׷� ��ü������ �� ��������Ʈ�� ���� ȣ��Ǵ� �Լ��δ� �������� ��ϵ� �ִ°�?
	* ���⼱ �Ʒ��� ���ٽ��� �����. 
	* ���ϸ� �ٸ� �̸� �ִ� �Լ��� �� �ٸ� ���ٽ��� ����Ͽ� ��Ƽĳ��Ʈ ����� ���� �� ����.
	* 
	* ABAnim �ʿ��� ��Ÿ���� �� ������ ������ ���ÿ� OnNextAttackCheck ��������Ʈ �ν��Ͻ��� BroadCast()�� ȣ��Ǿ�
	* ���⿡ AddLambda�� ���� ���ε��� �� �ִ� ���ٽ��� ����ǰ� ȣ��ȴ�.
	*/
	ABAnim->OnNextAttackCheck.AddLambda(
		// Lambda expression begins
		[this]() -> void
		{
			ABLOG(Warning, TEXT("OnNextAttackCheck"));
			CanNextCombo = false;

			if (IsComboInputOn)
			{
				AttackStartComboState();
				ABAnim->JumpToAttackMontageSection(CurrentCombo); 
			}
		}
		// Lambda expression ends
	);

	ABAnim->OnAttackHitCheck.AddUObject(this, &AABCharacter::AttackCheck);
}

float AABCharacter::TakeDamage(float _DamageAccount, FDamageEvent const& _DamageEvent, AController* _EventInstigator, AActor* _DamageCauser)
{
	float FinalDamage = Super::TakeDamage(_DamageAccount, _DamageEvent, _EventInstigator, _DamageCauser);
	ABLOG(Warning, TEXT(" Actor : %s took Damage : %f"), *GetName(), FinalDamage);
	
	if (FinalDamage > 0.f)
	{
		ABAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	}

	return FinalDamage;
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();

	FName WeaponSocket(TEXT("hand_rSocket"));
	auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
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
	case EControlMode::DIABLO:
		// å������ SpringArm->RelativeRotation��� ����� ����������, �� UE5 ���� �̷��� ����...
		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
		break;
	}

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		/*
		* SizeSquared()��, DirectionToMove�� ���� ���� ������������ �� ��Ҹ� �����Ͽ� ���� ����
		* �װ��� �������� ���ϴ� ���̴�.
		* ������ ���ϸ�, ������ ũ�⸦ ���� �� ����.
		* FRotationMatrix::MakeFromX(DirectionToMove).Rotator()�� ���ؼ��� WASD ������ Pawn�� ȸ�� ���� �����س���.
		*/
		if (DirectionToMove.SizeSquared() > 0.f) // �������� �߻��ϴ� ��� �ٵ�� ����
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator()); // WASD�� ����Ű�� �������� ���� ư��.
			AddMovementInput(DirectionToMove); // WASD�� ����Ű�� �������� �����δ�.
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

bool AABCharacter::CanSetWeapon()
{
	return (nullptr != CurrentWeapon);
}

void AABCharacter::SetWeapon(AABWeapon* _NewWeapon)
{
	ABCHECK(nullptr != _NewWeapon && nullptr == CurrentWeapon);
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != _NewWeapon) 
	{
		_NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		_NewWeapon->SetOwner(this);
		CurrentWeapon = _NewWeapon;
	}
	
}

void AABCharacter::UpDown(float _NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
            AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), _NewAxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.X = _NewAxisValue;
		break;
	}
}

void AABCharacter::LeftRight(float _NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), _NewAxisValue);
		break;
	case EControlMode::DIABLO:
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
	case EControlMode::GTA:
		AddControllerPitchInput(_NewAxisValue);
		break;
	}
}

void AABCharacter::Turn(float _NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
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

void AABCharacter::Attack() // ���콺 ���� ��ư�� ���ε� �� ����.
{
	/*
	//ABLOG_S(Warning);

	// ���� ���� ������ �����߿� �ִٸ� Attack() �Լ��� ������ �ٵ� �������� �ʰ� �����带 ����������.
	if (IsAttacking) return;

#pragma region 256
	// �� Ŭ������ �޽ÿ� �Ҵ�� �ִ� pAnimInstance ��ü�� �����´�.
	const auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	// �Ҵ�� AnimInstance�� ���� ��쿡�� ����������.
	if (nullptr == AnimInstance) return;

	ABAnim->PlayAttackMontage();
#pragma endregion 256
	
	// ABLOG(Warning, TEXT(" pAnimInstance->PlayAttackMontage() : %b"), AnimInstance->IsAnyMontagePlaying());

	IsAttacking = true;
	*/

	if (IsAttacking)
	{
		// IsWithinInclusive Ű����� <>Ÿ���� ������ �־ ù��° ������ ���� �� �Է� �� ���̿� ��� �ִ��� Ȯ���ϴ� �Լ��̴�.
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));

		if (CanNextCombo)
			IsComboInputOn = true;
	}
	else // ���콺 ���� ��ư�� ���� ���� ��Ÿ�� ó���� �����ϴ� �����̶��~
	{
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		ABAnim->PlayAttackMontage();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void AABCharacter::OnAttackMontageEnded(UAnimMontage* _Montage, bool _bInterrupted)
{
	ABCHECK(IsAttacking);
	ABCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
}

#pragma region SetComboState
void AABCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;

	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));

	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AABCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}
#pragma endregion SetComboState

void AABCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult, 
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.f),
		Params
	);

#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.f;

	DrawDebugCapsule(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);
#endif

	if (bResult) 
	{
		if (::IsValid(HitResult.GetActor()))
		{
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.GetActor()->GetName());

			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(50.0f, DamageEvent, GetController(), this);
		}
	}
}
