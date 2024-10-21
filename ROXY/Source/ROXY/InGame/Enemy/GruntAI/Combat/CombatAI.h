// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "CombatAI.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API ACombatAI : public AGruntAI
{
	GENERATED_BODY()
	
public:
	ACombatAI();

protected:
	UPROPERTY()
		class UCombatAnimInstance* CombatAnim;

protected:
	virtual void Die() override;

	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
};
