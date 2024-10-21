// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Blueprint/UserWidget.h"
#include "EnemyWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
		class AEnemyAI* AICharacter;

	void BindAIStat(class UHpComponent* NewAIStat);

	void SetWidgetOwner(AEnemyAI* ownerAI) { AICharacter = ownerAI; };

protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<class UHpComponent> CurrentAIStat;

	UPROPERTY()
		class UProgressBar* HPProgressBar;

public:
	UFUNCTION(BlueprintPure)
		float GetHpPercent();

	UFUNCTION(BlueprintImplementableEvent)
		void SetEliteCombat();
	UFUNCTION(BlueprintImplementableEvent)
		void SetEliteThrow();
	UFUNCTION(BlueprintImplementableEvent)
		void SetRevival();
};
