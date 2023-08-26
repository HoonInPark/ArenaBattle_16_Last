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

	/*�츮�� ����� ��������Ʈ �̺�Ʈ�� C++�� Blueprint ��ο��� ����ϴ� 
	���̳��� ��������Ʈ�̹Ƿ�, ����ϴ� �Լ��� Blueprint���� ����� �� �ֵ���
	UFUNCTION()�� �ٿ���� �Ѵ�.
	�Ʒ� �Լ��� �Ű������� ����� ��������Ʈ�� ���İ� ��ġ�ؾ� �Ѵ�.
	*/
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();		// �޺��� ���۵� �� ���� ����
	void AttackEndComboState();			// �޺��� �� �̻� ������� �ʾ��� �� ���� ����
	void AttackCheck();

	//DefaultArenaBattle.ini�� �о �ּ��� ���� �� �ε��� �Ϸ�Ǹ�
	void OnAssetLoadCompleted();		

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	bool IsAttacking;		// ����������

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	bool CanNextCombo;		// ���� �޺��� ���� �� �� �ִ���

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	bool IsComboInputOn;	// �޺� �̺�Ʈ�� �����ߴ���

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	int32 CurrentCombo;		// ���� �޺��ܰ�

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess=true))
	int32 MaxCombo;			// �ִ� �޺�����

	/*C++�� ��� 2���� Ŭ������ ��ȣ������ ���
	���� �� #include �� �ϰ� �Ǹ� ��� ������ �������� ���� ���¿���
	���ΰ� ��������� ������ �� �� �����Ƿ� ������ ������ �߻��Ѵ�.
	�̷� �� ������ class A;ó�� �����ؾ� �ذ�ȴ�.
	�׸��� A�� ����Ű�� ������ ������ �� A b; �̷��� �ϸ� �ȵǰ�
	A* b; ó�� �ؾ� �Ѵ�.
	������ A b; �� ��ü��ü�� �޸𸮿� �Ҵ��ؾ� �ϴµ� �̰� ���� ���� �¹��� 
	��Ȳ�̶� ��ü ũ�⸦ ���� ���¿��� �����Ϸ��� ���� �� ����.
	������ ������ ��� ������ ����Ű����� 32bitȯ�濡���� 32bitũ�⸦
	64bitȯ�濡���� 64bitũ�⸦ ������ �ǹǷ�
	Win64�� ������ �� A* b;�� 64bit�� �����Ϸ��� �Ҵ��� �� �����Ƿ�
	��ȣ ������ �ذ��� �� �ִ�.

	��������
	*/
	UPROPERTY()
	class UABAnimInstance* ABAnim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	int32 AssetIndex = 0;

	// Enemy�� Asset�� �о �����ϱ� ���� ����
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
