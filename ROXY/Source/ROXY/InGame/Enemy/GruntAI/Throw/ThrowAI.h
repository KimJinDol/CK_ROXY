// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "ThrowAI.generated.h"

DECLARE_MULTICAST_DELEGATE(FThrowAI_Delegate);
/**
 * 
 */
UCLASS()
class ROXY_API AThrowAI : public AGruntAI
{
	GENERATED_BODY()
	
public:
	AThrowAI();

	FThrowAI_Delegate OnShootTaskEnd;
	FThrowAI_Delegate OnSkullShootTaskEnd;

protected:
	UPROPERTY()
		class UThrowAnimInstance* ThrowAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Shoot, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class AThrowAIProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Shoot, Meta = (AllowPrivateAccess = true))
		AThrowAIProjectile* ThrowAIProjectile;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shoot, Meta = (AllowPrivateAccess = true))
		FVector _muzzleLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shoot, Meta = (AllowPrivateAccess = true))
		FRotator _muzzleRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shoot, Meta = (AllowPrivateAccess = true))
		FVector _targetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shoot, Meta = (AllowPrivateAccess = true))
		FVector _projectileLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shoot, Meta = (AllowPrivateAccess = true))
		float _projectileSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shoot, Meta = (AllowPrivateAccess = true))
		float _shootRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shoot, Meta = (AllowPrivateAccess = true))
		float _shootStiffenTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shoot, Meta = (AllowPrivateAccess = true))
		float _projectileLifeTime = 0.45f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shoot, Meta = (AllowPrivateAccess = true))
		float _canShootDist; // 타겟에게 투척 가능 한 거리, 투사체가 닿는 거리
	
	// 기본 Shoot
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shoot, Meta = (AllowPrivateAccess = true))
		bool _bIsShooting = false;

	// 특수 패턴 Skull
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkullShoot, Meta = (AllowPrivateAccess = true))
		bool _bIsSkullShooting = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkullShoot, Meta = (AllowPrivateAccess = true))
		bool _bCanSkullShoot = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkullShoot, Meta = (AllowPrivateAccess = true))
		float _skullShootCoolTime = 5.0f;
	UPROPERTY()
		FTimerHandle _skullShootTimer;

public:
	// 기본 Shoot
	UFUNCTION()
		void StartShoot();
	UFUNCTION()
		void Shoot();
	UFUNCTION()
		void EndShoot();

	// 특수 패턴 Skull
	UFUNCTION()
		void StartSkullShoot();
	UFUNCTION()
		void SkullShoot();
	UFUNCTION()
		void EndSkullShoot();

protected:
	virtual void Die() override;

	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

/* Get, Set */
public:
	UFUNCTION()
		float GetDamage() { return _damage_Attack; }
	UFUNCTION()
		float GetProjectileStiffenTime() { return _shootStiffenTime; }
	UFUNCTION()
		float GetSpecialAttackDamage() { return _damage_SpecialAttack; }

	UFUNCTION()
		bool GetIsBasicShoot() { return _bIsShooting; }

	UFUNCTION()
		bool GetIsSkullShoot() { return _bIsSkullShooting; }
	UFUNCTION()
		bool GetIsCanSkullShoot() { return _bCanSkullShoot; }
	
	UFUNCTION()
		float GetCanShootRange() { return _canShootDist;}
	
};
