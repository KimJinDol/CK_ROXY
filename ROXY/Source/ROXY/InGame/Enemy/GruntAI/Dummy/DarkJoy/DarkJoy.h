// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "DarkJoy.generated.h"

DECLARE_MULTICAST_DELEGATE(FDarkJoy_Delegate);
/**
 * 
 */
UCLASS()
class ROXY_API ADarkJoy : public AGruntAI
{
	GENERATED_BODY()
	
public:
	ADarkJoy();

	// µ®∏Æ∞‘¿Ã∆Æ
	FDarkJoy_Delegate OnAttackTaskEnd;

private:
	UPROPERTY()
		class UDarkJoyAnimInstance* DarkJoyAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		class UCapsuleComponent* WeaponCollision;

protected:
	virtual void Die() override;

	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
		bool _bCanMove = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Detect, Meta = (AllowPrivateAccess = true))
		bool _bCanDetect = true;

	UFUNCTION()
		void DestoyActor();

public: // Get, Set

	UFUNCTION()
		bool GetMovable();
	UFUNCTION()
		void SetMovable(bool value);

	UFUNCTION()
		bool GetCanDetect();
};
