// Fill out your copyright notice in the Description page of Project Settings.


#include "RevivalAI.h"
#include "InGame/Enemy/GruntAI/Revival/RevivalAIController.h"
#include "InGame/Enemy/GruntAI/Revival/RevivalAnimInstance.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Enemy/EnemyWidget.h"
#include "InGame/Enemy/Boss/BossProjectile.h"
#include "InGame/Enemy/GruntAI/Combat/CombatAI.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAI.h"
#include "InGame/Enemy/GruntAI/Charge/ChargeAI.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "Kismet/KismetMathLibrary.h"

ARevivalAI::ARevivalAI()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_REVIVAL(TEXT("/Game/Resources/Enemy/Grunt/Wizard_Undead/Mesh/SK_Wizard_Undead.SK_Wizard_Undead"));
	if (SK_REVIVAL.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_REVIVAL.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> REVIVAL_ANIM(TEXT("/Game/Resources/Enemy/Grunt/Wizard_Undead/BP_Revival_Anim.BP_Revival_Anim_C"));
	if (REVIVAL_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(REVIVAL_ANIM.Class);
	}

	// 캐릭터 머테리얼
	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_ORIGIN(TEXT("/Game/Resources/Enemy/Grunt/Wizard_Undead/Materials/M_Undead_B.M_Undead_B"));
	if (MAT_ORIGIN.Succeeded())
	{
		OriginalMat = MAT_ORIGIN.Object;
		GetMesh()->SetMaterial(0, OriginalMat);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_OPACITY(TEXT("/Game/Resources/Enemy/Grunt/Wizard_Undead/Materials/M_Undead_B_Inst.M_Undead_B_Inst"));
	if (MAT_OPACITY.Succeeded())
	{
		OpacityMat = MAT_OPACITY.Object;
	}

	// 책 머테리얼
	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_STAFF_ORIGIN(TEXT("/Game/Resources/Enemy/Grunt/Wizard_Undead/Materials/M_Undead_Book_B.M_Undead_Book_B"));
	if (MAT_STAFF_ORIGIN.Succeeded())
	{
		WeaponMat = MAT_STAFF_ORIGIN.Object;
		GetMesh()->SetMaterial(1, WeaponMat);
	}

	//static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_STAFF_OPACITY(TEXT("/Game/Resources/Enemy/Grunt/Wizard_Undead/Materials/M_Undead_Book_B_Inst.M_Undead_Book_B_Inst"));
	//if (MAT_STAFF_OPACITY.Succeeded())
	//{
	//	BookOpacityMat = MAT_STAFF_OPACITY.Object;
	//}

	// 파티클
	PC_ParticleComponent_02 = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent_02");
	PC_ParticleComponent_02->SetupAttachment(RootComponent);
	PC_ParticleComponent_02->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PC_ParticleComponent_02->SetVisibility(true);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_POWER(TEXT("/Game/Resources/Enemy/Particle/Fx_WizardPower/P_WizardPower.P_WizardPower"));
	if (PS_POWER.Succeeded())
	{
		PS_UndeadPower = PS_POWER.Object;
		PC_ParticleComponent_02->SetTemplate(PS_UndeadPower);
	}

	static ConstructorHelpers::FClassFinder<ABossProjectile> PROJECTILE(TEXT("/Game/Blueprints/InGame/Enemy/Boss/BP_BossProjectile.BP_BossProjectile_C"));
	if(PROJECTILE.Succeeded())
	{
		BossProjClass = PROJECTILE.Class;
	}

	static ConstructorHelpers::FClassFinder<ACombatAI> COMBAT_BP(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Combat/BP_CombatAI.BP_CombatAI_C"));
	if(COMBAT_BP.Succeeded())
	{
		combat_bp = COMBAT_BP.Class;
	}

	static ConstructorHelpers::FClassFinder<AThrowAI> THROW_BP(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Throw/BP_ThrowAI.BP_ThrowAI_C"));
	if(THROW_BP.Succeeded())
	{
		throw_bp = THROW_BP.Class;
	}

	static ConstructorHelpers::FClassFinder<AChargeAI> CHARGE_BP(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Charge/BP_ChargeAI.BP_ChargeAI_C"));
	if(CHARGE_BP.Succeeded())
	{
		charge_bp = CHARGE_BP.Class;
	}

	// AI
	AIControllerClass = ARevivalAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 부비적
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 150.0f;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCapsuleComponent()->SetCapsuleSize(100.f, 200.f);

	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
	GetMesh()->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));

	_defaultMeshLoc_Z = GetMesh()->GetRelativeTransform().GetLocation().Z;
	_descentZ = 100.f;
}

void ARevivalAI::ShockWave()
{
	TArray <FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(1000.0f),
		Params);

		if (bResult)
		{
			for (auto OverlapResult : OverlapResults)
			{
				ARoxyCharacter* roxy = Cast<ARoxyCharacter>(OverlapResult.GetActor());

				if (roxy)
				{
					LOG(Warning, "Shock Wave");

					if (!roxy->GetCharacterIsSwift())
					{
						roxy->SetStiffen(0.9f);

						// 밀쳐짐
						roxy->GetCharacterMovement()->Velocity = FVector::ZeroVector;

						FRotator rotation(roxy->GetActorRotation());
						rotation.Yaw = UKismetMathLibrary::FindLookAtRotation(roxy->GetActorLocation(), GetActorLocation()).Yaw;
						roxy->SetActorRotation(rotation);

						FVector pushVec = roxy->GetActorForwardVector() * -15000.f;
						roxy->GetCharacterMovement()->Velocity = pushVec;

						// 데미지
						FDamageEvent damageEvent;
						roxy->TakeDamage(1000.0f, damageEvent, GetController(), this);
					}
				}
			}
		}
}

void ARevivalAI::DieCheck()
{
	if (_bIsRevivalDie) return;

	_bIsRevivalDie = true;

	LOG(Warning, "Die Check");

	OnRevivalDie.Broadcast();
}

void ARevivalAI::DestroyBoss()
{
	if (!_bIsRevivalDie) return;

	Destroy();

	EliteWidget->RemoveFromViewport();
}

void ARevivalAI::StartRevival()
{
	if (_bIsRevive) return;

	_bIsRevive = true;
	RevivalAnim->PlayRevivalMontage();
	SetMoveToTarget(false);
	_bCanTurnToTarget = false;
}

void ARevivalAI::BeRevive()
{
	if (!_bIsRevive) return;

	int spawnNum = UKismetMathLibrary::RandomIntegerInRange(_revivalEnemyNum_min, _revivalEnemyNum_max); // 스폰할 적의 수 계산 (랜덤)
	int currentSpawnNum = 0;
	// 플레이어 캐릭터를 바라보는 방향 계산
	FVector lookAtTargetVec = Roxy->GetActorLocation() - GetActorLocation();
	lookAtTargetVec.Z = 0.f;
	lookAtTargetVec.Normalize();
	
	// 스폰할 중심 지점
	FVector spawnPoint = GetActorLocation() + (lookAtTargetVec * (GetCapsuleComponent()->GetScaledCapsuleRadius() + _revivalRadius));

	while(currentSpawnNum++ < spawnNum)
	{
		// 랜덤으로 스폰 위치 설정
		FVector2D randomSpawnVec = FMath::RandPointInCircle(_revivalRadius);
		FVector spawnLoc = FVector(spawnPoint.X + randomSpawnVec.X, spawnPoint.Y + randomSpawnVec.Y, GetActorLocation().Z);
		// 랜덤으로 스폰할 적 타입 설정
		int spawnEnemyTypeInt = UKismetMathLibrary::RandomIntegerInRange(0, 100);


		TSubclassOf<class AEnemyAI> enemyType;
		// 60% 의 확률로 컴뱃 스폰
		if(spawnEnemyTypeInt < 60)
		{
			enemyType = combat_bp;
		}
		// 20% 의 확률로 뜨로우 스폰
		else if(spawnEnemyTypeInt >= 60 && spawnEnemyTypeInt < 80)
		{
			enemyType = throw_bp;
		}
		// 20% 의 확률로 charge 스폰
		else
		{
			enemyType = charge_bp;
		}

		// 적 스폰
		AEnemyAI* spawnEnemyPtr = GetWorld()->SpawnActor<AEnemyAI>(enemyType, spawnLoc, lookAtTargetVec.Rotation());
		
		// 부활 시킨 적읠 섬멸했는지 확인하기 위한 rest enemys num 작업
		if(spawnEnemyPtr != nullptr)
		{
			_restEnemysNum++;
			FScriptDelegate scriptDelegate;
			scriptDelegate.BindUFunction(this, "DecreaseRestEnemyNum");
			spawnEnemyPtr->OnDestroyed.Add(scriptDelegate);
		}
	}
}

void ARevivalAI::EndRevival()
{
	if (!_bIsRevive) return;

	_bIsRevive = false;
	OnEndRevival.Broadcast();
	SetMoveToTarget(true);
	_bCanTurnToTarget = true;
}

ABossProjectile* ARevivalAI::SpawnProj(const FVector& spawnVec, const FRotator& targetRot)
{
	ABossProjectile* projectile = GetWorld()->SpawnActor<ABossProjectile>(BossProjClass, spawnVec, targetRot);
	projectile->SetDamageInstigatorInfo(GetController(), Cast<AActor>(this));
	return projectile;
}


FVector ARevivalAI::GetFloorLocation()
{
	FHitResult hitResult;
	FVector startVec(GetActorLocation());
	startVec.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector endVec(GetActorLocation());
	endVec.Z -= 100000.f;
	FCollisionQueryParams params(NAME_None, false, this);
	bool result = GetWorld()->LineTraceSingleByChannel(hitResult, startVec, endVec, ECollisionChannel::ECC_Visibility, params);

	if(result) {
		return hitResult.Location;
	}
	else {
		LOG(Error, "Boss is Not Exist On Floor!");
		return FVector::ZeroVector;
	}
}

void ARevivalAI::StartTired()
{
	RevivalAnim->PlayTiredMontage();
	SetMoveToTarget(false);
	_bCanTurnToTarget = false;

	_bIsInvincibleState = false;
}

void ARevivalAI::EndTired()
{
	OnEndTired.Broadcast();
	_bCanRising = false;
	SetMoveToTarget(true);
	_bCanTurnToTarget = true;

	_bIsInvincibleState = true;
}


void ARevivalAI::SelectRandomlyProjTypeAndShot()
{
	ERevivalProjectileType projectileType = (ERevivalProjectileType)(UKismetMathLibrary::RandomInteger((int32)ERevivalProjectileType::RPT_Size));


	switch(projectileType) {
// 	case ERevivalProjectileType::RPT_Horizontal:
// 		BeginShotProj_Horizontal();
// 		_currentProjectileState = ERevivalProjectileType::RPT_Horizontal;
// #if ENABLE_DRAW_DEBUG
// 		LOG_SCREEN("Boss behavior : Horizontal shot");
// #endif
// 		break;
	case ERevivalProjectileType::RPT_Delay:
		ShootProj_Delay();
#if ENABLE_DRAW_DEBUG
		LOG(Warning, "Boss behavior : Delay shot");
#endif
		break;
// 	case ERevivalProjectileType::RPT_Homing:
// 		BeginShotProj_Homing();
// 		_currentProjectileState = ERevivalProjectileType::RPT_Homing;
// #if ENABLE_DRAW_DEBUG
// 		LOG_SCREEN("Boss behavior : Homing shot");
// #endif
// 		break;
	case ERevivalProjectileType::RPT_Fake:
		ShootProj_Fake();
#if ENABLE_DRAW_DEBUG
		LOG(Warning, "Boss behavior : Fake shot");
#endif
		break;
	}

	OnShootProj.Broadcast();

}

void ARevivalAI::BeginShootProj()
{
	RevivalAnim->PlayShootProjMontage();
}

void ARevivalAI::ShootProj_Fake()
{
	SetTurnToTarget(false);

	_recreateProjCount++;

	FVector floorLoc = GetFloorLocation(); // 폰이 서 있는 바닥의 위치값을 가져온다.
	FVector spawnForwardLoc(GetActorLocation() + (GetActorForwardVector() * 300.f)); // forwardVector 벡터를 기준으로 스폰 할 것임.
	ARoxyCharacter* target = InGameManager->GetPlayerCharacter();
	FVector targetLoc = target->GetActorLocation();


	FVector projectileRotVec((targetLoc - GetActorLocation()).X, (targetLoc - GetActorLocation()).Y, 0); // 2차원 벡터처럼 발사하기 위해 Z값만 0으로 두고 타깃을 바라보는 방향 벡터를 구한다 
	FRotator projectileRot = projectileRotVec.Rotation();
	float spawnHeightGap = 150.f; // 발사체 높이의 갭
	float initHeight = floorLoc.Z; // 바닥을 기준으로 함

	FVector spawnLoc(spawnForwardLoc.X, spawnForwardLoc.Y, initHeight);

	for(int32 i = 0; i < _projNum_Fake; i++)
	{
		spawnLoc.Z += spawnHeightGap; // 발사체 높이 갭만큼 증가시켜줄 것.
		ABossProjectile* projectile = SpawnProj(spawnLoc, projectileRot);
		projectile->SetActiveAfterDelay(1.2f, 5000.f);

		projectile->PlayProjectileSound();

		// 속도가 활성화 됐을 때 타깃을 바라보는 방향으로 rotation 을 바꿔준다
		projectile->EndDelayActive.BindLambda([this](ARoxyCharacter* target, AActor* projectile, int32 recreateProjCount, int32 recreateMax) {
			FVector projRotVec = (target->GetActorLocation() - projectile->GetActorLocation());
			projRotVec.Z = 0.f;

			if(recreateProjCount == recreateMax && target->GetCurrentSpeed() != 0) {
				projRotVec += target->GetActorForwardVector() * 400.f;
			}

			FRotator projRot = projRotVec.Rotation();
			projectile->SetActorRotation(projRot);
		}, target, projectile, _recreateProjCount, _recreateProjMax_Fake);
	}

	// 재 발사 횟수가 만족되면 스킬을 끝낸다.
	if(_recreateProjCount >= _recreateProjMax_Fake) {
		_recreateProjCount = 0;
	}
	else {
		FTimerHandle delayHandle;
		FTimerDelegate delayDelegate;
		delayDelegate.BindUObject(this, &ARevivalAI::ShootProj_Fake);
		float recreateDuration = 0.5;
		GetWorld()->GetTimerManager().SetTimer(delayHandle, delayDelegate, recreateDuration, false);
	}
}

void ARevivalAI::ShootProj_Delay()
{
	SetMoveToTarget(false);

	_recreateProjCount++;

	FVector floorLoc = GetFloorLocation();

	float Y_angleGap = _proj_Y_AngleGap_Delay;
	Y_angleGap *= (float)_recreateProjCount; // 발사체 재생성시마다 일정한 Pitch 각도를 더해줄 것이므로 재생성 횟수를 곱한다

	float Z_angleGap = 360 / _projNum_Delay;

	FVector projectileFirstLoc(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 900.f);
	FRotator projectileRot = GetActorForwardVector().Rotation();
	float standardAngle = -70.f; // pitch 값 증가의 기준이 될 각도
	projectileRot = projectileRot.Add(standardAngle + Y_angleGap, -Z_angleGap, 0); // pitch 기준 각도에 pitch 각도 증가값을 더한다

	FVector projectileLoc;

	for(int32 i = 0; i < _projNum_Delay; i++) {
		projectileRot = projectileRot.Add(0, Z_angleGap, 0);
		projectileLoc = projectileFirstLoc + (projectileRot.Vector() * 400.f);
		projectileLoc.Z = projectileFirstLoc.Z;
		ABossProjectile* projectile = SpawnProj(projectileLoc, projectileRot);
		projectile->SetVelocity(1.0f);
		projectile->SetAccelerate(true);
	}

	// 발사 과정이 전부 끝나면 해당 스킬을 끝낸다.
	if(_recreateProjCount == _recreateProjMax_Delay) {
		_recreateProjCount = 0;
	}
	else {
		FTimerHandle delayHandle;
		FTimerDelegate delayDelegate;
		delayDelegate.BindUObject(this, &ARevivalAI::ShootProj_Delay);
		float delayDuration = 0.5;
		GetWorld()->GetTimerManager().SetTimer(delayHandle, delayDelegate, delayDuration, false);
	}
}

void ARevivalAI::EndShootProj()
{
	OnEndShootProj.Broadcast();
}

float ARevivalAI::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(_bCanTakeDamage)
	{
		if (Roxy->GetCharacterIsSwift())
		{
			// 연참 피격 이펙트
			UGameplayStatics::SpawnEmitterAttached(PS_SwiftHit, GetMesh(), FName("HitPoint"), GetActorLocation(),
				GetActorRotation(), FVector(1.0f, 1.0f, 1.0f), EAttachLocation::KeepWorldPosition, true);
		}
		else
		{
			UGameplayStatics::SpawnEmitterAttached(PS_Hit, GetMesh(), FName("HitPoint"), GetActorLocation(),
				GetActorRotation(), FVector(1.0f, 1.0f, 1.0f), EAttachLocation::KeepWorldPosition, true);
		}

		SetDamagedMat();

		return HpComponent->TakeDamage(DamageAmount);
	}
	else
		return 0;
}

