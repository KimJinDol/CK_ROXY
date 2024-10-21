// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Animation/AnimInstance.h"
#include "InGame/Enemy/Boss/NunuBossAI.h"
#include "BossAnimInstance.generated.h"

DECLARE_DELEGATE(FBoss_AnimNotifyDelegate);
/**
 * 
 */
UCLASS()
class ROXY_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UBossAnimInstance();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Instance, Meta = (AllowPrivateAccess = true))
	class ANunuBossAI* Owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Instance, Meta = (AllowPrivateAccess = true))
	EBossProjectileType OwnerProjType;
	

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION()
	void AnimNotify_RiseStart();

	UFUNCTION()
	void AnimNotify_EndEscapeStart();
	UFUNCTION()
	void AnimNotify_EndEscape();

	UFUNCTION()
	void AnimNotify_EndAvoidance();


	UFUNCTION()
	void AnimNotify_ShotProj_Horizontal();
	UFUNCTION()
	void AnimNotify_ShotProj_Fake();
	UFUNCTION()
	void AnimNotify_ShotProj_Delay();
	UFUNCTION()
	void AnimNotify_ShotProj_Homing();
	UFUNCTION()
	void AnimNotify_EndShotProj();
	UFUNCTION()
	void AnimNotify_RangeAttackCheck();

public:
	FBoss_AnimNotifyDelegate OnRiseStart;

	FBoss_AnimNotifyDelegate OnEndEscapeStartAnim;
	FBoss_AnimNotifyDelegate OnEndEscape;
	
	FBoss_AnimNotifyDelegate OnShotProj_Horizontal;
	FBoss_AnimNotifyDelegate OnShotProj_Fake;
	FBoss_AnimNotifyDelegate OnShotProj_Delay;
	FBoss_AnimNotifyDelegate OnShotProj_Homing;
	FBoss_AnimNotifyDelegate OnEndShotProj;

	FBoss_AnimNotifyDelegate OnRangeAttackCheck;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rise)
	bool bIsRise = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Escape)
	bool bIsEscapeFromPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Avoidance)
	bool bIsAvoidance = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProjectile)
	bool bIsShotProjectile = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProjectile)
	bool bIsShotProjectile_Horizontal = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProjectile)
	bool bIsShotProjectile_Fake = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProjectile)
	bool bIsShotProjectile_Delay = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShopProjectile)
	bool bIsShotProjectile_Homing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RangeAttack)
	bool bIsRangeAttack = false;

};
