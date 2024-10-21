// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/EnemyController.h"
#include "Boss_AIController.generated.h"

UENUM()
enum class EBossState {
	BS_IDLE,
	BS_CHANGE_PHASE,
	BS_ESCAPE,
	BS_AVOIDANCE,
	BS_LONG_RANGE_ATTACK,
	BS_SHORT_RANGE_ATTACK
	};

/**
 * 
 */
UCLASS()
class ROXY_API ABoss_AIController : public AEnemyController
{
	GENERATED_BODY()

public:
	ABoss_AIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

private:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void SetBossState(EBossState newState = EBossState::BS_IDLE);



public:
	FTimerDelegate EndStandByDelegate;
	FTimerDelegate AvoidanceDelegate;
private:
	FTimerHandle StandByTimer;
	FTimerHandle AvoidanceTimer;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Instance, Meta = (AllowPrivateAccess = true))
		EBossState _currentBossState = EBossState::BS_IDLE;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Instance, Meta = (AllowPrivateAccess = true))
		class ANunuBossAI* OwningPawn; // 해당 보스 컨트롤러가 조종 중인 pawn

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Instance, Meta = (AllowPrivateAccess = true))
		class ARoxyCharacter* Target; // 타깃 (Roxy)

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Instance, Meta = (AllowPrivateAccess = true))
		float _targetDistance; // 타깃과 보스의 거리

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Behaviors, Meta = (AllowPrivateAccess = true))
		float _playerRetentionTime = 0.f; // 플레이어가 근처에 머무른 시간

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IDLE, Meta = (AllowPrivateAccess = true))
		float _standByDuration = 4.f; // 대기상태 지속 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IDLE, Meta = (AllowPrivateAccess = true))
		bool _bIsStandBy = true; // 현재 대기 상태인지를 체크하는 변수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Avoidance, Meta = (AllowPrivateAccess = true))
		float _avoidanceCooltime = 4.f; // 회피 쿨 시간

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IDLE, Meta = (AllowPrivateAccess = true))
		bool _bIsAvoidanceCool = false; // 현재 대기 상태인지를 체크하는 변수

};
