// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "GameFramework/Actor.h"
#include "BossProjectile.generated.h"

DECLARE_DELEGATE(FBossProjectile_Delegate);

UCLASS()
class ROXY_API ABossProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, Meta = (AllowPrivateAccess = true))
		class UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, Meta = (AllowPrivateAccess = true))
		class USphereComponent* SphereCollision;

	UPROPERTY(VisibleDefaultsOnly, Category = Component, Meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* PC_Particle;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Particle;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Hit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		class USoundCue* SC_Shoot;

public:
	FBossProjectile_Delegate EndDelayActive;
public:	
	virtual void PostInitializeComponents() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDamageInstigatorInfo(AController* controller, AActor* actor);
	void SetActiveAfterDelay(float delay, float speed);
	void SetCollisionEnable(bool value);
	void SetCollisionRadius(float radius);
	void SetVelocity(float velocity);
	void SetAccelerate(bool value);
	void SetRotation(FRotator rotation);

	void PlayProjectileSound();

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Activa, Meta = (AllowPrivateAccess = true))
		bool _bIsAccelerate = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damage, Meta = (AllowPrivateAccess = true))
		AController* DamageInstigator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damage, Meta = (AllowPrivateAccess = true))
		AActor* DamageCauser;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damage, Meta = (AllowPrivateAccess = true))
		float _damage = 4000.0f;
};
