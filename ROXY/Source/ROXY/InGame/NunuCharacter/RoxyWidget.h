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
		TArray<AEnemyAI*> EnemysInfoForMinimap; // 미니맵에 쓰일 적 배열

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combo, meta = (AllowPrivateAccess = true))
		class UComboComponent* ComboComponent;


protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION()
		void BindOwner(class ARoxyCharacter* _playerCharacter);

	/* 콤보 */
	UFUNCTION(BlueprintImplementableEvent)
		void ApplyCombo();
	UFUNCTION(BlueprintImplementableEvent)
		void OffCombo();

	/* 화면 효과 */
	UFUNCTION(BlueprintImplementableEvent)
		void OnScreenBlood();
	UFUNCTION(BlueprintImplementableEvent)
		void OnFeverScreen(bool value);

	/* 상태 이상 */
	UFUNCTION(BlueprintImplementableEvent)
		void OnFaintCondition();
	UFUNCTION(BlueprintImplementableEvent)
		void OnFaintDefianceKey(bool value);
	UFUNCTION(BlueprintImplementableEvent)
		void OnAddictedCondition();

	/* 공격 효과음 */
	UFUNCTION(BlueprintImplementableEvent)
		void OnAttack_01(int32 value); /* value = 1 : 일반, 2 : 강타 */
	UFUNCTION(BlueprintImplementableEvent)
		void OnAttack_02(int32 value);
	UFUNCTION(BlueprintImplementableEvent)
		void OnAttack_03(int32 value);
	UFUNCTION(BlueprintImplementableEvent)
		void OnAttack_Critical();
	UFUNCTION(BlueprintImplementableEvent)
		void OnKick(); /* 발차기 */
	UFUNCTION(BlueprintImplementableEvent)
		void OnKickCancle(); /* 엘리트 캔슬한 발차기 */
	UFUNCTION(BlueprintImplementableEvent)
		void OnSlash(); /* 일섬 */
	UFUNCTION(BlueprintImplementableEvent)
		void OnSlam(); /* 연참 */

	/* 스탯 */
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
