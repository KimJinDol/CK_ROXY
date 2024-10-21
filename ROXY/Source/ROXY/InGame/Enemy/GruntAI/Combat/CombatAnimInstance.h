// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAnimInstance.h"
#include "CombatAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FCombatAI_AnimNotifyDelegate);
/**
 * 
 */
UCLASS()
class ROXY_API UCombatAnimInstance : public UGruntAnimInstance
{
	GENERATED_BODY()
	
public:
	UCombatAnimInstance();

	FCombatAI_AnimNotifyDelegate OnAttackSound;

	FCombatAI_AnimNotifyDelegate OnPushOut;
	FCombatAI_AnimNotifyDelegate OnPushOutEnd;

private:
	class ACombatAI* CombatAI;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PushOut, Meta = (AllowPrivateAccess = true))
		UAnimMontage* PushOutMontage;

public:
	void PlayPushOutMontage();

private:
	UFUNCTION()
		void AnimNotify_PlayAttackSound();

	UFUNCTION()
		void AnimNotify_PushOutCheck();
	UFUNCTION()
		void AnimNotify_EndPushOut();
	
private:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

};
