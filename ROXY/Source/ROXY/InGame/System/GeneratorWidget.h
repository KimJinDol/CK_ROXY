// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Blueprint/UserWidget.h"
#include "GeneratorWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UGeneratorWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Owner, meta = (AllowPrivateAccess = true))
	class AGenerator* OwnerGenerator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GeneratorGauge, meta = (AllowPrivateAccess = true))
		bool _bOnInteraction = false;


protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetVisibilityInteraction(bool value);

	UFUNCTION()
	void BindOwner(AGenerator* generator);

	UFUNCTION(BlueprintImplementableEvent)
	void SetVisibilitySkillCheck(bool value);

	UFUNCTION(BlueprintImplementableEvent)
	void SetSkillCheckPoint(int32 value);

	UFUNCTION(BlueprintImplementableEvent)
	void TakeDamageGenerator();

	UFUNCTION(BlueprintImplementableEvent)
	void BreakdownGenerator();

	UFUNCTION(BlueprintImplementableEvent)
	void RepairGenerator();
};
