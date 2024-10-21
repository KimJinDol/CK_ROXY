// Fill out your copyright notice in the Description page of Project Settings.


#include "NunuBossAI.h"
#include "InGame/Enemy/Boss/Boss_AIController.h"
#include "InGame/Enemy/Boss/BossAnimInstance.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Component/HpComponent.h"
#include "InGame/Enemy/Boss/BossProjectile.h"
#include "InGame/DataTable/BossDataTable.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

ANunuBossAI::ANunuBossAI()
{
	PrimaryActorTick.bCanEverTick = true;


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_REVIVAL(TEXT("/Game/Resources/Enemy/Grunt/Wizard_Undead/Mesh/SK_Wizard_Undead.SK_Wizard_Undead"));
	if(SK_REVIVAL.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_REVIVAL.Object);
	}

	//static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_BOSS(TEXT("/Game/Resources/Enemy/Boss/BP_BossAnim.BP_BossAnim_C"));
	//if (ANIM_BOSS.Succeeded())
	//{
	//	GetMesh()->SetAnimInstanceClass(ANIM_BOSS.Class);
	//}

	//static ConstructorHelpers::FObjectFinder<UMaterial> MAT_ORIGIN(TEXT("/Game/Resources/Enemy/Boss/Materials/M_Boss_Body.M_Boss_Body"));
	//if (MAT_ORIGIN.Succeeded())
	//{
	//	OriginalMat = MAT_ORIGIN.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UMaterialInstance> MAT_OPACITY(TEXT("/Game/Resources/Enemy/Boss/Materials/M_Boss_Body_Inst.M_Boss_Body_Inst"));
	//if (MAT_OPACITY.Succeeded())
	//{
	//	OpacityMat = MAT_OPACITY.Object;
	//}


	static ConstructorHelpers::FObjectFinder<UDataTable> DT_BOSS(TEXT("/Game/DataTable/DT_Boss.DT_Boss"));
	if (DT_BOSS.Succeeded())
	{
		DataTable = DT_BOSS.Object;
	}

	GetCapsuleComponent()->SetCapsuleSize(120.f, 280.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -240.f));

	GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Walking;

 	AIControllerClass = ABoss_AIController::StaticClass();
 	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	BossProjClass = ABossProjectile::StaticClass();

}

void ANunuBossAI::BeginPlay()
{
	Super::BeginPlay();

	HpComponent->SetMaxHp(100.f);
	ChangePhaseState(EPhaseState::PS_Phase1);


	FHitResult hitResult;
	FVector startVec(GetActorLocation());
	startVec.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector endVec(GetActorLocation());
	endVec.Z -= 10000.f;
	FCollisionQueryParams params(NAME_None, false, this);
	bool result = GetWorld()->LineTraceSingleByChannel(hitResult, startVec, endVec, ECollisionChannel::ECC_Visibility, params);

	if (result && hitResult.bBlockingHit) {
		if (hitResult.GetComponent() != nullptr)
			_floorMeshLoc = hitResult.GetComponent()->GetComponentLocation();
	}
}

void ANunuBossAI::Die()
{
	Super::Die();
}

void ANunuBossAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BossAnim = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());
	CHECK(BossAnim);
	BossAnim->OnRiseStart.BindUObject(this, &ANunuBossAI::DoRise);
	BossAnim->OnEndEscapeStartAnim.BindUObject(this, &ANunuBossAI::EscapeFromPlayer);
	BossAnim->OnEndEscape.BindUObject(this, &ANunuBossAI::EndEscape);
	BossAnim->OnShotProj_Horizontal.BindUObject(this, &ANunuBossAI::ShotProj_Horizontal);
	BossAnim->OnShotProj_Fake.BindUObject(this, &ANunuBossAI::ShotProj_Fake);
	BossAnim->OnShotProj_Delay.BindUObject(this, &ANunuBossAI::ShotProj_Delay);
	BossAnim->OnShotProj_Homing.BindUObject(this, &ANunuBossAI::ShotProj_Homing);
	BossAnim->OnEndShotProj.BindUObject(this, &ANunuBossAI::EndShotProjectile);
	BossAnim->OnRangeAttackCheck.BindUObject(this, &ANunuBossAI::RangeAttackCheck);


	if (DataTable)
	{
		FBossStatTableRow* statTableRow = DataTable->FindRow<FBossStatTableRow>(TEXT("Boss"), FString("Boss Data Table is not exist!"));

		if (statTableRow)
		{
			HpComponent->SetMaxHp(statTableRow->maxHp);
			HpComponent->SetHP(HpComponent->GetMaxHp());
			_smoothTurnRate = statTableRow->smoothTurnRate;
			_escapeSpeed = statTableRow->escapeSpeed;
			_escapeRandRadius = statTableRow->escapeRandRadius;
			_escapeMinRange = statTableRow->escapeMinRange;
			_avoidanceSpeed = statTableRow->avoidanceSpeed;
			_avoidanceDistance = statTableRow->avoidanceDistance;
			_projNum_Horizontal = statTableRow->ProjNum_Horizontal;
			_projAnglaGap_Horizontal = statTableRow->ProjAngleGap_Horizontal;
			_recreateProjMax_Delay = statTableRow->recreateProjNum_Delay;
			_projNum_Delay = statTableRow->ProjNum_Delay;
			_proj_Y_AngleGap_Delay = statTableRow->Proj_Y_AngleGap_Delay;
			_recreateProjMax_Homing = statTableRow->recreateProjNum_Homing;
			_recreateProjMax_Fake = statTableRow->recreateProjnum_Fake;
			_projNum_Fake = statTableRow->ProjNum_Fake;
			LOG(Warning, "--------------Boss Stat--------------");
			LOG(Warning, "Max Hp : %f", HpComponent->GetMaxHp());
			LOG(Warning, "Smooth Turn Rate : %f", _smoothTurnRate);
			LOG(Warning, "Escape Speed : %f", _escapeSpeed);
			LOG(Warning, "Escape Random Circla Radius : %f", _escapeRandRadius);
			LOG(Warning, "Escape Min Range : %f", _escapeMinRange);
			LOG(Warning, "Avoidance Speed : %f", _avoidanceSpeed);
			LOG(Warning, "Avoidance Distance : %f", _avoidanceDistance);
			LOG(Warning, "Horizontal Proj Num : %f", _projNum_Horizontal);
			LOG(Warning, "Delay Recreate Proj Num: %f", _recreateProjMax_Delay);
			LOG(Warning, "Delay Proj Num : %f", _projNum_Delay);
			LOG(Warning, "Delay Proj Y Angle Gap : %f", _proj_Y_AngleGap_Delay);
			LOG(Warning, "Homing Recreate Proj Num : %f", _recreateProjMax_Homing);
			LOG(Warning, "Fake Recreate Proj Num : %f", _recreateProjMax_Fake);
			LOG(Warning, "Fake Proj Num : %f", _projNum_Fake);
			LOG(Warning, "------------------------------------")
		}
	}

}

void ANunuBossAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_bTurnToTarget) {
		SmoothTurnToTarget(DeltaTime);
	}


	if (_bOnLerpZLocation) // �׷α� ������ ȣ���ϰų� ������ �ٲ� �� ȣ���ϸ� �ȴ�
	{
		if (!FMath::IsNearlyEqual(GetActorLocation().Z, _riseZLocation, 5.f))
		{
			float newZLoc = UKismetMathLibrary::FInterpTo(GetActorLocation().Z, _riseZLocation, DeltaTime, 3.f);
			FVector newVec = GetActorLocation();
			newVec.Z = newZLoc;
			SetActorLocation(newVec);
		}
		else {
			_bOnLerpZLocation = false;
			EndChangePhase(); // ��� �۾��� ���� ������ ��Ʈ�ѷ����� �˷� FSM ���¸� '���' ���·� �����
		}
	}

	if (_bCanEscape)
	{
		FVector newLoc = UKismetMathLibrary::VInterpTo(GetActorLocation(), _escapeLoc, DeltaTime, _escapeSpeed);
		SetActorLocation(newLoc);
		float distanceNewLoc = FVector::Dist2D(GetActorLocation(), _escapeLoc);
		if (distanceNewLoc < KINDA_SMALL_NUMBER)
		{
			_bCanEscape = false;
			EndEscape();
		}
	}
	if (_bCanAvoidance)
	{
		FVector newLoc = UKismetMathLibrary::VInterpTo(GetActorLocation(), _avoidanceLoc, DeltaTime, _avoidanceSpeed);
		SetActorLocation(newLoc);
		float distanceNewLoc = FVector::Dist2D(GetActorLocation(), _avoidanceLoc);
		if (distanceNewLoc < KINDA_SMALL_NUMBER)
		{
			_bCanAvoidance = false;
			EndAvoidance();
		}
	}


}

