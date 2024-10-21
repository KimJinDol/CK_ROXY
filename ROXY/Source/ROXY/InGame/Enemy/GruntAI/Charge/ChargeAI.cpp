// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargeAI.h"
#include "InGame/Enemy/GruntAI/Charge/ChargeAnimInstance.h"
#include "InGame/Enemy/GruntAI/Charge/ChargeAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

AChargeAI::AChargeAI()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARGE(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mesh/SK_Cactus_New.SK_Cactus_New"));
	if (SK_CHARGE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARGE.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> CHARGE_ANIM(TEXT("/Game/Resources/Enemy/Grunt/Cactus/BP_Charge_Anim.BP_Charge_Anim_C"));
	if (CHARGE_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(CHARGE_ANIM.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_ORIGIN(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Materials/M_Charge_Base.M_Charge_Base"));
	if (MAT_ORIGIN.Succeeded())
	{
		OriginalMat = MAT_ORIGIN.Object;
		GetMesh ()->SetMaterial (0, OriginalMat);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_OPACITY(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Materials/M_Charge_Alpha_Inst.M_Charge_Alpha_Inst"));
	if (MAT_OPACITY.Succeeded())
	{
		OpacityMat = MAT_OPACITY.Object;
	}

	// AI
	AIControllerClass = AChargeAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 부비적
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 150.0f;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetMesh()->SetRenderCustomDepth(true);
}

void AChargeAI::StartQuake()
{
	if (_bIsQuaking || !_bCanSpecialAttack) return;

	_bIsQuaking = true;

	_bIsSpecialAttack = true;

	//_bCanMoveToTarget = false;

	SetStiffState(true);

	ChargeAnim->PlayQuakeMontage();
}

void AChargeAI::Quake()
{
	if (!_bIsQuaking) return;

	LOG(Warning, "Quake");

	TArray <FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(_quakeRange),
		Params);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OverlapResult.GetActor());

			if (roxy)
			{
				if (!roxy->GetCharacterIsSwift())
				{
					LOG(Warning, "Quake Damage Check");

					// 기절 상태
					roxy->SetFaint(2.0f);

					// 밀쳐짐
					roxy->GetCharacterMovement()->Velocity = FVector::ZeroVector;

					FRotator rotation(roxy->GetActorRotation());
					rotation.Yaw = UKismetMathLibrary::FindLookAtRotation(roxy->GetActorLocation(), GetActorLocation()).Yaw;
					roxy->SetActorRotation(rotation);

					FVector pushVec = roxy->GetActorForwardVector() * -12000.f;
					roxy->GetCharacterMovement()->Velocity = pushVec;

					// 데미지
					FDamageEvent damageEvent;
					roxy->TakeDamage(_damage_SpecialAttack, damageEvent, GetController(), this);
				}
			}
		}
	}
}

void AChargeAI::EndQuake()
{
	if (!_bIsQuaking) return;

	_bIsQuaking = false;

	_bIsSpecialAttack = false;

	_bHitCheck = false;

	//_bCanMoveToTarget = true;
	SetStiffState(false);

	OnQuakeTaskEnd.Broadcast();

	SpecialAttackCoolApply();
}

void AChargeAI::Die()
{
	Super::Die();

}

void AChargeAI::BeginPlay()
{
	Super::BeginPlay();

	// SetTarget(InGameManager->FindActor(AGenerator::StaticClass()));
}

void AChargeAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (DataTable)
	{
		FGruntStatTableRow* statTableRow = DataTable->FindRow<FGruntStatTableRow> (TEXT ("Charge"), FString ("Charge Row is not exist!"));

		if (statTableRow)
		{
			HpComponent->SetMaxHp (statTableRow->maxHp);
			HpComponent->SetHP (HpComponent->GetMaxHp ());
			_moveSpeed = statTableRow->move_speed;
			_damage_Attack = statTableRow->attack_damage;
			_stiffenTime = statTableRow->sttifen_time;
			_damage_SpecialAttack = statTableRow->specialAttack_damage;
			_coolTime_SpecialAttack = statTableRow->specialAttack_coolTime;
			LOG (Warning, "--------------ChargeAI Stat--------------");
			LOG (Warning, "Max Hp : %f", HpComponent->GetMaxHp ());
			LOG (Warning, "Move Speed : %f", _moveSpeed);
			LOG (Warning, "Attack Damage : %f", _damage_Attack);
			LOG(Warning, "Attack Stiffen Time : %f", _stiffenTime);
			LOG (Warning, "Special Attack Damage : %f", _damage_SpecialAttack);
			LOG (Warning, "Special Attack CoolTime : %f", _coolTime_SpecialAttack);
			LOG (Warning, "------------------------------------")
		}
	}

	ChargeAnim = Cast<UChargeAnimInstance>(GetMesh()->GetAnimInstance());
	CHECK(nullptr != ChargeAnim);

	ChargeAnim->OnQuake.AddUObject(this, &AChargeAI::Quake);
	ChargeAnim->OnQuakeEnd.AddUObject(this, &AChargeAI::EndQuake);

	GetCharacterMovement()->MaxWalkSpeed = _moveSpeed;
}

void AChargeAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 남은 체력이 20퍼센트 이하일 시 특수패턴 실행 불가
	_currentHP = HpComponent->GetHpRatio();

	if (_currentHP < 20)
	{
		_bCanQuake = false;
	}

	if (_bIsQuaking)
	{
		_quakeRange += 10.0f;
	}
}