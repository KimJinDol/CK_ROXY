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
		class ANunuBossAI* OwningPawn; // �ش� ���� ��Ʈ�ѷ��� ���� ���� pawn

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Instance, Meta = (AllowPrivateAccess = true))
		class ARoxyCharacter* Target; // Ÿ�� (Roxy)

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Instance, Meta = (AllowPrivateAccess = true))
		float _targetDistance; // Ÿ��� ������ �Ÿ�

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Behaviors, Meta = (AllowPrivateAccess = true))
		float _playerRetentionTime = 0.f; // �÷��̾ ��ó�� �ӹ��� �ð�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IDLE, Meta = (AllowPrivateAccess = true))
		float _standByDuration = 4.f; // ������ ���� �ð�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IDLE, Meta = (AllowPrivateAccess = true))
		bool _bIsStandBy = true; // ���� ��� ���������� üũ�ϴ� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Avoidance, Meta = (AllowPrivateAccess = true))
		float _avoidanceCooltime = 4.f; // ȸ�� �� �ð�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IDLE, Meta = (AllowPrivateAccess = true))
		bool _bIsAvoidanceCool = false; // ���� ��� ���������� üũ�ϴ� ����

};
