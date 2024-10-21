// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "RevivalAIController.generated.h"

DECLARE_MULTICAST_DELEGATE(FRevivalAIController_Delegate);

UENUM()
enum class ERevivalState {
	RS_SPAWN, // 스폰 중인 상태
	RS_IDLE, // 무적 대기 상태
	RS_CANTAKEDAMAGE, // 쳐맞을 수 있는 상태
	RS_REVIVAL, // 부활을 사용하고 있는 상태
	RS_LONG_RANGE_ATTACK, // 원거리 공격 하고 있는 상태
	RS_SHORT_RANGE_ATTACK // 근거리 공격 하고 있는 상태
};
/**
 * 
 */
UCLASS()
class ROXY_API ARevivalAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ARevivalAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

private:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void SetBossState(ERevivalState newState = ERevivalState::RS_IDLE);

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = InGameManager, Meta = (AllowPrivateAccess = true))
		class ANunuInGameManager* InGameManager;

public:
	FTimerDelegate EndStandByDelegate; // 무적 대기 시간이 끝날 때 호출되는 델리게이트
	
private:
	FTimerHandle StandByTimer;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Instance, Meta = (AllowPrivateAccess = true))
		ERevivalState _currentBossState = ERevivalState::RS_SPAWN;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Instance, Meta = (AllowPrivateAccess = true))
		class ARevivalAI* OwningPawn; // 해당 보스 컨트롤러가 조종 중인 pawn

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Instance, Meta = (AllowPrivateAccess = true))
		class ARoxyCharacter* Target; // 타깃 (Roxy)

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Instance, Meta = (AllowPrivateAccess = true))
		float _targetDistance; // 타깃과 보스의 거리

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Behaviors, Meta = (AllowPrivateAccess = true))
		float _playerRetentionTime = 0.f; // 플레이어가 근처에 머무른 시간

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StandBy, Meta = (AllowPrivateAccess = true))
		float _standByDuration_hp80below60over = 10.f; // 80이하 60 초과 체력 대기상태 지속 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StandBy, Meta = (AllowPrivateAccess = true))
		float _standByDuration_hp60below30over = 7.f; // 60이하 30 초과 체력 대기상태 지속 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StandBy, Meta = (AllowPrivateAccess = true))
		float _standByDuration_hp30below = 4.f; // 30미만 체력 대기상태 지속 시간

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IDLE, Meta = (AllowPrivateAccess = true))
		bool _bIsStandBy = true; // 현재 대기 상태인지를 체크하는 변수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Avoidance, Meta = (AllowPrivateAccess = true))
		float _avoidanceCooltime = 4.f; // 회피 쿨 시간

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IDLE, Meta = (AllowPrivateAccess = true))
		bool _bIsAvoidanceCool = false; // 현재 대기 상태인지를 체크하는 변수
};
