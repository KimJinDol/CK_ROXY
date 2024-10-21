// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "GameFramework/Actor.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"
#include "renewalEnemySpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemySpawner_Dyna_OneParam, AEnemyAI*, spawnEnemyPointer);

/*스폰되는 몹 종류에 관련된 구조체(몹종류에 종속되는 사항들)*/
USTRUCT(BlueprintType)
struct FEnemyInfoStruct
{
	GENERATED_USTRUCT_BODY()

	/*스폰 되는 몹 종류*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyInfo)
		TSubclassOf<AGruntAI> _enemyType;
	/*스폰 닷 인덱스 번호*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyInfo)
		int32 _spawn_dot_index;
};


/*스폰 포인트와 관련된 구조체*/
USTRUCT(BlueprintType)
struct FEnemySpawnerStruct
{
	GENERATED_USTRUCT_BODY()


};

UCLASS()
class ROXY_API ArenewalEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ArenewalEnemySpawner();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Instance, meta = (AllowPrivateAccess = true))
		class AAIObjectPool* AIObjectPool;

	/*현재 레벨에 존재하는 Fight Zone 포인터*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Instance, meta = (AllowPrivateAccess = true))
		class AFightZoneFinder* FightZoneFinder;

	/*콜라이더*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
		class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
		class ARoxyCharacter* PlayerCharacter;

	/////// 기획자를 위한 변수들///////
	/*스폰되는 몹 종류에 대한 정보(구조체 배열)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner, meta = (AllowPrivateAccess = true))
		TArray<TSubclassOf<AGruntAI>> _enemyType;
	/*스폰 텀*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner, meta = (AllowPrivateAccess = true))
		float _spawn_term;
	/*스폰 닷 (배열)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner, meta = (AllowPrivateAccess = true))
		TArray<FVector> _spawn_dot_Loc;
	/*어느 웨이브에서 스폰할지*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner, meta = (AllowPrivateAccess = true))
		int32 _wave;
// 	/*스폰 될 총 ai 배열*/
// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemySpawner, meta = (AllowPrivateAccess = true))
// 		TArray<AGruntAI*> _enemys;
	//////////////////////////////////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemySpawner, meta = (AllowPrivateAccess = true))
	int32 _currentSpawnDotIndex = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemySpawner, meta = (AllowPrivateAccess = true))
	int32 _currentSpawnIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemySpawner, meta = (AllowPrivateAccess = true))
	bool _bCanSpawn = true;


	FTimerHandle SpawnTermTimer;

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite, Category = Delegate)
		FEnemySpawner_Dyna_OneParam OnSpawnBuffAI;
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite, Category = Delegate)
		FEnemySpawner_Dyna_OneParam OnSpawnEliteThrowAI;
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintReadWrite, Category = Delegate)
		FEnemySpawner_Dyna_OneParam OnSpawnAI;
	
private:
	/*fightZoneFinder의 웨이브와 해당 인스턴스의 웨이브 정보가 같다면 fightZone 에게 스폰포인트 개수 추가하라고 명령*/
	UFUNCTION()
		void AddSpawnPointAtFightZoneFinder();

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Spawn();

	// 적의 스폰을 fightZone에게 알린다.
	UFUNCTION()
	void InformTheFightZoneFinderOfTheSpawn(AEnemyAI* spawnEnemy);

public:
	UFUNCTION()
		bool GetEnableSpawn() { return _bCanSpawn; }
	UFUNCTION()
		void SetEnableSpawn(bool value);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void PostInitializeComponents() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