float ANunuBossAI::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// ���� ��� ------------------------------
	// TakeDamage ȣ��� �� totalRapidDamage �� ������ ��������ŭ 
	// ���� �ð��� �帣�� �ٽ� ���ش�
	FTimerHandle rapidDamagedHandle;
	FTimerDelegate rapidDamageDelegate;
	rapidDamageDelegate.BindLambda([this](float damage) {
		_totalRapidDamage -= damage;
		LOG(Warning, "- damage : %f, total : %f", damage, _totalRapidDamage);
	}, DamageAmount);
	GetWorld()->GetTimerManager().SetTimer(rapidDamagedHandle, rapidDamageDelegate, _rapidDamageCheckDuration, false);
	//----------------------------------------

	HpComponent->TakeDamage(DamageAmount);
	_totalRapidDamage += DamageAmount;
	LOG(Warning, "+ damage : %f, total : %f", DamageAmount, _totalRapidDamage);
	return HpComponent->GetHp();
}

FVector ANunuBossAI::GetFloorLocation()
{
	FHitResult hitResult;
	FVector startVec(GetActorLocation());
	startVec.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector endVec(GetActorLocation());
	endVec.Z -= 100000.f;
	FCollisionQueryParams params(NAME_None, false, this);
	bool result = GetWorld()->LineTraceSingleByChannel(hitResult, startVec, endVec, ECollisionChannel::ECC_Visibility, params);

	if (result) {
		return hitResult.Location;
	}
	else {
		LOG(Error, "Boss is Not Exist On Floor!");
		return FVector::ZeroVector;
	}
}


void ANunuBossAI::SmoothTurnToTarget(float DeltaTime)
{
	FRotator lookAtTargetRot(GetActorRotation());
	lookAtTargetRot.Yaw = UKismetMathLibrary::MakeRotFromX(InGameManager->GetPlayerCharacter()->GetActorLocation() - GetActorLocation()).Yaw;

	FRotator newRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), lookAtTargetRot, DeltaTime, _smoothTurnRate);
	SetActorRotation(newRot);
}

void ANunuBossAI::ChangePhaseState(EPhaseState newState)
{
	FVector floorLocation = GetFloorLocation();

	switch (newState) {
	case EPhaseState::PS_Phase1:
		_currentPhaseState = EPhaseState::PS_Phase1;
		// ������ 1���� ��� ������ ��ų�� ����
		break;
	case EPhaseState::PS_Phase2:
		_currentPhaseState = EPhaseState::PS_Phase2;
		// ������ 2���� ��� ������ ��ų�� ����
		break;
	case EPhaseState::PS_Phase3:
		_currentPhaseState = EPhaseState::PS_Phase3;
		// ���� ����
		// ������ 3���� ��� ������ ��ų�� ����
		break;
	}
	LOG_SCREEN("Current Boss Phase : %d", (int32)_currentPhaseState + 1);
}

void ANunuBossAI::DoRise()
{
	_bOnLerpZLocation = true;
}

void ANunuBossAI::EndChangePhase()
{
	BossAnim->bIsRise = false;
	OnEndRise.Broadcast();
}

void ANunuBossAI::EscapeStart()
{
	BossAnim->bIsEscapeFromPlayer = true;
}

void ANunuBossAI::EscapeFromPlayer()
{
	LOG_SCREEN("Escape");
	FVector2D escapeLoc2D = FMath::RandPointInCircle(_escapeRandRadius);
	escapeLoc2D.X += _floorMeshLoc.X;
	escapeLoc2D.Y += _floorMeshLoc.Y;
	_escapeLoc = FVector(escapeLoc2D.X, escapeLoc2D.Y, GetActorLocation().Z);
	
	float distance = FVector::Distance(GetActorLocation(), _escapeLoc);
	if (distance < _escapeMinRange)
	{
		EscapeFromPlayer(); // ���� ���� ������ �ɸ���...?
	}
	else
		_bCanEscape = true;
}

void ANunuBossAI::EndEscape()
{
	OnEndEscape.Broadcast();
}

