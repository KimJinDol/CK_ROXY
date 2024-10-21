// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "GameFramework/Actor.h"
#include "Generator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGenerator_Delegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGenerator_OneParam_Delegate, FName, name);

UCLASS()
class ROXY_API AGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void PostInitializeComponents() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void StartBreakDownTimer(); // 발전기 고장낼 시간을 세팅해주는 함수

public:
	UFUNCTION(BlueprintCallable)
		void SetEnableInteraction(); // 발전기 고장났을 때 호출 될 함수

	UFUNCTION(BlueprintCallable)
		void BreakDown();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayBreakDownParticle();

private:
	UFUNCTION()
		void ReleaseRepairPause(); // 스킬체크 실패로 일시정지된 수리상태를 재개시켜주는 함수

	UFUNCTION()
		void BeginSkillCheck();

	UFUNCTION()
		void SkillCheck();

	UFUNCTION()
		void SuccessSkillCheck(); // 스킬 체크 성공 시

	UFUNCTION()
		void FailedSkillCheck(); // 스킬 체크 실패 시

	UFUNCTION()
		void OnInteractionCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnInteractionCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 머테리얼
	UFUNCTION()
		void SetDamagedMat();
	UFUNCTION()
		void SetOriginMat();

	// 게임엔딩
	UFUNCTION()
		void OpenRoxyWinWidget();

public:
	// 스테이지 끝을 알리는 스킬체크
	UFUNCTION(BlueprintImplementableEvent)
		void LastSkillCheck();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintImplementableEvent)
		void PlayDamagedSound();

	UFUNCTION()
	void PlaySound(USoundBase* sound);
	
	UFUNCTION()
	void SetRp(float rp);

	UFUNCTION(BlueprintCallable)
	float GetRp();

	UFUNCTION(BlueprintCallable)
	float GetRpRatio();

	UFUNCTION(BlueprintCallable)
	float GetMaxRp();

	UFUNCTION(BlueprintCallable)
	bool IsDangerousGauge();

	UFUNCTION()
	void OnDangerousGauge();


	// first Check 실행했는지를 리턴
	UFUNCTION(BlueprintCallable)
	bool IsFirstCheck();

	UFUNCTION(BlueprintCallable)
	bool IsCanSkillCheck();

	UFUNCTION(BlueprintCallable)
	bool IsCanInteraction();

	// 지정된 스킬 체크 타임을 가져오는 함수
	UFUNCTION(BlueprintPure)
	float GetMaxSkillCheckTime();

	UFUNCTION(BlueprintPure)
	float GetSkillCheckTime();

	// 현재 스킬 체크 게이지를 가져오는 함수
	UFUNCTION(BlueprintPure)
	float GetCurrentSkillCheckTime();

	// 스킬 체크 허용 범위를 가져오는 함수
	UFUNCTION(BlueprintPure)
	float GetSkillCheckToleranceTime();

	UFUNCTION(BlueprintPure)
	float GetCurrentSkillCheckTimeRatio();

	// 스킬 체크 성공 횟수
	UFUNCTION(BlueprintCallable)
	int32 GetSkillCheckSuccessNum();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BoundaryCollision, Meta = (AllowPrivateAccess = true))
		class USphereComponent* InteractionCollision; // 플레이어와 상호작용 가능한 상태인지 판단할 콜리전

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, Meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
		class UGeneratorWidget* WidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = true))
		TSubclassOf<UGeneratorWidget> WidgetTemplate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		class UDataTable* DataTable;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = Roxyy, meta = (AllowPrivateAccess = true))
		class ARoxyCharacter* Roxy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Roxy, meta = (AllowPrivateAccess = true))
		class APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
		class UMaterial* OriginalMat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
		class UMaterial* DamagedMat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner, meta = (AllowPrivateAccess = "true"))
		class ANewEnemySpawner* EnemySpawner;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystemComponent* PC_ParticleComponent;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_Spark;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = Particle, meta = (AllowPrivateAccess = true))
		class UParticleSystem* PS_SparkAttack;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
		class USoundBase* SB_RepairStart;

// 	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = Sound, meta = (AllowPrivateAccess = true))
// 		class USoundBase* SB_Repair;
		
private:
	UPROPERTY()
		bool _bOnLast = false;
	// Rp
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _maxRp = 100; // Rp = Repair Point
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _currentRp = 80.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		bool _bCanRepair = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _RpIncrementPerSec; // 평소 초당 증가될 발전기 수리량

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _breakDownPeriod_Min; // 고장 주기 최소 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _breakDownPeriod_Max; // 고장 주기 최대 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		bool _bIsBreakDown = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _RpDecrementPerSec; // 고장났을 때 초당 감소될 발전기 수리량
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _decrementAtDamaged; // 피격 시 감소될 발전기 수리량
	/*게이지 위험상태인지를 나타내는 변수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		bool _bIsDangerousGauge = false;
	/*게이지 위험상태 알림이 울리는 타이머*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _RpLessThanDuration = 5;



	// Skill Check
	/*게임 시작 후 발전기 첫가동인지 판단하는 변수*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		bool _bIsFirstSkillCheck = true;
	/*스킬 체크가 가능한 상태인지 판단하는 변수*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		bool _bCanSkillCheck = false;
	/*스킬 체크 중인지 판단하는 변수*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		bool _bIsSkillCheck = false;
	/*스킬 체크 이벤트가 일어나는 최대 횟수*/
	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		int32 _skillCheckMaxNum = 2;
	/*스킬 체크 성공 횟수*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		int32 _skillCheckSuccessNum = 0;
	/*현재 스킬 체크 이벤트 회차*/
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		int32 _currentSkillCheckNum = 0;
	/*스킬 체크 이벤트가 활성화되는 최대 시간*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		float _maxSkillCheckTime;
	/*스킬 체크 지정 가능한 최소 시간*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		float _skillCheckTimeMinRange;
	/*스킬 체크 지정 시간*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		float _skillCheckTime;
	/*스킬 체크 허용 오차 시간*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Random, Meta = (AllowprivateAccess = true))
		float _toleranceTime; 
	/*현재 스킬 체크 시간*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Random, Meta = (AllowprivateAccess = true))
		float _currentSkillCheckTime = 0.f;
	/*_currentSkillCheckTime 이 증가할 수 있는 상태인지 판단.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Random, Meta = (AllowprivateAccess = true))
		bool _bIncreaseCheckTime = false;
	/*스킬 체크 실패 시 발전기 수리 일시 정지 시간*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _repairPauseDuration; 
	/*스킬 체크 성공 시 증가할 수리량*/
	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _RpIncrementAtSuccessSkillCheck = 0.05 * _maxRp;
	// 
	UPROPERTY()
		FTimerHandle _matTimer;

	UPROPERTY()
		FTimerHandle BreakDownTimer; // 발전기 고장 타이머
	UPROPERTY()
		FTimerHandle RepairPauseTimer; // 발전기 터뜨림으로써 일시정시 되는 타이머

	UPROPERTY()
		FTimerHandle FullChargeTimer; // 게이지를 다 채웠을 시 게임클리어 연출을 위한 타이머

	UPROPERTY()
		FTimerHandle DangerousGaugeTimer;

public:
	FTimerDelegate BreakDown_Delegate;
	FTimerDelegate RepairPause_Delegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = Delegate)
		FGenerator_Delegate OnFinishSkillCheck;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = Delegate)
		FGenerator_OneParam_Delegate DangerousGaugeNotify;

};
