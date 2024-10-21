// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/System/NunuInGameManager.h"
#include "Engine/GameInstance.h"
#include "NunuGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNunuGameInstance_Delegate);

/**
 * 
 */
UCLASS()
class ROXY_API UNunuGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UNunuGameInstance();

private:
	UPROPERTY()
		USoundMix* SoundMix;

	UPROPERTY()
		USoundClass* SoundClass_BGM;

	UPROPERTY ()
		USoundClass* SoundClass_SFX;

	// �ɼ� ���� --------------------------------
	// ���콺 ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player_Basic_Stat, Meta = (AllowPrivateAccess = true))
		float _mouseSensitivity = 1.f;
	// BGM ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Sound, Meta = (AllowPrivateAccess = true))
		float _soundVolume_BGM = 1.f;
	// SFX ����
	UPROPERTY (VisibleAnywhere, BlueprintReadWrite, Category = Sound, Meta = (AllowPrivateAccess = true))
		float _soundVolume_SFX = 1.f;


	// ���� �÷��� ���� ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InGameData, Meta = (AllowPrivateAccess = true))
		bool _bIsPlayCutScene_stage_opening = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InGameData, Meta = (AllowPrivateAccess = true))
		bool _bIsPlayTutorial = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InGameData, Meta = (AllowPrivateAccess = true))
		bool _bIsPlayBossSpawnCinematic = false;

	// ���� ����� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EndingData, Meta = (AllowPrivateAccess = true))
		int32 _numOfStep = 0; // ���� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EndingData, Meta = (AllowPrivateAccess = true))
		int32 _useBasicAttack = 0; // �⺻���� ��� Ƚ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EndingData, Meta = (AllowPrivateAccess = true))
		int32 _useSkill = 0; // ��ų ��� Ƚ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EndingData, Meta = (AllowPrivateAccess = true))
		int32 _numOfHit = 0; // ������ ���� ���� Ƚ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EndingData, Meta = (AllowPrivateAccess = true))
		int32 _numOfHitBlow = 0; // �� ���� �������� ���� ���� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EndingData, Meta = (AllowPrivateAccess = true))
		int32 _numOfDamaged = 0; // �ǰ� Ƚ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EndingData, Meta = (AllowPrivateAccess = true))
		int32 _numOfFaint = 0; // ���� Ƚ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EndingData, Meta = (AllowPrivateAccess = true))
		int32 _numOfAddicted = 0; // �ߵ� Ƚ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EndingData, Meta = (AllowPrivateAccess = true))
		int32 _missKick = 0; // ������ ������ Ƚ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EndingData, Meta = (AllowPrivateAccess = true))
		float _clearTime = 0.0f; // Ŭ���� Ÿ��
	UPROPERTY()
		FTimerHandle _clearTimeTimer;


	// roxy (���� ĳ����) ���� ���� --------------
	// �߰� �� roxy hp
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
		float _addedMaxHp = 0.f;
	// �߰� �� move speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
		float _addedMoveSpeed = 0.f; 
	// ġ��Ÿ (�������� ����)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = stat, meta = (AllowPrivateAccess = true))
		float _criticalHitMultiply = 1.f;
	// �߰� ���ݷ� (ĳ���� ���ݷ� : �⺻ ���ݷ� + �߰� ���ݷ�)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = stat, meta = (AllowPrivateAccess = true))
		float _addedDamage = 0.f;
	// �� (�������� �� % (�Ҽ��� ����))
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = stat, meta = (AllowPrivateAccess = true, ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "0.0"))
		float _decreasedCoolTimePercent = 0.f;

	// �ݶ� ���� --------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuffCola, meta = (AllowPrivateAccess = true))
		bool _bIsUpgradeTeunTeunCola = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuffCola, meta = (AllowPrivateAccess = true))
		bool _bIsUpgradeChungKangCola = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuffCola, meta = (AllowPrivateAccess = true))
		bool _bIsUpgradeSunSuCola = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuffCola, meta = (AllowPrivateAccess = true))
		bool _bIsUpgradeVitaCola = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BuffCola, meta = (AllowPrivateAccess = true))
		bool _bIsUpgradePowerCola = false;

public:
	FNunuGameInstance_Delegate ChangeMaxHp;
	FNunuGameInstance_Delegate ChangeMoveSpeed;
	FNunuGameInstance_Delegate ChangeCriticalHitMultiply;
	FNunuGameInstance_Delegate ChangeDamage;
	FNunuGameInstance_Delegate ChangeDecreasedCoolTimePer;

