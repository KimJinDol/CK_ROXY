// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAnimInstance.h"
#include "RevivalAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FRevivalAI_AnimNotifyDelegate);
/**
 * 
 */
UCLASS()
class ROXY_API URevivalAnimInstance : public UGruntAnimInstance
{
	GENERATED_BODY()
	
public:
	URevivalAnimInstance();

private:
	class ARevivalAI* RevivalAI;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Revival, Meta = (AllowPrivateAccess = true))
		UAnimMontage* RevivalMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Revival, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ShootMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Revival, Meta = (AllowPrivateAccess = true))
		UAnimMontage* TiredMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Tired, Meta = (AllowPrivateAccess = true))
		float _restTiredAnimMonTime= 0.f;

public:
	FRevivalAI_AnimNotifyDelegate OnRevive;
	FRevivalAI_AnimNotifyDelegate OnEndRevive;
	FRevivalAI_AnimNotifyDelegate OnShootProj;
	FRevivalAI_AnimNotifyDelegate OnEndShootProj;
	FRevivalAI_AnimNotifyDelegate OnDescent;
	FRevivalAI_AnimNotifyDelegate OnRising;
	FRevivalAI_AnimNotifyDelegate OnTired;
	FRevivalAI_AnimNotifyDelegate OnEndTakeDamage;
	FRevivalAI_AnimNotifyDelegate OnEndTired;
	FRevivalAI_AnimNotifyDelegate OnDie;
	FRevivalAI_AnimNotifyDelegate OnEndDie;

private:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

public:
	UFUNCTION()
		void AnimNotify_OnRevive();
	UFUNCTION()
		void AnimNotify_EndRevive();
	UFUNCTION()
		void AnimNotify_OnShootProj();
	UFUNCTION()
		void AnimNotify_EndShootProj();
	UFUNCTION()
		void AnimNotify_OnDescent();
	UFUNCTION()
		void AnimNotify_OnRising();
	UFUNCTION()
		void AnimNotify_OnTired();
	UFUNCTION()
		void AnimNotify_EndTakeDamage();
	UFUNCTION()
		void AnimNotify_EndTired();
	UFUNCTION()
		void AnimNotify_EndSwiftHit();
	UFUNCTION()
		void AnimNotify_OnDie();
	UFUNCTION()
		void AnimNotify_EndDie();

	UFUNCTION()
		void PlayRevivalMontage();
	UFUNCTION()
		void PlayAttackMontage();
	UFUNCTION()
		void PlayShootProjMontage();
	UFUNCTION()
		void PlayTiredMontage();
	UFUNCTION()
		void PlaySwiftHitMontage();
};
