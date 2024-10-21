// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "GameFramework/Actor.h"
#include "EliteThrowAISkullBomb.generated.h"

UCLASS()
class ROXY_API AEliteThrowAISkullBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	AEliteThrowAISkullBomb();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, Meta = (AllowPrivateAccess = true))
		class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, Meta = (AllowPrivateAccess = true))
		class UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, Meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* PC_Particle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Particle, Meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_SkullBomb;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Particle, Meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_BombExplosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_Bomb;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Meteor, Meta = (AllowPrivateAccess = true))
		float _bombSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damage, Meta = (AllowPrivateAccess = true))
		float _bombDamageToRoxy = 3000.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damage, Meta = (AllowPrivateAccess = true))
		float _bombDamageToEnemy = 6000.0;

public:
	void SetDropDirection(const FVector& direction, float speed);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
	void DestroyBomb();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

};
