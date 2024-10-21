// Fill out your copyright notice in the Description page of Project Settings.


#include "renewalEnemySpawner.h"
#include "InGame/System/FightZoneFinder.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Enemy/GruntAI/Buff/BuffAI.h"
#include "InGame/Enemy/GruntAI/Throw/EliteThrow/EliteThrowAI.h"
#include "InGame/Enemy/GruntAI/Combat/CombatAI.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAI.h"
#include "InGame/Enemy/GruntAI/Charge/ChargeAI.h"
#include "InGame/System/AIObjectPool.h"

// Sets default values
ArenewalEnemySpawner::ArenewalEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetCollisionProfileName("Trigger");

}

void ArenewalEnemySpawner::AddSpawnPointAtFightZoneFinder()
{
	if (_wave == FightZoneFinder->GetCurrentWaveNum())
	{
		FightZoneFinder->AddSpawnPointNum();
		FightZoneFinder->PushSpawnerArray(this);
	}
}

void ArenewalEnemySpawner::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (FightZoneFinder && _bCanSpawn)
	{
		if (OtherActor->IsA(ARoxyCharacter::StaticClass()) && FightZoneFinder->GetCurrentWaveNum() == _wave)
		{
			PlayerCharacter = Cast<ARoxyCharacter>(OtherActor);
			FightZoneFinder->StartSpawn(this);
			Spawn();
			GetWorldTimerManager().SetTimer(SpawnTermTimer, this, &ArenewalEnemySpawner::Spawn, _spawn_term, true);
			_bCanSpawn = false;
		}
	}
}

void ArenewalEnemySpawner::Spawn()
{
	FRotator lookAtPlayerRot = (PlayerCharacter->GetActorLocation() - GetActorLocation()).Rotation();  
	lookAtPlayerRot.Pitch = 0.f;
	lookAtPlayerRot.Roll = 0.f;

	AGruntAI* enemy = nullptr;
	
	// FString TypeName = _enemyType[_currentSpawnIndex]->GetName();

	TSubclassOf<AGruntAI> enemyType = _enemyType[_currentSpawnIndex];
	
	// Object Pool 에서 관리하는 AI 클래스라면 풀에서 Pop해온다.
	if (AIObjectPool->Contains(enemyType))
	{
		enemy = AIObjectPool->Pop(enemyType);
		// enemy Logic 활성화
		enemy->SetActorLocation(_spawn_dot_Loc[_currentSpawnDotIndex]);
		enemy->SetActorRotation(lookAtPlayerRot);
	}
	else
	{
		enemy = GetWorld()->SpawnActor<AGruntAI>(_enemyType[_currentSpawnIndex], _spawn_dot_Loc[_currentSpawnDotIndex], lookAtPlayerRot);
	}

	InformTheFightZoneFinderOfTheSpawn(enemy);

	if(Cast<ABuffAI>(enemy))
	{
		OnSpawnBuffAI.Broadcast(enemy);
	}
	else if(Cast<AEliteThrowAI>(enemy))
	{
		OnSpawnEliteThrowAI.Broadcast(enemy);
	}


	/*마지막 적까지 스폰했다면 해당 인스턴스 삭제*/
	if (_currentSpawnIndex == _enemyType.Num() - 1)
	{
		FightZoneFinder->RemoveSpawnPointNum();
		FightZoneFinder->RemoveSpawnerArray(this);
		Destroy();
	}
	else
	{ 
		_currentSpawnIndex++;
		if (_currentSpawnIndex < _spawn_dot_Loc.Num())
			_currentSpawnDotIndex++;
		else
			_currentSpawnDotIndex = 0;
	}
	
}

void ArenewalEnemySpawner::InformTheFightZoneFinderOfTheSpawn(AEnemyAI* spawnEnemy)
{
	FightZoneFinder->PushEnemyArray(spawnEnemy);
}

void ArenewalEnemySpawner::SetEnableSpawn(bool value)
{
	if(value)
	{ 
		_bCanSpawn = true;
		TSet<AActor*> overlappedActors;
		SphereCollision->GetOverlappingActors(overlappedActors);
		for (auto i : overlappedActors)
		{
			// 이미 플레이어가 겹쳐져 있다면
			if (i->IsA(ARoxyCharacter::StaticClass()) && FightZoneFinder->GetCurrentWaveNum() == _wave)
			{
				PlayerCharacter = Cast<ARoxyCharacter>(i);
				FightZoneFinder->StartSpawn(this);
				Spawn();
				GetWorldTimerManager().SetTimer(SpawnTermTimer, this, &ArenewalEnemySpawner::Spawn, _spawn_term, true);
				_bCanSpawn = false;
				break;
			}
		}
	}
	else
		_bCanSpawn = false;
}

// Called when the game starts or when spawned
void ArenewalEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if(FightZoneFinder)
	{ 
		FightZoneFinder->AddAllEnemysNum(_enemyType.Num());
	}
	else
	{
		LOG(Warning, "Is Not setting fightZoneFinder");
	}
}

void ArenewalEnemySpawner::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ArenewalEnemySpawner::OnCollisionBeginOverlap);

	if(FightZoneFinder)
		FightZoneFinder->NewWave.AddDynamic(this, &ArenewalEnemySpawner::AddSpawnPointAtFightZoneFinder);
}

// Called every frame
void ArenewalEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