void ANunuBossAI::AvoidanceStart()
{
	FHitResult hitResult;
	FVector startVec(GetActorLocation());
	FVector endVec(startVec + GetActorForwardVector() * -10000.f);
	FCollisionQueryParams params(NAME_None, false, this);
	bool result = GetWorld()->LineTraceSingleByChannel(hitResult, startVec, endVec, ECollisionChannel::ECC_Visibility, params);

	bool bIsBlocking = true;
	if (result) {
		if (hitResult.GetComponent() != nullptr) {
			if (FVector::Distance(startVec, hitResult.Location) > 500.f)
				bIsBlocking = false;
		}
		else if (hitResult.GetActor() != nullptr) {
			if (FVector::Distance(startVec, hitResult.Location) > 500.f)
				bIsBlocking = false;
		}
	}

/*	if (!bIsBlocking) {*/
	LOG_SCREEN("Success Avoidance");
		BossAnim->bIsAvoidance = true;
		_avoidanceLoc = GetActorLocation() + (GetActorForwardVector() * -_avoidanceDistance * -1.f);
		_bCanAvoidance = true;
// 	}
// 	else {
// 	LOG_SCREEN("Failed Avoidance");
// 		EndAvoidance();
// 	}
}


void ANunuBossAI::EndAvoidance()
{
	OnEndAvoidance.Broadcast();
}

void ANunuBossAI::SelectRandomlyProjTypeAndShot()
{
	EBossProjectileType projectileType = EBossProjectileType::BPT_Horizontal;

	if (_currentPhaseState == EPhaseState::PS_Phase1)
		projectileType = (EBossProjectileType)(FMath::RandHelper((int32)EBossProjectileType::BPT_Size - 1));
	else if (_currentPhaseState == EPhaseState::PS_Phase2)
		projectileType = (EBossProjectileType)(FMath::RandHelper((int32)EBossProjectileType::BPT_Size));


	switch (projectileType) {
	case EBossProjectileType::BPT_Horizontal:
		BeginShotProj_Horizontal();
		_currentProjectileState = EBossProjectileType::BPT_Horizontal;
#if ENABLE_DRAW_DEBUG
		LOG_SCREEN("Boss behavior : Horizontal shot");
#endif
		break;
	case EBossProjectileType::BPT_Delay:
		BeginShotProj_Delay();
		_currentProjectileState = EBossProjectileType::BPT_Delay;
#if ENABLE_DRAW_DEBUG
		LOG_SCREEN("Boss behavior : Delay shot");
#endif
		break;
	case EBossProjectileType::BPT_Homing:
		BeginShotProj_Homing();
		_currentProjectileState = EBossProjectileType::BPT_Homing;
#if ENABLE_DRAW_DEBUG
		LOG_SCREEN("Boss behavior : Homing shot");
#endif
		break;
	case EBossProjectileType::BPT_Fake:
		BeginShotProj_Fake();
		_currentProjectileState = EBossProjectileType::BPT_Fake;
#if ENABLE_DRAW_DEBUG
		LOG_SCREEN("Boss behavior : Fake shot");
#endif
		break;
	}

	BossAnim->bIsShotProjectile = true;
	_bTurnToTarget = false;
}

ABossProjectile* ANunuBossAI::SpawnProj(const FVector& spawnVec, const FRotator& targetRot)
{
	return GetWorld()->SpawnActor<ABossProjectile>(BossProjClass, spawnVec, targetRot);
}

void ANunuBossAI::BeginShotProj_Horizontal()
{
	BossAnim->bIsShotProjectile_Horizontal = true;
}

