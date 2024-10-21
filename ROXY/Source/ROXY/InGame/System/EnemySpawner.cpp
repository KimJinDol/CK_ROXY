
// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Enemy/EnemyAI.h"
#include "InGame/Enemy/GruntAI/Charge/ChargeAI.h"
#include "InGame/Enemy/GruntAI/Combat/CombatAI.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAI.h"
#include "InGame/System/FightZoneFinder.h"
#include "InGame/System/Generator.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<ACombatAI> COMBAT_AI(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Combat/BP_CombatAI.BP_CombatAI_C"));
	if (COMBAT_AI.Succeeded())
	{
		CombatAI = COMBAT_AI.Class;
	}

	static ConstructorHelpers::FClassFinder<AThrowAI> THROW_AI(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Throw/BP_ThrowAI.BP_ThrowAI_C"));
	if (THROW_AI.Succeeded())
	{
		ThrowAI = THROW_AI.Class;
	}

	static ConstructorHelpers::FClassFinder<AChargeAI> CHARGE_AI(TEXT("/Game/Blueprints/InGame/Enemy/Grunt/Charge/BP_ChargeAI.BP_ChargeAI_C"));
	if (CHARGE_AI.Succeeded())
	{
		ChargeAI = CHARGE_AI.Class;
	}

}

void AEnemySpawner::FirstInitSpawnInfo()
{
	_nextSpawnTimeGap = 20.f;

	_spawnNumMin_Phase1 = 8;
	_spawnNumMax_Phase1 = 10;
	_throwAISpawnNumMin_Phase1 = 2;
	_throwAISpawnNumMax_Phase1 = 3;
	_spawnNumMinPerPoint_Phase1 = 2;

	_spawnNumMin_Phase2 = 12;
	_spawnNumMax_Phase2 = 14;
	_throwAISpawnNumMin_Phase2 = 4;
	_throwAISpawnNumMax_Phase2 = 5;
	_spawnNumMinPerPoint_Phase2 = 4;

	_spawnNumMin_Phase3 = 16;
	_spawnNumMax_Phase3 = 18;
	_throwAISpawnNumMin_Phase3 = 6;
	_throwAISpawnNumMax_Phase3 = 7;
	_spawnNumMinPerPoint_Phase3 = 5;

	_spawnNumMin_Phase4 = 20;
	_spawnNumMax_Phase4 = 22;
	_throwAISpawnNumMin_Phase4 = 8;
	_throwAISpawnNumMax_Phase4 = 9;
	_spawnNumMinPerPoint_Phase4 = 6;
}


void AEnemySpawner::SetCurrentPhaseToNext(EEnemySpawnPhase newPhase)
{
	_currentPhase = newPhase;
}

void AEnemySpawner::StartEnemySpawn()
{
	EnemySpawnPhase();

	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawner::EnemySpawnPhase, _nextSpawnTimeGap, true);
}

