// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/Combat/CombatAI.h"
#include "EliteCombatAI.generated.h"

DECLARE_MULTICAST_DELEGATE(FCombatAI_Delegate);
/**
 * 
 */
UCLASS()
class ROXY_API AEliteCombatAI : public ACombatAI
{
	GENERATED_BODY()
	
public:
	AEliteCombatAI();

	FCombatAI_Delegate OnPushOutTaskEnd;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_Growl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_Spawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_EliteCombat_Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_EliteCombat_Death;

private:
	// 엘리트 특수 패턴 Push Out
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PushOut, Meta = (AllowPrivateAccess = true))
		bool _bIsPushOut = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PushOut, Meta = (AllowPrivateAccess = true))
		bool _bCanPushOut = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PushOut, Meta = (AllowPrivateAccess = true))
		float _pushOutRange = 900.0f;

public:
	UFUNCTION()
		void PlayAttackSound();
	
	UFUNCTION()
		void StartPushOut();
	UFUNCTION()
		void PushOut();
	UFUNCTION()
		void EndPushOut();


protected:
	virtual void Die() override;
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
};
