// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Core/NunuGameInstance.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "InGame/Interface/Hitable.h"
#include "VendingMachine.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVendingMachine_Delegate);


UCLASS()
class ROXY_API AVendingMachine : public AActor, public IHitable
{
	GENERATED_BODY()
	
public:	
	AVendingMachine();

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite, Category = Delegate)
		FVendingMachine_Delegate OnDropCola;


private:
	/* 콜리전 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BoundaryCollision, Meta = (AllowPrivateAccess = true))
		class USphereComponent* InteractionCollision; // 플레이어와 상호작용 가능한 상태인지 판단할 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, Meta = (AllowPrivateAccess = true))
		class UBoxComponent* BoxCollision; // 몸통..맞을거

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, Meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, Meta = (AllowPrivateAccess = true))
		class UStaticMesh* BrokenMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Material, meta = (AllowPrivateAccess = true))
		class UMaterial* Material;

	/* 파티클 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* PC_ParticleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Explosion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Spark;

	/* UI */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
		class UWidgetComponent* WidgetComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
		class UUserWidget* WidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = true))
		TSubclassOf<UUserWidget> WidgetTemplate;

	/* 사운드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_Active;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_Broken;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_Steam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_Spark;

	/* 콜라 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cola, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class AStatCola> StatColaClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cola, Meta = (AllowPrivateAccess = true))
		AStatCola* StatCola;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerCharacter, meta = (AllowPrivateAccess = true))
		class ARoxyCharacter* Roxy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InGameManager, meta = (AllowPrivateAccess = true))
		class ANunuInGameManager* InGameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FightZoneFinder, Meta = (AllowPrivateAccess = true))
		class AFightZoneFinder* NextFightZoneFinder;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Cola, Meta = (AllowPrivateAccess = true))
		bool _bIsBroken = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Cola, Meta = (AllowPrivateAccess = true))
		bool _bCanBroken = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cola, Meta = (AllowPrivateAccess = true))
		FVector _muzzleLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cola, Meta = (AllowPrivateAccess = true))
		FRotator _muzzleRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cola, Meta = (AllowPrivateAccess = true))
		FVector _targetLocation;
	UPROPERTY()
		FTimerHandle _dropColaTimer;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ColaStat, Meta = (AllowPrivateAccess = true))
		EColaType _colaType;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void OnInteractionCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnInteractionCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UFUNCTION()
		void ActiveInteractionCollision();

	UFUNCTION()
		void SetBrokenMesh();
	UFUNCTION()
		void DropCola();

	UFUNCTION(BlueprintPure)
		AStatCola* GetStatColaPtr() {return StatCola;}

	UFUNCTION()
		void RotateWidget();

	UFUNCTION()
		void SetNextFightZoneFinder(AFightZoneFinder* fightZone) { NextFightZoneFinder = fightZone; }

	UFUNCTION()
		EColaType GetColaType() {return _colaType;}

	UFUNCTION(BlueprintImplementableEvent)
		void PlayActiveSound();
	UFUNCTION(BlueprintImplementableEvent)
		void StopActiveSound();

	virtual void HitableTakeDamage() override;
};
