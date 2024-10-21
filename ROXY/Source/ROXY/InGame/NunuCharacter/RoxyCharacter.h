// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Core/NunuGameInstance.h"
#include "InGame/NunuCharacter/GhostSword.h"
#include "GameFramework/Character.h"
#include "RoxyCharacter.generated.h"

DECLARE_DELEGATE(FRoxy_Delegate);

// 스킬 쿨타임에 사용할 스킬타입 열거형
UENUM()
enum class ESkillType : uint8
{
	ESkill0 = 0, // 기본공격
	ESkill1, // 일섬
	ESkill2, // 연참
	ESkill3, // 환영검
	ESkill4,  // 발차기
	ESkill5 // 대시
};

UCLASS()
class ROXY_API ARoxyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARoxyCharacter();

private: /*컴포넌트*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HpComponent, meta = (AllowPrivateAccess = "true"))
		class UHpComponent* HpComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UltimateComponent, meta = (AllowPrivateAccess = "true"))
		class UUltimateComponent* UltimateComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UPN_SpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UPN_CameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AnimInstance, meta = (AllowPrivateAccess = "true"))
		class URoxyAnimInstance* RoxyAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
		class URoxyWidget* WidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* PC_ParticleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class URectLightComponent* RectLight;

	// 공격용 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		class UCapsuleComponent* WeaponCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		class UCapsuleComponent* SheathCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ForwardAttackCollision, Meta = (AllowPrivateAccess = true))
		class UBoxComponent* ForwardAttackCollision;

	// 포인터
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = InGameManager, meta = (AllowPrivateAccess = true))
		class UNunuGameInstance* GameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InGameManager, meta = (AllowPrivateAccess = true))
		class ANunuInGameManager* InGameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		class UDataTable* DataTable;

	// 리소스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Material, Meta = (AllowPrivateAccess = true))
		class UMaterial* BodyMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Material, Meta = (AllowPrivateAccess = true))
		class UMaterial* SwordMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = true))
		TSubclassOf<URoxyWidget> WidgetTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_SnatchBeam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_SwordParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_Critical;

	// 카메라 쉐이크
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraShake, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShake> CS_Attack1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraShake, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShake> CS_Attack2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraShake, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShake> CS_Attack3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraShake, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShake> CS_Attack4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraShake, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShake> CS_Pierce;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraShake, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShake> CS_PierceStart;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraShake, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShake> CS_Hit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraShake, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShake> CS_SwiftSlash;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraShake, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShake> CS_FeverStart;

	// 스폰 관련
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spawn, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AGhostSword> GhostSword;

	/*--------시스템 관련 변수들------------*/

	// 특정스킬을 쓸 때 자동으로 적을 봐야하는 경우가 있다
	// 이 때 주위 반경에 적이 있는지 찾기위해 스윕을 하는데 
	// 이 때 스윕에 사용될 반지름이다
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LookEnemy, Meta = (AllowPrivateAccess = true))
		AEnemyAI* _nearbyEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LookEnemy, Meta = (AllowPrivateAccess = true))
		bool _bOnLookOnNearbyEnemy; // 스킬 사용 혹은 공격 시 근처에 있는 적을 보게 할지에 대한 bool 타입 변수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterSystem, Meta = (AllowPrivateAccess = true))
		float _radiusForFindingEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterSystem, Meta = (AllowPrivateAccess = true))
		float _bIsBulletTime = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterSystem, Meta = (AllowPrivateAccess = true))
		bool _bIsDealing = false; // 딜 넣을 수 있는 스킬이 활성화 되어있는지 판단

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TakeDamage, Meta = (AllowPrivateAccess = true))
		bool _bCanTakeDamage = true;

public:
	FRoxy_Delegate OnBulletTimeEnd;

	/*-------------스킬 쿨타임 관련----------------*/
public:
	FTimerDelegate UltimateDuration_Delegate;

	FTimerDelegate Skill1_Delegate;
	FTimerDelegate Skill2_Delegate;
	FTimerDelegate Skill3_Delegate;
	FTimerDelegate Skill4_Delegate;
	FTimerDelegate Skill5_Delegate;

