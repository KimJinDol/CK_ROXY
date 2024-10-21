// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Core/NunuGameInstance.h"
#include "InGame/Component/HpComponent.h"
#include "GameFramework/Character.h"
#include "EnemyAI.generated.h"


DECLARE_MULTICAST_DELEGATE(FEnemyDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FEnemyDelegate_OneParam, AEnemyAI*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyDelegate_Dyna_OneParam, AEnemyAI*, enemyPointer);


UENUM()
enum class ECurrentMatType
{
	MT_ORIGIN,
	MT_DAMAGED,
	MT_OPACITY
};


UCLASS()
class ROXY_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyAI();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InGameManager, meta = (AllowPrivateAccess = true))
		class ANunuInGameManager* InGameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HpComponent, meta = (AllowPrivateAccess = true))
		class UHpComponent* HpComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
		class UWidgetComponent* LockOnWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		class UDataTable* DataTable;


	/* --------------------- ��ƼŬ �ý��� --------------------- */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* Effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Hit_01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Hit_02;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Hit_03;
	/* -------------------------------------------------------- */


	// ���׸���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
		class UMaterialInstance* OpacityMat; // ī�޶� ĳ���͸� ���� �� �ٲ��� ����ȭ material �ν��Ͻ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
		class UMaterial* OriginalMat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
		class UMaterial* DamagedMat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
		class UMaterial* WeaponMat; // ������ �������� ���� ���׸��� �ε����� ���� �������ִ� ģ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
		ECurrentMatType _currentMatType = ECurrentMatType::MT_ORIGIN;

protected:
	UPROPERTY()
		FTimerHandle DamagedMatOffTimer;

protected:
	UPROPERTY()
		bool _bIsOnTargeted;

	UPROPERTY()
		bool _bIsInAir = false;
	UPROPERTY()
		bool _bEnemyIsDie = false;


public:
	virtual void Possessed(AController* NewController);
	virtual void UnPossessed();

protected:
	virtual void BeginPlay() override;

	virtual void Die();

public:
	FEnemyDelegate OnDie;
	FEnemyDelegate_OneParam OnDie_OneParam;
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Delegate")
	FEnemyDelegate_Dyna_OneParam OnDie_Dyna_OneParam;

public:
	/*-----------���͸��� ����-------------------*/
	UFUNCTION()
		void SetDamagedMat();
	UFUNCTION()
		void SetOriginMat();

	UFUNCTION()
		void SetOpacityMat();

	UFUNCTION()
		ECurrentMatType GetCurrentMatType() { return _currentMatType; }

	UFUNCTION()
		void SetLockOnWidgetVisibility(bool value);

	UFUNCTION()
		bool GetIsLockOnModeOn();

	UFUNCTION()
		bool GetEnemyIsDie() {return _bEnemyIsDie;}


	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE class UHpComponent* GetHpComponent() const { return HpComponent; }

};