void ANunuBossAI::ShotProj_Horizontal()
{
	_bTurnToTarget = false;

	FVector spawnVec(GetActorLocation() + (GetActorForwardVector() * 300.f)); // �߻�ü�� ���� �� ��ġ. ���� ���� �տ��� �����Ѵ�.
	AActor* target = InGameManager->GetPlayerCharacter();
	FVector targetLoc = target->GetActorLocation(); // Ÿ���� ��ġ�� �޴´�.
	FVector lookAtTargetVec(targetLoc - GetActorLocation());
	lookAtTargetVec.Z = 0.f;
	FRotator projectileRot;
	FVector axisVector(0, 0, 1); // ȸ�� ���� �� ����	
	float initAngle = -_projAnglaGap_Horizontal * (_projNum_Horizontal / 2 + 1); // ���� 7�� ���� ������ �ǹ��Ѵ�. Ÿ�� �������� �� �� ������ ������ �߻�ü�� �߻��ϱ� ���ؼ� 2�� ������ 1�� ���Ѵ�.(for ������ angle Gap��ŭ �����ֱ� ����)

	for (int32 i = 0; i < _projNum_Horizontal; i++) {
		initAngle += _projAnglaGap_Horizontal;
		projectileRot = lookAtTargetVec.RotateAngleAxis(initAngle, axisVector).Rotation(); // �߻�ü�� ȸ�� ��Ų��.
		projectileRot.Pitch -= 5.f;
		ABossProjectile* projectile = SpawnProj(spawnVec, projectileRot);
		projectile->SetActiveAfterDelay(1.2f, 3000.f);
	}
}

void ANunuBossAI::BeginShotProj_Fake()
{
	BossAnim->bIsShotProjectile_Fake = true;
}

void ANunuBossAI::ShotProj_Fake()
{
	_bTurnToTarget = false;

	_recreateProjCount++;

	FVector floorLoc = GetFloorLocation(); // ���� �� �ִ� �ٴ��� ��ġ���� �����´�.
	FVector spawnForwardLoc(GetActorLocation() + (GetActorForwardVector() * 300.f)); // forwardVector ���͸� �������� ���� �� ����.
	ARoxyCharacter* target = InGameManager->GetPlayerCharacter();
	FVector targetLoc = target->GetActorLocation();


	FVector projectileRotVec((targetLoc - GetActorLocation()).X, (targetLoc - GetActorLocation()).Y, 0); // 2���� ����ó�� �߻��ϱ� ���� Z���� 0���� �ΰ� Ÿ���� �ٶ󺸴� ���� ���͸� ���Ѵ� 
	FRotator projectileRot = projectileRotVec.Rotation();
	float spawnHeightGap = 150.f; // �߻�ü ������ ��
	float initHeight = floorLoc.Z; // �ٴ��� �������� ��

	FVector spawnLoc(spawnForwardLoc.X, spawnForwardLoc.Y, initHeight);

	for (int32 i = 0; i < _projNum_Fake; i++)
	{
		spawnLoc.Z += spawnHeightGap; // �߻�ü ���� ����ŭ ���������� ��.
		ABossProjectile* projectile = SpawnProj(spawnLoc, projectileRot);
		projectile->SetActiveAfterDelay(1.2f, 5000.f);

		// �ӵ��� Ȱ��ȭ ���� �� Ÿ���� �ٶ󺸴� �������� rotation �� �ٲ��ش�
		projectile->EndDelayActive.BindLambda([this](ARoxyCharacter* target, AActor* projectile, int32 recreateProjCount, int32 recreateMax) {
			FVector projRotVec = (target->GetActorLocation() - projectile->GetActorLocation());
			projRotVec.Z = 0.f;

			if (recreateProjCount == recreateMax && target->GetCurrentSpeed() != 0) {
				projRotVec += target->GetActorForwardVector() * 400.f;
			}

			FRotator projRot = projRotVec.Rotation();
			projectile->SetActorRotation(projRot);
		}, target, projectile, _recreateProjCount, _recreateProjMax_Fake);
	}

	// �� �߻� Ƚ���� �����Ǹ� ��ų�� ������.
	if (_recreateProjCount >= _recreateProjMax_Fake) {
		_recreateProjCount = 0;
		BossAnim->AnimNotify_EndShotProj(); // �߻� ������ ���� ������ �ش� ��ų�� ������.
	}
	else {
		FTimerHandle delayHandle;
		FTimerDelegate delayDelegate;
		delayDelegate.BindUObject(this, &ANunuBossAI::ShotProj_Fake);
		float recreateDuration = 0.5;
		GetWorld()->GetTimerManager().SetTimer(delayHandle, delayDelegate, recreateDuration, false);
	}
}

void ANunuBossAI::BeginShotProj_Delay()
{
	BossAnim->bIsShotProjectile_Delay = true;
}

