// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAnimInstance.h"
#include "EliteThrowAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FEliteThrowAI_AnimNotifyDelegate);
/**
 * 
 */
UCLASS()
class ROXY_API UEliteThrowAnimInstance : public UGruntAnimInstance
{
	GENERATED_BODY()

public:
	UEliteThrowAnimInstance();

	FEliteThrowAI_AnimNotifyDelegate OnShoot;
	FEliteThrowAI_AnimNotifyDelegate OnShootEnd;

	FEliteThrowAI_AnimNotifyDelegate OnMeteor;
	FEliteThrowAI_AnimNotifyDelegate OnMeteorEnd;

private:
	class AEliteThrowAI* EliteThrowAI;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shoot, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Meteor, Meta = (AllowPrivateAccess = true))
		UAnimMontage* MeteorMontage;

public:
	void PlayShootMontage(float value = 1.0f);

	void PlayMeteorMontage();

private:
	UFUNCTION()
		void AnimNotify_ShootCheck();
	UFUNCTION()
		void AnimNotify_EndShoot();

	UFUNCTION()
		void AnimNotify_MeteorCheck();
	UFUNCTION()
		void AnimNotify_MeteorEnd();

private:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

};
