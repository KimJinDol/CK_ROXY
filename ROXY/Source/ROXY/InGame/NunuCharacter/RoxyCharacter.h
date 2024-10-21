// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Core/NunuGameInstance.h"
#include "InGame/NunuCharacter/GhostSword.h"
#include "GameFramework/Character.h"
#include "RoxyCharacter.generated.h"

DECLARE_DELEGATE(FRoxy_Delegate);

// ��ų ��Ÿ�ӿ� ����� ��ųŸ�� ������
UENUM()
enum class ESkillType : uint8
{
	ESkill0 = 0, // �⺻����
	ESkill1, // �ϼ�
	ESkill2, // ����
	ESkill3, // ȯ����
	ESkill4,  // ������
	ESkill5 // ���
};

UCLASS()
class ROXY_API ARoxyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARoxyCharacter();

private: /*������Ʈ*/
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

	// ���ݿ� �ݸ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		class UCapsuleComponent* WeaponCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		class UCapsuleComponent* SheathCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ForwardAttackCollision, Meta = (AllowPrivateAccess = true))
		class UBoxComponent* ForwardAttackCollision;

	// ������
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = InGameManager, meta = (AllowPrivateAccess = true))
		class UNunuGameInstance* GameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InGameManager, meta = (AllowPrivateAccess = true))
		class ANunuInGameManager* InGameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		class UDataTable* DataTable;

	// ���ҽ�
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

	// ī�޶� ����ũ
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

	// ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spawn, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AGhostSword> GhostSword;

	/*--------�ý��� ���� ������------------*/

	// Ư����ų�� �� �� �ڵ����� ���� �����ϴ� ��찡 �ִ�
	// �� �� ���� �ݰ濡 ���� �ִ��� ã������ ������ �ϴµ� 
	// �� �� ������ ���� �������̴�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LookEnemy, Meta = (AllowPrivateAccess = true))
		AEnemyAI* _nearbyEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LookEnemy, Meta = (AllowPrivateAccess = true))
		bool _bOnLookOnNearbyEnemy; // ��ų ��� Ȥ�� ���� �� ��ó�� �ִ� ���� ���� ������ ���� bool Ÿ�� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterSystem, Meta = (AllowPrivateAccess = true))
		float _radiusForFindingEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterSystem, Meta = (AllowPrivateAccess = true))
		float _bIsBulletTime = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterSystem, Meta = (AllowPrivateAccess = true))
		bool _bIsDealing = false; // �� ���� �� �ִ� ��ų�� Ȱ��ȭ �Ǿ��ִ��� �Ǵ�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TakeDamage, Meta = (AllowPrivateAccess = true))
		bool _bCanTakeDamage = true;

public:
	FRoxy_Delegate OnBulletTimeEnd;

	/*-------------��ų ��Ÿ�� ����----------------*/
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
		float _coolTime_Skill1; // �ϼ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		float _coolTime_Skill2; // ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		float _coolTime_Skill3; // ȯ���� 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		float _coolTime_Skill4; // ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		float _coolTime_Skill5; // ���

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
	/*-----------�ý��� ����-------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = System, Meta = (AllowPrivateAccess = true))
		bool _bCanPlayBGM = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = System, Meta = (AllowPrivateAccess = true))
		bool _bIsGameOver = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = System, Meta = (AllowPrivateAccess = true))
		bool _bIsGameClear = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = System, Meta = (AllowPrivateAccess = true))
		bool _bCanInteractionVM = false; // ���Ǳ�� ��ȣ�ۿ��� ������ ��������

	/*---------------GameInstance stat ����---------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 _addedHPCount = 0; // ü�� ���� ������ ȹ�� �� Ƚ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 _addedDamageCount = 0; // ���ݷ� ���� ������ ȹ�� �� Ƚ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 _decreasedCoolTimeCount = 0;// ��Ÿ�� ���� ������ ȹ�� �� Ƚ��

	/*-----------���� ����-------------*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = LockOnSystem, Meta = (AllowPrivateAccess = true))
		bool _bCanLockOnInput = true;

private:
	//////////////////��� ���ݰ� ���õ� ��ų///////////////////////////////

	/*���ݷ�*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _ad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool _bIsAddedDamage = false;

	/* --------------- Attack ���� --------------- */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool _bIsAttacking = false;

	/*�⺻���� ���� ������*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _attack_damage;
	/*�⺻ ���� ���*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float _attack_ratio;
	/*�⺻ ���� ������ ����*/
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

	// �Ҹ�Ÿ��
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

	/* ----------Kick ����--------- */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		bool _bIsKicking = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		bool _bIsVMKicking = false; // ���Ǳ⸦ ������
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		bool _bCanKick = true;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		float _kick_BulletTime_dilation;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		float _kick_BulletTime_duration;

	/*������ ���� ������*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		float _kick_damage;
	/*������ ���*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		float _kick_ratio;
	/*������ ������ ����*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Kick, Meta = (AllowPrivateAccess = true))
		float _kick_stat;


	//////////////////////////////////* ���� ��ų ���� *//////////////////////////////////