void ANunuBossAI::ShotProj_Delay()
{
	_bTurnToTarget = false;

	_recreateProjCount++;

	FVector floorLoc = GetFloorLocation();

	float Y_angleGap = _proj_Y_AngleGap_Delay;
	Y_angleGap *= (float)_recreateProjCount; // �߻�ü ������ø��� ������ Pitch ������ ������ ���̹Ƿ� ����� Ƚ���� ���Ѵ�

	float Z_angleGap = 360 / _projNum_Delay;

	FVector projectileFirstLoc(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 900.f);
	FRotator projectileRot = GetActorForwardVector().Rotation();
	float standardAngle = -30.f; // pitch �� ������ ������ �� ����
	projectileRot = projectileRot.Add(standardAngle + Y_angleGap, -Z_angleGap, 0); // pitch ���� ������ pitch ���� �������� ���Ѵ�

	FVector projectileLoc;

	for (int32 i = 0; i < _projNum_Delay; i++) {
		projectileRot = projectileRot.Add(0, Z_angleGap, 0);
		projectileLoc = projectileFirstLoc + (projectileRot.Vector() * 400.f);
		projectileLoc.Z = projectileFirstLoc.Z;
		ABossProjectile* projectile = SpawnProj(projectileLoc, projectileRot);
		projectile->SetVelocity(1.0f);
		projectile->SetAccelerate(true);
	}

	if (_recreateProjCount == _recreateProjMax_Delay) {
		_recreateProjCount = 0;
		BossAnim->AnimNotify_EndShotProj(); // �߻� ������ ���� ������ �ش� ��ų�� ������.
	}
	else {
		FTimerHandle delayHandle;
		FTimerDelegate delayDelegate;
		delayDelegate.BindUObject(this, &ANunuBossAI::ShotProj_Delay);
		float delayDuration = 0.5;
		GetWorld()->GetTimerManager().SetTimer(delayHandle, delayDelegate, delayDuration, false);
	}
}


void ANunuBossAI::BeginShotProj_Homing()
{
	BossAnim->bIsShotProjectile_Homing = true;
}

void ANunuBossAI::ShotProj_Homing()
{
	_bTurnToTarget = false;

	_recreateProjCount++;

	FVector spawnForwardLoc = GetActorLocation() + (GetActorForwardVector() * 300.f);
	AActor* target = InGameManager->GetPlayerCharacter();
	FVector targetLoc = target->GetActorLocation();
	FVector lookatTargetVec(targetLoc - GetActorLocation());
	FRotator projectileRot = lookatTargetVec.Rotation();

	ABossProjectile* projectile = SpawnProj(spawnForwardLoc, projectileRot);
	projectile->SetVelocity(1.5f);
	projectile->SetAccelerate(true);

	if (_recreateProjCount >= _recreateProjMax_Homing) {
		_recreateProjCount = 0;
		BossAnim->AnimNotify_EndShotProj(); // �߻� ������ ���� ������ �ش� ��ų�� ������.
	}
	else {
		FTimerHandle delayHandle;
		FTimerDelegate delayDelegate;
		delayDelegate.BindUObject(this, &ANunuBossAI::ShotProj_Homing);
		float delayDuration = 0.5;
		GetWorld()->GetTimerManager().SetTimer(delayHandle, delayDelegate, delayDuration, false);
	}
}

void ANunuBossAI::EndShotProjectile()
{
	OnEndShotProj.Broadcast();
	_bTurnToTarget = true;
}

void ANunuBossAI::BeginRangeAttack()
{
	BossAnim->bIsRangeAttack = true;
}

void ANunuBossAI::RangeAttack()
{
	_bTurnToTarget = false;
	// ���� ����Ʈ ������ ��. ������ ���ҽ��� ����
}

void ANunuBossAI::RangeAttackCheck()
{
	FHitResult hitResult;
	FCollisionQueryParams param;
	FVector start = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 10000.f);
	FVector end = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 10000.f);
	bool result = GetWorld()->SweepSingleByChannel(
		hitResult,
		start,
		end,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeCapsule(_rangeAttackRadius, 10000.f),
		param);

	DrawDebugCapsule(GetWorld(), GetActorLocation(), 10000.f, _rangeAttackRadius, FQuat::Identity, FColor::Purple, false, 2.f, (uint8)'\000', 5.f);

	if (result)
	{
		ARoxyCharacter* player = Cast<ARoxyCharacter>(hitResult.GetActor());
		if (player)
		{
#if ENABLE_DRAW_DEBUG
			LOG_SCREEN("Boss Range Attack is Hit Player");
#endif
		}
	}
}