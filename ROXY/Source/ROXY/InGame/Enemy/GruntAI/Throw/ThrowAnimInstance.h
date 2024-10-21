// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGame/Enemy/GruntAI/GruntAnimInstance.h"
#include "ThrowAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FThrowAI_AnimNotifyDelegate);
/**
 * 
 */
UCLASS()
class ROXY_API UThrowAnimInstance : public UGruntAnimInstance
{
	GENERATED_BODY()
	
public:
	UThrowAnimInstance();

	FThrowAI_AnimNotifyDelegate OnShoot;
	FThrowAI_AnimNotifyDelegate OnShootEnd;

	FThrowAI_AnimNotifyDelegate	OnSkullShoot;
	FThrowAI_AnimNotifyDelegate OnSkullShootEnd;

	FThrowAI_AnimNotifyDelegate OnMeteor;
	FThrowAI_AnimNotifyDelegate OnMeteorEnd;

private:
	class AThrowAI* ThrowAI;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shoot, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkullShoot, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SkullShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Meteor, Meta = (AllowPrivateAccess = true))
		UAnimMontage* MeteorMontage;

public:
	void PlayShootMontage(float value = 1.0f);

	void PlaySkullShootMontage(float value = 1.0f);

	void PlayMeteorMontage();

private:
	UFUNCTION()
		void AnimNotify_ShootCheck();
	UFUNCTION()
		void AnimNotify_EndShoot();

	UFUNCTION()
		void AnimNotify_SkullShootCheck();
	UFUNCTION()
		void AnimNotify_EndSkullShoot();

	UFUNCTION()
		void AnimNotify_MeteorCheck();
	UFUNCTION()
		void AnimNotify_MeteorEnd();

private:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
};
