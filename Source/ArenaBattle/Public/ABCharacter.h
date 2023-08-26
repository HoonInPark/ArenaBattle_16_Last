// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Character.h"
#include "ABCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class ARENABATTLE_API AABCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacter();
	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;
	int32 GetExp() const;
	float GetFinalAttackRange() const;
	float GetFinalAttackDamage() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		GTA,
		DIABLO,
		NPC
	};

	void SetControlMode(EControlMode ControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;

	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool CanSetWeapon();
	void SetWeapon(class AABWeapon* NewWeapon);

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	class AABWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UABCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;


	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAcces = true))
		bool bIsPlayer;

	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	void ViewChange();

	/*우리가 등록할 델리게이트 이벤트가 C++과 Blueprint 모두에서 사용하는 
	다이내믹 델리게이트이므로, 등록하는 함수도 Blueprint에서 사용할 수 있도록
	UFUNCTION()을 붙여줘야 한다.
	아래 함수의 매개변수는 등록할 델리게이트의 형식과 일치해야 한다.
	*/
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();		// 콤보가 시작될 때 변수 설정
	void AttackEndComboState();			// 콤보가 더 이상 진행되지 않았을 때 변수 설정
	void AttackCheck();

	//DefaultArenaBattle.ini을 읽어서 애셋을 선택 후 로딩이 완료되면
	void OnAssetLoadCompleted();		

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	bool IsAttacking;		// 공격중인지

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	bool CanNextCombo;		// 다음 콤보를 실행 할 수 있는지

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	bool IsComboInputOn;	// 콤보 이벤트를 수신했는지

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	int32 CurrentCombo;		// 현재 콤보단계

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	int32 MaxCombo;			// 최대 콤보갯수

	/*C++의 경우 2개의 클래스가 상호참조할 경우
	양쪽 다 #include 를 하게 되면 어느 한쪽이 정해지지 않은 상태에서
	서로가 헤더파일을 복사해 올 수 없으므로 컴파일 에러가 발생한다.
	이럴 때 한쪽은 class A;처럼 선언해야 해결된다.
	그리고 A를 가리키는 변수를 선언할 때 A b; 이렇게 하면 안되고
	A* b; 처럼 해야 한다.
	이유는 A b; 는 객체자체를 메모리에 할당해야 하는데 이것 역시 서로 맞물린 
	상황이라 객체 크기를 현재 상태에서 컴파일러가 정할 수 없다.
	포인터 변수는 어는 변수를 가리키더라고 32bit환경에서는 32bit크기를
	64bit환경에서는 64bit크기를 가지게 되므로
	Win64로 빌드할 때 A* b;는 64bit로 컴파일러는 할당할 수 있으므로
	상호 참조를 해결할 수 있다.

	전방참조
	*/
	UPROPERTY()
	class UABAnimInstance* ABAnim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	int32 AssetIndex = 0;

	// Enemy에 Asset을 읽어서 적용하기 위한 변수
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;


	UPROPERTY()
	class AABAIController* ABAIController;

	UPROPERTY()
	class AABPlayerController* ABPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer;

	FTimerHandle DeadTimerHandle = {};
};