private:
	FTimerHandle UltimateDurationTimer;

	FTimerHandle Skill1_CoolTimer;
	FTimerHandle Skill2_CoolTimer;
	FTimerHandle Skill3_CoolTimer;
	FTimerHandle Skill4_CoolTimer;
	FTimerHandle Skill5_CoolTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		float _coolTime_Skill1; // 일섬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		float _coolTime_Skill2; // 연참
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		float _coolTime_Skill3; // 환영검 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		float _coolTime_Skill4; // 발차기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		float _coolTime_Skill5; // 대시

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		bool _bIsSkillActive_Skill1 = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		bool _bIsSkillActive_Skill2 = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		bool _bIsSkillActive_Skill3 = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		bool _bIsSkillActive_Skill4 = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		bool _bIsSkillActive_Skill5 = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		bool _bIsCoolApply_Skill1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		bool _bIsCoolApply_Skill2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		bool _bIsCoolApply_Skill3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		bool _bIsCoolApply_Skill4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		bool _bIsCoolApply_Skill5;

private:
	/*-----------시스템 관련-------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = System, Meta = (AllowPrivateAccess = true))
		bool _bCanPlayBGM = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = System, Meta = (AllowPrivateAccess = true))
		bool _bIsGameOver = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = System, Meta = (AllowPrivateAccess = true))
		bool _bIsGameClear = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = System, Meta = (AllowPrivateAccess = true))
		bool _bCanInteractionVM = false; // 자판기와 상호작용이 가능한 상태인지

	/*---------------GameInstance stat 관련---------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 _addedHPCount = 0; // 체력 증가 스탯을 획득 한 횟수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 _addedDamageCount = 0; // 공격력 증가 스탯을 획득 한 횟수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 _decreasedCoolTimeCount = 0;// 쿨타임 감소 스탯을 획득 한 횟수

	/*-----------락온 관련-------------*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = LockOnSystem, Meta = (AllowPrivateAccess = true))
		bool _bCanLockOnInput = true;

private:
	//////////////////모든 공격과 관련된 스킬///////////////////////////////

	/*공격력*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _ad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool _bIsAddedDamage = false;

	/* --------------- Attack 관련 --------------- */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool _bIsAttacking = false;

	/*기본공격 고유 데미지*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _attack_damage;
	/*기본 공격 계수*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _attack_ratio;
	/*기본 공격 데미지 스탯*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _attack_stat;

	TArray<AActor*> _weaponOverlapActor;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 _currentCombo = 0;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 _maxCombo = 3;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool _bCanNextCombo = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool _bCanInputNextCombo = false;

	// 불릿타임
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _attack1_BulletTime_dilation;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _attack1_BulletTime_duration;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _attack2_BulletTime_dilation;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _attack2_BulletTime_duration;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _attack3_BulletTime_dilation;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _attack3_BulletTime_duration;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _lastAttack_BulletTime_dilation;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _lastAttack_BulletTime_duration;

	/* ----------Kick 관련--------- */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		bool _bIsKicking = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		bool _bIsVMKicking = false; // 자판기를 찬건지
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		bool _bCanKick = true;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		float _kick_BulletTime_dilation;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		float _kick_BulletTime_duration;

	/*발차기 고유 데미지*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		float _kick_damage;
	/*발차기 계수*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		float _kick_ratio;
	/*발차기 데미지 스탯*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		float _kick_stat;


	//////////////////////////////////* 공격 스킬 관련 *//////////////////////////////////
