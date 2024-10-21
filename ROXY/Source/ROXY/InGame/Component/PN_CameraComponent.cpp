// Fill out your copyright notice in the Description page of Project Settings.


#include "PN_CameraComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

UPN_CameraComponent::UPN_CameraComponent()
{
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> M_RADIALBLUR_INST1(TEXT("/Game/Resources/PostProcess/M_RadialBlur_Inst.M_RadialBlur_Inst"));
	if (M_RADIALBLUR_INST1.Succeeded())
	{
		RadialBlur_Inst1 = M_RADIALBLUR_INST1.Object;
		RadialBlur_Blendable.Weight = 1;
		RadialBlur_Blendable.Object = M_RADIALBLUR_INST1.Object;
		PostProcessSettings.WeightedBlendables.Array.Add(RadialBlur_Blendable);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> M_RADIALBLUR_INST2(TEXT("/Game/Resources/PostProcess/M_RadialBlur_Inst2.M_RadialBlur_Inst2"));
	if (M_RADIALBLUR_INST2.Succeeded())
	{
		RadialBlur_Inst2 = M_RADIALBLUR_INST2.Object;
	}
	
	PostProcessSettings.ChromaticAberrationStartOffset = 0.f;
	PostProcessSettings.bOverride_ChromaticAberrationStartOffset = 5.f;

	OffPostProcess();
}

void UPN_CameraComponent::OnPostProcess(float duration /*= 0.f*/)
{
	SetPostProcessBlendWeight(1.f);
	
	if (duration != 0)
		GetWorld()->GetTimerManager().SetTimer(HitFeelTimer, this, &UPN_CameraComponent::OffPostProcess, duration, false);
}

void UPN_CameraComponent::OffPostProcess()
{
	SetPostProcessBlendWeight(0.f);
}

void UPN_CameraComponent::ChangeRadialBlurMat(int32 value /*= 1*/)
{
	if (value == 1)
	{
		RadialBlur_Blendable.Object = RadialBlur_Inst1;
	}
	else if (value == 2)
	{
		RadialBlur_Blendable.Object = RadialBlur_Inst2;
	}
}

void UPN_CameraComponent::SetRadialBlurMatParam(float blurValue /*= 0.4f*/)
{
	/*int32 blurIndex = PostProcessSettings.WeightedBlendables.Array.Find(RadialBlur_Blendable);*/
	PostProcessSettings.WeightedBlendables.Array[0].Weight = blurValue; 
}