public:
	ANunuInGameManager* GetNunuInGameManager();

	// ĳ���� ���� ���� ����
	UFUNCTION(BlueprintCallable)
		void SetAddedMaxHp(const float& addedMaxHp);
	UFUNCTION(BlueprintCallable)
		void SetAddedMoveSpeed(const float& addedMoveSpeed);
	UFUNCTION(BlueprintCallable)
		void SetCriticalHitMultiply(const float& criticalHitMultiply);
	UFUNCTION(BlueprintCallable)
		void SetAddedDamage(const float& addedDamage);
	UFUNCTION(BlueprintCallable)
		void SetDecreasedCoolTimePercent(const float& decreasedCoolTimePercent);


	UFUNCTION()
		float GetAddedMaxHp() { return _addedMaxHp; }
	UFUNCTION()
		float GetAddedMoveSpped() { return _addedMoveSpeed; }
	UFUNCTION()
		float GetCriticalHitMultiply() { return _criticalHitMultiply; }
	UFUNCTION()
		float GetAddedDamage() { return _addedDamage; }
	UFUNCTION()
		float GetDecreasedCoolTimePercent() { return _decreasedCoolTimePercent; }


	// ���� �ݶ� ����
	UFUNCTION(BlueprintCallable)
		void SetIsUpgradeTeunTeunCola(bool value) { _bIsUpgradeTeunTeunCola = value;}
	UFUNCTION(BlueprintCallable)
		void SetIsUpgradeChungKangCola(bool value) { _bIsUpgradeChungKangCola = value; }
	UFUNCTION(BlueprintCallable)
		void SetIsUpgradeSunSuCola(bool value) { _bIsUpgradeSunSuCola = value; }
	UFUNCTION(BlueprintCallable)
		void SetIsUpgradeVitaCola(bool value) { _bIsUpgradeVitaCola = value; }
	UFUNCTION(BlueprintCallable)
		void SetIsUpgradePowerCola(bool value) { _bIsUpgradePowerCola = value; }


	UFUNCTION(BlueprintPure)
		bool GetIsUpgradeTeunTeunCola() { return _bIsUpgradeTeunTeunCola;}
	UFUNCTION(BlueprintPure)
		bool GetIsUpgradeChungKangCola() { return _bIsUpgradeChungKangCola; }
	UFUNCTION(BlueprintPure)
		bool GetIsUpgradeSunSuCola() { return _bIsUpgradeSunSuCola; }
	UFUNCTION(BlueprintPure)
		bool GetIsUpgradeVitaCola() { return _bIsUpgradeVitaCola; }
	UFUNCTION(BlueprintPure)
		bool GetIsUpgradePowerCola() { return _bIsUpgradePowerCola; }


	// ���� ����� ������
	UFUNCTION(BlueprintCallable)
		int32 GetNumOfStep() { return _numOfStep; }
	UFUNCTION(BlueprintCallable)
		int32 GetUseBasicAttack() { return _useBasicAttack; }
	UFUNCTION(BlueprintCallable)
		int32 GetUseSkill() { return _useSkill; }
	UFUNCTION(BlueprintCallable)
		int32 GetNumOfHit() { return _numOfHit; }
	UFUNCTION(BlueprintCallable)
		int32 GetNumOfHitBlow() { return _numOfHitBlow; }
	UFUNCTION(BlueprintCallable)
		int32 GetNumOfDamaged() { return _numOfDamaged; }
	UFUNCTION(BlueprintCallable)
		int32 GetNumOfFaint() { return _numOfFaint; }
	UFUNCTION(BlueprintCallable)
		int32 GetNumOfAddicted() { return _numOfAddicted; }
	UFUNCTION(BlueprintCallable)
		int32 GetMissKick() { return _missKick; }
	UFUNCTION(BlueprintCallable)
		float GetClearTime() { return _clearTime; }

	UFUNCTION()
		void SetNumOfStep() { _numOfStep += 1; }
	UFUNCTION()
		void SetUseBasicAttack() { _useBasicAttack += 1; }
	UFUNCTION()
		void SetUseSkill() { _useSkill += 1; }
	UFUNCTION()
		void SetNumOfHit() { _numOfHit += 1; }
	UFUNCTION()
		void SetNumOfHitBlow(int32 value) { _numOfHitBlow = value; }
	UFUNCTION()
		void SetNumOfDamaged() { _numOfDamaged += 1; }
	UFUNCTION()
		void SetNumOfFaint() { _numOfFaint += 1; }
	UFUNCTION()
		void SetNumOfAddicted() { _numOfAddicted += 1; }
	UFUNCTION()
		void SetMissKick() { _missKick += 1; }
	UFUNCTION(BlueprintCallable)
		void ClearTimeCountStart();
	UFUNCTION()
		void InCreaseClearTime();
	UFUNCTION(BlueprintCallable)
		void ClearTimeCountStop();
	UFUNCTION(BlueprintCallable)
		void ResetAllEndingData();


	// �÷��̾� ���콺 ���� �������ִ� �Լ�
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SetPlayerSensitivity(float value);

	UFUNCTION (BlueprintCallable, BlueprintImplementableEvent)
		float GetPlayerSensitivity();

	// ������ ������ �����ϴ� �Լ�
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SetSoundVolume_BGM(float value);

	// ȿ���� ������ �����ϴ� �Լ�
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void SetSoundVolume_SFX (float value);

	// Ʃ�丮�� �÷��� ���� ��� �Լ�
	UFUNCTION(BlueprintCallable)
		void SetPlayedTutorial();
	UFUNCTION(BlueprintPure)
		bool WhetherPlayTotorial() {return _bIsPlayTutorial;}

	// �ƾ� ��� ���� ��� �Լ�
	UFUNCTION(BlueprintCallable)
		void SetPlayedCutScene_stage_opening();
	UFUNCTION(BlueprintPure)
		bool WhetherPlayCutScene_stage_opening() {return _bIsPlayCutScene_stage_opening;}

	// ������ �ó׸�ƽ �÷��� ���� ��� �Լ�
	UFUNCTION(BlueprintCallable)
		void SetPlayedCinematic_BossSpawn();
	UFUNCTION(BlueprintPure)
		bool WhetherPlayCinematic_BossSpawn() {
		return _bIsPlayBossSpawnCinematic;
	}

	// ���� ��� ���� ������ �������� �Լ�
// ���������� ����� �� �ְ� ®��
	UFUNCTION (BlueprintCallable, BlueprintImplementableEvent)
		float GetSoundVolume_BGM ();

	UFUNCTION (BlueprintCallable, BlueprintImplementableEvent)
		float GetSoundVolume_SFX ();
	
};
