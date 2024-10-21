// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "BuffAI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBuffAI_Delegate);
/**
 * 
 */
UCLASS()
class ROXY_API ABuffAI : public AGruntAI
{
	GENERATED_BODY()
	
public:
	ABuffAI();

private:
	UPROPERTY()
		class UBuffAnimInstance* BuffAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = true))
		class UMaterial* StaffOriginMat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = true))
		class UMaterialInstance* StaffOpacityMat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* PC_ParticleComponent_02;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_WizardShield;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Invincible, Meta = (AllowPrivateAccess = true))
		bool _bIsFirstInvincible = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Invincible, Meta = (AllowPrivateAccess = true))
		bool _bIsInvincible = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Invincible, Meta = (AllowPrivateAccess = true))
		int32 _minGruntNum = 3; // 무적 패턴을 실행하기 위한 레벨 내 잡몹의 최소 마리 수
	UPROPERTY()
		FTimerHandle _invinTimer;

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite, Category = Delegate)
		FBuffAI_Delegate OnFirstInvincibility;

public:
	UFUNCTION()
		void StartInvincibility();
	UFUNCTION()
		void MakeInvincible();
	UFUNCTION()
		void EndInvincibility();

private:
	virtual void Die() override;
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	
};
