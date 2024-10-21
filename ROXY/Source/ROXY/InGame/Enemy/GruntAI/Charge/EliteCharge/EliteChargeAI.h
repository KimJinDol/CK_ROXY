// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/Charge/ChargeAI.h"
#include "EliteChargeAI.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API AEliteChargeAI : public AChargeAI
{
	GENERATED_BODY()
	
public:
	AEliteChargeAI();

	FChargeAI_Delegate OnProvokeTaskEnd;

private:
	// 엘리트 특수 패턴 Provoke
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Provoke, Meta = (AllowPrivateAccess = true))
		bool _bIsProvoke = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Provoke, Meta = (AllowPrivateAccess = true))
		bool _bCanProvoke = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Provoke, Meta = (AllowPrivateAccess = true))
		bool _bProvokeCheck = false; // 도발에 성공했는가?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Provoke, Meta = (AllowPrivateAccess = true))
		float _provokeRange = 10.0f;

public:
	UFUNCTION()
		void StartProvoke();
	UFUNCTION()
		void Provoke();
	UFUNCTION()
		void EndProvoke();

protected:
	virtual void Die() override;
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
