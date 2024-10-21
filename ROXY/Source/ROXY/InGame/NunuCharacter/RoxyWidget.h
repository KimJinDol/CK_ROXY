// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/EnemyAI.h"
#include "Blueprint/UserWidget.h"
#include "RoxyWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API URoxyWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InGameManager, meta = (AllowPrivateAccess = true))
		class ANunuInGameManager* InGameManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, meta = (AllowPrivateAccess = true))
		class ARoxyCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Minimap, meta = (AllowPrivateAccess = true))
		TArray<AEnemyAI*> EnemysInfoForMinimap; // �̴ϸʿ� ���� �� �迭

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combo, meta = (AllowPrivateAccess = true))
		class UComboComponent* ComboComponent;


protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION()
		void BindOwner(class ARoxyCharacter* _playerCharacter);

	/* �޺� */
	UFUNCTION(BlueprintImplementableEvent)
		void ApplyCombo();
	UFUNCTION(BlueprintImplementableEvent)
		void OffCombo();

	/* ȭ�� ȿ�� */
	UFUNCTION(BlueprintImplementableEvent)
		void OnScreenBlood();
	UFUNCTION(BlueprintImplementableEvent)
		void OnFeverScreen(bool value);

	/* ���� �̻� */
	UFUNCTION(BlueprintImplementableEvent)
		void OnFaintCondition();
	UFUNCTION(BlueprintImplementableEvent)
		void OnFaintDefianceKey(bool value);
	UFUNCTION(BlueprintImplementableEvent)
		void OnAddictedCondition();

	/* ���� ȿ���� */
	UFUNCTION(BlueprintImplementableEvent)
		void OnAttack_01(int32 value); /* value = 1 : �Ϲ�, 2 : ��Ÿ */
	UFUNCTION(BlueprintImplementableEvent)
		void OnAttack_02(int32 value);
	UFUNCTION(BlueprintImplementableEvent)
		void OnAttack_03(int32 value);
	UFUNCTION(BlueprintImplementableEvent)
		void OnAttack_Critical();
	UFUNCTION(BlueprintImplementableEvent)
		void OnKick(); /* ������ */
	UFUNCTION(BlueprintImplementableEvent)
		void OnKickCancle(); /* ����Ʈ ĵ���� ������ */
	UFUNCTION(BlueprintImplementableEvent)
		void OnSlash(); /* �ϼ� */
	UFUNCTION(BlueprintImplementableEvent)
		void OnSlam(); /* ���� */

	/* ���� */
	UFUNCTION(BlueprintImplementableEvent)
		void OnAddedStat();
	UFUNCTION(BlueprintImplementableEvent)
		void OnAddedHP();
	UFUNCTION(BlueprintImplementableEvent)
		void OnAddedDamage();
	UFUNCTION(BlueprintImplementableEvent)
		void OnDereasedCoolTime();

// 	UFUNCTION(BlueprintImplementableEvent)
// 		void SetRepairWidgetVisibility(bool value);
	
};
