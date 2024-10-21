// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/EnemyAI.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UENUM()
enum class EEnemySpawnPoint: uint8
{
	EPoint_1,
	EPoint_2,
	EPoint_3,
	Size,
	EPoint_random
};

UENUM()
enum class EEnemySpawnPhase : uint8 
{
	EPhase_1,
	EPhase_2,
	EPhase_3,
	EPhase_4
};



UCLASS()
class ROXY_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

private:
	/*현재 레벨에 존재하는 InGameManager*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InGameManager, meta = (AllowPrivateAccess = "true"))
		class ANunuInGameManager* InGameManager;

	/*현재 레벨에 존재하는 Fight Zone 포인터*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnInfo, meta = (AllowPrivateAccess = "true"))
		class AFightZoneFinder* FightZoneFinder;

	/*현재 레벨에 배치 된 발전기*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnInfo, meta = (AllowPrivateAccess = "true"))
		class AGenerator* Generator; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemySpawnInfo, meta = (AllowPrivateAccess = "true"))
		class ARoxyCharacter* Roxy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EnemySpawnInfo, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class ACombatAI> CombatAI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EnemySpawnInfo, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class AThrowAI> ThrowAI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = EnemySpawnInfo, Meta = (AllowPrivateAccess = true))
		TSubclassOf<class AChargeAI> ChargeAI;

private:
	/*1번 스폰 포인트 위치*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnPoint, meta = (AllowPrivateAccess = "true"))
		FVector _spawnLocation_Point1; 
	
	/*2번 스폰 포인트 위치*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnPoint, meta = (AllowPrivateAccess = "true"))
		FVector _spawnLocation_Point2; 

	/*3번 스폰 포인트 위치*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnPoint, meta = (AllowPrivateAccess = "true"))
		FVector _spawnLocation_Point3; 

	/*땅에서 태어나는 애들 스폰 할거?*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnCES, meta = (AllowPrivateAccess = "true"))
		bool _bSpawnCES = true;

	/*땅에서 태어날 애들 스폰 위치 배열(CES : Charge Enemy Spawn의 약자)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnCES, meta = (AllowPrivateAccess = "true"))
		TArray<FVector> _CESLocationArray; 

	/*땅에서 태어날 애들이 생성 될 발전기 비율(CES : Charge Enemy Spawn의 약자)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnCES, meta = (AllowPrivateAccess = "true"))
		TArray<float> _CESGeneratorRatio; 

	/*현재까지 스폰된 발전기 비율 배열의 인덱스*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemySpawnCES, meta = (AllowPrivateAccess = "true"))
		int32 _CESCurrentIndex = 0; 

	/*현재까지의 플레이 시간.이 시간을 기준으로 웨이브를 설정한다.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemySpawnInfo, meta = (AllowPrivateAccess = "true"))
		float _sumPlayTime = 0.f; 

	/*현제 페이즈*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnInfo, meta = (AllowPrivateAccess = "true"))
		EEnemySpawnPhase _currentPhase = EEnemySpawnPhase::EPhase_1;

	/*체크 시 디폴트 값으로 설정 (Pd님이 기본 설정하신 시간으로 세팅할 것인가?)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnInfo, meta = (AllowPrivateAccess = "true"))
		bool _bSetDefault = true; 

	/*다음 스폰까지 걸리는 시간*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawnInfo, meta = (AllowPrivateAccess = "true"))
		float _nextSpawnTimeGap; 


	// 1 Phase
	/*1웨이브에서 스폰할 최소 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase1, meta = (AllowPrivateAccess = "true"))
		int32 _spawnNumMin_Phase1; 
	/*1웨이브에서 스폰할 최대 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase1, meta = (AllowPrivateAccess = "true"))
		int32 _spawnNumMax_Phase1; 
	/*1웨이브에서 스폰할 원거리 ai 최소 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase1, meta = (AllowPrivateAccess = "true"))
		int32 _throwAISpawnNumMin_Phase1;
	/*1웨이브에서 스폰할 원거리 ai 최대 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase1, meta = (AllowPrivateAccess = "true"))
		int32 _throwAISpawnNumMax_Phase1; 
	/*1웨이브에서 포인트 당 스폰할 최소 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase1, meta = (AllowPrivateAccess = "true"))
		int32 _spawnNumMinPerPoint_Phase1;

	// 2 Phase
	/*2웨이브에서 스폰할 최소 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase2, meta = (AllowPrivateAccess = "true"))
		int32 _spawnNumMin_Phase2; 
	/*2웨이브에서 스폰할 최대 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase2, meta = (AllowPrivateAccess = "true"))
		int32 _spawnNumMax_Phase2; 
	/*2웨이브에서 스폰할 원거리 ai 최소 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase2, meta = (AllowPrivateAccess = "true"))
		int32 _throwAISpawnNumMin_Phase2;
	/*2웨이브에서 스폰할 원거리 ai 최대 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase2, meta = (AllowPrivateAccess = "true"))
		int32 _throwAISpawnNumMax_Phase2; 
	/*2웨이브에서 포인트 당 스폰할 최소 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase2, meta = (AllowPrivateAccess = "true"))
		int32 _spawnNumMinPerPoint_Phase2; 

	// 3 Phase
	/*3웨이브에서 스폰할 최소 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase3, meta = (AllowPrivateAccess = "true"))
		int32 _spawnNumMin_Phase3; 
	/*3웨이브에서 스폰할 최대 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase3, meta = (AllowPrivateAccess = "true"))
		int32 _spawnNumMax_Phase3; 
	/*3웨이브에서 스폰할 원거리 ai 최소 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase3, meta = (AllowPrivateAccess = "true"))
		int32 _throwAISpawnNumMin_Phase3; 
	/*3웨이브에서 스폰할 원거리 ai 최대 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase3, meta = (AllowPrivateAccess = "true"))
		int32 _throwAISpawnNumMax_Phase3; 
	/*3웨이브에서 포인트 당 스폰할 최소 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase3, meta = (AllowPrivateAccess = "true"))
		int32 _spawnNumMinPerPoint_Phase3;

	// 4 Phase
	/*4웨이브에서 스폰할 최소 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase4, meta = (AllowPrivateAccess = "true"))
		int32 _spawnNumMin_Phase4; 
	/*4웨이브에서 스폰할 최대 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase4, meta = (AllowPrivateAccess = "true"))
		int32 _spawnNumMax_Phase4; 
	/*4웨이브에서 스폰할 원거리 ai 최소 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase4, meta = (AllowPrivateAccess = "true"))
		int32 _throwAISpawnNumMin_Phase4;
	/*4웨이브에서 스폰할 원거리 ai 최대 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase4, meta = (AllowPrivateAccess = "true"))
		int32 _throwAISpawnNumMax_Phase4; 
	/*4웨이브에서 포인트 당 스폰할 최소 마릿수*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawn_Phase4, meta = (AllowPrivateAccess = "true"))
		int32 _spawnNumMinPerPoint_Phase4; 



	UPROPERTY()
		FTimerHandle SpawnTimer; // 다음 스폰을 세팅하는 타이머

public:
	UFUNCTION()
		void FirstInitSpawnInfo(); // 시작할 때 초기화할 스폰 정보

	UFUNCTION()
		void SetCurrentPhaseToNext(EEnemySpawnPhase newPhase); // 다음 웨이브로 세팅해주는 함수

	UFUNCTION()
		void StartEnemySpawn();

	UFUNCTION()
		void EnemySpawnPhase();

	
		void EnemySpawn(TSubclassOf<AEnemyAI> enemyType, EEnemySpawnPoint spawnPoint);
		void EnemySpawn(TSubclassOf<AEnemyAI> enemyType, FVector spawnLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void PostInitializeComponents() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
