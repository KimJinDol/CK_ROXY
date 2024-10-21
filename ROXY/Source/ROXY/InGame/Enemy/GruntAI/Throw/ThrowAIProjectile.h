// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "GameFramework/Actor.h"
#include "ThrowAIProjectile.generated.h"

UENUM()
enum class EBulletType : uint8 /* 투사체 타입 */
{
	B_ORIGIN = 1, // 기본 투사체
	B_SKULL // 특수패턴 투사체
};

UCLASS()
class ROXY_API AThrowAIProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AThrowAIProjectile();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, Meta = (AllowPrivateAccess = true))
		class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, Meta = (AllowPrivateAccess = true))
		class UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, Meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* PC_Particle;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Particle, Meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Bullet; // 기본 투사체 이펙트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Particle, Meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_SpecialBullet; // 특수패턴 스컬슛을 위한 투사체 이펙트

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Material, Meta = (AllowPrivateAccess = true))
		class UMaterial* SphereMaterial;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Shoot, Meta = (AllowPrivateAccess = true))
		float _projectileSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = true))
		int32 _roxySkillNum;

public:
	void SetShootDirection(const FVector& direction, float speed);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void SetBulletEffect(EBulletType bulletType);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

};
