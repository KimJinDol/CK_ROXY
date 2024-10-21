// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "RevivalAI.generated.h"

DECLARE_MULTICAST_DELEGATE(FRevivalAI_Delegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRevivalDyna_Delegate);
/**
 * 
 */
UENUM(BlueprintType)
enum class ERevivalProjectileType : uint8 {
// 	RPT_Horizontal UMETA(DisplayName = "BPT_Horizontal"),
 	RPT_Delay UMETA(DisplayName = "BPT_Delay"),
// 	RPT_Homing UMETA(DisplayName = "BPT_Homing"),
	RPT_Fake UMETA(DisplayName = "RPT_Fake"),
	RPT_Size UMETA(DisplayName = "RPT_Size")
};

UCLASS()
class ROXY_API ARevivalAI : public AGruntAI
{
	GENERATED_BODY()
	
public:
	ARevivalAI();

private:
	UPROPERTY()
		class URevivalAnimInstance* RevivalAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = true))
		class UMaterial* BookOriginMat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = true))
		class UMaterialInstance* BookOpacityMat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* PC_ParticleComponent_02;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_UndeadPower;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class ABossProjectile> BossProjClass;


private:
	/* �⺻ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Basic, Meta = (AllowPrivateAccess = true))
		bool _bIsRevivalDie = false;

	/*��Ȱ ����*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		bool _bIsRevive = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		int _revivalEnemyNum_min = 10; // ��Ȱ ��ų ���� �ּ� ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		int _revivalEnemyNum_max = 15; // ��Ȱ ��ų ���� �ִ� ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		float _revivalRadius = 1000.f; // ��Ȱ ��ų ���� (�� ���)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		int _restEnemysNum = 0; // ��Ȱ��Ų ������ ���� ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class ACombatAI> combat_bp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class AThrowAI> throw_bp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class AChargeAI> charge_bp;

	/*�߻� ����*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		int32 _recreateProjCount = 0;

	/*Fake �߻�------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		int32 _projNum_Fake = 4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		int32 _recreateProjMax_Fake = 4;

	/*Delay �߻� ----------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		int32 _recreateProjMax_Delay = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		int32 _projNum_Delay = 8;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		int32 _proj_Y_AngleGap_Delay = 8;

	/*ó�±� ���� ------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CanTakeDamageState, Meta = (AllowPrivateAccess = true))
		bool _bCanDescent = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CanTakeDamageState, Meta = (AllowPrivateAccess = true))
		bool _bCanRising = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CanTakeDamageState, Meta = (AllowPrivateAccess = true))
		bool _bCanTakeDamage = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CanTakeDamageState, Meta = (AllowPrivateAccess = true))
		float _defaultMeshLoc_Z;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CanTakeDamageState, Meta = (AllowPrivateAccess = true))
		float _descentZ = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CanTakeDamageState, Meta = (AllowPrivateAccess = true))
		float _descentSpeed = 5.f;

public:
	/*��������Ʈ*/
	FRevivalAI_Delegate OnEndRevival;
	FRevivalAI_Delegate OnEndShootProj;
	FRevivalAI_Delegate OnEndAttack;
	FRevivalAI_Delegate OnEndTired;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite, Category = Delegate)
		FRevivalDyna_Delegate OnTired;
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite, Category = Delegate)
		FRevivalDyna_Delegate OnShootProj;
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite, Category = Delegate)
		FRevivalDyna_Delegate OnRevivalDie;

public:
	/*�⺻ ------------------------------------------------------------------*/
	UFUNCTION()
		void ShockWave();
	UFUNCTION()
		void DieCheck();
	UFUNCTION()
		void DestroyBoss();


	/*��Ȱ ���� -----------------------------------------------------------------------*/
	UFUNCTION()
		void StartRevival();
	UFUNCTION()
		void BeRevive();
	UFUNCTION()
		void EndRevival();
	UFUNCTION()
		void DecreaseRestEnemyNum() {_restEnemysNum--;}
	

	/*�߻� ���� ----------------------------------------------------------------------*/
	// ���� �߻� ���� �Լ�------------------------------
	void SelectRandomlyProjTypeAndShot(); // �������� �߻� Ÿ���� �����ϰ� �����Ų��
	ABossProjectile* SpawnProj(const FVector& spawnVec, const FRotator& targetRot); // �߻�ü�� ������Ų��

	// ���� �߻� ���� (����ũ)
	void BeginShootProj();
	void ShootProj_Fake();
	void EndShootProj();

	// ������ �߻�
	void BeginShootProj_Delay();
	void ShootProj_Delay();



	// ��Ȱ��Ų ���� ���� ��
	UFUNCTION()
		int GetRestEnemysNum() {return _restEnemysNum;}


	/*�ĸ±� ���� ���� ----------------------------------------------------------------*/
	UFUNCTION()
		void StartTired();
	UFUNCTION()
		void SetCanTakeDamage(){_bCanTakeDamage = true;}
	UFUNCTION()
		void EndTired();
	UFUNCTION()
		void SetDoNotTakeDamage() {_bCanTakeDamage = false;}
	UFUNCTION()
		void Descent() {_bCanDescent = true;}
	UFUNCTION()
		void Rising() {_bCanDescent = false; _bCanRising = true;}

	/*�⺻ �Լ� -----------------------------------------------------------------------*/
	// ���� pawn �� ���ִ� ���� hit ��ġ
	UFUNCTION()
		FVector GetFloorLocation();

	UFUNCTION()
		bool GetRevivalIsDie() { return _bIsRevivalDie; }

private:
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	virtual void Die() override;
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

};
