// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Component/HpComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"


// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_DAMAGED(TEXT("/Game/Resources/Enemy/Materials/Enemy_hit_Mat.Enemy_hit_Mat"));
	if (MAT_DAMAGED.Succeeded())
	{
		DamagedMat = MAT_DAMAGED.Object;
	}

	RootComponent = GetCapsuleComponent();

	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	GetCapsuleComponent()->SetCollisionProfileName("Enemy");
	GetCapsuleComponent()->SetCapsuleHalfHeight(100.f);
	GetCapsuleComponent()->SetCapsuleRadius(34.f);
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// 락온 UI
	LockOnWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LOCKONWIDGET"));
	LockOnWidget->SetupAttachment(GetMesh());
	LockOnWidget->SetVisibility(false);
	LockOnWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));
	LockOnWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> LOCKON_HUD(TEXT("/Game/Blueprints/InGame/Enemy/BP_LockOnWidget.BP_LockOnWidget_C"));
	if (LOCKON_HUD.Succeeded())
	{
		LockOnWidget->SetWidgetClass(LOCKON_HUD.Class);
		LockOnWidget->SetDrawSize(FVector2D(100.0f, 100.0f));
	}


	// 파티클 시스템 ///////////////////////////////////////////////////
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	Effect->SetupAttachment(GetMesh());
	Effect->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_HIT_01(TEXT("/Game/Resources/Enemy/Particle/Eff_Hit+Claw/Hit/Eff_P_Hit/Eff_P_Hit00.Eff_P_Hit00"));
	if (PS_HIT_01.Succeeded())
	{
		PS_Hit_01 = PS_HIT_01.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_HIT_02(TEXT("/Game/Resources/Enemy/Particle/Eff_Hit+Claw/Hit/Eff_P_Hit/Eff_P_Hit0200.Eff_P_Hit0200"));
	if(PS_HIT_02.Succeeded())
	{
		PS_Hit_02 = PS_HIT_02.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_HIT_03(TEXT("/Game/Resources/Enemy/Particle/Eff_Hit+Claw/Hit/Eff_P_Hit/Eff_P_Hit0300.Eff_P_Hit0300"));
	if(PS_HIT_03.Succeeded())
	{
		PS_Hit_03 = PS_HIT_03.Object;
	}
	///////////////////////////////////////////////////////////////////////

	HpComponent = CreateDefaultSubobject<UHpComponent>(TEXT("HpComponent"));

	GetCharacterMovement()->GravityScale = 2.5f;
}

void AEnemyAI::Possessed(AController* NewController)
{

}

void AEnemyAI::UnPossessed()
{
	Super::UnPossessed();

}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();

	BindNunuInGameManager(InGameManager);
	CHECK(InGameManager);

	GetCharacterMovement()->Velocity = FVector::ZeroVector;
}

void AEnemyAI::Die()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	OnDie.Broadcast();
	OnDie_OneParam.Broadcast(this);
	OnDie_Dyna_OneParam.Broadcast(this);
	_bEnemyIsDie =  true;
	// Destroy();
	// 죽는 애니 재생 후 몇 초 뒤 삭제시킨다.
	// 모든 상호 작용 중지.. 콜리전 삭제 후 메시 콜리전 레그돌로 만들기
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetCharacterMovement()->CurrentFloor.bWalkableFloor) {
		_bIsInAir = true;
	}
	else
		_bIsInAir = false;

}


void AEnemyAI::SetDamagedMat()
{
	CHECK(DamagedMat);
	_currentMatType = ECurrentMatType::MT_DAMAGED;
	int32 materialsNum = GetMesh()->GetNumMaterials();
	for (int32 i = 0; i < materialsNum; i++) {
		GetMesh()->SetMaterial(i, DamagedMat);
	}
	GetWorldTimerManager().SetTimer(DamagedMatOffTimer, this, &AEnemyAI::SetOriginMat, 0.15f, false);
}

void AEnemyAI::SetOriginMat()
{
	_currentMatType = ECurrentMatType::MT_ORIGIN;
	int32 materialsNum = GetMesh()->GetNumMaterials();
	if (WeaponMat)
	{
		GetMesh()->SetMaterial(0, OriginalMat);
		GetMesh()->SetMaterial(1, WeaponMat);
	}
	else
	{
		for (int32 i = 0; i < materialsNum; i++) {
			GetMesh()->SetMaterial(i, OriginalMat);
		}
	}
	GetWorldTimerManager().ClearTimer(DamagedMatOffTimer);
}


void AEnemyAI::SetOpacityMat()
{
	CHECK(OpacityMat);
	_currentMatType = ECurrentMatType::MT_OPACITY;
	int32 materialsNum = GetMesh()->GetNumMaterials();
	for (int32 i = 0; i < materialsNum; i++) {
		GetMesh()->SetMaterial(i, OpacityMat);
	}
}

void AEnemyAI::SetLockOnWidgetVisibility(bool value)
{
	_bIsOnTargeted = value;

	if (_bIsOnTargeted)
	{
		LockOnWidget->SetVisibility(true);
	}
	else
	{
		LockOnWidget->SetVisibility(false);
	}
}

bool AEnemyAI::GetIsLockOnModeOn()
{
	return _bIsOnTargeted;
}

void AEnemyAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//HpComponent->OnDie.AddUObject(this, &AEnemyAI::Die);
}

float AEnemyAI::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	/*히트 이펙트 출력------------------------------------------*/
	//Effect->bAutoActivate = true;
	//Effect->SetTemplate(PS_Hit_03);

	return HpComponent->TakeDamage(DamageAmount);
}