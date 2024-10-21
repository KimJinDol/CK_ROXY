// Fill out your copyright notice in the Description page of Project Settings.


#include "NewEnemySpawner.h"
#include "InGame/System/Generator.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/System/FightZoneFinder.h"
#include "InGame/System/EnemySpawnerWidget.h"
#include "Core/NunuGameInstance.h"

// Sets default values
ANewEnemySpawner::ANewEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ANewEnemySpawner::BeginSpawnWave()
{
	SetCurrentWaveToNext();
}

void ANewEnemySpawner::SetCurrentWaveToNext()
{

	// ���� ���̺갡 �������� �ƴ� ���� ���� ���̺�� �Ѿ�ش�.
	if (_ESWaveArr.Num() > _curWaveIndex + 1 && !_bCanSpawn)
	{
		_curWaveIndex++;
		_sumPlayTime = 0.f;
		_curSpawnIndex = 0;
		_bCanSpawn = true;

		WidgetInstance->StartNewWave(_curWaveIndex);

		Roxy->SetPlayBGM();
	}
	else
	{
		_bCanSpawn = false;
	}
}

// ���� ���̺��� ���� ������ �������� �������ִ� �Լ�
void ANewEnemySpawner::SetCurrentSpawnInfoToNext()
{
	if (_ESWaveArr[_curWaveIndex]._ESInfos.Num() > _curSpawnIndex + 1)
	{
		_curSpawnIndex++;
	}
	/*���� �ε����� ������ �ε������ ���̺긦 �������Ѵ�*/
	else
	{
		_bCanSpawn = false;
		_bOnEndWaveBroadCast = false;
		WidgetInstance->FinishCurrentWave();
	}
}

void ANewEnemySpawner::SpawnEnemy (TSubclassOf<AEnemyAI> enemyType, EEnemySpawnPoint_2 spawnPoint)
{
	FVector2D addedRandVec = FMath::RandPointInCircle(300.f);
	FVector resultSpawnLoc(FVector(addedRandVec.X, addedRandVec.Y, 0.f));


	switch (spawnPoint)
	{
	case EEnemySpawnPoint_2::EPoint_1:
		resultSpawnLoc += _spawnLocation_Point1;
		break;
	case EEnemySpawnPoint_2::EPoint_2:
		resultSpawnLoc += _spawnLocation_Point2;
		break;
	case EEnemySpawnPoint_2::EPoint_3:
		resultSpawnLoc += _spawnLocation_Point3;
		break;
	case EEnemySpawnPoint_2::EPoint_4:
		resultSpawnLoc += _spawnLocation_Point4;
		break;
	case EEnemySpawnPoint_2::EPoint_5:
		resultSpawnLoc += _spawnLocation_Point5;
		break;
	case EEnemySpawnPoint_2::EPoint_6:
		resultSpawnLoc += _spawnLocation_Point6;
		break;
	}
	GetWorld ()->SpawnActor<AEnemyAI> (enemyType, resultSpawnLoc, FRotator::ZeroRotator);

}

int32 ANewEnemySpawner::GetCurrentWaveIndex()
{
	return _curWaveIndex;
}

bool ANewEnemySpawner::IsLastWave()
{
	return _ESWaveArr.Num() <= _curWaveIndex + 1;
}

// Called when the game starts or when spawned
void ANewEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if(WidgetTemplate != NULL)
	{ 
// 		WidgetInstance = CreateWidget<UEnemySpawnerWidget>(GetWorld(), WidgetTemplate);
// 		WidgetInstance->BindOwner(this);
// 		WidgetInstance->AddToViewport();
	}
	BindNunuInGameManager(InGameManager);
	Roxy = InGameManager->GetPlayerCharacter();
	CHECK(Roxy != nullptr);
}

// Called every frame
void ANewEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	// ���� ���̺갡 �������� �ƴ϶�� ���� �� ����� �� ��ȣ�ۿ� ������ ���·� ���Ѵ�
	if (!_bOnEndWaveBroadCast && _ESWaveArr.Num() >= _curWaveIndex + 1 && !Generator->IsFirstCheck() && !Generator->IsCanInteraction() && !_bCanSpawn && FightZoneFinder->IsEmpty())
	{
		OnEndAnWave.Broadcast();
		_bOnEndWaveBroadCast = true;
		if (Generator->GetRpRatio() < 1.f && _ESWaveArr.Num() > _curWaveIndex + 1)
			Generator->BreakDown();
		else if (_ESWaveArr.Num() == _curWaveIndex + 1)
			Generator->BreakDown();
	}

	if (_ESWaveArr.Num() == 0 || !_bCanSpawn) return;
	if(_ESWaveArr[_curWaveIndex]._ESInfos.Num() == 0) return;

	_sumPlayTime += DeltaTime;
	FEnemySpawnInfoStruct currentSpawnInfo = _ESWaveArr[_curWaveIndex]._ESInfos[_curSpawnIndex];
	/*���� ���̺� �迭�� ��� ���� ������� �����Ѵ�*/
	if (currentSpawnInfo._spawnTime <= _sumPlayTime)
	{
		for(int i = 0; i < currentSpawnInfo._spawnNum; i++)
		{ 
			SpawnEnemy(currentSpawnInfo._enemyType, currentSpawnInfo._spawnPoint);
		}
		SetCurrentSpawnInfoToNext();
	}

}

