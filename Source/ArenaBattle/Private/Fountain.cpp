// Fill out your copyright notice in the Description page of Project Settings.


#include "Fountain.h"

// Sets default values
AFountain::AFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/*C++���� �׳� "BODY"��� �ϸ� �� �����ڴ� 1BYTE�� ó���ȴ�.
	* TEXT("BODY")�� �ϸ� �� �����ڴ� 2BYTE�� ó���ȴ�.
	* �𸮾� �����ӿ�ũ�� ��� ���ڿ��� 2BYTE�� ó���ϵ��� �����ߴ�.
	* �׷��Ƿ� ���ڿ��� ������ �� TEXT("BODY")�� �����Ѵ�.
	*/

	// ������Ʈ�� ��ü ����
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
	Splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"));
	Movement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("MOVEMENT"));

	// ��Ʈ������Ʈ�� Body�� �������ְ� 
	// Water, Light, Splash�� Body�� �ڽ����� �����Ѵ�.
	RootComponent = Body;
	Water->SetupAttachment(Body);
	Light->SetupAttachment(Body);
	Splash->SetupAttachment(Body);

	// Water�� z���� ��ġ�� 135, Light, Splash�� 195�� �����ش�
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 135.0f));
	Light->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	Splash->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));

	// ������Ʈ�� ���� �ּ��� ������ �� �� �Ʒ��� ���� ��θ� ����Ѵ�.
	// �ּ¸� ã�Ƽ� Ctrl + C�� �ϰų� ���콺 ������ư > ���۷��� ����
	/*
	* // 1��° ���
	StaticMesh'/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'
	* // 2��° ���
	/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01
	*/

	// SM_BODY�� �ּ��� �ε��Ѵ�.
	// static�� �ϴ� ������ Fountain��ü�� ���� �� ������ ������ �Ʒ� �ּ��� �ε��ϰ� �Ǹ�
	// �ƹ����� ���ϰ� �ɸ���. �׷��Ƿ� ���ʿ� 1�� �ε��� �� static���� �����ϸ� ���ķδ�
	// static���� �̹� �ε��� ��ü�� ��� �����ϹǷ� ���ϰ� ��������.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BODY(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'"));
	if (SM_BODY.Succeeded())
	{
		Body->SetStaticMesh(SM_BODY.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WATER(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Effects/FX_Meshes/Env/SM_Plains_Fountain_02.SM_Plains_Fountain_02'"));
	if (SM_WATER.Succeeded())
	{
		Water->SetStaticMesh(SM_WATER.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_SPLASH(TEXT("ParticleSystem'/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_01.P_Water_Fountain_Splash_01'"));
	if (PS_SPLASH.Succeeded())
	{
		Splash->SetTemplate(PS_SPLASH.Object);
	}

	RotateSpeed = 30.0f;

	Movement->RotationRate = FRotator(0.0f, RotateSpeed, 0.0f);
}

// Called when the game starts or when spawned
void AFountain::BeginPlay()
{
	Super::BeginPlay();
	
	/*
	UE_LOG(ArenaBattle, Warning, TEXT("Actor Name : %s, ID : %d, Location X : %.3f"),
			*GetName(), ID, GetActorLocation().X);
	*/
	// ArenaBattle ī�װ�, Warning����, �Լ��̸�, ��� ���� ��ġ
	ABLOG_S(Warning);

	// ArenaBattle ī�װ�, Warning����, �Լ��̸�, ��� ���� ��ġ + ���˿� ������ �߰� ����
	ABLOG(Warning, TEXT("Actor Name : %s, ID : %d, Location X : %.3f"),
		*GetName(), ID, GetActorLocation().X);
}

void AFountain::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ABLOG_S(Warning);
}

void AFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABLOG_S(Warning);
}

// Called every frame
void AFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Pitch, Yaw, Roll
	AddActorLocalRotation(FRotator(0.0f, RotateSpeed * DeltaTime, 0.0f));
}