void AEnemySpawner::EnemySpawnPhase()
{
	int32 spawnNum = 0;
	int32 throwAiSpawnNum = 0;
	TArray<EEnemySpawnPoint> throwSpawnPointArr;
	int32 spawnNumMinPerPoint = 0;

	switch (_currentPhase)
	{
	case EEnemySpawnPhase::EPhase_1:
		// �� ���� �������� ���Ѵ�.
		spawnNum = UKismetMathLibrary::RandomIntegerInRange(_spawnNumMin_Phase1, _spawnNumMax_Phase1);
		// �� ����Ʈ ���� ������ �ּ� ���� ���� ������ ���� �����Ѵ�.
		spawnNumMinPerPoint = _spawnNumMinPerPoint_Phase1;
		// ���Ÿ� ai ���� ���� ���Ѵ�.
		throwAiSpawnNum = UKismetMathLibrary::RandomIntegerInRange(_throwAISpawnNumMin_Phase1, _throwAISpawnNumMax_Phase1);
		break;

	case EEnemySpawnPhase::EPhase_2:
		// �� ���� �������� ���Ѵ�.
		spawnNum = UKismetMathLibrary::RandomIntegerInRange(_spawnNumMin_Phase2, _spawnNumMax_Phase2);
		// �� ����Ʈ ���� ������ �ּ� ���� ���� ������ ���� �����Ѵ�.
		spawnNumMinPerPoint = _spawnNumMinPerPoint_Phase2;
		// ���Ÿ� ai ���� ���� ���Ѵ�.
		throwAiSpawnNum = UKismetMathLibrary::RandomIntegerInRange(_throwAISpawnNumMin_Phase2, _throwAISpawnNumMax_Phase2);
		break;

	case EEnemySpawnPhase::EPhase_3:
		// �� ���� �������� ���Ѵ�.
		spawnNum = UKismetMathLibrary::RandomIntegerInRange(_spawnNumMin_Phase3, _spawnNumMax_Phase3);
		// �� ����Ʈ ���� ������ �ּ� ���� ���� ������ ���� �����Ѵ�.
		spawnNumMinPerPoint = _spawnNumMinPerPoint_Phase3;
		// ���Ÿ� ai ���� ���� ���Ѵ�.
		throwAiSpawnNum = UKismetMathLibrary::RandomIntegerInRange(_throwAISpawnNumMin_Phase3, _throwAISpawnNumMax_Phase3);	
		break;

	case EEnemySpawnPhase::EPhase_4:
		// �� ���� �������� ���Ѵ�.
		spawnNum = UKismetMathLibrary::RandomIntegerInRange(_spawnNumMin_Phase4, _spawnNumMax_Phase4);
		// �� ����Ʈ ���� ������ �ּ� ���� ���� ������ ���� �����Ѵ�.
		spawnNumMinPerPoint = _spawnNumMinPerPoint_Phase4;
		// ���Ÿ� ai ���� ���� ���Ѵ�.
		throwAiSpawnNum = UKismetMathLibrary::RandomIntegerInRange(_throwAISpawnNumMin_Phase4, _throwAISpawnNumMax_Phase4);
		break;
	}

	// ���Ÿ� ai ���� �� <= �� ���� ���� �� �� Ȯ��.
	CHECK(throwAiSpawnNum <= spawnNum);

	// ���Ÿ� ������ �����Ѵ�.
	for (int i = 0; i < throwAiSpawnNum; i++)
	{
		EEnemySpawnPoint throwSpawnPoint = (EEnemySpawnPoint)(UKismetMathLibrary::RandomInteger((int32)EEnemySpawnPoint::Size));
		EnemySpawn(ThrowAI, throwSpawnPoint);
		throwSpawnPointArr.Push(throwSpawnPoint);
		spawnNum--;
	}
	// �� ����Ʈ���� �����ؾ��� �ּ� �������� �޼����ش�. (���Ÿ� ����)
	for (int i = 0; i < spawnNumMinPerPoint; i++)
	{
		if (i < throwSpawnPointArr.Num())
		{
			if (throwSpawnPointArr[i] == EEnemySpawnPoint::EPoint_1)
			{
				EnemySpawn(CombatAI, EEnemySpawnPoint::EPoint_2);
				EnemySpawn(CombatAI, EEnemySpawnPoint::EPoint_3);
			}
			else if (throwSpawnPointArr[i] == EEnemySpawnPoint::EPoint_2)
			{
				EnemySpawn(CombatAI, EEnemySpawnPoint::EPoint_1);
				EnemySpawn(CombatAI, EEnemySpawnPoint::EPoint_3);
			}
			else if (throwSpawnPointArr[i] == EEnemySpawnPoint::EPoint_3)
			{
				EnemySpawn(CombatAI, EEnemySpawnPoint::EPoint_1);
				EnemySpawn(CombatAI, EEnemySpawnPoint::EPoint_2);
			}
			spawnNum -= 2;
		}
		else
		{
			EnemySpawn(CombatAI, EEnemySpawnPoint::EPoint_1);
			EnemySpawn(CombatAI, EEnemySpawnPoint::EPoint_2);
			EnemySpawn(CombatAI, EEnemySpawnPoint::EPoint_3);
			spawnNum -= 3;
		}
	}

	// ���� �������� ���� ����Ʈ���� �����Ѵ�
	for (int i = 0; i < spawnNum; i++)
	{
		EnemySpawn(CombatAI, EEnemySpawnPoint::EPoint_random);
	}
}

