// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "Camera/CameraComponent.h"
#include "PN_CameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROXY_API UPN_CameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UPN_CameraComponent();

	UFUNCTION()
	void OnPostProcess(float duration = 0.f);

	UFUNCTION()
	void OffPostProcess();

	UFUNCTION()
	void ChangeRadialBlurMat(int32 value = 1);

	UFUNCTION()
	void SetRadialBlurMatParam(float blur = 0.4f);
	

private:
	UPROPERTY()
	UMaterialInstance* RadialBlur_Inst1;
	UPROPERTY()
	UMaterialInstance* RadialBlur_Inst2;

	UPROPERTY()
	FWeightedBlendable RadialBlur_Blendable;

	UPROPERTY()
	FTimerHandle HitFeelTimer;
	
};
