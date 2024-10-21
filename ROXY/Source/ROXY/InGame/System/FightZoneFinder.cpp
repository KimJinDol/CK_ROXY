// Fill out your copyright notice in the Description page of Project Settings.


#include "FightZoneFinder.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Enemy/EnemyAI.h"
#include "InGame/System/VendingMachine.h"
#include "Ingame/System/renewalEnemySpawner.h"
#include "InGame/System/EnemySpawnerWidget.h"
#include "LockOnSystem.h"

// Sets default values
AFightZoneFinder::AFightZoneFinder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

// 	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
// 	RootComponent = Box;
// 	Box->SetCollisionProfileName(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionProfileName(TEXT("BlockOnlyPawn")); // 활성화 된 파이트 존만 OverlapOnlyPawn으로 바꿔주기
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MESH(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));
	if (MESH.Succeeded())
	{
		Mesh->SetStaticMesh(MESH.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MATERIAL(TEXT("/Game/Resources/Shader/wallshader/M_wallshader_Inst.M_wallshader_Inst"));
	if (MATERIAL.Succeeded())
	{
		Material = MATERIAL.Object;
		Mesh->SetMaterial(0, Material);
	}

}

void AFightZoneFinder::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void AFightZoneFinder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
// 
// 	ARoxyCharacter* player = Cast<ARoxyCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
// 	if(player)
// 	{
// 		bool bOnMaterial = false;
// 		FVector playerPos = player->GetActorLocation();
// 		FVector endPos = playerPos + (player->GetActorForwardVector() * 1000.f); 
// 		TArray<FHitResult> hitResults;
// 
// 		bool result = GetWorld()->LineTraceMultiByChannel(hitResults, playerPos, endPos, ECollisionChannel::ECC_GameTraceChannel5);
// 		
// 		for (auto i : hitResults)
// 		{
// 			if (i.GetActor() == this)
// 			{
// 				bOnMaterial = true;
// 			}
// 		}
// 
// 		if (bOnMaterial)
// 		{
// 			SetMaterialParam(1.f);
// 		}
// 		else
// 		{
// 			SetMaterialParam(0.f);
// 		}
// 	}
}

// Called when the game starts or when spawned
void AFightZoneFinder::BeginPlay()
{
	Super::BeginPlay();
	
	if (_bIsActive)
	{
		FightZoneActive();
	}
	FirstCheck();
	NewWave.Broadcast();
}

void AFightZoneFinder::SetMaterialParam(float value)
{
	if (Material)
	{
		Mesh->SetScalarParameterValueOnMaterials("opacity_pow", value);
	}
}

void AFightZoneFinder::FightZoneActive()
{
	Mesh->SetCollisionProfileName(TEXT("Trigger"));
	_bIsActive = true;
}

void AFightZoneFinder::EnterPlayer(ARoxyCharacter* Roxy)
{
	PlayerCharacter = Roxy;


	OnEnterPlayer.Broadcast();

	LOG_SCREEN("FightZone : Enter %s", *(PlayerCharacter->GetName()));

}

TArray<AEnemyAI*> AFightZoneFinder::GetEnemys()
{
	TSet<AActor*> overlappedActors;
	Mesh->GetOverlappingActors(overlappedActors);

	bool _bEnterPlayer = false;

	for (auto i : overlappedActors) {
		if (overlappedActors.Contains(i) && !_enemys.Contains(i) && i->IsA(AEnemyAI::StaticClass()) /*|| i->GetClass()->ImplementsInterface(UInterface_Enemy::StaticClass())*/) {
			PushEnemyArray(Cast<AEnemyAI>(i));
		}
	}
	return _enemys;
}

int32 AFightZoneFinder::GetEnemysNum()
{
	return _enemys.Num();
}

void AFightZoneFinder::FirstCheck()
{
	_bOnFirstCheck = true;
	TSet<AActor*> alreadyOverlappedActors;
	Mesh->GetOverlappingActors(alreadyOverlappedActors);

	ARoxyCharacter* Roxy = nullptr;

	// 먼저 적들 배열부터 완성 시키고 
	// (그래야지 enterPlayer함수에서 현재 전장에 있는 적의 정보를 완벽히 전달할테니까) 
	// 플레이어를 맨 마지막에 들어온 것으로 처리하기 위한 변수
	bool _bEnterPlayer = false;

	for (auto i : alreadyOverlappedActors) {
		if (!_enemys.Contains(i) && i->IsA(AEnemyAI::StaticClass()) /*|| i->GetClass()->ImplementsInterface(UInterface_Enemy::StaticClass())*/) {
			/*LOG_SCREEN("FightZone : First Check %s", *(i->GetName()));*/
			PushEnemyArray(Cast<AEnemyAI>(i));
		}
		else if (i->IsA(ARoxyCharacter::StaticClass())) {
			/*LOG_SCREEN("FightZone : First Check %s", *(i->GetName()));*/
			_bEnterPlayer = true;
			Roxy = Cast<ARoxyCharacter>(i);
		}
	}
	if (_bEnterPlayer && Roxy)
		EnterPlayer(Roxy);
}

void AFightZoneFinder::ClearEnemyCheck()
{
	if (_enemys.Num() == 0 && PlayerCharacter)
		ClearEnemy();
}

void AFightZoneFinder::ClearEnemy()
{
	
}

bool AFightZoneFinder::IsEmpty ()
{
	return _enemys.Num () == 0 ? true : false;
}

