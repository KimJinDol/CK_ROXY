
#pragma once

#include "ProjectNunu.h"
#include "Animation/AnimInstance.h"
#include "RoxyAnimInstance.generated.h"

UENUM() // 스내치 상태
enum class ESnatchState {
	EPUSH,
	EPULL
};

DECLARE_MULTICAST_DELEGATE(FRoxy_AnimNotifyDelegate);

/**
 * 
 */
UCLASS()
class ROXY_API URoxyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	URoxyAnimInstance();

public:
	/* 기본 공격 */
	FRoxy_AnimNotifyDelegate OnCanInputNextCombo;
	FRoxy_AnimNotifyDelegate OnComboCheck;

	FRoxy_AnimNotifyDelegate OnClearActor;
	FRoxy_AnimNotifyDelegate OnOffCollision;
	/* 발차기 */
	FRoxy_AnimNotifyDelegate OnKickCheck;
	FRoxy_AnimNotifyDelegate OnKickEnd;

	/* 일섬 */
	FRoxy_AnimNotifyDelegate OnPierceCheck;
	FRoxy_AnimNotifyDelegate OnStopPierce;
	FRoxy_AnimNotifyDelegate OnPierceEnd;

	/* 연참 */
	FRoxy_AnimNotifyDelegate OnSwiftSlash;
	FRoxy_AnimNotifyDelegate OnSwiftSlashEnd;

	/* 대쉬 */
	FRoxy_AnimNotifyDelegate OnEvadeCheck;
	FRoxy_AnimNotifyDelegate OnGhostTrail;
	FRoxy_AnimNotifyDelegate OnStopLaunch;
	FRoxy_AnimNotifyDelegate OnEvadeEnd;

	/* 환영검 */
	FRoxy_AnimNotifyDelegate OnInstallSword;
	FRoxy_AnimNotifyDelegate OnEndInstallSword;
	FRoxy_AnimNotifyDelegate OnRushTowardSword;

	/* 데이터 관련 */
	FRoxy_AnimNotifyDelegate OnStepCheck;

private:
	class ARoxyCharacter* Roxy;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage; /* 기본 공격 */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* KickMontage; /* 발차기 */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		UAnimMontage* PierceMontage; /* 일섬 */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = SwiftSlash, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SwiftSlashMontage; /* 연참 */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		UAnimMontage* EvadeMontage; /* 대쉬 */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hit, Meta = (AllowPrivateAccess = true))
		UAnimMontage* HitMontage; /* 피격 */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Sheath, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SheathMontage; /* 납도 */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		UAnimMontage* GhostSwordInstallMontage; /* 환영검 설치 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		UAnimMontage* GhostSwordMontage; /* 환영검 실행 */

public:
	void PlayAttackMontage();
	void JumpToAttackMontageSection(float newSection);
	void StopAttackMontage();

	void PlayKickMontage();
	void StopKickMontage();

	void PlayPierceMontage();

	void PlaySwiftSlashMontage();
	
	void PlayEvadeMontage();

	void PlayHitMontage();

	void SetGroggyAnim(bool value) { _bAnimIsGroggy = value; }

	void SetDieAnim() { _bAnimInDie = true; }

	void PlaySheathMontage();

	void PlayGhostSwordInstallMontage();
	void PlayGhostSwordMontage();

private:
	/* 기본 공격 */
	UFUNCTION()
		void AnimNotify_CanInputNextCombo();
	UFUNCTION()
		void AnimNotify_ComboCheck();

	UFUNCTION()
		void AnimNotify_ClearOverlapActor();
	UFUNCTION()
		void AnimNotify_OffAttackCollision();
	/* 발차기 */
	UFUNCTION()
		void AnimNotify_KickCheck();
	UFUNCTION()
		void AnimNotify_EndKick();

	/* 일섬 */
	UFUNCTION()
		void AnimNotify_PierceCheck();
	UFUNCTION()
		void AnimNotify_StopPierce();
	UFUNCTION()
		void AnimNotify_EndPierce();

	/* 연참 */
	UFUNCTION()
		void AnimNotify_SwiftSlash();
	UFUNCTION()
		void AnimNotify_EndSwiftSlash();

	/* 대쉬 */
	UFUNCTION()
		void AnimNotify_EvadeCheck();
	UFUNCTION()
		void AnimNotify_GhostTrail();
	UFUNCTION()
		void AnimNotify_StopLaunch();
	UFUNCTION()
		void AnimNotify_EndEvade();

	/* 환영검 */
	UFUNCTION()
		void AnimNotify_InstallSword();
	UFUNCTION()
		void AnimNotify_EndInstallSword();
	UFUNCTION()
		void AnimNotify_RushTowardSword();

	/* 데이터 관련 */
	UFUNCTION()
		void AnimNotify_StepCheck(); // 걸음수 체크

public:
	FName GetAttackMontageSectionName(int32 section);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Instance, Meta = (AllowPrivateAccess = true))
		float _currentSpeedRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Instance, Meta = (AllowPrivateAccess = true))
		float _currentForwardSpeedRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Instance, Meta = (AllowPrivateAccess = true))
		float _currentRightSpeedRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Instance, Meta = (AllowPrivateAccess = true))
		bool _bOnMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bAnimIsDamaged = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bAnimIsGroggy = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damaged, Meta = (AllowPrivateAccess = true))
		bool _bAnimInDie = false;


	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

};
