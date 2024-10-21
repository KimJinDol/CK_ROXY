// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/EnemyAI.h"
#include "GameFramework/Actor.h"
#include "NewEnemySpawner.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNewEnemySpawner_Delegate);

UENUM()
enum class EEnemySpawnPoint_2 : uint8
{
	EPoint_1,
	EPoint_2,
	EPoint_3,
	EPoint_4,
	EPoint_5,
	EPoint_6
};

USTRUCT(BlueprintType)
struct FEnemySpawnInfoStruct
{
	GENERATED_USTRUCT_BODY()

	/*스폰 시간*/
	UPROPERTY (EditAnywhere, BlueprintReadOnly, Category = SpawnEnemyInfo)
		float _spawnTime;
	/*적 타입*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnEnemyInfo)
		TSubclassOf<AEnemyAI> _enemyType;
	/*해당 타입의 적을 몇 마리 스폰할 건지*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnEnemyInfo)
		int32 _spawnNum;
	/*스폰할 지점*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnEnemyInfo)
		EEnemySpawnPoint_2 _spawnPoint;
};

USTRUCT(BlueprintType)
struct FEnemySpawnWaveStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnInfo, meta = (AllowPrivateAccess = true))
		TArray<FEnemySpawnInfoStruct> _ESInfos;
};

UCLASS()
class ROXY_API ANewEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANewEnemySpawner();

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = Delegate)
		FNewEnemySpawner_Delegate OnEndAnWave;


private:
	/*현재 레벨에 존재하는 InGameManager*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Instance, meta = (AllowPrivateAccess = true))
		class ANunuInGameManager* InGameManager;

	/*현재 레벨에 존재하는 Fight Zone 포인터*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Instance, meta = (AllowPrivateAccess = true))
		class AFightZoneFinder* FightZoneFinder;

	/*현재 레벨에 배치 된 발전기*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Instance, meta = (AllowPrivateAccess = true))
		class AGenerator* Generator;

	/*플레이어 캐릭터*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Instance, meta = (AllowPrivateAccess = true))
		class ARoxyCharacter* Roxy;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
		class UEnemySpawnerWidget* WidgetInstance;

	UPROPERTY (EditAnywhere, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = true))
		TSubclassOf<UEnemySpawnerWidget> WidgetTemplate;

	/*스폰에 관련된 정보를 담는 웨이브 배열*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnInfo, meta = (AllowPrivateAccess = true))
		TArray<FEnemySpawnWaveStruct> _ESWaveArr;

	/*현재 스폰 가능한 상태인지(웨이브 진행 중인지)*/
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = EnemySpawnInfo, meta = (AllowPrivateAccess = true))
		bool _bCanSpawn = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemySpawnInfo, meta = (AllowPrivateAccess = true))
		bool _bOnEndWaveBroadCast = false;

	/*현재 웨이브 인덱스*/
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = EnemySpawnInfo, meta = (AllowPrivateAccess = true))
		int32 _curWaveIndex = -1;

	/*현재 웨이브의 스폰 인덱스*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemySpawnInfo, meta = (AllowPrivateAccess = true))
		int32 _curSpawnIndex = 0;

	/*현재까지의 플레이 시간.이 시간을 기준으로 웨이브를 설정한다.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemySpawnInfo, meta = (AllowPrivateAccess = true))
		float _sumPlayTime = 0.f;

	/*1번 스폰 포인트 위치*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnPoint, meta = (AllowPrivateAccess = true))
		FVector _spawnLocation_Point1;

	/*2번 스폰 포인트 위치*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnPoint, meta = (AllowPrivateAccess = true))
		FVector _spawnLocation_Point2;

	/*3번 스폰 포인트 위치*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnPoint, meta = (AllowPrivateAccess = true))
		FVector _spawnLocation_Point3;

	/*4번 스폰 포인트 위치*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnPoint, meta = (AllowPrivateAccess = true))
		FVector _spawnLocation_Point4;

	/*5번 스폰 포인트 위치*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnPoint, meta = (AllowPrivateAccess = true))
		FVector _spawnLocation_Point5;

	/*6번 스폰 포인트 위치*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnPoint, meta = (AllowPrivateAccess = true))
		FVector _spawnLocation_Point6;

public:
	/*스폰 웨이브 시작*/
	UFUNCTION(BlueprintCallable)
		void BeginSpawnWave();

	/*현재 스폰 웨이브 인덱스를 다음으로 넘겨준다. 현재 인덱스가 마지막 인덱스라면 넘기지 않는다*/
	UFUNCTION()
		void SetCurrentWaveToNext();
	/*현재 스폰 정보 인덱스를 다음으로 넘겨준다. 현재 인덱스가 마지막 인덱스라면 웨이브를 마무리한다*/
	UFUNCTION()
		void SetCurrentSpawnInfoToNext();

	UFUNCTION()
		void SpawnEnemy(TSubclassOf<AEnemyAI> enemyType, EEnemySpawnPoint_2 spawnPoint);

public:
	UFUNCTION(BlueprintCallable)
		int32 GetCurrentWaveIndex();

	UFUNCTION()
		bool GetIsCanSpawn() { return _bCanSpawn; }

	UFUNCTION()
		float GetSumPlayTime() { return _sumPlayTime; }

	// 현재 웨이브가 마지막인지
	UFUNCTION(BlueprintCallable)
		bool IsLastWave();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};