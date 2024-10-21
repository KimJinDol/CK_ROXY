// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "ChargeAI.generated.h"

DECLARE_MULTICAST_DELEGATE(FChargeAI_Delegate);
/**
 * 
 */
UCLASS()
class ROXY_API AChargeAI : public AGruntAI
{
	GENERATED_BODY()
	
public:
	AChargeAI();

	FChargeAI_Delegate OnQuakeTaskEnd;

protected:
	UPROPERTY()
		class UChargeAnimInstance* ChargeAnim;

private:
	// 특수 패턴 Quake
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quake, Meta = (AllowPrivateAccess = true))
		bool _bIsQuaking = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quake, Meta = (AllowPrivateAccess = true))
		bool _bCanQuake = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quake, Meta = (AllowPrivateAccess = true))
		float _quakeRange = 10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float _currentHP;
		
public:
	UFUNCTION()
		void StartQuake();
	UFUNCTION()
		void Quake();
	UFUNCTION()
		void EndQuake();

protected:
	virtual void Die() override;

	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

/* Get, Set */
public:
	UFUNCTION()
		bool GetIsQuake() { return _bIsQuaking; }
	UFUNCTION()
		bool GetIsCanQuake() { return _bCanQuake; }

};
