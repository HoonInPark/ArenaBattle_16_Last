// Fill out your copyright notice in the Description page of Project Settings.


#include "ABItemBox.h"
#include "ABWeapon.h"

// Sets default values
AABItemBox::AABItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.f, 42.f, 30.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/Book/StaticMesh/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (SM_BOX.Succeeded())
		Box->SetStaticMesh(SM_BOX.Object);

	Box->SetRelativeLocation(FVector(0.f, -3.5f, -30.f));

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	WeaponItemClass = AABWeapon::StaticClass();
}

void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnCharacterOverlap);
}

// Called when the game starts or when spawned
void AABItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AABItemBox::OnCharacterOverlap(UPrimitiveComponent* _OverlapComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	ABLOG_S(Warning);
}
