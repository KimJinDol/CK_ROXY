// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Blueprint/UserWidget.h"
#include "GruntWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UGruntWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
		class AGruntAI* AICharacter;

	void BindAIStat(class UHpComponent* NewAIStat);

	void BindDamage(float damage);

protected:
	virtual void NativeConstruct() override;
	/*void UpdateHPWidget();*/

private:
	TWeakObjectPtr<class UHpComponent> CurrentAIStat;

	UPROPERTY()
		class UProgressBar* HPProgressBar;

	UPROPERTY()
		float DamageAmount;

public:
	// BlueprintPure = �Լ��� �������Ʈ���� ȣ���� �� �ֵ��� ���ִ� ������Ƽ
	UFUNCTION(BlueprintPure)
		float GetHpPercent();

	UFUNCTION(BlueprintPure)
		float GetDamage();

public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnDamageIcon(bool value);
	UFUNCTION(BlueprintImplementableEvent)
		void PlayDamageAnim();

	UFUNCTION(BlueprintImplementableEvent)
		void OnEliteIcon(bool value);

};