private:
	TArray<AEnemyAI*> _forwardAttackTargets; // 전면 공격을 쳐맞을 타겟들 배열\

	float _finalDamageAmount = 0.0f;

	float _criticalHitProbability = 0.2f; // 치명타 확률
	bool _bIsCritical = false;

	// 게임인스턴스에서 저장된 값과 관련
	float _criticalHitMultiply = 1.5f; // 치명타 피해량
	float _addedMaxHp = 0.f;
	float _addedMoveSpeed = 0.f;
	float _addedDamage = 0.f;
	float _decreasedCoolTimePercent = 0.f;

	bool _bCanChangeSwordColor = false; // 검 색깔을 변화시킬 수 있는가?(스킬 사용 시 활성화)
	bool _bCanChangeSwordColorOrigin = false; // 검 색깔을 원래대로 변화시킬 수 있는가?(스킬 종료 시 활성화)
	float _swordColor; // 스킬 사용 시 변화시켜줄 검 색깔 파라미터 값

	/*-------------Ultimate 관련---------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ultimate, Meta = (AllowPrivateAccess = true))
		bool _bIsUltimateOn = false; // 궁극기 (스탯 업그레이드) 중일 시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ultimate, Meta = (AllowPrivateAccess = true))
		bool _bCanUseUltimate = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ultimate, Meta = (AllowPrivateAccess = true))
		float _ultimateDuration = 10.f;

	/*-------------Pierce(일섬) 관련---------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		bool _bIsPierce = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		bool _bCanPierce = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		bool _bResetPierceCool = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		float _pierce_damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		float _pierce_ratio;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		float _pierce_stat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true)) 
		float _pierceSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		FVector _pierceDirection;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		float _pierce_targetArmLength;
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		bool _bIsPierceEnemyCheck = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		int32 _maxPierceNum = 2; // 2회 충전식
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		int32 _currentPierceNum = 2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		float _remainingPierceTime;
	UPROPERTY()
		FTimerHandle _pierceTimer;

	/*-------------SwiftSlash(연참) 관련---------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SwiftSlash, Meta = (AllowPrivateAccess = true))
		bool _bIsSwiftianSlash = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SwiftSlash, Meta = (AllowPrivateAccess = true))
		bool _bCanSwiftSlash = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SwiftSlash, Meta = (AllowPrivateAccess = true))
		FVector _swiftApproachLoc;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SwiftSlash, Meta = (AllowPrivateAccess = true))
		float _swiftSlash_damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SwiftSlash, Meta = (AllowPrivateAccess = true))
		float _swiftSlash_ratio;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SwiftSlash, Meta = (AllowPrivateAccess = true))
		float _swiftSlash_stat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SwiftSlash, Meta = (AllowPrivateAccess = true))
		float _swiftSlash_influenceRangeRadius = 600.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SwiftSlash, Meta = (AllowPrivateAccess = true))
		TArray<AEnemyAI*> _swiftSlashEnemys;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SwiftSlash, Meta = (AllowPrivateAccess = true))
		int32 _swiftSlash_DamagingIndex = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SwiftSlash, Meta = (AllowPrivateAccess = true))
		float _swiftSlash_DamagingPeriod = 0.1f;
	UPROPERTY()
		FTimerHandle _swiftSlash_DamagingTmer;

	/*-------------Evade 관련---------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		bool _bIsEvasion = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		bool _bCanEvade = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		bool _bCheckForwardFirstInput = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		bool _bCheckRightFirstInput = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		bool _bCanEvadeCharacter = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		bool _bCanInputNextEvade = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		bool _bIsInputNextEvade = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		FVector _evadeVelocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		FVector _evadeDirection;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		FVector _firstInputVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		FVector _secondInputVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		float _evadeSpeed;
	UPROPERTY()
		FTimerHandle _canEvadeOffTimer;

	/*-------------Ghost SWORD 관련----------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		bool _bCanGhostSword = true;
	// 저장 할 수 있는 환영검 최대 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		int32 _storableMaxNum_GhostSword = 3;

	// 현재 사용 가능한 환영검의 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		int32 _currentStorableNum_GhostSword = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		AGhostSword* TargetGhostSword;

	// 환영검(돌진)을 할 수 있는 상황인지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		bool _bCanRushTowardGhostSword = false;
	// 현재 타겟 환영검으로 돌진 중인지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		bool _bIsRushTowardGhostSword = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		float _ghostSword_damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		float _ghostSword_ratio;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		float _ghostSword_stat;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		float _ghostSword_fixedDamage; // 환영검 고정 데미지(표식 남긴 적에게 또 표식 남길 때 적용되는 데미지)

	/*-------------Damaged(피해, 상태이상, 죽음) 관련---------------*/
	/* Stiffen : 몬스터 기본 공격에 피격시 경직(스턴) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bCanDamaged = true; // 피격당할 수 있는 상태인가? 거짓일 시 무적
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bIsStiffen = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		float _stiffenTime;
	UPROPERTY()
		FTimerHandle _stiffenTimer;
	
	/* Faint : 몬스터 특수 패턴에 피격시 기절 상태(혼란) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bIsFaint = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		int32 _faintDefianceNum = 0; // 기절을 풀기 위한 반항(도발 해제 키를 누른) 횟수
	UPROPERTY()
		FTimerHandle _faintTimer;

	/* KnockBack : 몬스터 특수 패턴에 피격시 넉백 상태 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bIsKnockBack = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		float _knockbackDamage = 25.0f;

	/* Fury : 몬스터 특수 패턴에 피격시 도발 상태  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bIsFury = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		int32 _furyDefianceNum = 0; // 도발을 풀기 위한 반항(도발 해제 키를 누른) 횟수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		AGruntAI* _furyAttacker; // 도발 상태에 걸리게 한 가해몬스터
	UPROPERTY()
		FTimerHandle _furyTimer;

	/* Addicted : 몬스터 특수 패턴에 피격시 중독 상태 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bIsAddicted = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		float _poisonDamage = 300.0f; // 중독 상태에 빠질 시 초당 입을 데미지
	UPROPERTY()
		FTimerHandle _addictedTimer;
	UPROPERTY()
		FTimerHandle _poisonTimer;

	/* 혼란(스킬 잠금) */
