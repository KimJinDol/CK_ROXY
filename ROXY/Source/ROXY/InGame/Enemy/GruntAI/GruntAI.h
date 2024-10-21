// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/EnemyAI.h"
#include "InGame/Item/Item_HP.h"
#include "InGame/Item/Item_Fever.h"
#include "InGame/DataTable/GruntDataTable.h"
#include "GruntAI.generated.h"

DECLARE_MULTICAST_DELEGATE(FGruntAI_Delegate);

UENUM()
enum class EGruntStateMode
{
	SM_IDLE,
	SM_STIFF,
	SM_KNOCKBACK,
	SM_MARK,
	SM_INVINCI, /* 무적 */
	SM_REVIVE, /* 부활 */
	SM_DIE
};
/**
 * 
 */
UCLASS()
class ROXY_API AGruntAI : public AEnemyAI
{
	GENERATED_BODY()

public:
	AGruntAI();

	// 델리게이트
	FGruntAI_Delegate OnAttackTaskEnd;
	FGruntAI_Delegate OnSpawnEnd;

	FTimerDelegate stiffEndDelegate;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AnimInstance, Meta = (AllowPrivateAccess = true))
		class UGruntAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		class UCapsuleComponent* AttackCollision;

	/* UI */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = true))
		class UWidgetComponent* WidgetComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
		class UGruntWidget* GruntWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
		class UEnemyWidget* EliteWidget; // 엘리트 체력바용 위젯으로 쓸랍니다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = true))
		TSubclassOf<UUserWidget> EliteWidgetTemplate;

	/* 파티클 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* PC_ParticleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Hit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_SwiftHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_InvinciSheild; // 무적 상태 시 발동 될 오라
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
	//	class UParticleSystem* PS_EliteIcon;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
	//	class UParticleSystem* PS_BuffIcon;

	/* 사운드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_Invincible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_Death;

	/* 아이템 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, Meta = (AllowPrivateAccess = true))
		TSubclassOf<AItem_HP> DropItem_HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, Meta = (AllowPrivateAccess = true))
		TSubclassOf<AItem_Fever> DropItem_Fever;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = GruntStateMode, Meta = (AllowPrivateAccess = true))
		EGruntStateMode _currentStateMode;

protected: /* 잡몹 공통 사용 변수 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Target, Meta = (AllowPrivateAccess = true))
		ARoxyCharacter* Roxy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Target, Meta = (AllowPrivateAccess = true))
		AActor* Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemDrop, Meta = (AllowPrivateAccess = true))
		bool _bCanItemDrop = true;

	// Move
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
		bool _bCanMoveToTarget = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
		bool _bCanTurnToTarget = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
		float _moveSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
		float _startLogicTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
		float _spawnAnimRate;

	// Sensor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		bool _bAvoiding = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		bool _bIsAllAvoiding = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		FVector _frontSensorPos = FVector(110.f, 0.f, GetActorLocation().Z);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		FVector _frontRightSensorPos = FVector(60.f, 100.f, GetActorLocation().Z);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		FVector _frontLeftSensorPos = FVector(60.f, -100.f, GetActorLocation().Z);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		FVector _leftSensor = FVector(0.f, -110.f, GetActorLocation().Z);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		FVector _rightSensor = FVector(0.f, 110.f, GetActorLocation().Z);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		FVector _behindSensor = FVector(-110.f, 0.f, GetActorLocation().Z);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		bool _bOnFrontSensor = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		bool _bOnFrontLeftSensor = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		bool _bOnFrontRightSensor = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		bool _bOnRightSensor = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		bool _bOnLeftSensor = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		bool _bOnBehindSensor = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		FVector _avoidVec;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sensor, Meta = (AllowPrivateAccess = true))
		FRotator _avoidRot;

	// Attack
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool _bIsAttacking = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _damage_Attack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool _bHitCheck = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _stiffenTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		TArray<AActor*> _overalppedActors;

	//////////////* 특수 패턴 관련 *//////////////
	// 특수 잡몹
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Type, Meta = (AllowPrivateAccess = true))
		bool _bIsEliteGrunt = false; // 엘리트형 몬스터인가?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Type, Meta = (AllowPrivateAccess = true))
		bool _bIsBuffGrunt = false; // 버프형 몬스터인가?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Type, Meta = (AllowPrivateAccess = true))
		bool _bIsBossMonster = false; // 보스 몬스터인가?

	// 일반 잡몹 특수 패턴
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpecialAttack, Meta = (AllowPrivateAccess = true))
		bool _bIsSpecialAttack = false; // 특수 패턴 실행중인가?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpecialAttack, Meta = (AllowPrivateAccess = true))
		bool _bCanSpecialAttack = true; // 특수 패턴 실행이 가능한가?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpecialAttack, Meta = (AllowPrivateAccess = true))
		float _damage_SpecialAttack; // 특수 패턴 데미지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpecialAttack, Meta = (AllowPrivateAccess = true))
		float _coolTime_SpecialAttack; // 특수 패턴 쿨타임
	UPROPERTY()
		FTimerHandle _specialAttackTimer;

	// 엘리트 몬스터 특수 패턴
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EliteSpecialAttack, Meta = (AllowPrivateAccess = true))
		bool _bIsEliteSpecialAttack = false; // 엘리트 몬스터의 특수 패턴이 실행중인가?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EliteSpecialAttack, Meta = (AllowPrivateAccess = true))
		bool _bCanEliteSpecialAttack = true; // 엘리트 몬스터의 특수 패턴 실행이 가능한가?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EliteSpecialAttack, Meta = (AllowPrivateAccess = true))
		float _damage_EliteSpecialAttack; // 엘리트 몬스터의 특수 패턴 데미지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EliteSpecialAttack, Meta = (AllowPrivateAccess = true))
		float _coolTime_EliteSpecialAttack; // 엘리트 몬스터의 특수 패턴 쿨타임
	UPROPERTY()
		FTimerHandle _eliteSpecialAttackTimer;

	//////////////*상태 이상 모드 관련*//////////////
	// Damaged
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bIsOnDamageUI = false;
	UPROPERTY()
		FTimerHandle DamageUITimer;

	// Stiffen
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SM_Stiffen, Meta = (AllowPrivateAccess = true))
		bool _bIsStiff = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SM_Stiffen, Meta = (AllowPrivateAccess = true))
		float _offStiffTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SM_Stiffen, Meta = (AllowPrivateAccess = true))
		float _stiffenProbability; // 기본 공격으로 경직을 먹일 확률
	UPROPERTY()
		FTimerHandle StiffTimer;

	// Mark (표식)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SM_Mark, Meta = (AllowPrivateAccess = true))
		bool _bIsMarking = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SM_Mark, Meta = (AllowPrivateAccess = true))
		float _markStateDuration = 15.f;
	UPROPERTY()
		FTimerHandle MarkDurationTimer;

		FTimerDelegate MarkTimerDelegate;

	// Buff (버프:무적, 부활)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SM_Buff, Meta = (AllowPrivateAccess = true))
		bool _bIsInvincibleState = true; // 무적 상태인지?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SM_Buff, Meta = (AllowPrivateAccess = true))
		bool _bIsReviveState = false; // 부활한 상태인지?

	// Die
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Die, Meta = (AllowPrivateAccess = true))
		bool _bIsDie = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Die, Meta = (AllowPrivateAccess = true))
		FVector _deadPosition; // 죽은 위치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Die, Meta = (AllowPrivateAccess = true))
		float _matOpacity = 1.0f;
	////////////////////////////////////////////////