void AEnemySpawner::EnemySpawn(TSubclassOf<AEnemyAI> enemyType, EEnemySpawnPoint spawnPoint)
{
	FVector2D randLocation = FMath::RandPointInCircle(800.f);
	FVector spawnLocation(FVector(randLocation.X, randLocation.Y, 0.f));

	if (spawnPoint == EEnemySpawnPoint::EPoint_random)
	{
		spawnPoint = (EEnemySpawnPoint)(UKismetMathLibrary::RandomInteger((int32)EEnemySpawnPoint::Size));
	}

	switch (spawnPoint)
	{
	case EEnemySpawnPoint::EPoint_1:
		spawnLocation += _spawnLocation_Point1;
		break;
	case EEnemySpawnPoint::EPoint_2:
		spawnLocation += _spawnLocation_Point2;
		break;
	case EEnemySpawnPoint::EPoint_3:
		spawnLocation += _spawnLocation_Point3;
		break;
	}

	GetWorld()->SpawnActor<AEnemyAI>(enemyType, spawnLocation, FRotator::ZeroRotator);
}

void AEnemySpawner::EnemySpawn(TSubclassOf<AEnemyAI> enemyType, FVector spawnLocation)
{
	GetWorld()->SpawnActor<AEnemyAI>(enemyType, spawnLocation, FRotator::ZeroRotator);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (InGameManager != nullptr)
		Roxy = InGameManager->GetPlayerCharacter();
	else
		LOG_SCREEN("Enemy Spawner : Failed finding InGameManager!");

	if (_bSetDefault)
	{
		FirstInitSpawnInfo();
	}
}

void AEnemySpawner::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_sumPlayTime += DeltaTime;
	if (_sumPlayTime > 60.f && _sumPlayTime <= 120.f && _currentPhase == EEnemySpawnPhase::EPhase_1)
		SetCurrentPhaseToNext(EEnemySpawnPhase::EPhase_2);
	else if (_sumPlayTime > 120.f && _sumPlayTime <= 240.f && _currentPhase == EEnemySpawnPhase::EPhase_2)
		SetCurrentPhaseToNext(EEnemySpawnPhase::EPhase_3);
	else if (_sumPlayTime > 240.f && _currentPhase == EEnemySpawnPhase::EPhase_4)
		SetCurrentPhaseToNext(EEnemySpawnPhase::EPhase_4);


	// ������ �������� ���� ���� �̳ʹ� ���� �������� �Ѿ�� ����
	if (!_bSpawnCES || _CESGeneratorRatio.Num() <= _CESCurrentIndex || Generator == nullptr) return;

	if (_CESGeneratorRatio.Num() > 0 && _CESLocationArray.Num() > 0)
	{
		if (Generator->GetRpRatio() >= _CESGeneratorRatio[_CESCurrentIndex])
		{
			int indexTemp = 0;
			for (int i = 0; i < _CESLocationArray.Num() - 1; i++)
			{
				float distance = FVector::Distance(Roxy->GetActorLocation(), _CESLocationArray[i]);
				float distanceNext = FVector::Distance(Roxy->GetActorLocation(), _CESLocationArray[i + 1]);

				if (distance < distanceNext)
					indexTemp = i + 1;
				else
					indexTemp = i;
			}

			EnemySpawn(ChargeAI, _CESLocationArray[indexTemp]);
			LOG_SCREEN("Spawn Success Charge AI");

			_CESCurrentIndex++;
		}
	}
	else
	{
		LOG_SCREEN("CES Location Array Or CES GeneratorRatio Array is Empty! Please Set Value");
	}		
}