public:
	FTimerDelegate Skill1Lock_Delegate;
	FTimerDelegate Skill2Lock_Delegate;
	FTimerDelegate Skill3Lock_Delegate;

private:
	FTimerHandle Skill1Lock_CoolTimer;
	FTimerHandle Skill2Lock_CoolTimer;
	FTimerHandle Skill3Lock_CoolTimer;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SkillLock, Meta = (AllowPrivateAccess = true))
		bool _bIsConfusion = false; // 혼란 상태(스킬 잠금 상태)인가?
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SkillLock, Meta = (AllowPrivateAccess = true))
		float _skillLockCoolTime = 3.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SkillLock, Meta = (AllowPrivateAccess = true))
		bool _bIsSkillLocked = false; // 스킬이 하나라도 잠겨있는가?
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SkillLock, Meta = (AllowPrivateAccess = true))
		bool _bIsSkillLocked_Skill1 = false; // 일섬
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SkillLock, Meta = (AllowPrivateAccess = true))
		bool _bIsSkillLocked_Skill2 = false; // 연참
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SkillLock, Meta = (AllowPrivateAccess = true))
		bool _bIsSkillLocked_Skill3 = false; // 환영검

private:
	/* Die */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bIsDie = false;
	UPROPERTY()
		FTimerHandle _dieTimer;
//////////////////////////////////////////////////////////////////////////////////////////

	
private: /*일반 변수*/

	// 카메라 ------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = true))
		float _currentTargetArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = true))
		float _targetArmLengthOrigin; // 기본 타겟암렝스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = true))
		FVector _socketOffsetOrigin; // 기본 소켓 오프셋

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = true))
		FVector _cameraLocOrigin; // 카메라 기본 설정 위치

	// ---------------------------Basic Movement --------------------------
	bool _bCanMove = true;
	bool _bCanTurn = true;
	bool _bCanAttack = true;

	float _circlePrajTimer = 0.f; // 원 궤적 달리기 시 사용 될 마우스 non input 타이머
	FRotator _oldRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _moveSpeed;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentSpeed = 0.0f; // 현재 속도 
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentSpeedRate = 0.0f; // 현재 속도율
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentForwardSpeed = 0.0f; // 현재 전방 속도
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentForwardSpeedRate = 0.0f; // 현재 전방 진행 속도율
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentForwardSpeedRateAbs; // 현재 전방 진행 속도율 절댓값
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentRightSpeed = 0.0f; // 현재 우측 속도
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentRightSpeedRate = 0.0f; // 현재 우측 진행 속도율
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentRightSpeedRateAbs; // 현재 우측 진행 속도율 절댓값
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		bool _bOnMove = false; // 움직이고 있는가
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		bool _bFixMoveDirection = false; // 가고있는 방향이 마우스의 영향을 받을지 안받을지
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		FRotator _fixedRotation = FRotator::ZeroRotator;

