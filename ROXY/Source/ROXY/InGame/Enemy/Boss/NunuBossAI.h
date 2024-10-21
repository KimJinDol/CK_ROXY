// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/EnemyAI.h"
#include "InGame/Enemy/Boss/BossProjectile.h"
#include "NunuBossAI.generated.h"

DECLARE_MULTICAST_DELEGATE(FBoss_Delegate);

UENUM()
enum class EPhaseState{ // ���� ���� ����� ��Ÿ���� ������
	PS_Phase1,
	PS_Phase2,
	PS_Phase3
};


UENUM(BlueprintType)
enum class EBossProjectileType : uint8 {
	BPT_Horizontal UMETA(DisplayName = "BPT_Horizontal"),
	BPT_Delay UMETA(DisplayName = "BPT_Delay"),
	BPT_Homing UMETA(DisplayName = "BPT_Homing"),
	BPT_Fake UMETA(DisplayName = "BPT_Fake"),
	BPT_Size UMETA(DisplayName = "BPT_Size")
};

/**
 * 
 */
UCLASS()
class ROXY_API ANunuBossAI : public AEnemyAI
{
	GENERATED_BODY()
	
public:
	ANunuBossAI();

protected:
	virtual void BeginPlay() override;
	virtual void Die() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


public:
	UFUNCTION()
	FVector GetFloorLocation();
	UFUNCTION()
	void SmoothTurnToTarget(float DeltaTime);
	UFUNCTION()
		void SetLookAtTarget(bool value) { _bTurnToTarget = value; }
	// ������ ���� �Լ�-------------------------------
	void ChangePhaseState(EPhaseState newState);
	EPhaseState GetPhaseState() { return _currentPhaseState; }
	void DoRise();
	void EndChangePhase();

	FBoss_Delegate OnEndRise;
	

	// ���� ���� �Լ� --------------------------------

	void EscapeStart();
	void EscapeFromPlayer();
	void EndEscape();

	FBoss_Delegate OnEndEscape;

	// ȸ�� ���� �Լ� ---------------------------------

	void AvoidanceStart();
	void EndAvoidance();

	FBoss_Delegate OnEndAvoidance;

	// ���� �߻� ���� �Լ�------------------------------
	void SelectRandomlyProjTypeAndShot(); // �������� �߻� Ÿ���� �����ϰ� �����Ų��
	ABossProjectile* SpawnProj(const FVector& spawnVec, const FRotator& targetRot); // �߻�ü�� ������Ų��
	UFUNCTION()
	EBossProjectileType GetProjectileState() { return _currentProjectileState; }

	/*���� �߻�*/
	void BeginShotProj_Horizontal(); // ���� �߻� ����
	void ShotProj_Horizontal(); // ���� �߻�ü �߻�

	/*���� �߻�*/
	void BeginShotProj_Fake(); // Fake �߻� ����
	void ShotProj_Fake(); // ��Fake�� �߻�ü �߻�

	/*������ �߻�*/
	void BeginShotProj_Delay();
	void ShotProj_Delay();

	/*���� �߻�*/
	void BeginShotProj_Homing();
	void ShotProj_Homing();

	void EndShotProjectile(); // � �߻� Ÿ���̵� �߻簡 ������ �� �Լ��� ȣ��.

	FBoss_Delegate OnEndShotProj;

	// ���� ���� ���� �Լ� -----------------------------
	void BeginRangeAttack();
	void RangeAttack();
	void RangeAttackCheck();

	// ���� ���� �Լ� ----------------------------------
	float GetTotalRapidDamage() { return _totalRapidDamage; };

	// ���ƴٴϴ� �� ���� ------------------------------
	UFUNCTION(BlueprintImplementableEvent)
	void FlyInBossZone();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimInstance, meta = (AllowPrivateAccess = true))
		class UBossAnimInstance* BossAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class ABossProjectile> BossProjClass;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PhaseState, Meta = (AllowPrivateAccess = true))
		EPhaseState _currentPhaseState = EPhaseState::PS_Phase1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		EBossProjectileType _currentProjectileState;


	// ----------- Boss Basic Movement ---------------
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = BasicMovement, Meta = (AllowPrivateAccess = true))
		bool _bTurnToTarget;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = BasicMovement, Meta = (AllowPrivateAccess = true))
		float _smoothTurnRate;

	// ���� ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Escape, Meta = (AllowPrivateAccess = true))
		bool _bCanEscape = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Escape, Meta = (AllowPrivateAccess = true))
		FVector _escapeLoc; // ������ ������ �� ��ġ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Escape, Meta = (AllowPrivateAccess = true))
		float _escapeSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Escape, Meta = (AllowPrivateAccess = true))
		float _escapeRandRadius;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Escape, Meta = (AllowPrivateAccess = true))
		float _escapeMinRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Escape, Meta = (AllowPrivateAccess = true))
		FVector _floorMeshLoc; // ������ �� �ִ� �ٴ��� ��ġ

	// ���� ȸ�� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Avoidance, Meta = (AllowPrivateAccess = true))
		bool _bCanAvoidance = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Avoidance, Meta = (AllowPrivateAccess = true))
		FVector _avoidanceLoc;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Avoidance, Meta = (AllowPrivateAccess = true))
		float _avoidanceSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Avoidance, Meta = (AllowPrivateAccess = true))
		float _avoidanceDistance;

	// ����� ��� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BossLocation, Meta = (AllowPrivateAccess = true))
		float _riseHeight_Fhase1 = 0.f; // ������1, ������ ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BossLocation, Meta = (AllowPrivateAccess = true))
		float _riseHeight_Fhase2 = 500.f; // ������2, ������ ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BossLocation, Meta = (AllowPrivateAccess = true))
		float _riseHeight_Fhase3 = 800.f; // ������3, ������ ��� ����

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = BossLocation, Meta = (AllowPrivateAccess = true))
		float _riseZLocation; // ������ ��³��̸� ����, ���� Z �� ��ġ��

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = BossLocation, Meta = (AllowPrivateAccess = true))
		bool _bOnLerpZLocation = false;

	// �߻� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
	int32 _recreateProjCount = 0;
	/*Horizontal �߻�----------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile_Horizontal, Meta = (AllowPrivateAccess = true))
	int32 _projNum_Horizontal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile_Horizontal, Meta = (AllowPrivateAccess = true))
	int32 _projAnglaGap_Horizontal;
	/*Fake �߻�----------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
	int32 _recreateProjMax_Fake;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
	int32 _projNum_Fake;
	/*Delay �߻�-------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
	int32 _recreateProjMax_Delay;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
	int32 _projNum_Delay;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
	int32 _proj_Y_AngleGap_Delay;
	/*Homing �߻�------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
	int32 _recreateProjMax_Homing;


	// ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RangeAttack, Meta = (AllowPrivateAccess = true))
	float _rangeAttackRadius = 400.f;

	// ���� ���� �Լ�---------------------------------
	float _rapidDamageCheckDuration = 3.f; // ���� üũ�� �� �ð�
	float _totalRapidDamage = 0.f; // rapidDamageCheckDuration ���� ���� �� ��������
};
