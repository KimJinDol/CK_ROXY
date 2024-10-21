// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAnimInstance.h"
#include "ChargeAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FChargeAI_AnimNotifyDelegate);
/**
 * 
 */
UCLASS()
class ROXY_API UChargeAnimInstance : public UGruntAnimInstance
{
	GENERATED_BODY()
	
public:
	UChargeAnimInstance();

	FChargeAI_AnimNotifyDelegate OnQuake;
	FChargeAI_AnimNotifyDelegate OnQuakeEnd;

	FChargeAI_AnimNotifyDelegate OnProvoke;
	FChargeAI_AnimNotifyDelegate OnProvokeEnd;

private:
	class AChargeAI* ChargeAI;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quake, Meta = (AllowPrivateAccess = true))
		UAnimMontage* QuakeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Provoke, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ProvokeMontage;

public:
	UFUNCTION()
		void PlayQuakeMontage();

	UFUNCTION()
		void PlayProvokeMontage();

private:
	UFUNCTION()
		void AnimNotify_QuakeCheck();
	UFUNCTION()
		void AnimNotify_QuakeEnd();

	UFUNCTION()
		void AnimNotify_ProvokeCheck();
	UFUNCTION()
		void AnimNotify_ProvokeEnd();

private:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
};