public:
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = Camera)
		float TurnRateOrigin;
	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = Camera)
		float LookUpRateOrigin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float BaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float BaseLookUpRate;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


private:
	/*---------------GameInstance stat 관련---------------*/
	UFUNCTION()
		void SetAddedMaxHp();
	UFUNCTION()
		void SetAddedMoveSpeed();
	UFUNCTION()
		void SetCriticalHitMultiply();
	UFUNCTION()
		void SetAddedDamage();
	UFUNCTION()
		void SetDecreasedCoolTimePercent();

	/* 엔딩 데이터 */
	UFUNCTION()
		void SetNumOfStep();
	UFUNCTION()
		void SetUseBasicAttack();
	UFUNCTION()
		void SetUseSkill();
	UFUNCTION()
		void SetNumOfHit();
	UFUNCTION()
		void SetNumOfHitBlow(int32 value);
	UFUNCTION()
		void SetNumOfDamaged();
	UFUNCTION()
		void SetNumOfFaint();
	UFUNCTION()
		void SetNumOfAddicted();
	UFUNCTION()
		void SetMissKick();


	/*--------------자주 쓰이는 함수----------------*/
	UFUNCTION()
		// Sphere 스윕을 이용하여 근처에 있는 적들을 반환하는 함수
		TArray<FHitResult> GetHitResults(FVector StartVec, FVector EndVec, float SphereRadius);

	UFUNCTION()
		// 락온 모드 = false 시, 스킬사용이나 공격 시 근처에 적이 있으면 적을 보게 한다. 함수의 내용은 틱에서 처리한다.
		void SetLookOnNearbyEnemy(bool value) { _bOnLookOnNearbyEnemy = value; }
	// Look On Nearby Enemy 에서 스윕에 쓰일 반지름을 설정하는 함수
	UFUNCTION()
		void SetRadiusForFindingEnemy(float radius) { _radiusForFindingEnemy = radius; }

	UFUNCTION()
		void LookOnNearbyEnemy();
	// 공격이나 스킬 사용 시, 콜리전으로 overlap 결과를 받아와야 할 때 쓰일 함수
	UFUNCTION()
		void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnCollisionBeginHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void StartBulletTime(float dilation, float duration, float targetArmLength, FVector cameraLoc);

	UFUNCTION()
		void EndBulletTime();

	UFUNCTION(BlueprintCallable)
		void SetActiveRoxyUI(bool value);

	UFUNCTION(BlueprintCallable)
		void SetEnableTakeDamage(bool value) {_bCanTakeDamage = value;}

public:
	// 현재 딜링 가능한 스킬을 사용 중인지
	UFUNCTION()
		bool GetIsDealing() { return _bIsDealing; }

	UFUNCTION(BlueprintImplementableEvent)
		void PlayCameraShake(TSubclassOf<UCameraShake> cameraShake);

private:
	/*--------------PN_SpringArm 관련 함수-------------------*/
	UFUNCTION(BlueprintPure, Category = Getter)
		float GetCurrentTargetArmLength() { return _currentTargetArmLength; }


	/*-------------스킬 쿨타임 관련 함수----------------*/
	UFUNCTION()
		void SkillCoolApply(const ESkillType& skillNum);

