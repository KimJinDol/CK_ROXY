// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "GameFramework/Actor.h"
#include "FightZoneFinder.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFightZoneFinder_Delegate);

UCLASS(ClassGroup = (Custom))
class ROXY_API AFightZoneFinder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFightZoneFinder();


private:
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
// 		class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, Meta = (AllowPrivateAccess = true))
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
		class UMaterialInstance* Material;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = FightZoneFinder, meta = (AllowPrivateAccess = "true"))
	TArray<AEnemyAI*> _enemys; 

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = FightZoneFinder, meta = (AllowPrivateAccess = "true"))
	TArray<ArenewalEnemySpawner*> _spawners;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerCharacter, meta = (AllowPrivateAccess = "true"))
		class ARoxyCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FightZoneFinder, meta = (AllowPrivateAccess = "true"))
		class AFightZoneFinder* nextFightZone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BuffCola, meta = (AllowPrivateAccess = "true"))
		class AVendingMachine* VendingMachine;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FightZoneFinder, meta = (AllowPrivateAccess = "true"))
		bool _bIsActive = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = FightZoneFinder, meta = (AllowPrivateAccess = "true"))
		bool _bOnFirstCheck;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = FightZoneFinder, meta = (AllowPrivateAccess = "true"))
		bool _bOnFirstSpawnCheck = false;

	/*현재 웨이브*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Wave, meta = (AllowPrivateAccess = "true"))
		int32 _currentWaveNum = 1;

	/*최대 웨이브*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Wave, meta = (AllowPrivateAccess = "true"))
		int32 _maxWaveNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpawnPoint, meta = (AllowPrivateAccess = "true"))
		int32 _spawnPointNum = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpawnPoint, meta = (AllowPrivateAccess = "true"))
		bool _bIsSpawning = false;


	// 해당 전투가 끝나면 버프콜라를 선택할 수 있는지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BuffCola, meta = (AllowPrivateAccess = "true"))
		bool _isCanSelectBuffCola = false;


	// 해당 전투존에서 처치해야 할 모든 몹의 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FightZoneFinder, meta = (AllowPrivateAccess = "true"))
		int32 _allEnemysNum = 0;


public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite, Category = Delegate)
		FFightZoneFinder_Delegate OnEnterPlayer;

	// 새로운 웨이브에 진입할떄마다 실행
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite, Category = Delegate)
		FFightZoneFinder_Delegate NewWave;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite, Category = Delegate)
		FFightZoneFinder_Delegate OnFirstSpawn;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite, Category = Delegate)
		FFightZoneFinder_Delegate OnEndFight;

public:
	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void SetMaterialParam(float value);

	UFUNCTION()
		void SetAllEnemyNum(int allEnemyNum) { _allEnemysNum = allEnemyNum; }

	UFUNCTION()
		void AddAllEnemysNum(int enemyNum) { _allEnemysNum += enemyNum; }

	UFUNCTION()
		void DecreaseAllEnemysNum() { _allEnemysNum--; }


	UFUNCTION()
		int32 GetAllEnemysNum() { return _allEnemysNum; }

	
	UFUNCTION(BlueprintCallable)
	// 플레이어 캐릭터가 전투 존에 입장함을 알린다
		void EnterPlayer(ARoxyCharacter* Roxy);

	UFUNCTION()
		// 안에 있는 적들을 모두 반환한다
		TArray<AEnemyAI*> GetEnemys();

	UFUNCTION()
		int32 GetEnemysNum();

	UFUNCTION()
	// 스폰이 아닌 월드에 이미 배치되어 있는 적들을 체크한다
		void FirstCheck();

	UFUNCTION()
		void ClearEnemyCheck();

	UFUNCTION()
		void ClearEnemy();

	UFUNCTION()
		bool IsEmpty();

	UFUNCTION(BlueprintCallable)
		void EndFight();

	UFUNCTION()
		void AddSpawnPointNum() {_spawnPointNum ++;}

	UFUNCTION()
		void RemoveSpawnPointNum() {_spawnPointNum--;}


private:
	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UFUNCTION()
		void PushEnemyArray(AEnemyAI* enemy);

	UFUNCTION()
		void RemoveEnemyArray(AEnemyAI* enemy);

	UFUNCTION()
		void FightZoneActive();
	UFUNCTION()
		void PushSpawnerArray(ArenewalEnemySpawner* spawner);
	UFUNCTION()
		void RemoveSpawnerArray(ArenewalEnemySpawner* spawner);
	UFUNCTION()
		int32 GetCurrentWaveNum() { return _currentWaveNum; }
	UFUNCTION()
		void Reward();


	UFUNCTION()
		void StartSpawn(ArenewalEnemySpawner* spawner);

};
