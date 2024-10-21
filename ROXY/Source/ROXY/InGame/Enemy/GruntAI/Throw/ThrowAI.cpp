// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowAI.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAIController.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAnimInstance.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAIProjectile.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/Generator.h"
#include "InGame/System/NunuInGameManager.h"
#include "Components/WidgetComponent.h"

AThrowAI::AThrowAI()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_THROW(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mesh/SK_Throw.SK_Throw"));
	if (SK_THROW.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_THROW.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> THROW_ANIM(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/BP_Skeleton_Anim.BP_Skeleton_Anim_C"));
	if (THROW_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(THROW_ANIM.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_ORIGIN(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Materials/M_Throw_Skeleton_Base.M_Throw_Skeleton_Base"));
	if (MAT_ORIGIN.Succeeded())
	{
		OriginalMat = MAT_ORIGIN.Object;
		GetMesh()->SetMaterial(0, OriginalMat);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_OPACITY(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Materials/M_Throw_Skeleton_Base_Inst.M_Throw_Skeleton_Base_Inst"));
	if (MAT_OPACITY.Succeeded())
	{
		OpacityMat = MAT_OPACITY.Object;
	}

	ProjectileClass = AThrowAIProjectile::StaticClass();

	AIControllerClass = AThrowAIController::StaticClass();
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

void AThrowAI::StartShoot()
{
	if (_bIsShooting) return;

	_bIsShooting = true;

	_bCanMoveToTarget = false;
	//SetMoveToTarget(false);

	//SetStiffState(true);

	ThrowAnim->PlayShootMontage();
}

void AThrowAI::Shoot()
{
	if (!_bIsShooting) return;

	if (ProjectileClass)
	{
		_muzzleLocation = GetMesh()->GetSocketLocation("Bip001-Spine1");
		_muzzleRotation = GetActorRotation();

		if (GetWorld())
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = Instigator;

			ThrowAIProjectile = GetWorld()->SpawnActor<AThrowAIProjectile>(ProjectileClass, _muzzleLocation, _muzzleRotation, spawnParams);
			//projectile->SetActorLocation(_muzzleLocation);

			if (ThrowAIProjectile)
			{
				ThrowAIProjectile->SetBulletEffect(EBulletType::B_ORIGIN);

				FVector shootDirection = _muzzleRotation.Vector() * 10.0f;
				ThrowAIProjectile->SetShootDirection(shootDirection, _projectileSpeed);

				// 발사 후 투사체가 살아있는 시간
				//ThrowAIProjectile->SetLifeSpan(_projectileLifeTime);
			}
		}
	}
}

void AThrowAI::EndShoot()
{
	if (!_bIsShooting) return;
	
	_bIsShooting = false;

	_bCanMoveToTarget = true;

	//SetStiffState(false);

	OnShootTaskEnd.Broadcast();
}

void AThrowAI::StartSkullShoot()
{
	if (_bIsSkullShooting) return;

	//LOG(Warning, "Skull Shoot");

	_bIsSkullShooting = true;

	_bIsSpecialAttack = true;

	//_bCanMoveToTarget = false;
	SetMoveToTarget(false);

	SetStiffState(true);

	ThrowAnim->PlaySkullShootMontage();
}

void AThrowAI::SkullShoot()
{
	if (!_bIsSkullShooting) return;

	if (ProjectileClass)
	{
		_muzzleLocation = GetMesh()->GetSocketLocation("Bip001-Spine1");
		_muzzleRotation = GetActorRotation();

		if (GetWorld())
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = Instigator;

			ThrowAIProjectile = GetWorld()->SpawnActor<AThrowAIProjectile>(ProjectileClass, _muzzleLocation, _muzzleRotation, spawnParams);
			//projectile->SetActorLocation(_muzzleLocation);

			if (ThrowAIProjectile)
			{
				//LOG(Warning, "Projectile");

				ThrowAIProjectile->SetBulletEffect(EBulletType::B_ORIGIN);

				FVector shootDirection = _muzzleRotation.Vector() * 10.0f;
				ThrowAIProjectile->SetShootDirection(shootDirection, _projectileSpeed);

				// 발사 후 투사체가 살아있는 시간
				//ThrowAIProjectile->SetLifeSpan(_projectileLifeTime);
			}
		}
	}
}

void AThrowAI::EndSkullShoot()
{
	if (!_bIsSkullShooting) return;

	_bIsSkullShooting = false;

	_bIsSpecialAttack = false;

	//_bCanMoveToTarget = true;

	SetStiffState(false);

	OnSkullShootTaskEnd.Broadcast();

	SpecialAttackCoolApply();
}

void AThrowAI::Die()
{
	Super::Die();

}

void AThrowAI::BeginPlay()
{
	Super::BeginPlay();
	
	//SetTarget(InGameManager->FindActor(AGenerator::StaticClass()));
}

void AThrowAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (DataTable)
	{
		FGruntStatTableRow* statTableRow = DataTable->FindRow<FGruntStatTableRow> (TEXT ("Throw"), FString ("Throw Row is not exist!"));

		if (statTableRow)
		{
			HpComponent->SetMaxHp (statTableRow->maxHp);
			HpComponent->SetHP (HpComponent->GetMaxHp ());
			_moveSpeed = statTableRow->move_speed;
			_damage_Attack = statTableRow->attack_damage;
			_stiffenTime = statTableRow->sttifen_time;
			_projectileSpeed = statTableRow->projectile_speed;
			_shootRange = statTableRow->projectile_shoot_range;
			_shootStiffenTime = statTableRow->projectile_stiffen_time;
			_damage_SpecialAttack = statTableRow->specialAttack_damage;
			_coolTime_SpecialAttack = statTableRow->specialAttack_coolTime;
			LOG (Warning, "--------------ThrowAI Stat--------------");
			LOG (Warning, "Max Hp : %f", HpComponent->GetMaxHp ());
			LOG (Warning, "Move Speed : %f", _moveSpeed);
			LOG (Warning, "Attack Damage : %f", _damage_Attack);
			LOG (Warning, "Attack Stiffen Time : %f", _stiffenTime);
			LOG (Warning, "Projectile Speed : %f", _projectileSpeed);
			LOG (Warning, "Projectile Shoot Range : %f", _shootRange);
			LOG	(Warning, "Projectile Shoot Stiffen Time : %f", _shootStiffenTime);
			LOG (Warning, "Special Attack Damage : %f", _damage_SpecialAttack);
			LOG (Warning, "Special Attack CoolTime : %f", _coolTime_SpecialAttack);
			LOG (Warning, "------------------------------------")
		}
	}

	ThrowAnim = Cast<UThrowAnimInstance>(GetMesh()->GetAnimInstance());
	CHECK(nullptr != ThrowAnim);

	ThrowAnim->OnShoot.AddUObject(this, &AThrowAI::Shoot);
	ThrowAnim->OnShootEnd.AddUObject(this, &AThrowAI::EndShoot);

	ThrowAnim->OnSkullShoot.AddUObject(this, &AThrowAI::SkullShoot);
	ThrowAnim->OnSkullShootEnd.AddUObject(this, &AThrowAI::EndSkullShoot);

	GetCharacterMovement()->MaxWalkSpeed = _moveSpeed;

	_canShootDist = FMath::RandRange(2800.0f, 3000.0f);
}

void AThrowAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ThrowAIProjectile)
	{
		_projectileLocation = ThrowAIProjectile->GetActorLocation();

		float distFromProjectile = FVector::Dist(_projectileLocation, _muzzleLocation);

		if (distFromProjectile > _shootRange)
		{
			ThrowAIProjectile->Destroy();
		}
	}
}

float AThrowAI::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (_currentStateMode == EGruntStateMode::SM_IDLE)
	{
		EndShoot();
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