public:
	// 스킬 쿨 타임
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE float GetCoolTime_Skill1() { return _coolTime_Skill1; }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE float GetCoolTime_Skill2() { return _coolTime_Skill2; }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE float GetCoolTime_Skill3() { return _coolTime_Skill3; }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE float GetCoolTime_Skill4() { return _coolTime_Skill4; }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE float GetCoolTime_Skill5() { return _coolTime_Skill5; }

	// 스킬이 활성화 되어 있는 상태인지
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE bool IsSkillActive_Skill1() { return _bIsSkillActive_Skill1; }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE bool IsSkillActive_Skill2() { return _bIsSkillActive_Skill2; }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE bool IsSkillActive_Skill3() { return _bIsSkillActive_Skill3; }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE bool IsSkillActive_Skill4() { return _bIsSkillActive_Skill4; }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE bool IsSkillActive_Skill5() { return _bIsSkillActive_Skill5; }

	// 스킬 쿨이 적용 되고 있는지
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE bool IsSkillCoolApply_Skill1() { return _bIsCoolApply_Skill1; }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE bool IsSkillCoolApply_Skill2() { return _bIsCoolApply_Skill2; }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE bool IsSkillCoolApply_Skill3() { return _bIsCoolApply_Skill3; }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE bool IsSkillCoolApply_Skill4() { return _bIsCoolApply_Skill4; }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE bool IsSkillCoolApply_Skill5() { return _bIsCoolApply_Skill5; }

	// 스킬의 남은 쿨 타임
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE float GetRemainingCoolTime_Skill1() { return GetWorldTimerManager().GetTimerRemaining(Skill1_CoolTimer); }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE float GetRemainingCoolTime_Skill2() { return GetWorldTimerManager().GetTimerRemaining(Skill2_CoolTimer); }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE float GetRemainingCoolTime_Skill3() { return GetWorldTimerManager().GetTimerRemaining(Skill3_CoolTimer); }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE float GetRemainingCoolTime_Skill4() { return GetWorldTimerManager().GetTimerRemaining(Skill4_CoolTimer); }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE float GetRemainingCoolTime_Skill5() { return GetWorldTimerManager().GetTimerRemaining(Skill5_CoolTimer); }


	/////////////////////*------------- 스킬 관련 -------------*/////////////////////
