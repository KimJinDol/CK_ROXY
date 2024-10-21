// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectNunu.h"
#include "InGame/Enemy/GruntAI/GruntAnimInstance.h"
#include "DarkJoyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FDarkJoy_AimNotifyDelegate);
/**
 * 
 */
UCLASS()
class ROXY_API UDarkJoyAnimInstance : public UGruntAnimInstance
{
	GENERATED_BODY()

public:
	UDarkJoyAnimInstance();

private:
	class ADarkJoy* DarkJoy;

private:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
};
