// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAIController.h"
#include "RevivalAIController.generated.h"

DECLARE_MULTICAST_DELEGATE(FRevivalAIController_Delegate);

UENUM()
enum class ERevivalState {
	RS_SPAWN, // ���� ���� ����
	RS_IDLE, // ���� ��� ����
	RS_CANTAKEDAMAGE, // �ĸ��� �� �ִ� ����
	RS_REVIVAL, // ��Ȱ�� ����ϰ� �ִ� ����
	RS_LONG_RANGE_ATTACK, // ���Ÿ� ���� �ϰ� �ִ� ����
	RS_SHORT_RANGE_ATTACK // �ٰŸ� ���� �ϰ� �ִ� ����
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
	FTimerDelegate EndStandByDelegate; // ���� ��� �ð��� ���� �� ȣ��Ǵ� ��������Ʈ
	
private:
	FTimerHandle StandByTimer;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Instance, Meta = (AllowPrivateAccess = true))
		ERevivalState _currentBossState = ERevivalState::RS_SPAWN;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Instance, Meta = (AllowPrivateAccess = true))
		class ARevivalAI* OwningPawn; // �ش� ���� ��Ʈ�ѷ��� ���� ���� pawn

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Instance, Meta = (AllowPrivateAccess = true))
		class ARoxyCharacter* Target; // Ÿ�� (Roxy)

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Instance, Meta = (AllowPrivateAccess = true))
		float _targetDistance; // Ÿ��� ������ �Ÿ�

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Behaviors, Meta = (AllowPrivateAccess = true))
		float _playerRetentionTime = 0.f; // �÷��̾ ��ó�� �ӹ��� �ð�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StandBy, Meta = (AllowPrivateAccess = true))
		float _standByDuration_hp80below60over = 10.f; // 80���� 60 �ʰ� ü�� ������ ���� �ð�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StandBy, Meta = (AllowPrivateAccess = true))
		float _standByDuration_hp60below30over = 7.f; // 60���� 30 �ʰ� ü�� ������ ���� �ð�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StandBy, Meta = (AllowPrivateAccess = true))
		float _standByDuration_hp30below = 4.f; // 30�̸� ü�� ������ ���� �ð�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IDLE, Meta = (AllowPrivateAccess = true))
		bool _bIsStandBy = true; // ���� ��� ���������� üũ�ϴ� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Avoidance, Meta = (AllowPrivateAccess = true))
		float _avoidanceCooltime = 4.f; // ȸ�� �� �ð�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IDLE, Meta = (AllowPrivateAccess = true))
		bool _bIsAvoidanceCool = false; // ���� ��� ���������� üũ�ϴ� ����
};