void AFightZoneFinder::EndFight()
{
	LOG(Warning, "%%%%%%%%%%%% End Fight %%%%%%%%%%%%%");

	OnEndFight.Broadcast();
	if (_isCanSelectBuffCola)
	{
		if (VendingMachine)
		{
			LOG(Warning, "%%%%%%%%%%%%% Can Select Cola %%%%%%%%%%%%");

			if(nextFightZone)
			{
				VendingMachine->SetNextFightZoneFinder(nextFightZone);
			}
			Reward();
		}
		else
		{
			LOG(Warning, "Please specify vendingMachine.");
		}
	}
	else
	{
		if (nextFightZone)
		{
			LOG(Warning, "%%%%%%%%%%% Active Next FightZone %%%%%%%%%%%%");

			nextFightZone->FightZoneActive();
		}
		else
		{
			PlayerCharacter->OpenWinWidget();
		}
	}
	
	Destroy();
}

// Begin Play 전에 실행 되더이다....
void AFightZoneFinder::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!_bOnFirstCheck) return;

	if (!_enemys.Contains(OtherActor) && (OtherActor->IsA(AEnemyAI::StaticClass()) /*|| OtherActor->GetClass()->ImplementsInterface(UInterface_Enemy::StaticClass()*/)) {
		AEnemyAI* enemy = Cast<AEnemyAI>(OtherActor);
		PushEnemyArray(enemy);

		enemy->OnDie.AddLambda([this](AEnemyAI* enemy) {
			RemoveEnemyArray(enemy);
		}, enemy);
	}

	else if (OtherActor->IsA(ARoxyCharacter::StaticClass()))
	{
		EnterPlayer(Cast<ARoxyCharacter>(OtherActor));
	}
}

void AFightZoneFinder::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (_enemys.Contains(OtherActor)) {
		LOG_SCREEN("enemy remove");
		_enemys.Remove(Cast<AEnemyAI>(OtherActor));
		ClearEnemyCheck();
		// 적을 다 처치했고 현재 웨이브에서 남은 스폰 포인트가 있는지 체크
		// 남은 스폰포인트가 없고 다음 웨이브가 존재하면 다음 웨이브로 넘어가고 
		// 다음 웨이브가 없다면 현재 fightZone 종료
		
		if (_enemys.Num() == 0 && _spawnPointNum == 0)
		{
			if (_currentWaveNum < _maxWaveNum)
			{
				LOG(Warning, "%%%%%%%%%%%%%%%%%%%%%%%%%% Next Spawner %%%%%%%%%%%%%%%%%%%%%%%%%%");
				_currentWaveNum++;
				NewWave.Broadcast();
			}
			else if (_currentWaveNum >= _maxWaveNum)
			{
				LOG(Warning, "%%%%%%%%%%%%%%%%%%%%%%%%%% Clear All Spawner %%%%%%%%%%%%%%%%%%%%%%%%%%");
				EndFight();
			}
		}
		
	}
	else if (OtherActor->IsA(ARoxyCharacter::StaticClass())) 
	{
		ARoxyCharacter* Roxy = Cast<ARoxyCharacter>(OtherActor);
	}
}

void AFightZoneFinder::PushEnemyArray(AEnemyAI* enemy)
{
	if (_bOnFirstCheck && !_enemys.Contains(enemy))
	{
		_enemys.Push(enemy);
		enemy->OnDie_OneParam.AddUFunction(this, "RemoveEnemyArray", enemy);
	}
	LOG(Warning, "Enter %s", *(enemy->GetName()));
}

void AFightZoneFinder::RemoveEnemyArray (AEnemyAI* enemy)
{
	if (_bOnFirstCheck && _enemys.Contains(enemy))
	{
		_enemys.Remove(enemy);
		DecreaseAllEnemysNum();

		if (_spawnPointNum == 0)
		{
			if(_enemys.Num() == 0)
			{
				if (_currentWaveNum < _maxWaveNum)
				{
					_currentWaveNum++;
					NewWave.Broadcast();
				}
				else
				{
					LOG(Warning, "END FIGHT");
					EndFight();
				}
			}
			else
			{
				bool bIsEmpty = true;
				LOG(Warning, "Empty = true");
				for (auto i : _enemys)
				{
					if (i != nullptr)
					{
						bIsEmpty = false;
						LOG(Warning, "Empty = false");
					}
				}
				if (bIsEmpty)
				{
					if (_currentWaveNum < _maxWaveNum)
					{
						_currentWaveNum++;
						NewWave.Broadcast();
					}
					else if (_currentWaveNum >= _maxWaveNum)
					{
						EndFight();
						LOG(Warning, "End Fight");
					}
				}
			}
		}
	}
	LOG(Warning, "Remove %s", *(enemy->GetName()));
}

void AFightZoneFinder::PushSpawnerArray(ArenewalEnemySpawner* spawner)
{
	if (!_spawners.Contains(spawner))
	{
		_spawners.Push(spawner);
	}
}

void AFightZoneFinder::RemoveSpawnerArray(ArenewalEnemySpawner* spawner)
{
	if (_spawners.Contains(spawner))
	{
		_spawners.Remove(spawner);
		_bIsSpawning = false;
		for (int i = 0;i < _spawners.Num(); i++)
		{
			_spawners[i]->SetEnableSpawn(true);
		}
	}
}

void AFightZoneFinder::Reward()
{
	VendingMachine->ActiveInteractionCollision();
}

void AFightZoneFinder::StartSpawn(ArenewalEnemySpawner* spawner)
{
	if (_spawners.Contains(spawner))
	{
		if(!_bOnFirstSpawnCheck)
		{
			_bOnFirstSpawnCheck = true;
			OnFirstSpawn.Broadcast();
		}

		_bIsSpawning = true;
		for (int i = 0; i<_spawners.Num();i++)
		{
			if (_spawners[i] != spawner)
			{
				_spawners[i]->SetEnableSpawn(false);
			}
		}
	}
}
