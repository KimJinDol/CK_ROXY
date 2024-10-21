// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/EnemyAI.h"
#include "InGame/Enemy/Boss/BossProjectile.h"
#include "NunuBossAI.generated.h"

DECLARE_MULTICAST_DELEGATE(FBoss_Delegate);

UENUM()
enum class EPhaseState{ // 보스 패턴 페이즈를 나타내는 열거형
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
	// 페이즈 관련 함수-------------------------------
	void ChangePhaseState(EPhaseState newState);
	EPhaseState GetPhaseState() { return _currentPhaseState; }
	void DoRise();
	void EndChangePhase();

	FBoss_Delegate OnEndRise;
	

	// 도주 관련 함수 --------------------------------

	void EscapeStart();
	void EscapeFromPlayer();
	void EndEscape();

	FBoss_Delegate OnEndEscape;

	// 회피 관련 함수 ---------------------------------

	void AvoidanceStart();
	void EndAvoidance();

	FBoss_Delegate OnEndAvoidance;

	// 랜덤 발사 관련 함수------------------------------
	void SelectRandomlyProjTypeAndShot(); // 랜덤으로 발사 타입을 지정하고 실행시킨다
	ABossProjectile* SpawnProj(const FVector& spawnVec, const FRotator& targetRot); // 발사체를 스폰시킨다
	UFUNCTION()
	EBossProjectileType GetProjectileState() { return _currentProjectileState; }

	/*가로 발사*/
	void BeginShotProj_Horizontal(); // 가로 발사 시작
	void ShotProj_Horizontal(); // 가로 발사체 발사

	/*세로 발사*/
	void BeginShotProj_Fake(); // Fake 발사 시작
	void ShotProj_Fake(); // 세Fake로 발사체 발사

	/*딜레이 발사*/
	void BeginShotProj_Delay();
	void ShotProj_Delay();

	/*유도 발사*/
	void BeginShotProj_Homing();
	void ShotProj_Homing();

	void EndShotProjectile(); // 어떤 발사 타입이든 발사가 끝나면 이 함수를 호출.

	FBoss_Delegate OnEndShotProj;

	// 범위 공격 관련 함수 -----------------------------
	void BeginRangeAttack();
	void RangeAttack();
	void RangeAttackCheck();

	// 폭딜 관련 함수 ----------------------------------
	float GetTotalRapidDamage() { return _totalRapidDamage; };

	// 날아다니는 거 관련 ------------------------------
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

	// 보스 도주 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Escape, Meta = (AllowPrivateAccess = true))
		bool _bCanEscape = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Escape, Meta = (AllowPrivateAccess = true))
		FVector _escapeLoc; // 보스가 빤스런 할 위치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Escape, Meta = (AllowPrivateAccess = true))
		float _escapeSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Escape, Meta = (AllowPrivateAccess = true))
		float _escapeRandRadius;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Escape, Meta = (AllowPrivateAccess = true))
		float _escapeMinRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Escape, Meta = (AllowPrivateAccess = true))
		FVector _floorMeshLoc; // 보스가 서 있는 바닥의 위치

	// 보스 회피 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Avoidance, Meta = (AllowPrivateAccess = true))
		bool _bCanAvoidance = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Avoidance, Meta = (AllowPrivateAccess = true))
		FVector _avoidanceLoc;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Avoidance, Meta = (AllowPrivateAccess = true))
		float _avoidanceSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Avoidance, Meta = (AllowPrivateAccess = true))
		float _avoidanceDistance;

	// 페이즈별 상승 높이 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BossLocation, Meta = (AllowPrivateAccess = true))
		float _riseHeight_Fhase1 = 0.f; // 페이즈1, 보스의 상승 높이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BossLocation, Meta = (AllowPrivateAccess = true))
		float _riseHeight_Fhase2 = 500.f; // 페이즈2, 보스의 상승 높이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BossLocation, Meta = (AllowPrivateAccess = true))
		float _riseHeight_Fhase3 = 800.f; // 페이즈3, 보스의 상승 높이

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = BossLocation, Meta = (AllowPrivateAccess = true))
		float _riseZLocation; // 보스의 상승높이를 더한, 최종 Z 축 위치값

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = BossLocation, Meta = (AllowPrivateAccess = true))
		bool _bOnLerpZLocation = false;

	// 발사 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
	int32 _recreateProjCount = 0;
	/*Horizontal 발사----------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile_Horizontal, Meta = (AllowPrivateAccess = true))
	int32 _projNum_Horizontal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile_Horizontal, Meta = (AllowPrivateAccess = true))
	int32 _projAnglaGap_Horizontal;
	/*Fake 발사----------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
	int32 _recreateProjMax_Fake;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
	int32 _projNum_Fake;
	/*Delay 발사-------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
	int32 _recreateProjMax_Delay;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
	int32 _projNum_Delay;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
	int32 _proj_Y_AngleGap_Delay;
	/*Homing 발사------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
	int32 _recreateProjMax_Homing;


	// 범위 공격 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RangeAttack, Meta = (AllowPrivateAccess = true))
	float _rangeAttackRadius = 400.f;

	// 폭딜 관련 함수---------------------------------
	float _rapidDamageCheckDuration = 3.f; // 폭딜 체크를 할 시간
	float _totalRapidDamage = 0.f; // rapidDamageCheckDuration 동안 받은 총 데미지량
};
