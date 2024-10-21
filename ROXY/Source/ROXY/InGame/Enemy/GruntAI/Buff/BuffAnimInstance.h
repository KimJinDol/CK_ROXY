// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAnimInstance.h"
#include "BuffAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FBuffAI_AnimNotifyDelegate);
/**
 * 
 */
UCLASS()
class ROXY_API UBuffAnimInstance : public UGruntAnimInstance
{
	GENERATED_BODY()
	
public:
	UBuffAnimInstance();

	FBuffAI_AnimNotifyDelegate OnInvincibility;
	FBuffAI_AnimNotifyDelegate OnInvincibilityEnd;

private:
	class ABuffAI* BuffAI;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Invincible, Meta = (AllowPrivateAccess = true))
		UAnimMontage* InvincibleMontage;
	
public:
	UFUNCTION()
		void PlayInvinciMontage();

private:
	UFUNCTION()
		void AnimNotify_InvincibleCheck();
	UFUNCTION()
		void AnimNotify_InvincibleEnd();

private:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
};
