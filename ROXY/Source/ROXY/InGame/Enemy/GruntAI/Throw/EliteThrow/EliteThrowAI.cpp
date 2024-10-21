// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteThrowAI.h"
//#include "InGame/Enemy/GruntAI/Throw/ThrowAnimInstance.h"
#include "InGame/Enemy/GruntAI/Throw/EliteThrow/EliteThrowAnimInstance.h"
#include "InGame/Enemy/GruntAI/Throw/EliteThrow/EliteThrowAIController.h"
#include "InGame/Enemy/GruntAI/Throw/EliteThrow/EliteThrowAISkullBomb.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAIProjectile.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Enemy/EnemyWidget.h"
#include "Kismet/KismetMathLibrary.h"

AEliteThrowAI::AEliteThrowAI()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ELITE_THROW(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mesh/SK_Throw_Elite_03.SK_Throw_Elite_03"));
	if (SK_ELITE_THROW.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_ELITE_THROW.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ELITETHROW_ANIM(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/BP_Elite_Skeleton_Anim.BP_Elite_Skeleton_Anim_C"));
	if (ELITETHROW_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ELITETHROW_ANIM.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_ORIGIN(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Materials/M_EliteThrow_Skeleton_Base.M_EliteThrow_Skeleton_Base"));
	if (MAT_ORIGIN.Succeeded())
	{
		OriginalMat = MAT_ORIGIN.Object;
		GetMesh()->SetMaterial(0, OriginalMat);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_OPACITY(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Materials/M_EliteThrow_Skeleton_Base_Inst.M_EliteThrow_Skeleton_Base_Inst"));
	if (MAT_OPACITY.Succeeded())
	{
		OpacityMat = MAT_OPACITY.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_JANGPAN(TEXT("/Game/Resources/Enemy/Particle/Fx_elite_throw_range/Fx_P_elite_throw_range/Fx_P_elite_throw_range.Fx_P_elite_throw_range"));
	if (PS_JANGPAN.Succeeded())
	{
		PS_Jangpan = PS_JANGPAN.Object;
	}

	SkullBombClass = AEliteThrowAISkullBomb::StaticClass();

	AIControllerClass = AEliteThrowAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

/* -------------------- Elite Basic Shoot -------------------- */
void AEliteThrowAI::StartEliteShoot()
{
	if (_bIsShooting) return;

	_bIsShooting = true;

	_bCanMoveToTarget = false;

	SetMoveToTarget(false);

	//SetStiffState(true);

	EliteThrowAnim->PlayShootMontage();
}

void AEliteThrowAI::EliteShoot()
{
	if (!_bIsShooting) return;

	if (ProjectileClass)
	{
		_muzzleLocation = GetMesh()->GetSocketLocation("Bip001-Spine");
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
			}
		}
	}
}

void AEliteThrowAI::EndEliteShoot()
{
	if (!_bIsShooting) return;

	LOG_SCREEN("ADad");

	_bIsShooting = false;

	_bCanMoveToTarget = true;

	SetMoveToTarget(true);

	//SetStiffState(false);

	OnShootTaskEnd.Broadcast();
}

/* -------------------- Meteor -------------------- */
void AEliteThrowAI::StartMeteor()
{
	if (!_bCanMeteor || _bIsMeteor || !_bCanEliteSpecialAttack) return;

	LOG(Warning, "Meteor");

	_bIsMeteor = true;

	_bIsEliteSpecialAttack = true;

	SetMoveToTarget(false);

	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	_curBombNum = 0;

	EliteThrowAnim->PlayMeteorMontage();
}

void AEliteThrowAI::Meteor()
{
	if (!_bIsMeteor) return;

	if (SkullBombClass)
	{
		if (GetWorld())
		{
			FActorSpawnParameters _spawnParams;
			_spawnParams.Owner = this;
			_spawnParams.Instigator = Instigator;

			_muzzleLocation = GetActorLocation() + (GetActorUpVector() * 400.0f);
			_muzzleRotation = GetActorRotation();

			for (int32 i = 0; i < _maxBobmNum; i++)
			{
				_muzzleLocation.Y += 70.0f;

				EliteThrowSkullBomb = GetWorld()->SpawnActor<AEliteThrowAISkullBomb>(SkullBombClass, _muzzleLocation, _muzzleRotation, _spawnParams);
				//LOG(Warning, "Spawn Bomb");

				float _addRandLocX = FMath::FRandRange(-500.0f, 500.0f); // ÆøÅºÀ» ´øÁú Å¸°Ù ÁÖº¯¿¡ ´õÇÒ ·£´ý°ª
				float _addRandLocY = FMath::FRandRange(-500.0f, 500.0f); // ÆøÅºÀ» ´øÁú Å¸°Ù ÁÖº¯¿¡ ´õÇÒ ·£´ý°ª
				//LOG(Warning, "random range : %f", _addRandLoc);

				FVector _bombLoc = EliteThrowSkullBomb->GetActorLocation();
				FRotator _bombRot = EliteThrowSkullBomb->GetActorRotation();

				AActor* _target = InGameManager->GetPlayerCharacter();
				FVector _targetLoc = _target->GetActorLocation() + FVector(_addRandLocX, _addRandLocY, 105.0f);

				FRotator _tempRot = UKismetMathLibrary::FindLookAtRotation(_bombLoc, _targetLoc);
				FRotator _bombNewRot = FRotator(_tempRot.Pitch, _tempRot.Yaw, _tempRot.Roll);

				EliteThrowSkullBomb->SetActorRotation(_bombNewRot);

				FVector _jangpanLoc = FVector(_targetLoc.X, _targetLoc.Y, _targetLoc.Z - 210.0f);
				FTransform _jangopanTransform = FTransform(GetActorRotation(), _jangpanLoc, FVector(0.7f, 0.7f, 1.0f));

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_Jangpan, _jangopanTransform, true);

				//FHitResult _hitResult;
				//
				//FVector _startVec = _targetLoc;
				//_startVec.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
				//FVector _endVec = _targetLoc;
				//_endVec.Z -= 1000.0f;
				//FCollisionQueryParams params(NAME_None, false, this);
				//
				//bool _result = GetWorld()->LineTraceSingleByChannel(_hitResult, _startVec, _endVec, ECollisionChannel::ECC_WorldStatic, params);
				//
				//if (_result)
				//{
				//	float _spawnHeight = _hitResult.Location.Z;
				//	FVector _jangpanLoc = FVector(_targetLoc.X, _targetLoc.Y, _spawnHeight);
				//	FTransform _jangopanTransform = FTransform(GetActorRotation(), _jangpanLoc, FVector(0.7f, 0.7f, 1.0f));
				//
				//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS_Jangpan, _jangopanTransform, true);
				//}

				FVector _shootDitection = _bombNewRot.Vector();
				EliteThrowSkullBomb->SetDropDirection(_shootDitection, _meteorSpeed);
			}
		}
	}

	// 1ÃÊ¿¡ ÇÑ°³¾¿ ¶³±À
	//GetWorldTimerManager().SetTimer(_meteorTimer, this, &AEliteThrowAI::DropBomb, 0.3f, true);
}

void AEliteThrowAI::DropBomb()
{
	// ±× ¿¾³¯ Âð ¸ÞÅ×¿À..
	//if (SkullBombClass)
	//{
	//	_muzzleLocation = (GetActorLocation() + (GetActorUpVector() * 400.0f));
	//	_muzzleRotation = GetActorRotation();
	//
	//	AActor* target = InGameManager->GetPlayerCharacter();
	//	FVector targetLoc = target->GetActorLocation();
	//
	//	if (GetWorld())
	//	{
	//		FActorSpawnParameters spawnParams;
	//		spawnParams.Owner = this;
	//		spawnParams.Instigator = Instigator;
	//
	//		EliteThrowSkullBomb = GetWorld()->SpawnActor<AEliteThrowAISkullBomb>(SkullBombClass, _muzzleLocation, _muzzleRotation, spawnParams);
	//
	//		if (EliteThrowSkullBomb)
	//		{
	//			//EliteThrowSkullBomb->SetBulletEffect(EBulletType::B_SKULL);
	//
	//			FVector projectileLoc = EliteThrowSkullBomb->GetActorLocation();
	//			FRotator projectileRot = EliteThrowSkullBomb->GetActorRotation();
	//			FRotator temRot = UKismetMathLibrary::FindLookAtRotation(projectileLoc, targetLoc);
	//			FRotator projectileNewRot = FRotator(temRot.Pitch, projectileRot.Yaw, projectileRot.Roll);
	//			EliteThrowSkullBomb->SetActorRotation(projectileNewRot);
	//
	//			FVector shootDirection = projectileNewRot.Vector();
	//			EliteThrowSkullBomb->SetDropDirection(shootDirection, _meteorSpeed);
	//
	//			_curBombNum += 1;
	//
	//			if (_curBombNum == _maxBobmNum)
	//			{
	//				LOG(Warning, "Max Meteor");
	//				GetWorldTimerManager().ClearTimer(_meteorTimer);
	//				_curBombNum = 0;
	//			}
	//		}
	//	}
	//}
}

void AEliteThrowAI::EndMeteor()
{
	if (!_bIsMeteor) return;

	_bIsMeteor = false;

	_bIsEliteSpecialAttack = false;

	SetMoveToTarget(true);

	EliteSpecialAttackCoolApply();

	if (_bIsInvincibleState)
	{
		SetInvinciState(false);
	}

	OnMeteorTaskEnd.Broadcast();
}

void AEliteThrowAI::Die()
{
	Super::Die();

	EliteWidget->RemoveFromViewport();
}

void AEliteThrowAI::BeginPlay()
{
	Super::BeginPlay();

	// ¿¤¸®Æ® ¸ó½ºÅÍ Ã¼·Â¹Ù Ç¥½Ã
	EliteWidget->AddToViewport();

	// ¿¤¸®Æ® ¾ÆÀÌÄÜ Ç¥½Ã
	//FVector _iconLoc = GetActorLocation() + (GetActorUpVector() * 300.0f);
	//FVector _iconScale = FVector(0.4f, 0.4f, 0.4f);
	//
	//UGameplayStatics::SpawnEmitterAttached(PS_EliteIcon, GetCapsuleComponent(), FName("IconPoint"), _iconLoc,
	//	GetActorRotation(), _iconScale, EAttachLocation::KeepWorldPosition, false);

	FRotator _eliteThrowRot = FRotator(0.0f, -120.0f, 0.0f);
	SetActorRotation(_eliteThrowRot);
}

void AEliteThrowAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (DataTable)
	{
		FGruntStatTableRow* statTableRow = DataTable->FindRow<FGruntStatTableRow>(TEXT("EliteThrow"), FString("EliteThrow Row is not exist!"));

		if (statTableRow)
		{
			HpComponent->SetMaxHp(statTableRow->maxHp);
			HpComponent->SetHP(HpComponent->GetMaxHp());
			_moveSpeed = statTableRow->move_speed;
			_damage_Attack = statTableRow->attack_damage;
			_stiffenTime = statTableRow->sttifen_time;
			_projectileSpeed = statTableRow->projectile_speed;
			_shootRange = statTableRow->projectile_shoot_range;
			_shootStiffenTime = statTableRow->projectile_stiffen_time;
			_damage_SpecialAttack = statTableRow->specialAttack_damage;
			_damage_EliteSpecialAttack = statTableRow->eliteSpecialAttack_damage;
			_coolTime_EliteSpecialAttack = statTableRow->eliteSpecialAttack_coolTime;
			_meteorSpeed = statTableRow->projectile_meteor_speed;
			_meteorStiffenTime = statTableRow->projectile_meteor_stiffen_time;
			LOG(Warning, "--------------EliteThrowAI Stat--------------");
			LOG(Warning, "Max Hp : %f", HpComponent->GetMaxHp());
			LOG(Warning, "Move Speed : %f", _moveSpeed);
			LOG(Warning, "Attack Damage : %f", _damage_Attack);
			LOG(Warning, "Attack Stiffen Time : %f", _stiffenTime);
			LOG(Warning, "Projectile Speed : %f", _projectileSpeed);
			LOG(Warning, "Projectile Shoot Range : %f", _shootRange);
			LOG(Warning, "Projectile Shoot Stiffen Time : %f", _shootStiffenTime);
			LOG(Warning, "Special Attack Damage : %f", _damage_SpecialAttack);
			LOG(Warning, "Elite Special Attack Damage : %f", _damage_EliteSpecialAttack);
			LOG(Warning, "Elite Special Attack CoolTime : %f", _coolTime_EliteSpecialAttack);
			LOG(Warning, "Meteor Projectile Speed : %f", _meteorSpeed);
			LOG(Warning, "Meteor Projectile Stiffen Time : %f", _meteorStiffenTime);
			LOG(Warning, "------------------------------------")
		}
	}

	EliteThrowAnim = Cast<UEliteThrowAnimInstance>(GetMesh()->GetAnimInstance());
	CHECK(nullptr != EliteThrowAnim);

	EliteThrowAnim->OnShoot.AddUObject(this, &AEliteThrowAI::EliteShoot);
	EliteThrowAnim->OnShootEnd.AddUObject(this, &AEliteThrowAI::EndEliteShoot);

	EliteThrowAnim->OnMeteor.AddUObject(this, &AEliteThrowAI::Meteor);
	EliteThrowAnim->OnMeteorEnd.AddUObject(this, &AEliteThrowAI::EndMeteor);

	GetCharacterMovement()->MaxWalkSpeed = _moveSpeed;

	// À§Á¬¿¡ ³ª´Â ¾²·Î¿ì´Ù ¾Ë¸²
	EliteWidget->SetEliteThrow();

	_bIsEliteGrunt = true;
}

void AEliteThrowAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AEliteThrowAI::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (_bIsMeteor && _currentStateMode == EGruntStateMode::SM_KNOCKBACK)
	{
		// ¿¤¸®Æ® Æ¯¼ö ÆÐÅÏ ½ÇÇà Áß ³Ë¹é´çÇÒ ½Ã ÆÐÅÏ Äµ½½

		AnimInstance->StopAllMontages(0.2f);
		EndMeteor();

		if (_bIsAttacking)
		{
			AttackEnd();
		}
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