private:
	TArray<AEnemyAI*> _forwardAttackTargets; // ���� ������ �ĸ��� Ÿ�ٵ� �迭\

	float _finalDamageAmount = 0.0f;

	float _criticalHitProbability = 0.2f; // ġ��Ÿ Ȯ��
	bool _bIsCritical = false;

	// �����ν��Ͻ����� ����� ���� ����
	float _criticalHitMultiply = 1.5f; // ġ��Ÿ ���ط�
	float _addedMaxHp = 0.f;
	float _addedMoveSpeed = 0.f;
	float _addedDamage = 0.f;
	float _decreasedCoolTimePercent = 0.f;

	bool _bCanChangeSwordColor = false; // �� ������ ��ȭ��ų �� �ִ°�?(��ų ��� �� Ȱ��ȭ)
	bool _bCanChangeSwordColorOrigin = false; // �� ������ ������� ��ȭ��ų �� �ִ°�?(��ų ���� �� Ȱ��ȭ)
	float _swordColor; // ��ų ��� �� ��ȭ������ �� ���� �Ķ���� ��

	/*-------------Ultimate ����---------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ultimate, Meta = (AllowPrivateAccess = true))
		bool _bIsUltimateOn = false; // �ñر� (���� ���׷��̵�) ���� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ultimate, Meta = (AllowPrivateAccess = true))
		bool _bCanUseUltimate = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ultimate, Meta = (AllowPrivateAccess = true))
		float _ultimateDuration = 10.f;

	/*-------------Pierce(�ϼ�) ����---------------*/
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
		int32 _maxPierceNum = 2; // 2ȸ ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		int32 _currentPierceNum = 2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		float _remainingPierceTime;
	UPROPERTY()
		FTimerHandle _pierceTimer;

	/*-------------SwiftSlash(����) ����---------------*/
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

	/*-------------Evade ����---------------*/
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

	/*-------------Ghost SWORD ����----------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		bool _bCanGhostSword = true;
	// ���� �� �� �ִ� ȯ���� �ִ� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		int32 _storableMaxNum_GhostSword = 3;

	// ���� ��� ������ ȯ������ ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		int32 _currentStorableNum_GhostSword = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		AGhostSword* TargetGhostSword;

	// ȯ����(����)�� �� �� �ִ� ��Ȳ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		bool _bCanRushTowardGhostSword = false;
	// ���� Ÿ�� ȯ�������� ���� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		bool _bIsRushTowardGhostSword = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		float _ghostSword_damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		float _ghostSword_ratio;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		float _ghostSword_stat;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		float _ghostSword_fixedDamage; // ȯ���� ���� ������(ǥ�� ���� ������ �� ǥ�� ���� �� ����Ǵ� ������)

	/*-------------Damaged(����, �����̻�, ����) ����---------------*/
	/* Stiffen : ���� �⺻ ���ݿ� �ǰݽ� ����(����) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bCanDamaged = true; // �ǰݴ��� �� �ִ� �����ΰ�? ������ �� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bIsStiffen = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		float _stiffenTime;
	UPROPERTY()
		FTimerHandle _stiffenTimer;
	
	/* Faint : ���� Ư�� ���Ͽ� �ǰݽ� ���� ����(ȥ��) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bIsFaint = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		int32 _faintDefianceNum = 0; // ������ Ǯ�� ���� ����(���� ���� Ű�� ����) Ƚ��
	UPROPERTY()
		FTimerHandle _faintTimer;

	/* KnockBack : ���� Ư�� ���Ͽ� �ǰݽ� �˹� ���� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bIsKnockBack = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		float _knockbackDamage = 25.0f;

	/* Fury : ���� Ư�� ���Ͽ� �ǰݽ� ���� ����  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bIsFury = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		int32 _furyDefianceNum = 0; // ������ Ǯ�� ���� ����(���� ���� Ű�� ����) Ƚ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		AGruntAI* _furyAttacker; // ���� ���¿� �ɸ��� �� ���ظ���
	UPROPERTY()
		FTimerHandle _furyTimer;

	/* Addicted : ���� Ư�� ���Ͽ� �ǰݽ� �ߵ� ���� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bIsAddicted = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		float _poisonDamage = 300.0f; // �ߵ� ���¿� ���� �� �ʴ� ���� ������
	UPROPERTY()
		FTimerHandle _addictedTimer;
	UPROPERTY()
		FTimerHandle _poisonTimer;

	/* ȥ��(��ų ���) */
