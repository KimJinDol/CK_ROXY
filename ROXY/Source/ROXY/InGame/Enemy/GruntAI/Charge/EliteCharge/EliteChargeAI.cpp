// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteChargeAI.h"
#include "InGame/Enemy/GruntAI/Charge/ChargeAnimInstance.h"
#include "InGame/Enemy/GruntAI/Charge/EliteCharge/EliteChargeAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Enemy/EnemyWidget.h"

AEliteChargeAI::AEliteChargeAI()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ELITE_CHARGE(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mesh/SK_Cactus_Elite_02.SK_Cactus_Elite_02"));
	if (SK_ELITE_CHARGE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_ELITE_CHARGE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_ORIGIN(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Materials/M_Elite_Charge_Base.M_Elite_Charge_Base"));
	if (MAT_ORIGIN.Succeeded())
	{
		OriginalMat = MAT_ORIGIN.Object;
		GetMesh()->SetMaterial(0, OriginalMat);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_OPACITY(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Materials/M_Elite_Charge_Base_Inst.M_Elite_Charge_Base_Inst"));
	if (MAT_OPACITY.Succeeded())
	{
		OpacityMat = MAT_OPACITY.Object;
	}

	AIControllerClass = AEliteChargeAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

/* -------------------- Provoke -------------------- */
void AEliteChargeAI::StartProvoke()
{
	if (!_bCanProvoke || _bIsProvoke || !_bCanEliteSpecialAttack) return;

	_bIsProvoke = true;

	_bIsSpecialAttack = true;

	_bCanMoveToTarget = false;

	OffAttackCollision();

	ChargeAnim->PlayProvokeMontage();
}

void AEliteChargeAI::Provoke()
{
	if (!_bIsProvoke) return;

	TArray <FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(_provokeRange),
		Params);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OverlapResult.GetActor());

			if (roxy && !roxy->GetCharacterIsSwift())
			{
				LOG(Warning, "Provoke Attack Check");

				_bProvokeCheck = true;

				roxy->GetCharacterMovement()->Velocity = FVector::ZeroVector;

				// 도발 상태
				roxy->SetFury();
				roxy->SetFuryAttacker(this);

				// 데미지
				FDamageEvent damageEvent;
				roxy->TakeDamage(_damage_EliteSpecialAttack, damageEvent, GetController(), this);
			}
		}
	}
}

void AEliteChargeAI::EndProvoke()
{
	if (!_bIsProvoke) return;

	_bIsProvoke = false;

	_bIsSpecialAttack = false;

	_bCanMoveToTarget = true;

	EliteSpecialAttackCoolApply();

	OnProvokeTaskEnd.Broadcast();
}

void AEliteChargeAI::Die()
{
	Super::Die();

	EliteWidget->RemoveFromViewport();
}

void AEliteChargeAI::BeginPlay()
{
	Super::BeginPlay();

	EliteWidget->AddToViewport();

	// 엘리트 아이콘 표시
	//FVector _iconLoc = GetActorLocation() + (GetActorUpVector() * 350.0f);
	//FVector _iconScale = FVector(0.4f, 0.4f, 0.4f);
	//
	//UGameplayStatics::SpawnEmitterAttached(PS_EliteIcon, GetCapsuleComponent(), FName("IconPoint"), _iconLoc,
	//	GetActorRotation(), _iconScale, EAttachLocation::KeepWorldPosition, false);
}

void AEliteChargeAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (DataTable)
	{
		FGruntStatTableRow* statTableRow = DataTable->FindRow<FGruntStatTableRow>(TEXT("EliteCharge"), FString("EliteCharge Row is not exist!"));

		if (statTableRow)
		{
			HpComponent->SetMaxHp(statTableRow->maxHp);
			HpComponent->SetHP(HpComponent->GetMaxHp());
			_moveSpeed = statTableRow->move_speed;
			_damage_Attack = statTableRow->attack_damage;
			_stiffenTime = statTableRow->sttifen_time;
			_damage_SpecialAttack = statTableRow->specialAttack_damage;
			_damage_EliteSpecialAttack = statTableRow->eliteSpecialAttack_damage;
			_coolTime_EliteSpecialAttack = statTableRow->eliteSpecialAttack_coolTime;
			LOG(Warning, "--------------ChargeAI Stat--------------");
			LOG(Warning, "Max Hp : %f", HpComponent->GetMaxHp());
			LOG(Warning, "Move Speed : %f", _moveSpeed);
			LOG(Warning, "Attack Damage : %f", _damage_Attack);
			LOG(Warning, "Attack Stiffen Time : %f", _stiffenTime);
			LOG(Warning, "Special Attack Damage : %f", _damage_SpecialAttack);
			LOG(Warning, "Elite Special Attack Damage : %f", _damage_EliteSpecialAttack);
			LOG(Warning, "Elite Special Attack CoolTime : %f", _coolTime_EliteSpecialAttack);
			LOG(Warning, "------------------------------------")
		}
	}

	ChargeAnim->OnProvoke.AddUObject(this, &AEliteChargeAI::Provoke);
	ChargeAnim->OnProvokeEnd.AddUObject(this, &AEliteChargeAI::EndProvoke);

	GetCharacterMovement()->MaxWalkSpeed = _moveSpeed;

	_bIsEliteGrunt = true;
}

void AEliteChargeAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_bIsProvoke)
	{
		_provokeRange += 10.0f;
	}
}

float AEliteChargeAI::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (_bProvokeCheck)
	{
		return Super::TakeDamage(DamageAmount / 2, DamageEvent, EventInstigator, DamageCauser);
	}
	else
	{
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
}