public:
	UFUNCTION(BlueprintCallable)
		void SetCanItemDrop(bool value) {_bCanItemDrop = value;}
	UFUNCTION()
		void SetTarget(AActor* targetActor);
	UFUNCTION()
		void SetTurnToTarget(bool value);
	UFUNCTION()
		void TurnToTarget(AActor* targetActor, float deltaSec);
	UFUNCTION()
		void SetMoveToTarget(bool value);
	UFUNCTION()
		void MoveToTarget(float deltaTime);
	// 장애물을 감지하고 피하는 함수
	UFUNCTION()
		bool Sensor();
	UFUNCTION()
		bool IsAvoiding() {return _bAvoiding;}

protected:
	UFUNCTION()
		void ChangeMatOpacity(float deltaTime);
	UFUNCTION()
		void DestroyActor();

public:
	UFUNCTION()
		void ObjectPoolReturn();
	UFUNCTION()
		void DeactiveAI();
	UFUNCTION()
		void RevivalAI();

public:
	/* 기본 근접 공격 */
	UFUNCTION()
		void StartAttack();
	UFUNCTION()
		void OnAttackCollision();
	UFUNCTION()
		void AttackCheck(AActor* attackTarget);
	UFUNCTION()
		void OffAttackCollision();
	UFUNCTION()
		void AttackEnd();