public:
	FTimerDelegate Skill1Lock_Delegate;
	FTimerDelegate Skill2Lock_Delegate;
	FTimerDelegate Skill3Lock_Delegate;

private:
	FTimerHandle Skill1Lock_CoolTimer;
	FTimerHandle Skill2Lock_CoolTimer;
	FTimerHandle Skill3Lock_CoolTimer;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SkillLock, Meta = (AllowPrivateAccess = true))
		bool _bIsConfusion = false; // ȥ�� ����(��ų ��� ����)�ΰ�?
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SkillLock, Meta = (AllowPrivateAccess = true))
		float _skillLockCoolTime = 3.0f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SkillLock, Meta = (AllowPrivateAccess = true))
		bool _bIsSkillLocked = false; // ��ų�� �ϳ��� ����ִ°�?
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SkillLock, Meta = (AllowPrivateAccess = true))
		bool _bIsSkillLocked_Skill1 = false; // �ϼ�
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SkillLock, Meta = (AllowPrivateAccess = true))
		bool _bIsSkillLocked_Skill2 = false; // ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = SkillLock, Meta = (AllowPrivateAccess = true))
		bool _bIsSkillLocked_Skill3 = false; // ȯ����

private:
	/* Die */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bIsDie = false;
	UPROPERTY()
		FTimerHandle _dieTimer;
//////////////////////////////////////////////////////////////////////////////////////////

	
private: /*�Ϲ� ����*/

	// ī�޶� ------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = true))
		float _currentTargetArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = true))
		float _targetArmLengthOrigin; // �⺻ Ÿ�پϷ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = true))
		FVector _socketOffsetOrigin; // �⺻ ���� ������

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = true))
		FVector _cameraLocOrigin; // ī�޶� �⺻ ���� ��ġ

	// ---------------------------Basic Movement --------------------------
	bool _bCanMove = true;
	bool _bCanTurn = true;
	bool _bCanAttack = true;

	float _circlePrajTimer = 0.f; // �� ���� �޸��� �� ��� �� ���콺 non input Ÿ�̸�
	FRotator _oldRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _moveSpeed;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentSpeed = 0.0f; // ���� �ӵ� 
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentSpeedRate = 0.0f; // ���� �ӵ���
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentForwardSpeed = 0.0f; // ���� ���� �ӵ�
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentForwardSpeedRate = 0.0f; // ���� ���� ���� �ӵ���
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentForwardSpeedRateAbs; // ���� ���� ���� �ӵ��� ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentRightSpeed = 0.0f; // ���� ���� �ӵ�
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentRightSpeedRate = 0.0f; // ���� ���� ���� �ӵ���
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		float _currentRightSpeedRateAbs; // ���� ���� ���� �ӵ��� ����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		bool _bOnMove = false; // �����̰� �ִ°�
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Basic_Movement, Meta = (AllowPrivateAccess = true))
		bool _bFixMoveDirection = false; // �����ִ� ������ ���콺�� ������ ������ �ȹ�����
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
	/*---------------GameInstance stat ����---------------*/
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

	/* ���� ������ */
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


	/*--------------���� ���̴� �Լ�----------------*/
	UFUNCTION()
		// Sphere ������ �̿��Ͽ� ��ó�� �ִ� ������ ��ȯ�ϴ� �Լ�
		TArray<FHitResult> GetHitResults(FVector StartVec, FVector EndVec, float SphereRadius);

	UFUNCTION()
		// ���� ��� = false ��, ��ų����̳� ���� �� ��ó�� ���� ������ ���� ���� �Ѵ�. �Լ��� ������ ƽ���� ó���Ѵ�.
		void SetLookOnNearbyEnemy(bool value) { _bOnLookOnNearbyEnemy = value; }
	// Look On Nearby Enemy ���� ������ ���� �������� �����ϴ� �Լ�
	UFUNCTION()
		void SetRadiusForFindingEnemy(float radius) { _radiusForFindingEnemy = radius; }

	UFUNCTION()
		void LookOnNearbyEnemy();
	// �����̳� ��ų ��� ��, �ݸ������� overlap ����� �޾ƿ;� �� �� ���� �Լ�
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
	// ���� ���� ������ ��ų�� ��� ������
	UFUNCTION()
		bool GetIsDealing() { return _bIsDealing; }

	UFUNCTION(BlueprintImplementableEvent)
		void PlayCameraShake(TSubclassOf<UCameraShake> cameraShake);

private:
	/*--------------PN_SpringArm ���� �Լ�-------------------*/
	UFUNCTION(BlueprintPure, Category = Getter)
		float GetCurrentTargetArmLength() { return _currentTargetArmLength; }


	/*-------------��ų ��Ÿ�� ���� �Լ�----------------*/
	UFUNCTION()
		void SkillCoolApply(const ESkillType& skillNum);

