// Fill out your copyright notice in the Description page of Project Settings.


#include "Fountain.h"

// Sets default values
AFountain::AFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/*C++에서 그냥 "BODY"라고 하면 각 영문자는 1BYTE로 처리된다.
	* TEXT("BODY")로 하면 각 영문자는 2BYTE로 처리된다.
	* 언리얼 프레임워크는 모든 문자열을 2BYTE로 처리하도록 구성했다.
	* 그러므로 문자열을 전달할 때 TEXT("BODY")로 전달한다.
	*/

	// 컴포넌트의 객체 생성
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
	Splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"));
	Movement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("MOVEMENT"));

	// 루트컴포넌트로 Body를 지정해주고 
	// Water, Light, Splash를 Body의 자식으로 부착한다.
	RootComponent = Body;
	Water->SetupAttachment(Body);
	Light->SetupAttachment(Body);
	Splash->SetupAttachment(Body);

	// Water의 z높이 위치를 135, Light, Splash는 195로 정해준다
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 135.0f));
	Light->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	Splash->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));

	// 컴포넌트에 직접 애셋을 지정해 줄 때 아래와 같은 경로를 사용한다.
	// 애셋를 찾아서 Ctrl + C를 하거나 마우스 우측버튼 > 레퍼런스 복사
	/*
	* // 1번째 방법
	StaticMesh'/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'
	* // 2번째 방법
	/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01
	*/

	// SM_BODY에 애셋을 로딩한다.
	// static를 하는 이유는 Fountain객체가 여러 개 생성될 때마다 아래 애셋을 로딩하게 되면
	// 아무래도 부하가 걸린다. 그러므로 최초에 1번 로딩할 때 static으로 생성하면 이후로는
	// static으로 이미 로딩된 객체를 계속 지정하므로 부하가 적어진다.
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
	// ArenaBattle 카테고리, Warning레벨, 함수이름, 출력 라인 위치
	ABLOG_S(Warning);

	// ArenaBattle 카테고리, Warning레벨, 함수이름, 출력 라인 위치 + 포맷에 설정한 추가 정보
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

