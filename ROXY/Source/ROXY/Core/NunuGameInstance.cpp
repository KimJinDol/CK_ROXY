// Fill out your copyright notice in the Description page of Project Settings.


#include "NunuGameInstance.h"


UNunuGameInstance::UNunuGameInstance ()
{
	
}

ANunuInGameManager* UNunuGameInstance::GetNunuInGameManager()
{
	TArray<AActor*> inGameManager;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANunuInGameManager::StaticClass(), inGameManager);

	// ã���� �ϴ� ���Ͱ� ���� �󿡼� �����ϴٸ� �ش� ���͸� ��ȯ�Ѵ�.
	if (inGameManager.Num() == 1) return Cast<ANunuInGameManager>(inGameManager[0]);

	LOG_SCREEN("Not Exist InGameManager!");
	return nullptr;
}

/* ĳ���� �ν��Ͻ� ���� */
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

/* Ŭ���� Ÿ�� ī��Ʈ */
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

/* ���� �÷ο� ���� */
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
