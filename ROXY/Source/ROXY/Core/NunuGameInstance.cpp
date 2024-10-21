// Fill out your copyright notice in the Description page of Project Settings.


#include "NunuGameInstance.h"


UNunuGameInstance::UNunuGameInstance ()
{
	
}

ANunuInGameManager* UNunuGameInstance::GetNunuInGameManager()
{
	TArray<AActor*> inGameManager;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANunuInGameManager::StaticClass(), inGameManager);

	// 찾고자 하는 액터가 월드 상에서 유일하다면 해당 액터를 반환한다.
	if (inGameManager.Num() == 1) return Cast<ANunuInGameManager>(inGameManager[0]);

	LOG_SCREEN("Not Exist InGameManager!");
	return nullptr;
}

/* 캐릭터 인스턴스 관련 */
void UNunuGameInstance::SetAddedMaxHp(const float& addedMaxHp)
{
	_addedMaxHp += addedMaxHp; 
	ChangeMaxHp.Broadcast();
}

void UNunuGameInstance::SetAddedMoveSpeed(const float& addedMoveSpeed)
{
	_addedMoveSpeed += addedMoveSpeed;
	ChangeMoveSpeed.Broadcast();
}

void UNunuGameInstance::SetCriticalHitMultiply(const float& criticalHitMultiply)
{
	_criticalHitMultiply += criticalHitMultiply;
	ChangeCriticalHitMultiply.Broadcast();
}

void UNunuGameInstance::SetAddedDamage(const float& addedDamage)
{
	_addedDamage += addedDamage;
	ChangeDamage.Broadcast();
}

void UNunuGameInstance::SetDecreasedCoolTimePercent(const float& decreasedCoolTimePercent)
{
	_decreasedCoolTimePercent += decreasedCoolTimePercent;
	ChangeDecreasedCoolTimePer.Broadcast();
}

/* 클리어 타임 카운트 */
void UNunuGameInstance::ClearTimeCountStart()
{
	FTimerDelegate _clearTimeDelegate;
	_clearTimeDelegate.BindUFunction(this, "IncreaseClearTime");

	LOG(Warning, "Set Clear Timer");

	GetWorld()->GetTimerManager().SetTimer(_clearTimeTimer, _clearTimeDelegate, 1.0f, true);
}

void UNunuGameInstance::InCreaseClearTime()
{
	_clearTime++;
}

void UNunuGameInstance::ClearTimeCountStop()
{
	LOG(Warning, "Pause Clear Timer");
	GetWorld()->GetTimerManager().PauseTimer(_clearTimeTimer);
}

void UNunuGameInstance::ResetAllEndingData()
{
	_numOfStep = 0;
	_useBasicAttack = 0; 
	_useSkill = 0;
	_numOfHit = 0;
	_numOfHitBlow = 0;
	_numOfDamaged = 0;
	_numOfFaint = 0;
	_numOfAddicted = 0;
	_missKick = 0;
	_clearTime = 0.0f;
}

/* 게임 플로우 관련 */
void UNunuGameInstance::SetPlayedTutorial()
{
	_bIsPlayTutorial = true;
}

void UNunuGameInstance::SetPlayedCutScene_stage_opening()
{
	_bIsPlayCutScene_stage_opening = true;
}

void UNunuGameInstance::SetPlayedCinematic_BossSpawn()
{
	_bIsPlayBossSpawnCinematic = true;
}

// void UNunuGameInstance::SetVolume_BGM (float value)
// {
// // 	if (SoundClass_BGM != nullptr)
// // 	{
// // 		UGameplayStatics::SetSoundMixClassOverride(this,)
// // 	}
// // 	else
// // 		LOG(Error, "NunuGameInstance : Not Exist BGM Sound Class");
// }
