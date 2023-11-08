// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "ABAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "ABWeapon.h"
#include "ABCharacterStatComponent.h"

// Sets default values
AABCharacter::AABCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	CharacterStat = CreateDefaultSubobject<UABCharacterStatComponent>(TEXT("CHARACTERSTAT"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(
		TEXT("/Game/Book/SkeletalMesh/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_CARDBOARD.Succeeded())
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ABCharacter")); // 왜 Character의 Collision 이름이 Custom으로 뜰까?
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
	* ABCharacter와 AnimInstance의 델리게이트 아키텍처 이해하기
	* 
	* DECLARE_MULTICAST_DELEGATE() 키워드를 통해 인수로 델리게이트를 등록.
	* 여기서는 FOnNextAttackCheckDelegate를 등록함.
	* 그리고 헤더에서 다음과 같이 변수 선언.
	* FOnNextAttackCheckDelegate OnNextAttackCheck;
	* 이 OnNextAttackCheck 변수의 멤버함수를 다음과 같이 호출하면 다른 클래스의 함수들, 즉 멀티캐스트 델리게이트로 등록된 함수들이 모두 호출되는 것임.
	* OnNextAttackCheck.BroadCast();
	* 
	* 여기서 호출 주체는 ABAnimInstance이고, 호출되는 함수들은 ABCharacter에 있다.
	* ABAnimInstance에서 DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate) 명령어로 델리게이트 통로를 파 뒀다.
	* 이 호출을 발동시키는 명령어는 OnNextAttackCheck.BroadCast()인데, 이는 애님 노티파이 시점에 호출되는 AnimNotify_AttackHitCheck() 함수 내에 있다.
	* 
	* 그럼 구체적으로 이 델리게이트를 통해 호출되는 함수로는 무엇들이 등록돼 있는가?
	* 여기선 아래의 람다식을 등록함. 
	* 원하면 다른 이름 있는 함수나 또 다른 람다식을 등록하여 멀티캐스트 명령을 받을 수 있음.
	* 
	* ABAnim 쪽에서 몽타주의 한 섹션이 끝남과 동시에 OnNextAttackCheck 델리게이트 인스턴스가 BroadCast()로 호출되어
	* 여기에 AddLambda를 통해 바인딩이 돼 있던 람다식이 선언되고 호출된다.
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

	/*
	FName WeaponSocket(TEXT("hand_rSocket"));
	auto CurWeapon = GetWorld()->SpawnActor<AABWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	*/
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
	case EControlMode::DIABLO:
		// 책에서는 SpringArm->RelativeRotation라는 멤버에 접근하지만, 현 UE5 기준 이런건 없다...
		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
		break;
	}

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		/*
		* SizeSquared()는, DirectionToMove와 같은 벡터 데이터형식의 각 요소를 제곱하여 더한 다음
		* 그것의 제곱근을 구하는 것이다.
		* 간단히 말하면, 벡터의 크기를 구할 때 쓴다.
		* FRotationMatrix::MakeFromX(DirectionToMove).Rotator()를 통해서는 WASD 값에서 Pawn의 회전 값을 도출해낸다.
		*/
		if (DirectionToMove.SizeSquared() > 0.f) // 움직임이 발생하는 경우 바디로 진입
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator()); // WASD가 가리키는 방향으로 몸을 튼다.
			AddMovementInput(DirectionToMove); // WASD가 가리키는 방향으로 움직인다.
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
	return (nullptr == CurrentWeapon);
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
* 다음 AddController~Input은 마우스입력 신호값을 PlayerController의 컨트롤 회전 값으로 변경하는 함수이다.
* 실행 중 마우스를 탈출시켜 ABPlayerController를 선택해 보면, 이것의 Transform 값이
* displayall PlayerController ControlRotation으로 나오는 값과 일치하는 것을 확인할 수 있다.
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

void AABCharacter::Attack() // 마우스 왼쪽 버튼에 바인딩 돼 있음.
{
	/*
	//ABLOG_S(Warning);

	// 만약 현재 공격이 실행중에 있다면 Attack() 함수의 나머지 바디를 실행하지 않고 스레드를 돌려보낸다.
	if (IsAttacking) return;

#pragma region 256
	// 이 클래스의 메시에 할당돼 있는 pAnimInstance 객체를 가져온다.
	const auto AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	// 할당된 AnimInstance가 없는 경우에도 돌려보낸다.
	if (nullptr == AnimInstance) return;

	ABAnim->PlayAttackMontage();
#pragma endregion 256
	
	// ABLOG(Warning, TEXT(" pAnimInstance->PlayAttackMontage() : %b"), AnimInstance->IsAnyMontagePlaying());

	IsAttacking = true;
	*/

	if (IsAttacking)
	{
		// IsWithinInclusive 키워드는 <>타입의 변수를 넣어서 첫번째 변수가 뒤의 두 입력 값 사이에 들어 있는지 확인하는 함수이다.
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));

		if (CanNextCombo)
			IsComboInputOn = true;
	}
	else // 마우스 왼쪽 버튼이 눌린 것이 연타의 처음을 시작하는 공격이라면~
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
