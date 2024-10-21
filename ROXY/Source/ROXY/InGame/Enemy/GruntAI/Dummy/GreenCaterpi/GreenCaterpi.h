// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "GreenCaterpi.generated.h"

/**
 *
 */
UCLASS()
class ROXY_API AGreenCaterpi : public AGruntAI
{
	GENERATED_BODY()

public:
	AGreenCaterpi();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
		class UCaterpiAnimInstance* CaterpiAnim;

	virtual void Die();

private:
	UPROPERTY()
		FTimerHandle OnTimer;

public:

	virtual void PostInitializeComponents() override;


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
