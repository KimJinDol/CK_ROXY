
#pragma once

#include "ProjectNunu.h"
#include "Animation/AnimInstance.h"
#include "RoxyAnimInstance.generated.h"

UENUM() // ����ġ ����
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
	/* �⺻ ���� */
	FRoxy_AnimNotifyDelegate OnCanInputNextCombo;
	FRoxy_AnimNotifyDelegate OnComboCheck;

	FRoxy_AnimNotifyDelegate OnClearActor;
	FRoxy_AnimNotifyDelegate OnOffCollision;
	/* ������ */
	FRoxy_AnimNotifyDelegate OnKickCheck;
	FRoxy_AnimNotifyDelegate OnKickEnd;

	/* �ϼ� */
	FRoxy_AnimNotifyDelegate OnPierceCheck;
	FRoxy_AnimNotifyDelegate OnStopPierce;
	FRoxy_AnimNotifyDelegate OnPierceEnd;

	/* ���� */
	FRoxy_AnimNotifyDelegate OnSwiftSlash;
	FRoxy_AnimNotifyDelegate OnSwiftSlashEnd;

	/* �뽬 */
	FRoxy_AnimNotifyDelegate OnEvadeCheck;
	FRoxy_AnimNotifyDelegate OnGhostTrail;
	FRoxy_AnimNotifyDelegate OnStopLaunch;
	FRoxy_AnimNotifyDelegate OnEvadeEnd;

	/* ȯ���� */
	FRoxy_AnimNotifyDelegate OnInstallSword;
	FRoxy_AnimNotifyDelegate OnEndInstallSword;
	FRoxy_AnimNotifyDelegate OnRushTowardSword;

	/* ������ ���� */
	FRoxy_AnimNotifyDelegate OnStepCheck;

private:
	class ARoxyCharacter* Roxy;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage; /* �⺻ ���� */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* KickMontage; /* ������ */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pierce, Meta = (AllowPrivateAccess = true))
		UAnimMontage* PierceMontage; /* �ϼ� */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = SwiftSlash, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SwiftSlashMontage; /* ���� */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Evade, Meta = (AllowPrivateAccess = true))
		UAnimMontage* EvadeMontage; /* �뽬 */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hit, Meta = (AllowPrivateAccess = true))
		UAnimMontage* HitMontage; /* �ǰ� */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Sheath, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SheathMontage; /* ���� */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		UAnimMontage* GhostSwordInstallMontage; /* ȯ���� ��ġ */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = GhostSword, Meta = (AllowPrivateAccess = true))
		UAnimMontage* GhostSwordMontage; /* ȯ���� ���� */

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
	/* �⺻ ���� */
	UFUNCTION()
		void AnimNotify_CanInputNextCombo();
	UFUNCTION()
		void AnimNotify_ComboCheck();

	UFUNCTION()
		void AnimNotify_ClearOverlapActor();
	UFUNCTION()
		void AnimNotify_OffAttackCollision();
	/* ������ */
	UFUNCTION()
		void AnimNotify_KickCheck();
	UFUNCTION()
		void AnimNotify_EndKick();

	/* �ϼ� */
	UFUNCTION()
		void AnimNotify_PierceCheck();
	UFUNCTION()
		void AnimNotify_StopPierce();
	UFUNCTION()
		void AnimNotify_EndPierce();

	/* ���� */
	UFUNCTION()
		void AnimNotify_SwiftSlash();
	UFUNCTION()
		void AnimNotify_EndSwiftSlash();

	/* �뽬 */
	UFUNCTION()
		void AnimNotify_EvadeCheck();
	UFUNCTION()
		void AnimNotify_GhostTrail();
	UFUNCTION()
		void AnimNotify_StopLaunch();
	UFUNCTION()
		void AnimNotify_EndEvade();

	/* ȯ���� */
	UFUNCTION()
		void AnimNotify_InstallSword();
	UFUNCTION()
		void AnimNotify_EndInstallSword();
	UFUNCTION()
		void AnimNotify_RushTowardSword();

	/* ������ ���� */
	UFUNCTION()
		void AnimNotify_StepCheck(); // ������ üũ

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