protected:
	//////////////* 특수 패턴 관련 *//////////////
	/* 일반 잡몹 특수 패턴 */
	UFUNCTION()
		void SpecialAttackCoolApply();
	UFUNCTION()
		void OnCanSpecialAttack();

	/* 엘리트 몬스터 특수 패턴 */
	UFUNCTION()
		void EliteSpecialAttackCoolApply();
	UFUNCTION()
		void OnCanEliteSpecialAttack();

protected:
	UFUNCTION()
		void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Die() override;

	virtual void BeginPlay() override;

public:
	virtual void Possessed(AController* NewController);
	virtual void UnPossessed();

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	/*-----------스테이트 모드 관련---------------*/
	UFUNCTION()
		void SetGruntStateMode(EGruntStateMode newStateMode);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void SetStiffState(bool value = true, float time = 0.0f);

	UFUNCTION()
		void SetInvinciState(bool value);

	UFUNCTION()
		void SpawningState(bool value);

	/*----------UI---------------*/
	UFUNCTION(BlueprintCallable)
		void SetEliteHUDVisibility(bool value);

	// 데미지 폰트
	UFUNCTION()
		void SetDamageUI();
	UFUNCTION()
		void OffDamageUI();

	// 엘리트 아이콘
	UFUNCTION()
		void RotateEliteIcon();

	// 표식 상태일 때 UI 붙여주기
	UFUNCTION()
		void SetMarkUI();
	UFUNCTION()
		void SetMarkTimer();
	// 표식 상태인지
	UFUNCTION()
		bool IsMarkState() { return _bIsMarking; }

public: /* Get, Set */
	UFUNCTION()
		UWidgetComponent* GetGruntHUDWidget();

	UFUNCTION()
		bool GetIsStiff();

	UFUNCTION()
		bool GetIsInvinci() { return _bIsInvincibleState; }
	UFUNCTION()
		bool GetIsRevivla() { return _bIsReviveState; }
	UFUNCTION()
		void SetRevivalState(bool value) { _bIsReviveState = value; }

	UFUNCTION()
		bool GetIsDie();

	UFUNCTION()
		bool GetIsSpecialAttack() { return _bIsSpecialAttack; }
	UFUNCTION()
		bool GetIsCanSpecialAttack() { return _bCanSpecialAttack; }

	UFUNCTION()
		bool GetIsEliteSpecialAttack() { return _bIsEliteSpecialAttack; }
	UFUNCTION()
		bool GetIsCanEliteSpecialAttack() { return _bCanEliteSpecialAttack; }

	UFUNCTION()
		bool GetIsSpecialGrunt() { return _bIsEliteGrunt; }
	UFUNCTION()
		bool GetIsBuffGrunt() { return _bIsBuffGrunt; }
	UFUNCTION()
		bool GetIsBossMonster() { return _bIsBossMonster; }
	UFUNCTION()
		void SetIsSpecialGrunt(bool value) { _bIsEliteGrunt = value; }
};
