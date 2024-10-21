// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteCombatAI.h"
#include "InGame/Enemy/GruntAI/Combat/CombatAnimInstance.h"
#include "InGame/Enemy/GruntAI/Combat/EliteCombat/EliteCombatAIController.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "InGame/Enemy/EnemyWidget.h"

AEliteCombatAI::AEliteCombatAI()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ELITE_COMBAT(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mesh/SK_Cactus_Elite.SK_Cactus_Elite"));
	if (SK_ELITE_COMBAT.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_ELITE_COMBAT.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_ORIGIN(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Materials/M_Elite_Combat_Base.M_Elite_Combat_Base"));
	if (MAT_ORIGIN.Succeeded())
	{
		OriginalMat = MAT_ORIGIN.Object;
		GetMesh()->SetMaterial(0, OriginalMat);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_OPACITY(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Materials/M_Elite_Combat_Base_Inst.M_Elite_Combat_Base_Inst"));
	if (MAT_OPACITY.Succeeded())
	{
		OpacityMat = MAT_OPACITY.Object;
	}

	/* 사운드 */
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_GROWL(TEXT("/Game/Resources/Sound/Enemy/Elite/SC_EliteCombat_Growl.SC_EliteCombat_Growl"));
	if (SC_GROWL.Succeeded())
	{
		SC_Growl = SC_GROWL.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_SPAWN(TEXT("/Game/Resources/Sound/Enemy/Elite/SC_EliteCombat_Spawn.SC_EliteCombat_Spawn"));
	if (SC_SPAWN.Succeeded())
	{
		SC_Spawn = SC_SPAWN.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_ATTACK(TEXT("/Game/Resources/Sound/Enemy/Elite/SC_EliteCombat_Attack.SC_EliteCombat_Attack"));
	if (SC_ATTACK.Succeeded())
	{
		SC_EliteCombat_Attack = SC_ATTACK.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_DEATH(TEXT("/Game/Resources/Sound/Enemy/Elite/SC_EliteCombat_Death.SC_EliteCombat_Death"));
	if (SC_DEATH.Succeeded())
	{
		SC_EliteCombat_Death = SC_DEATH.Object;
	}

	AIControllerClass = AEliteCombatAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEliteCombatAI::PlayAttackSound()
{
	// 엘리트 컴뱃용 공격 사운드
	UGameplayStatics::PlaySoundAtLocation(this, SC_EliteCombat_Attack, GetActorLocation());
}

/* -------------------- Push Out -------------------- */
void AEliteCombatAI::StartPushOut()
{
	if (!_bCanPushOut || _bIsPushOut || !_bCanEliteSpecialAttack) return;

	_bIsPushOut = true;

	_bIsEliteSpecialAttack = true;

	_bCanMoveToTarget = false;

	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CombatAnim->PlayPushOutMontage();
}

void AEliteCombatAI::PushOut()
{
	if (!_bIsPushOut) return;

	TArray <FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(_pushOutRange),
		Params);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OverlapResult.GetActor());

			if (roxy && !roxy->GetCharacterIsSwift())
			{
				LOG(Warning, "Check Push Out");

				roxy->SetFaint(3.0f);

				roxy->GetCharacterMovement()->Velocity = FVector::ZeroVector;

				FDamageEvent damageEvent;
				roxy->TakeDamage(_damage_EliteSpecialAttack, damageEvent, GetController(), this);
			}
		}
	}

	//ARoxyCharacter* roxy = Cast<ARoxyCharacter>(pushTarget);
	//
	//if (roxy && !roxy->GetCharacterIsSwift() && !roxy->GetCharacterIsDie())
	//{
	//	LOG(Warning, "Elite Combat Push Out");
	//
	//	// 기절 3초
	//	roxy->SetFaint(3.0f);
	//
	//	roxy->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	//
	//	FRotator rotation(roxy->GetActorRotation());
	//	rotation.Yaw = UKismetMathLibrary::FindLookAtRotation(roxy->GetActorLocation(), GetActorLocation()).Yaw;
	//	roxy->SetActorRotation(rotation);
	//
	//	FVector pushVec = roxy->GetActorForwardVector() * -15000.f;
	//	roxy->GetCharacterMovement()->Velocity = pushVec;
	//
	//	FDamageEvent damageEvent;
	//	roxy->TakeDamage(_damage_EliteSpecialAttack, damageEvent, GetController(), this);
	//}
}

void AEliteCombatAI::EndPushOut()
{
	if (!_bIsPushOut) return;

	_bIsPushOut = false;

	_bIsEliteSpecialAttack = false;

	_bCanMoveToTarget = true;

	OnPushOutTaskEnd.Broadcast();

	EliteSpecialAttackCoolApply();
}

void AEliteCombatAI::Die()
{
	Super::Die();

	EliteWidget->RemoveFromViewport();

	UGameplayStatics::PlaySoundAtLocation(this, SC_EliteCombat_Death, GetActorLocation());
}

void AEliteCombatAI::BeginPlay()
{
	Super::BeginPlay();

	// 엘리트 몬스터 체력바 표시
	EliteWidget->AddToViewport();

	// 등장 시 울음소리
	UGameplayStatics::PlaySoundAtLocation(this, SC_Spawn, GetActorLocation());

	// 엘리트 아이콘 표시
	//FVector _iconLoc = GetActorLocation() + (GetActorUpVector() * 200.0f);
	//FVector _iconScale = FVector(0.4f, 0.4f, 0.4f);
	//
	//UGameplayStatics::SpawnEmitterAttached(PS_EliteIcon, GetCapsuleComponent(), FName("IconPoint"), _iconLoc,
	//	GetActorRotation(), _iconScale, EAttachLocation::KeepWorldPosition, false);
}

void AEliteCombatAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (DataTable)
	{
		FGruntStatTableRow* statTableRow = DataTable->FindRow<FGruntStatTableRow>(TEXT("EliteCombat"), FString("EliteCombat Row is not exist!"));

		if (statTableRow)
		{
			HpComponent->SetMaxHp(statTableRow->maxHp);
			HpComponent->SetHP(HpComponent->GetMaxHp());
			_moveSpeed = statTableRow->move_speed;
			_damage_Attack = statTableRow->attack_damage;
			_stiffenTime = statTableRow->sttifen_time;
			_damage_EliteSpecialAttack = statTableRow->eliteSpecialAttack_damage;
			_coolTime_EliteSpecialAttack = statTableRow->eliteSpecialAttack_coolTime;
			LOG(Warning, "--------------Elite CombatAI Stat--------------");
			LOG(Warning, "Max Hp : %f", HpComponent->GetMaxHp());
			LOG(Warning, "Move Speed : %f", _moveSpeed);
			LOG(Warning, "Attack Damage : %f", _damage_Attack);
			LOG(Warning, "Attack Stiffen Time : %f", _stiffenTime);
			LOG(Warning, "Elite Special Attack Damage : %f", _damage_EliteSpecialAttack);
			LOG(Warning, "Elite Special Attack CoolTime : %f", _coolTime_EliteSpecialAttack);
			LOG(Warning, "------------------------------------")
		}
	}

	CombatAnim->OnAttackSound.AddUObject(this, &AEliteCombatAI::PlayAttackSound);
	CombatAnim->OnPushOut.AddUObject(this, &AEliteCombatAI::PushOut);
	CombatAnim->OnPushOutEnd.AddUObject(this, &AEliteCombatAI::EndPushOut);

	GetCharacterMovement()->MaxWalkSpeed = _moveSpeed;

	// 위젯에 나는 컴뱃이다 알림
	EliteWidget->SetEliteCombat();

	_bIsEliteGrunt = true;
}

void AEliteCombatAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AEliteCombatAI::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (_bIsPushOut && _currentStateMode == EGruntStateMode::SM_KNOCKBACK)
	{
		// 엘리트 특수 패턴 실행 중 넉백당할 시 패턴 캔슬
		
		AnimInstance->StopAllMontages(0.2f);
		EndPushOut();
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
