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
		void StartBreakDownTimer(); // ������ ���峾 �ð��� �������ִ� �Լ�

public:
	UFUNCTION(BlueprintCallable)
		void SetEnableInteraction(); // ������ ���峵�� �� ȣ�� �� �Լ�

	UFUNCTION(BlueprintCallable)
		void BreakDown();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayBreakDownParticle();

private:
	UFUNCTION()
		void ReleaseRepairPause(); // ��ųüũ ���з� �Ͻ������� �������¸� �簳�����ִ� �Լ�

	UFUNCTION()
		void BeginSkillCheck();

	UFUNCTION()
		void SkillCheck();

	UFUNCTION()
		void SuccessSkillCheck(); // ��ų üũ ���� ��

	UFUNCTION()
		void FailedSkillCheck(); // ��ų üũ ���� ��

	UFUNCTION()
		void OnInteractionCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnInteractionCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// ���׸���
	UFUNCTION()
		void SetDamagedMat();
	UFUNCTION()
		void SetOriginMat();

	// ���ӿ���
	UFUNCTION()
		void OpenRoxyWinWidget();

public:
	// �������� ���� �˸��� ��ųüũ
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


	// first Check �����ߴ����� ����
	UFUNCTION(BlueprintCallable)
	bool IsFirstCheck();

	UFUNCTION(BlueprintCallable)
	bool IsCanSkillCheck();

	UFUNCTION(BlueprintCallable)
	bool IsCanInteraction();

	// ������ ��ų üũ Ÿ���� �������� �Լ�
	UFUNCTION(BlueprintPure)
	float GetMaxSkillCheckTime();

	UFUNCTION(BlueprintPure)
	float GetSkillCheckTime();

	// ���� ��ų üũ �������� �������� �Լ�
	UFUNCTION(BlueprintPure)
	float GetCurrentSkillCheckTime();

	// ��ų üũ ��� ������ �������� �Լ�
	UFUNCTION(BlueprintPure)
	float GetSkillCheckToleranceTime();

	UFUNCTION(BlueprintPure)
	float GetCurrentSkillCheckTimeRatio();

	// ��ų üũ ���� Ƚ��
	UFUNCTION(BlueprintCallable)
	int32 GetSkillCheckSuccessNum();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BoundaryCollision, Meta = (AllowPrivateAccess = true))
		class USphereComponent* InteractionCollision; // �÷��̾�� ��ȣ�ۿ� ������ �������� �Ǵ��� �ݸ���

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
		float _RpIncrementPerSec; // ��� �ʴ� ������ ������ ������

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _breakDownPeriod_Min; // ���� �ֱ� �ּ� �ð�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _breakDownPeriod_Max; // ���� �ֱ� �ִ� �ð�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		bool _bIsBreakDown = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _RpDecrementPerSec; // ���峵�� �� �ʴ� ���ҵ� ������ ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _decrementAtDamaged; // �ǰ� �� ���ҵ� ������ ������
	/*������ ������������� ��Ÿ���� ����*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		bool _bIsDangerousGauge = false;
	/*������ ������� �˸��� �︮�� Ÿ�̸�*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _RpLessThanDuration = 5;



	// Skill Check
	/*���� ���� �� ������ ù�������� �Ǵ��ϴ� ����*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		bool _bIsFirstSkillCheck = true;
	/*��ų üũ�� ������ �������� �Ǵ��ϴ� ����*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		bool _bCanSkillCheck = false;
	/*��ų üũ ������ �Ǵ��ϴ� ����*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		bool _bIsSkillCheck = false;
	/*��ų üũ �̺�Ʈ�� �Ͼ�� �ִ� Ƚ��*/
	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		int32 _skillCheckMaxNum = 2;
	/*��ų üũ ���� Ƚ��*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		int32 _skillCheckSuccessNum = 0;
	/*���� ��ų üũ �̺�Ʈ ȸ��*/
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		int32 _currentSkillCheckNum = 0;
	/*��ų üũ �̺�Ʈ�� Ȱ��ȭ�Ǵ� �ִ� �ð�*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		float _maxSkillCheckTime;
	/*��ų üũ ���� ������ �ּ� �ð�*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		float _skillCheckTimeMinRange;
	/*��ų üũ ���� �ð�*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillCheck, Meta = (AllowprivateAccess = true))
		float _skillCheckTime;
	/*��ų üũ ��� ���� �ð�*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Random, Meta = (AllowprivateAccess = true))
		float _toleranceTime; 
	/*���� ��ų üũ �ð�*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Random, Meta = (AllowprivateAccess = true))
		float _currentSkillCheckTime = 0.f;
	/*_currentSkillCheckTime �� ������ �� �ִ� �������� �Ǵ�.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Random, Meta = (AllowprivateAccess = true))
		bool _bIncreaseCheckTime = false;
	/*��ų üũ ���� �� ������ ���� �Ͻ� ���� �ð�*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _repairPauseDuration; 
	/*��ų üũ ���� �� ������ ������*/
	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = RepairState, Meta = (AllowprivateAccess = true))
		float _RpIncrementAtSuccessSkillCheck = 0.05 * _maxRp;
	// 
	UPROPERTY()
		FTimerHandle _matTimer;

	UPROPERTY()
		FTimerHandle BreakDownTimer; // ������ ���� Ÿ�̸�
	UPROPERTY()
		FTimerHandle RepairPauseTimer; // ������ �Ͷ߸����ν� �Ͻ����� �Ǵ� Ÿ�̸�

	UPROPERTY()
		FTimerHandle FullChargeTimer; // �������� �� ä���� �� ����Ŭ���� ������ ���� Ÿ�̸�

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
