// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffAI.h"
#include "InGame/Enemy/GruntAI/Buff/BuffAIController.h"
#include "InGame/Enemy/GruntAI/Buff/BuffAnimInstance.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "InGame/Enemy/EnemyWidget.h"

ABuffAI::ABuffAI()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_BUFF(TEXT("/Game/Resources/Enemy/Grunt/Wizard/Mesh/SK_WizardSuper.SK_WizardSuper"));
	if (SK_BUFF.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BUFF.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> BUFF_ANIM(TEXT("/Game/Resources/Enemy/Grunt/Wizard/BP_Buff_Anim.BP_Buff_Anim_C"));
	if (BUFF_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BUFF_ANIM.Class);
	}

	// 캐릭터 머테리얼
	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_ORIGIN(TEXT("/Game/Resources/Enemy/Grunt/Wizard/Materials/M_WizardSuper_D.M_WizardSuper_D"));
	if (MAT_ORIGIN.Succeeded())
	{
		OriginalMat = MAT_ORIGIN.Object;
		GetMesh()->SetMaterial(0, OriginalMat);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_OPACITY(TEXT("/Game/Resources/Enemy/Grunt/Wizard/Materials/M_WizardSuper_D_Inst.M_WizardSuper_D_Inst"));
	if (MAT_OPACITY.Succeeded())
	{
		OpacityMat = MAT_OPACITY.Object;
	}

	// 지팡이 머테리얼
	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_STAFF_ORIGIN(TEXT("/Game/Resources/Enemy/Grunt/Wizard/Materials/M_WizardSuper_Staff_D.M_WizardSuper_Staff_D"));
	if (MAT_STAFF_ORIGIN.Succeeded())
	{
		WeaponMat = MAT_STAFF_ORIGIN.Object;
		GetMesh()->SetMaterial(1, WeaponMat);
	}

	//static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_STAFF_OPACITY(TEXT("/Game/Resources/Enemy/Grunt/Wizard/Materials/M_WizardSuper_Staff_D_Inst.M_WizardSuper_Staff_D_Inst"));
	//if (MAT_STAFF_OPACITY.Succeeded())
	//{
	//	StaffOpacityMat = MAT_STAFF_OPACITY.Object;
	//}

	PC_ParticleComponent_02 = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent_02");
	PC_ParticleComponent_02->SetupAttachment(RootComponent);
	PC_ParticleComponent_02->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PC_ParticleComponent_02->SetVisibility(false);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_WIZARDSHIELD(TEXT("/Game/Resources/Enemy/Particle/Fx_Sheild/Fx_P_Enemy_Sheild.Fx_P_Enemy_Sheild"));
	if (PS_WIZARDSHIELD.Succeeded())
	{
		PS_WizardShield = PS_WIZARDSHIELD.Object;
		PC_ParticleComponent_02->SetTemplate(PS_WizardShield);
	}

	// AI
	AIControllerClass = ABuffAIController::StaticClass();
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

void ABuffAI::StartInvincibility()
{
	if (_bIsInvincible) return;

	_bIsInvincible = true;

	// 무적 패턴 캔슬..당하지 않게 패턴 쓰는동안 무적 만들어줌
	_bIsInvincibleState = true;

	SetMoveToTarget(false);

	BuffAnim->PlayInvinciMontage();
}

void ABuffAI::MakeInvincible()
{
	//if (!_bIsInvincible) return;

	// 자신을 제외한 레벨에 존재하는 모든 몬스터에게 무적버프

	//LOG(Warning, "Make Invincible");

	//AGruntAI* grunt = Cast<AGruntAI>(StaticClass());
	//
	//if (grunt && grunt != this && !grunt->GetIsInvinci())
	//{
	//	grunt->SetInvinciState(true);
	//}

	PC_ParticleComponent_02->SetVisibility(true);

	TArray <FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(10000.0f),
		Params);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			//LOG(Warning, "Overlap Grunt Num : %d", OverlapResults.Num());

			// 무적 패턴을 실행하기 위해 레벨에 최소 _minGruntNum마리 이상의 잡몹이 존재해야 함
			if (OverlapResults.Num() - 1 > _minGruntNum)
			{
				AGruntAI* grunt = Cast<AGruntAI>(OverlapResult.GetActor());

				if (grunt && grunt != this && !grunt->GetIsInvinci())
				{
					grunt->SetInvinciState(true);
				}
			}
			else
			{
				LOG(Warning, "Few Grunt");
			}
		}
	}
}

void ABuffAI::EndInvincibility()
{
	if (!_bIsInvincible) return;

	if(!_bIsFirstInvincible)
	{
		OnFirstInvincibility.Broadcast();
		_bIsFirstInvincible = true;
	}
	_bIsInvincible = false;

	_bIsInvincibleState = false;

	SpawningState(false);

	SetMoveToTarget(true);
}

void ABuffAI::Die()
{
	Super::Die();

	// 얘가 죽으면 걸려있던 버프 해제됨

	PC_ParticleComponent_02->SetVisibility(false);

	TArray <FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(10000.0f),
		Params);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			AGruntAI* grunt = Cast<AGruntAI>(OverlapResult.GetActor());

			if (grunt && grunt != this && grunt->GetIsInvinci())
			{
				grunt->SetInvinciState(false);
			}
		}
	}
}

void ABuffAI::BeginPlay()
{
	Super::BeginPlay();

	SetTarget(InGameManager->FindActor(ARoxyCharacter::StaticClass()));

	//FVector _iconLoc = GetActorLocation() + (GetActorUpVector() * 300.0f);
	//FVector _iconScale = FVector(0.5f, 0.5f, 0.5f);
	//
	//UGameplayStatics::SpawnEmitterAttached(PS_BuffIcon, GetCapsuleComponent(), FName("IconPoint"), _iconLoc,
	//	GetActorRotation(), _iconScale, EAttachLocation::KeepWorldPosition, false);

	// 시작하자마자 버프 실행시키면 스폰애니랑 꼬여서 스폰애니 다 끝나고 실행
	GetWorldTimerManager().SetTimer(_invinTimer, this, &ABuffAI::StartInvincibility, 1.5f, false);
}

void ABuffAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (DataTable)
	{
		FGruntStatTableRow* statTableRow = DataTable->FindRow<FGruntStatTableRow>(TEXT("Buff"), FString("Buff Row is not exist!"));

		if (statTableRow)
		{
			HpComponent->SetMaxHp(statTableRow->maxHp);
			HpComponent->SetHP(HpComponent->GetMaxHp());
			_moveSpeed = statTableRow->move_speed;
			_damage_Attack = statTableRow->attack_damage;
			_stiffenTime = statTableRow->sttifen_time;
			LOG(Warning, "--------------BuffAI Stat--------------");
			LOG(Warning, "Max Hp : %f", HpComponent->GetMaxHp());
			LOG(Warning, "Move Speed : %f", _moveSpeed);
			LOG(Warning, "Attack Damage : %f", _damage_Attack);
			LOG(Warning, "Attack Stiffen Time : %f", _stiffenTime);
			LOG(Warning, "------------------------------------");
		}
	}

	BuffAnim = Cast<UBuffAnimInstance>(GetMesh()->GetAnimInstance());
	CHECK(nullptr != BuffAnim);

	BuffAnim->OnInvincibility.AddUObject(this, &ABuffAI::MakeInvincible);
	BuffAnim->OnInvincibilityEnd.AddUObject(this, &ABuffAI::EndInvincibility);

	GetCharacterMovement()->MaxWalkSpeed = _moveSpeed;

	_bIsBuffGrunt = true;
}

void ABuffAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