private:
	UFUNCTION(BlueprintCallable)
		void SetEnableSkill(bool attackEnable = true, bool pierceEnable = true, bool kickEnable = true, bool swiftSlashEnable = true, bool evadeEnable = true, bool GhostSwordEnable = true);

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsAttack() { return _bIsAttacking; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsEvade() { return _bIsEvasion; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsPierce() { return _bIsPierce; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsKick() { return _bIsKicking; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsSwiftSlash() { return _bIsSwiftianSlash; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsGhostSword() { return _bIsRushTowardGhostSword; }

	UFUNCTION()
		void ChangeSwordColor(float deltaTime);
	UFUNCTION()
		void ChangeSwordColorOrigin(float deltaTime);


	/*-------------Ultimate 관련---------------*/
	UFUNCTION()
		void SetUsableUltimate();
	UFUNCTION()
		void UltimateStart();
	UFUNCTION()
		void UltimateEnd();


	/*-------------------Attack(기본공격) 관련----------------------*/
	UFUNCTION()
		void ComboAttack();
	UFUNCTION()
		void CanInputNextCombo();
	UFUNCTION()
		void AttackComboCheck();

	UFUNCTION() // 공격용 콜리전에 맞은 액터배열 비우기
		void ClearWeaponOverlapActor();
	UFUNCTION() // 공격용 콜리전 끄기
		void OffAttackCollision();

	UFUNCTION()
		void OnAttackEnded();

	UFUNCTION()
		void AttackStartComboState();
	UFUNCTION()
		void AttackEndComboState();

	UFUNCTION()
		void OnAttackUI(int32 value);

public:
	UFUNCTION()
		int32 GetComboAttackMaxNum();
	UFUNCTION()
		void LastAttackEffect();

private:
	/* ---------- Kick(발차기) 관련 ---------- */
	UFUNCTION()
		void StartKick();
	UFUNCTION() // 발차기용 콜리전 켜주기
		void OnKickCollision();
	UFUNCTION()
		void KickEnd();

private:

	/*-------------Pierce(일섬) 관련---------------*/
	UFUNCTION()
		void PierceStart();
	UFUNCTION()
		void Pierce();
	UFUNCTION()
		void FillPierceNum();
	UFUNCTION()
		void PierceDamageCheck(AEnemyAI* enemy);
	UFUNCTION()
		void PierceStop();
	UFUNCTION()
		void PierceEnd();


	/*-------------SwiftSlash(연참) 관련---------------*/
	UFUNCTION()
		void SwiftSlashStart();
	UFUNCTION()
		void SwiftSlash();
	UFUNCTION()
		void ApplySwiftSlashDamaging();
	UFUNCTION()
		void RemoveSwiftOverlapActor();
	UFUNCTION()
		void SwiftSlashEnd();


	/*-------------Evade 관련---------------*/
	UFUNCTION()
		void EvadeStart();
	UFUNCTION()
		void Evade();
	UFUNCTION()
		void StopEvade();
	UFUNCTION()
		void EvadeEnd();
	UFUNCTION()
		void CanInputNextEvade() { _bCanInputNextEvade = true; }
	UFUNCTION()
		void OffCanEvade();


	/*-------------Ghost Sword 관련 ------------*/
	UFUNCTION()
		void StartInstallGhostSword();
	// install a ghost sword
	UFUNCTION()
		void InstallGhostSword();
	UFUNCTION()
		void EndInstallGhostSword();
	// it is Start to Ghost sword charge timer
	UFUNCTION()
		void ChargeGhostSword();
	// it is check there is sword at home
	UFUNCTION()
		void CheckCursorAndSword();
	UFUNCTION()
		void RunGhostSwordAttack();
	// it is Rush towards Ghost sword on cursor
	UFUNCTION()
		void StartRushTowardGhostSword();
	UFUNCTION()
		void RushTowardsGhostSword(float deltaTime);
		

	///////////*------------- Damaged : 피해, 상태이상 관련 ---------------*///////////
public:
	/* Stiffen : 몬스터 기본 공격에 피격시 경직(스턴) */
	UFUNCTION()
		void SetStiffen(float stiffenTime);
	UFUNCTION()
		void GetFreeOfStiffen();

	/* Faint : 몬스터 특수 패턴에 피격시 기절 상태(혼란) */
	UFUNCTION()
		void SetFaint(float faintTime);
	UFUNCTION()
		void GetFreeOfFaint();
	UFUNCTION()
		void DefianceFaint();

	/* KnockBack : 몬스터 특수 패턴에 피격시 넉백 상태 */
	UFUNCTION()
		void SetKnockBack();
	//UFUNCTION()
	//	void GetFreeOfKnockBack();

	/* Fury : 몬스터 특수 패턴에 피격시 도발 상태  */
	UFUNCTION()
		void SetFury();
	UFUNCTION()
		void GetFreeOfFury();
	UFUNCTION()
		void DefianceFury();
	UFUNCTION()
		void SetFuryAttacker(AGruntAI* attacker);
	UFUNCTION()
		void MoveToFuryAttacker(float deltaTime);

	/* Addicted : 몬스터 특수 패턴에 피격시 중독 상태 */
	UFUNCTION()
		void SetAddicted();
	UFUNCTION()
		void PoisonDamaged();
	UFUNCTION()
		void GetFreeOfAddicted();

public:
	/*-------------Skill Lock(스킬 잠금) 관련---------------*/
	UFUNCTION()
		void SkillLockApply(int32 skillNum);

	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE float GetSkillLockTime() { return _skillLockCoolTime; }

	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE bool IsSkill1LockCoolApply() { return _bIsSkillLocked_Skill1; }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE bool IsSkill2LockCoolApply() { return _bIsSkillLocked_Skill2; }
	UFUNCTION(BlueprintPure, Category = Getter)
		FORCEINLINE bool IsSkill3LockCoolApply() { return _bIsSkillLocked_Skill3; }

	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void MakeGhostTrail();

	UFUNCTION()
		void ApplyComboWidget();


public:
	// 플레이어의 입력(움직임, 턴, 공격)을 받을지, 차단 할지를 정하는 함수
	UFUNCTION(BlueprintCallable)
	void SetEnableInput(bool canMove = true, bool canTurn = true, bool canAttack = true);

	// 마우스 감도 조절
	UFUNCTION(BlueprintCallable)
	void SetSensitivity(float value);
private:

	UFUNCTION()
	void MoveForward(float Value);
	UFUNCTION()
	void MoveRight(float Value);
	UFUNCTION()
	void TurnAtRate(float Rate);
	UFUNCTION()
	void LookUpAtRate(float Rate);
	UFUNCTION()
	void ExpandCamera(float Rate);
	UFUNCTION()
	void ReduceCamera(float Rate);
	UFUNCTION()
	void FixMoveDirection();
	UFUNCTION()
	void UnFixMoveDirection();

	UFUNCTION()
		void InitializeVelocity(); // 조절한 캐릭터 velocity 원래대로 초기화

	/*-------------Die 관련---------------*/
	UFUNCTION()
		void OnDie();
	UFUNCTION()
		void OpenGameOverLevel();

	/////////////////////////* 커맨드 */////////////////////////
public:
	UFUNCTION()
		void SetDie(); // 바로 죽음
	UFUNCTION()
		void SetFullHP(); // 풀피
	UFUNCTION()
		void SkillCoolClear(); // 모든 스킬 쿨 해제


public: // 블루프린트 이벤트
	UFUNCTION(BlueprintImplementableEvent)
		void OpenLoseWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void OpenWinWidget();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayFightBGM();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayRelaxBGM();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayActorSequencer_Die();


public:
	UFUNCTION(BlueprintCallable)
	virtual void Possessed(AController* NewController);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		float InflictDamage(ESkillType skillType, AEnemyAI* DamageVictim); // 데미지를 입히는 함수

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public: // Getter

	/** Returns CameraBoom subobject **/
	FORCEINLINE class UPN_SpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UPN_CameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UHpComponent* GetHpComponent() const { return HpComponent; }

	FORCEINLINE class UUltimateComponent* GetUltimateComponent() const { return UltimateComponent; }



	/** 기본 타겟암렝스 길이 반환**/
	float GetTargetArmLengthOrigin() const { return _targetArmLengthOrigin; }

public:/* 기타 get 함수들 */
	UFUNCTION()
		int32 GetCurrentCombo(); // 기본공격 콤보

	UFUNCTION()
		int32 GetCurrentPierceNum();

	UFUNCTION(BlueprintCallable)
		bool WhetherToRushTowardGhostSword() { return TargetGhostSword? true:false; }

	UFUNCTION()
		bool GetIsSkillLocked() { return _bIsSkillLocked; }

	UFUNCTION()
		void SetIsCanInteractionVM(bool value) { _bCanInteractionVM = value; }
	
	UFUNCTION()
		int32 GetAddedHPCount() { return _addedHPCount; }
	UFUNCTION()
		int32 GetAddedDamageCount() { return _addedDamage; }
	UFUNCTION()
		int32 GetDecreasedCoolTimeCount() { return _decreasedCoolTimeCount; }

	UFUNCTION()
		void SetClearTimerStop() { GameInstance->ClearTimeCountStop(); }

	UFUNCTION()
		bool GetCharacterIsDie();
	UFUNCTION()
		bool GetCharacterIsAttack();
	UFUNCTION()
		bool GetCharacterIsKick() { return _bIsKicking; }
	UFUNCTION()
		bool GetCharacterIsPierce();
	UFUNCTION()
		bool GetCharacterIsEvade();
	UFUNCTION()
		bool GetCharacterIsSwift();

	/*-----------시스템 관련 Get, Set-------------*/
	UFUNCTION()
		void SetPlayBGM() { if (_bCanPlayBGM) return; _bCanPlayBGM = true; }
	UFUNCTION()
		bool GetIsGameOver() { return _bIsGameOver; }
	UFUNCTION()
		void SetIsGameOver(bool value) { _bIsGameOver = value; }
	UFUNCTION()
		bool GetIsGameClear() { return _bIsGameClear; }
	UFUNCTION()
		void SetIsGameClear(bool value) { _bIsGameClear = value; }


public:/* 움직임 관련 get 함수 */
	UFUNCTION(BlueprintPure, Category = Character_Basic_Stats, Meta = (BlueprintThreadSafe))
		float GetCurrentSpeed() { return _currentSpeed; }
	UFUNCTION(BlueprintPure, Category = Character_Basic_Stats, Meta = (BlueprintThreadSafe))
		float GetCurrentSpeedRate() { return _currentSpeedRate; }
	UFUNCTION(BlueprintPure, Category = Character_Basic_Stats, Meta = (BlueprintThreadSafe))
		float GetForwardSpeedRate() { return _currentForwardSpeedRate; }
	UFUNCTION(BlueprintPure, Category = Character_Basic_Stats, Meta = (BlueprintThreadSafe))
		float GetRightSpeedRate() { return _currentRightSpeedRate; }
	UFUNCTION(BlueprintPure, Category = Character_Basic_Stats, Meta = (BlueprintThreadSafe))
		bool OnMove() { return _bOnMove; }

};
