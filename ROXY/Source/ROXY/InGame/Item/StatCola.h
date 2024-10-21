// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "GameFramework/Actor.h"
#include "StatCola.generated.h"

UENUM()
enum class EColaType
{
	CT_TENTEN,
	CT_CHUNGKANG,
	CT_VITA
};

UCLASS()
class ROXY_API AStatCola : public AActor
{
	GENERATED_BODY()
	
public:	
	AStatCola();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = NunuGameInstance, Meta = (AllowPrivateAccess = true))
		class UNunuGameInstance* NunuGameInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Owner, Meta = (AllowPrivateAccess = true))
		class AVendingMachine* OwnerVendingMachine;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
		class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, Meta = (AllowPrivateAccess = true))
		class UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, Meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* ColaMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Material, Meta = (AllowPrivateAccess = true))
		class UMaterialInstance* ColaMaterial; // �⺻ ���׸���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Material, Meta = (AllowPrivateAccess = true))
		class UMaterialInstance* HPMaterial; // ü������ �ݶ� ���׸���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Material, Meta = (AllowPrivateAccess = true))
		class UMaterialInstance* DamageMaterial; // ���ݷ����� �ݶ� ���׸���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Material, Meta = (AllowPrivateAccess = true))
		class UMaterialInstance* CoolTimeMaterial; // ��Ÿ�Ӱ��� �ݶ� ���׸���

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Particle, Meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* PC_ParticleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Drop; // �ʵ忡 �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Hororok; // ĳ���Ͱ� ȹ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_ColaDrop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_ColaGet;

private:
	UPROPERTY()
		FTimerHandle _activeColaTimer;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ColaType, Meta = (AllowPrivateAccess = true))
		EColaType _colaType;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void SetOwnerVendingMachine(AVendingMachine* pointer);

	UFUNCTION()
		void SetColaType(EColaType newColaType);

	UFUNCTION()
		void SetDropDirection(const FVector& direction);

	UFUNCTION()
		void ActiveColaCollision();

	UFUNCTION()
		void DestroyCola();

private:
	UFUNCTION()
		virtual void OnColaCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
