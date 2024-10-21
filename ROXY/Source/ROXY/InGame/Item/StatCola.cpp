

#include "StatCola.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/VendingMachine.h"
#include "GameFramework/ProjectileMovementComponent.h"

AStatCola::AStatCola()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CapsuleComponent"));
	SphereComponent->SetSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName("IgnoreOnlyPawn");
	RootComponent = SphereComponent;

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileComponent->SetUpdatedComponent(SphereComponent);
	ProjectileComponent->InitialSpeed = 1.0f;
	ProjectileComponent->MaxSpeed = 100.0f;
	ProjectileComponent->bShouldBounce = false;
	ProjectileComponent->ProjectileGravityScale = 1.0f;

	ColaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ColaMesh->SetCollisionProfileName("NoCollision");
	ColaMesh->SetupAttachment(RootComponent);
	ColaMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	ColaMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_COLA(TEXT("/Game/Resources/Background/prop/cans/can_1/drinkCan001_low.drinkCan001_low"));
	if (SM_COLA.Succeeded())
	{
		ColaMesh->SetStaticMesh(SM_COLA.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_COLA(TEXT("/Game/Resources/Background/prop/cans/can_1/Met_can_1.Met_can_1"));
	if (MAT_COLA.Succeeded())
	{
		ColaMaterial = MAT_COLA.Object;
		ColaMesh->SetMaterial(0, ColaMaterial);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_HPCOLA(TEXT("/Game/Resources/Background/prop/cans/can_1/Met_can_1_05.Met_can_1_05"));
	if (MAT_HPCOLA.Succeeded())
	{
		HPMaterial = MAT_HPCOLA.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_DAMAGECOLA(TEXT("/Game/Resources/Background/prop/cans/can_1/Met_can_1_03.Met_can_1_03"));
	if (MAT_DAMAGECOLA.Succeeded())
	{
		DamageMaterial = MAT_DAMAGECOLA.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_COOLTIMECOLA(TEXT("/Game/Resources/Background/prop/cans/can_1/Met_can_1_04.Met_can_1_04"));
	if (MAT_COOLTIMECOLA.Succeeded())
	{
		CoolTimeMaterial = MAT_COOLTIMECOLA.Object;
	}

	/* 파티클 */
	PC_ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	PC_ParticleComponent->SetupAttachment(ColaMesh);

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

	/* 사운드 */
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_COLA_DROP(TEXT("/Game/Resources/Sound/Item/SC_Cola_Drop.SC_Cola_Drop"));
	if (SC_COLA_DROP.Succeeded())
	{
		SC_ColaDrop = SC_COLA_DROP.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_COLA_GET(TEXT("/Game/Resources/Sound/Item/SC_Cola_Get.SC_Cola_Get"));
	if (SC_COLA_GET.Succeeded())
	{
		SC_ColaGet = SC_COLA_GET.Object;
	}
}

void AStatCola::BeginPlay()
{
	Super::BeginPlay();

	BindNunuGameInstance(NunuGameInstance);

	UGameplayStatics::PlaySoundAtLocation(this, SC_ColaDrop, GetActorLocation());
	
	// 드랍 후 콜리전이 몇초 후 활성화 되도록
	GetWorldTimerManager().SetTimer(_activeColaTimer, this, &AStatCola::ActiveColaCollision, 1.0f, false);
}

void AStatCola::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AStatCola::OnColaCollisionBeginOverlap);
}

void AStatCola::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStatCola::SetOwnerVendingMachine(AVendingMachine* pointer)
{
	OwnerVendingMachine = pointer;
}

void AStatCola::SetColaType(EColaType newColaType)
{
	_colaType = newColaType;

	switch (newColaType)
	{
	case EColaType::CT_TENTEN:
		ColaMesh->SetMaterial(0, HPMaterial);
		break;
	case EColaType::CT_CHUNGKANG:
		ColaMesh->SetMaterial(0, DamageMaterial);
		break;
	case EColaType::CT_VITA:
		ColaMesh->SetMaterial(0, CoolTimeMaterial);
		break;
	default:
		break;
	}
}

void AStatCola::SetDropDirection(const FVector& direction)
{
	ProjectileComponent->Velocity = direction * ProjectileComponent->InitialSpeed;
}

void AStatCola::ActiveColaCollision()
{
	SphereComponent->SetCollisionProfileName("RespondOnlyRoxy");

	ProjectileComponent->ProjectileGravityScale = 0.0f;
	ProjectileComponent->bShouldBounce = false;
	ProjectileComponent->Velocity = FVector::ZeroVector;
}

void AStatCola::DestroyCola()
{
	UGameplayStatics::PlaySoundAtLocation(this, SC_ColaGet, GetActorLocation());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_Hororok, GetTransform(), true);

	Destroy();
}

void AStatCola::OnColaCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this)
	{
		ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OtherActor);

		if (roxy && NunuGameInstance)
		{
			// 매 페이즈가 끝나고 콜라 획득하면 풀피 채워주자
			roxy->SetFullHP();

			switch(_colaType) {
			case EColaType::CT_TENTEN:
			{
				NunuGameInstance->SetAddedMaxHp(15000.f);
				LOG(Warning, "Roxy Hp + 15000");
				break;
			}
			case EColaType::CT_CHUNGKANG:
			{
				NunuGameInstance->SetAddedDamage(700.f);
				LOG(Warning, "Roxy Damage + 700");
				break;
			}
			case EColaType::CT_VITA:
			{
				NunuGameInstance->SetDecreasedCoolTimePercent(0.15f);
				LOG(Warning, "Roxy Cool - 0.15");
				break;
			}
			}
			// 콜라를...먹고 스탯을 어케 얻을까?

			DestroyCola();
		}
	}
}

