// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Animation/AnimInstance.h"
#include "GruntAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(GruntAnimNotify_Delegate);
DECLARE_MULTICAST_DELEGATE_OneParam(GruntAnimNotify_ParamDelegate, bool);

UENUM()
enum class EHitDirection{
	HD_F, // 앞 (X축 양의 방향)0
	HD_B, // 뒤 (X축 음의 방향)
	HD_L, // 좌 (Y축 음의 방향)
	HD_R,  // 우 (Y축 양의 방향)
};
/**
 * 
 */
UCLASS()
class ROXY_API UGruntAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UGruntAnimInstance();

	GruntAnimNotify_Delegate OnAttackCheck;
	GruntAnimNotify_Delegate OnAttackCheckEnd;
	GruntAnimNotify_Delegate OnOffAttackCollision;
	GruntAnimNotify_Delegate OnAttackEnd;

	GruntAnimNotify_Delegate OnKnockbackEnd;

	GruntAnimNotify_Delegate OnChangeToOpacityMat;
	GruntAnimNotify_Delegate OnDieAnimEnd;

	GruntAnimNotify_ParamDelegate OnSpawnAnimStart;
	GruntAnimNotify_ParamDelegate OnSpawnAnimEnd;

private:
	class AGruntAI* Grunt;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BasicAnim, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BasicAnim, Meta = (AllowPrivateAccess = true))
		UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BasicAnim, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SideHitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BasicAnim, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SwiftHitMontage; // 연참에 맞을 시 재생
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BasicAnim, Meta = (AllowPrivateAccess = true))
		UAnimMontage* KnockBackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BasicAnim, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SpawnMontage;

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void PlayAttackMontage();
	void StopAttackMontage();

	void PlayHitMontage(/*EHitDirection direction*/);
	void PlaySideHitMontage(EHitDirection direction);
	void PlaySwiftHitMontage();
	void PlayKnockBackMontage();

	void PlaySpawnMontage(float value);

	void SetDieAnim(bool value) { _bAnimIsDie = value; }
	void SetKnockbackDieAnim(bool value) { _bAnimIsKnockbackDie = value; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Instance, Meta = (AllowPrivateAccess = true))
		float _currentSpeedRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Die, Meta = (AllowPrivateAccess = true))
		bool _bAnimIsDie = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Die, Meta = (AllowPrivateAccess = true))
		bool _bAnimIsKnockbackDie = false;

private:
	UFUNCTION()
		void AnimNotify_AttackCheck();
	UFUNCTION()
		void AnimNotify_AttackCheckEnd();
	UFUNCTION()
		void AnimNotify_OffAttackCollision();
	UFUNCTION()
		void AnimNotify_AttackEnd();

	UFUNCTION()
		void AnimNotify_EndKnockback();

	UFUNCTION ()
		void AnimNotify_ChangeToOpacityMat();
	UFUNCTION()
		void AnimNotify_EndDieAnim();

	UFUNCTION()
		void AnimNotify_SpawnStart();
	UFUNCTION()
		void AnimNotify_SpawnEnd();

public:
	UFUNCTION()
		UAnimMontage* GetSideHitMontage() { return SideHitMontage; }
	
};
