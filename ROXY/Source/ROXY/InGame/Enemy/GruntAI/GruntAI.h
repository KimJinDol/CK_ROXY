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
	SM_INVINCI, /* ���� */
	SM_REVIVE, /* ��Ȱ */
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

	// ��������Ʈ
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
		class UEnemyWidget* EliteWidget; // ����Ʈ ü�¹ٿ� �������� �����ϴ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = true))
		TSubclassOf<UUserWidget> EliteWidgetTemplate;

	/* ��ƼŬ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* PC_ParticleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Hit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_SwiftHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_InvinciSheild; // ���� ���� �� �ߵ� �� ����
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
	//	class UParticleSystem* PS_EliteIcon;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
	//	class UParticleSystem* PS_BuffIcon;

	/* ���� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_Invincible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_Death;

	/* ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, Meta = (AllowPrivateAccess = true))
		TSubclassOf<AItem_HP> DropItem_HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, Meta = (AllowPrivateAccess = true))
		TSubclassOf<AItem_Fever> DropItem_Fever;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = GruntStateMode, Meta = (AllowPrivateAccess = true))
		EGruntStateMode _currentStateMode;

protected: /* ��� ���� ��� ���� */
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

	//////////////* Ư�� ���� ���� *//////////////
	// Ư�� ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Type, Meta = (AllowPrivateAccess = true))
		bool _bIsEliteGrunt = false; // ����Ʈ�� �����ΰ�?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Type, Meta = (AllowPrivateAccess = true))
		bool _bIsBuffGrunt = false; // ������ �����ΰ�?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Type, Meta = (AllowPrivateAccess = true))
		bool _bIsBossMonster = false; // ���� �����ΰ�?

	// �Ϲ� ��� Ư�� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpecialAttack, Meta = (AllowPrivateAccess = true))
		bool _bIsSpecialAttack = false; // Ư�� ���� �������ΰ�?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpecialAttack, Meta = (AllowPrivateAccess = true))
		bool _bCanSpecialAttack = true; // Ư�� ���� ������ �����Ѱ�?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpecialAttack, Meta = (AllowPrivateAccess = true))
		float _damage_SpecialAttack; // Ư�� ���� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpecialAttack, Meta = (AllowPrivateAccess = true))
		float _coolTime_SpecialAttack; // Ư�� ���� ��Ÿ��
	UPROPERTY()
		FTimerHandle _specialAttackTimer;

	// ����Ʈ ���� Ư�� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EliteSpecialAttack, Meta = (AllowPrivateAccess = true))
		bool _bIsEliteSpecialAttack = false; // ����Ʈ ������ Ư�� ������ �������ΰ�?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EliteSpecialAttack, Meta = (AllowPrivateAccess = true))
		bool _bCanEliteSpecialAttack = true; // ����Ʈ ������ Ư�� ���� ������ �����Ѱ�?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EliteSpecialAttack, Meta = (AllowPrivateAccess = true))
		float _damage_EliteSpecialAttack; // ����Ʈ ������ Ư�� ���� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EliteSpecialAttack, Meta = (AllowPrivateAccess = true))
		float _coolTime_EliteSpecialAttack; // ����Ʈ ������ Ư�� ���� ��Ÿ��
	UPROPERTY()
		FTimerHandle _eliteSpecialAttackTimer;

	//////////////*���� �̻� ��� ����*//////////////
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
		float _stiffenProbability; // �⺻ �������� ������ ���� Ȯ��
	UPROPERTY()
		FTimerHandle StiffTimer;

	// Mark (ǥ��)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SM_Mark, Meta = (AllowPrivateAccess = true))
		bool _bIsMarking = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SM_Mark, Meta = (AllowPrivateAccess = true))
		float _markStateDuration = 15.f;
	UPROPERTY()
		FTimerHandle MarkDurationTimer;

		FTimerDelegate MarkTimerDelegate;

	// Buff (����:����, ��Ȱ)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SM_Buff, Meta = (AllowPrivateAccess = true))
		bool _bIsInvincibleState = true; // ���� ��������?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SM_Buff, Meta = (AllowPrivateAccess = true))
		bool _bIsReviveState = false; // ��Ȱ�� ��������?

	// Die
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Die, Meta = (AllowPrivateAccess = true))
		bool _bIsDie = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Die, Meta = (AllowPrivateAccess = true))
		FVector _deadPosition; // ���� ��ġ
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
	// ��ֹ��� �����ϰ� ���ϴ� �Լ�
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
	/* �⺻ ���� ���� */
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
	//////////////* Ư�� ���� ���� *//////////////
	/* �Ϲ� ��� Ư�� ���� */
	UFUNCTION()
		void SpecialAttackCoolApply();
	UFUNCTION()
		void OnCanSpecialAttack();

	/* ����Ʈ ���� Ư�� ���� */
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

	/*-----------������Ʈ ��� ����---------------*/
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

	// ������ ��Ʈ
	UFUNCTION()
		void SetDamageUI();
	UFUNCTION()
		void OffDamageUI();

	// ����Ʈ ������
	UFUNCTION()
		void RotateEliteIcon();

	// ǥ�� ������ �� UI �ٿ��ֱ�
	UFUNCTION()
		void SetMarkUI();
	UFUNCTION()
		void SetMarkTimer();
	// ǥ�� ��������
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
