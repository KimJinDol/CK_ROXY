// Fill out your copyright notice in the Description page of Project Settings.


#include "CaterpiAnimInstance.h"
#include "InGame/Enemy/GruntAI/Dummy/GreenCaterpi/GreenCaterpi.h"

UCaterpiAnimInstance::UCaterpiAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Hit_MONTAGE(TEXT("/Game/Resources/Enemy/Grunt/GreenCaterpi/Box_Damaged_Mon.Box_Damaged_Mon"));
	if (Hit_MONTAGE.Succeeded())
	{
		HitMontage = Hit_MONTAGE.Object;
	}
}