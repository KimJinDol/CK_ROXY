// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_AIController.h"
#include "InGame/System/NunuInGameManager.h"
#include "InGame/Enemy/Boss/NunuBossAI.h"
#include "InGame/Component/HpComponent.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"


ABoss_AIController::ABoss_AIController()
{

}

void ABoss_AIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	EndStandByDelegate.BindLambda([this]()->void {_bIsStandBy = false; });
	AvoidanceDelegate.BindLambda([this]()->void { _bIsAvoidanceCool = false; });
	
}

void ABoss_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void ABoss_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어 쪽으로 몸을 회전 시킨다
	if (OwningPawn == nullptr) return;

	/*------------------------- FSM --------------------------*/
	_targetDistance = FVector::Dist2D(Target->GetActorLocation(), GetPawn()->GetActorLocation());
	_targetDistance < 500.f ? _playerRetentionTime += DeltaTime : _playerRetentionTime = 0.f;

	switch (OwningPawn->GetPhaseState()) {
	case EPhaseState::PS_Phase1:
		// 플레이어 캐릭터가 일정 범위 내에 n 초 동안 머물렀을 시 도주키 true
		if (_currentBossState == EBossState::BS_IDLE)
		{

			if (OwningPawn->GetHpComponent()->GetHpRatio() < 0.8f 
				&& OwningPawn->GetHpComponent()->GetHpRatio() > 0.3f
				&& !_bIsStandBy) {
				SetBossState(EBossState::BS_CHANGE_PHASE);
				OwningPawn->ChangePhaseState(EPhaseState::PS_Phase2);
			}
			else if (_playerRetentionTime > 7.f && !_bIsStandBy) {
				SetBossState(EBossState::BS_ESCAPE);
			}
			else if (Target->GetIsDealing() && !_bIsAvoidanceCool && _targetDistance < 400.f && _bIsStandBy) {
				SetBossState(EBossState::BS_AVOIDANCE);
			}
			else if (!_bIsStandBy) {
				SetBossState(EBossState::BS_LONG_RANGE_ATTACK);
			}
		}
		break;
	case EPhaseState::PS_Phase2:
		if (_currentBossState == EBossState::BS_IDLE)
		{
			if (OwningPawn->GetHpComponent()->GetHpRatio() < 0.3f && !_bIsStandBy)
				OwningPawn->ChangePhaseState(EPhaseState::PS_Phase3);
			else if (OwningPawn->GetTotalRapidDamage() >= 10.f && _playerRetentionTime < 4.f && !_bIsStandBy)
				SetBossState(EBossState::BS_ESCAPE);
			else if (Target->GetIsDealing() && !_bIsAvoidanceCool && _targetDistance < 400.f && _bIsStandBy)
				SetBossState(EBossState::BS_AVOIDANCE);
			else if (_playerRetentionTime > 3.f && !_bIsStandBy)
				SetBossState(EBossState::BS_SHORT_RANGE_ATTACK);
			else if (!_bIsStandBy)
				SetBossState(EBossState::BS_LONG_RANGE_ATTACK);
		}
		break;
	}
	/*-----------------------------------------------------------*/
}

void ABoss_AIController::BeginPlay()
{
	Super::BeginPlay();

	CHECK(InGameManager);
	Target = InGameManager->GetPlayerCharacter();

	OwningPawn = Cast<ANunuBossAI>(GetPawn());
	CHECK(OwningPawn);

	OwningPawn->OnEndEscape.AddUFunction(this, "SetBossState", EBossState::BS_IDLE);
	OwningPawn->OnEndAvoidance.AddUFunction(this, "SetBossState", EBossState::BS_IDLE);
	OwningPawn->OnEndShotProj.AddUFunction(this, "SetBossState", EBossState::BS_IDLE);
	OwningPawn->OnEndRise.AddUFunction(this, "SetBossState", EBossState::BS_IDLE);

	SetBossState();
}

void ABoss_AIController::SetBossState(EBossState newState /*= EBossState::BS_IDLE*/)
{
	switch (newState) {
	case EBossState::BS_IDLE:
		_bIsStandBy = true;
		_currentBossState = EBossState::BS_IDLE;
		OwningPawn->SetLookAtTarget(true);
		GetWorldTimerManager().SetTimer(StandByTimer, EndStandByDelegate, _standByDuration, false);
		break;
	case EBossState::BS_CHANGE_PHASE:
		_currentBossState = EBossState::BS_CHANGE_PHASE;
		SetBossState(EBossState::BS_IDLE);
		break;
	case EBossState::BS_ESCAPE:
		_currentBossState = EBossState::BS_ESCAPE;
		OwningPawn->EscapeStart(); 
		break;
	case EBossState::BS_AVOIDANCE:
		_bIsAvoidanceCool = true;
		_currentBossState = EBossState::BS_AVOIDANCE;
		OwningPawn->AvoidanceStart();
		GetWorldTimerManager().SetTimer(AvoidanceTimer, AvoidanceDelegate, _avoidanceCooltime, false);
		break;
	case EBossState::BS_LONG_RANGE_ATTACK:
		_currentBossState = EBossState::BS_LONG_RANGE_ATTACK;
		OwningPawn->SelectRandomlyProjTypeAndShot();
		break;
	case EBossState::BS_SHORT_RANGE_ATTACK:
		_currentBossState = EBossState::BS_SHORT_RANGE_ATTACK;

		// 원거리 공격들과 범위 공격중에서 랜덤으로 실행
		int32 randNum = FMath::RandHelper((int32)EBossProjectileType::BPT_Size);
		if (randNum == 0) {
			OwningPawn->BeginRangeAttack();
		}
		else {
			OwningPawn->SelectRandomlyProjTypeAndShot();
		}
		break;
	 }
}
