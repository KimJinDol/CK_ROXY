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
	/* 기본 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Basic, Meta = (AllowPrivateAccess = true))
		bool _bIsRevivalDie = false;

	/*부활 관련*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		bool _bIsRevive = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		int _revivalEnemyNum_min = 10; // 부활 시킬 적의 최소 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		int _revivalEnemyNum_max = 15; // 부활 시킬 적의 최대 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		float _revivalRadius = 1000.f; // 부활 시킬 범위 (원 모양)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		int _restEnemysNum = 0; // 부활시킨 적들의 남은 수

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class ACombatAI> combat_bp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class AThrowAI> throw_bp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Revival, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class AChargeAI> charge_bp;

	/*발사 관련*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		int32 _recreateProjCount = 0;

	/*Fake 발사------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		int32 _projNum_Fake = 4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		int32 _recreateProjMax_Fake = 4;

	/*Delay 발사 ----------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		int32 _recreateProjMax_Delay = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		int32 _projNum_Delay = 8;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = true))
		int32 _proj_Y_AngleGap_Delay = 8;

	/*처맞기 상태 ------------*/
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
	/*델리게이트*/
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
	/*기본 ------------------------------------------------------------------*/
	UFUNCTION()
		void ShockWave();
	UFUNCTION()
		void DieCheck();
	UFUNCTION()
		void DestroyBoss();


	/*부활 관련 -----------------------------------------------------------------------*/
	UFUNCTION()
		void StartRevival();
	UFUNCTION()
		void BeRevive();
	UFUNCTION()
		void EndRevival();
	UFUNCTION()
		void DecreaseRestEnemyNum() {_restEnemysNum--;}
	

	/*발사 관련 ----------------------------------------------------------------------*/
	// 랜덤 발사 관련 함수------------------------------
	void SelectRandomlyProjTypeAndShot(); // 랜덤으로 발사 타입을 지정하고 실행시킨다
	ABossProjectile* SpawnProj(const FVector& spawnVec, const FRotator& targetRot); // 발사체를 스폰시킨다

	// 유도 발사 관련 (페이크)
	void BeginShootProj();
	void ShootProj_Fake();
	void EndShootProj();

	// 딜레이 발사
	void BeginShootProj_Delay();
	void ShootProj_Delay();



	// 부활시킨 적의 남은 수
	UFUNCTION()
		int GetRestEnemysNum() {return _restEnemysNum;}


	/*쳐맞기 상태 관련 ----------------------------------------------------------------*/
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

	/*기본 함수 -----------------------------------------------------------------------*/
	// 현재 pawn 이 서있는 땅의 hit 위치
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