public:
	// ��ų �� Ÿ��
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

	// ��ų�� Ȱ��ȭ �Ǿ� �ִ� ��������
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

	// ��ų ���� ���� �ǰ� �ִ���
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

	// ��ų�� ���� �� Ÿ��
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


	/////////////////////*------------- ��ų ���� -------------*/////////////////////
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


	/*-------------Ultimate ����---------------*/
	UFUNCTION()
		void SetUsableUltimate();
	UFUNCTION()
		void UltimateStart();
	UFUNCTION()
		void UltimateEnd();


	/*-------------------Attack(�⺻����) ����----------------------*/
	UFUNCTION()
		void ComboAttack();
	UFUNCTION()
		void CanInputNextCombo();
	UFUNCTION()
		void AttackComboCheck();

	UFUNCTION() // ���ݿ� �ݸ����� ���� ���͹迭 ����
		void ClearWeaponOverlapActor();
	UFUNCTION() // ���ݿ� �ݸ��� ����
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
	/* ---------- Kick(������) ���� ---------- */
	UFUNCTION()
		void StartKick();
	UFUNCTION() // ������� �ݸ��� ���ֱ�
		void OnKickCollision();
	UFUNCTION()
		void KickEnd();

private:

	/*-------------Pierce(�ϼ�) ����---------------*/
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


	/*-------------SwiftSlash(����) ����---------------*/
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


	/*-------------Evade ����---------------*/
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


	/*-------------Ghost Sword ���� ------------*/
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
		

	///////////*------------- Damaged : ����, �����̻� ���� ---------------*///////////
public:
	/* Stiffen : ���� �⺻ ���ݿ� �ǰݽ� ����(����) */
	UFUNCTION()
		void SetStiffen(float stiffenTime);
	UFUNCTION()
		void GetFreeOfStiffen();

	/* Faint : ���� Ư�� ���Ͽ� �ǰݽ� ���� ����(ȥ��) */
	UFUNCTION()
		void SetFaint(float faintTime);
	UFUNCTION()
		void GetFreeOfFaint();
	UFUNCTION()
		void DefianceFaint();

	/* KnockBack : ���� Ư�� ���Ͽ� �ǰݽ� �˹� ���� */
	UFUNCTION()
		void SetKnockBack();
	//UFUNCTION()
	//	void GetFreeOfKnockBack();

	/* Fury : ���� Ư�� ���Ͽ� �ǰݽ� ���� ����  */
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

	/* Addicted : ���� Ư�� ���Ͽ� �ǰݽ� �ߵ� ���� */
	UFUNCTION()
		void SetAddicted();
	UFUNCTION()
		void PoisonDamaged();
	UFUNCTION()
		void GetFreeOfAddicted();

public:
	/*-------------Skill Lock(��ų ���) ����---------------*/
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
	// �÷��̾��� �Է�(������, ��, ����)�� ������, ���� ������ ���ϴ� �Լ�
	UFUNCTION(BlueprintCallable)
	void SetEnableInput(bool canMove = true, bool canTurn = true, bool canAttack = true);

	// ���콺 ���� ����
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
		void InitializeVelocity(); // ������ ĳ���� velocity ������� �ʱ�ȭ

	/*-------------Die ����---------------*/
	UFUNCTION()
		void OnDie();
	UFUNCTION()
		void OpenGameOverLevel();

	/////////////////////////* Ŀ�ǵ� */////////////////////////
public:
	UFUNCTION()
		void SetDie(); // �ٷ� ����
	UFUNCTION()
		void SetFullHP(); // Ǯ��
	UFUNCTION()
		void SkillCoolClear(); // ��� ��ų �� ����


public: // �������Ʈ �̺�Ʈ
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
		float InflictDamage(ESkillType skillType, AEnemyAI* DamageVictim); // �������� ������ �Լ�

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public: // Getter

	/** Returns CameraBoom subobject **/
	FORCEINLINE class UPN_SpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UPN_CameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE class UHpComponent* GetHpComponent() const { return HpComponent; }

	FORCEINLINE class UUltimateComponent* GetUltimateComponent() const { return UltimateComponent; }



	/** �⺻ Ÿ�پϷ��� ���� ��ȯ**/
	float GetTargetArmLengthOrigin() const { return _targetArmLengthOrigin; }

public:/* ��Ÿ get �Լ��� */
	UFUNCTION()
		int32 GetCurrentCombo(); // �⺻���� �޺�

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

	/*-----------�ý��� ���� Get, Set-------------*/
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


public:/* ������ ���� get �Լ� */
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
