// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class ROXY_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, Meta = (AllowPrivateAccess = true))
		class USkeletalMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
		class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AnimInstance, Meta = (AllowPrivateAccess = true))
		class UAnimInstance* ItemAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Particle, Meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* PC_ParticleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Drop; // 필드에 드랍시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Hororok; // 캐릭터가 아이템을 먹을시

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundCue* SC_ItemGet;

private:
	UPROPERTY()
		FTimerHandle _activeItemTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void PostInitializeComponents() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void ActiveItemCollision();

	UFUNCTION()
		void DestroyItem();

protected:
	UFUNCTION()
		virtual void OnItemCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};