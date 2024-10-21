// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetSphereRadius(80.f);
	SphereComponent->SetCollisionProfileName("RespondOnlyRoxy");
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = SphereComponent;

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	ItemMesh->SetRelativeLocationAndRotation(FVector(30.0f, 0.0f, -80.0f), FRotator(30.0f, 0.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ITEM(TEXT("/Game/Resources/Item/Mesh/SK_Item_02.SK_Item_02"));
	if (SK_ITEM.Succeeded())
	{
		ItemMesh->SetSkeletalMesh(SK_ITEM.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ITEM_ANIM(TEXT("/Game/Resources/Item/BP_Item_02_Anim.BP_Item_02_Anim_C"));
	if (ITEM_ANIM.Succeeded())
	{
		ItemMesh->SetAnimInstanceClass(ITEM_ANIM.Class);
	}

	/* 파티클 */
	PC_ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	PC_ParticleComponent->SetupAttachment(ItemMesh);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_DROP(TEXT("/Game/Resources/Roxy/Particle/Fx_Item_Drop/Fx_P_ItemDrop.Fx_P_ItemDrop"));
	if (PS_DROP.Succeeded())
	{
		PS_Drop = PS_DROP.Object;
		PC_ParticleComponent->SetTemplate(PS_Drop);
		PC_ParticleComponent->bAutoActivate = true;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_HOROROK(TEXT("/Game/Resources/Roxy/Particle/Fx_Item_Hororock/Fx_P_Hororock_Particle.Fx_P_Hororock_Particle"));
	if (PS_HOROROK.Succeeded())
	{
		PS_Hororok = PS_HOROROK.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_ITEM_GET(TEXT("/Game/Resources/Sound/Item/SC_Item_Get.SC_Item_Get"));
	if (SC_ITEM_GET.Succeeded())
	{
		SC_ItemGet = SC_ITEM_GET.Object;
	}

	// InitialLifeSpan = 10.f;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	// 생성 후 조금있다가 활성화 되도록. 죽일때 몬스터랑 너무 가까우면 보이기도 전에 처먹음
	GetWorldTimerManager().SetTimer(_activeItemTimer, this, &AItem::ActiveItemCollision, 1.3f, false);
}

void AItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnItemCollisionBeginOverlap);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::ActiveItemCollision()
{
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AItem::DestroyItem()
{
	// 아이템을 획득하는 효과 넣고 액터 삭제

	UGameplayStatics::PlaySoundAtLocation(this, SC_ItemGet, GetActorLocation());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_Hororok, GetTransform(), true);

	Destroy();
}

void AItem::OnItemCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 부모해서 행할 행동 (사운드 or 이펙트?)

}