void ARevivalAI::Die()
{
	Super::Die();

	LOG(Warning, "Boss Is Die");

	Roxy->SetClearTimerStop();

	_currentMatType = ECurrentMatType::MT_OPACITY;

	//EliteWidget->RemoveFromViewport();
}

void ARevivalAI::BeginPlay()
{
	Super::BeginPlay();

	SetTarget(InGameManager->FindActor(ARoxyCharacter::StaticClass()));
	
	// 엘리트 몬스터 체력바 표시
	EliteWidget->AddToViewport();

	_bIsInvincibleState = true;

	// 버프몹 아이콘
	//FVector _iconLoc = GetActorLocation() + (GetActorUpVector() * 450.0f);
	//FVector _iconScale = FVector(0.5f, 0.5f, 0.5f);
	//
	//UGameplayStatics::SpawnEmitterAttached(PS_BuffIcon, GetCapsuleComponent(), FName("IconPoint"), _iconLoc,
	//	GetActorRotation(), _iconScale, EAttachLocation::KeepWorldPosition, false);
}

void ARevivalAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (DataTable)
	{
		FGruntStatTableRow* statTableRow = DataTable->FindRow<FGruntStatTableRow>(TEXT("Revival"), FString("Revival Row is not exist!"));

		if (statTableRow)
		{
			HpComponent->SetMaxHp(statTableRow->maxHp);
			HpComponent->SetHP(HpComponent->GetMaxHp());
			_moveSpeed = statTableRow->move_speed;
			_damage_Attack = statTableRow->attack_damage;
			_stiffenTime = statTableRow->sttifen_time;
			LOG(Warning, "--------------RevivalAI Stat--------------");
			LOG(Warning, "Max Hp : %f", HpComponent->GetMaxHp());
			LOG(Warning, "Move Speed : %f", _moveSpeed);
			LOG(Warning, "Attack Damage : %f", _damage_Attack);
			LOG(Warning, "Attack Stiffen Time : %f", _stiffenTime);
			LOG(Warning, "------------------------------------");
		}
	}

	RevivalAnim = Cast<URevivalAnimInstance>(GetMesh()->GetAnimInstance());
	RevivalAnim->OnRevive.AddUObject(this, &ARevivalAI::BeRevive);
	RevivalAnim->OnEndRevive.AddUObject(this, &ARevivalAI::EndRevival);
	RevivalAnim->OnShootProj.AddUObject(this, &ARevivalAI::SelectRandomlyProjTypeAndShot);
	RevivalAnim->OnEndShootProj.AddUObject(this, &ARevivalAI::EndShootProj);
	RevivalAnim->OnDescent.AddUObject(this, &ARevivalAI::Descent);
	RevivalAnim->OnRising.AddUObject(this, &ARevivalAI::Rising);
	RevivalAnim->OnRising.AddUObject(this, &ARevivalAI::ShockWave);
	RevivalAnim->OnTired.AddUObject(this, &ARevivalAI::SetCanTakeDamage);
	RevivalAnim->OnEndTakeDamage.AddUObject(this, &ARevivalAI::SetDoNotTakeDamage);
	RevivalAnim->OnEndTired.AddUObject(this, &ARevivalAI::EndTired);
	RevivalAnim->OnDie.AddUObject(this, &ARevivalAI::DieCheck);
	RevivalAnim->OnEndDie.AddUObject(this, &ARevivalAI::DestroyBoss);
	
	CHECK(nullptr != RevivalAnim);

	GetCharacterMovement()->MaxWalkSpeed = _moveSpeed;

	// 위젯에 나는 부활이다 알림
	EliteWidget->SetRevival();

	_bIsBossMonster = true;
}

void ARevivalAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(_bCanDescent)
	{
		FVector currentMeshLoc = GetMesh()->GetRelativeTransform().GetLocation();
		if(!FMath::IsNearlyEqual(currentMeshLoc.Z, _defaultMeshLoc_Z - _descentZ))
		{
			float newZ = FMath::FInterpTo(currentMeshLoc.Z, _defaultMeshLoc_Z - _descentZ, DeltaTime, _descentSpeed);
			GetMesh()->SetRelativeLocation(FVector(currentMeshLoc.X, currentMeshLoc.Y, newZ));
		}
	}
	else if(_bCanRising)
	{
		FVector currentMeshLoc = GetMesh()->GetRelativeTransform().GetLocation();
		if(!FMath::IsNearlyEqual(currentMeshLoc.Z, _defaultMeshLoc_Z))
		{
			float newZ = FMath::FInterpTo(currentMeshLoc.Z, _defaultMeshLoc_Z, DeltaTime, _descentSpeed);
			GetMesh()->SetRelativeLocation(FVector(currentMeshLoc.X, currentMeshLoc.Y, newZ));
		}
	}

	if (_bIsRevivalDie)
	{
		ChangeMatOpacity(DeltaTime);
	}
}
