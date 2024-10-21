// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance.h"
#include "InGame/Enemy/Boss/NunuBossAI.h"

UBossAnimInstance::UBossAnimInstance()
{

}

void UBossAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	Owner = Cast<ANunuBossAI>(TryGetPawnOwner());
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner) 
	{
		OwnerProjType = Owner->GetProjectileState();
	}
}

void UBossAnimInstance::AnimNotify_RiseStart()
{
	OnRiseStart.ExecuteIfBound();
}


void UBossAnimInstance::AnimNotify_EndEscapeStart()
{
	OnEndEscapeStartAnim.ExecuteIfBound();
}

void UBossAnimInstance::AnimNotify_EndEscape()
{
	bIsEscapeFromPlayer = false;
	OnEndEscape.ExecuteIfBound();
}


void UBossAnimInstance::AnimNotify_EndAvoidance()
{
	bIsAvoidance = false;
}

void UBossAnimInstance::AnimNotify_ShotProj_Horizontal()
{
	OnShotProj_Horizontal.ExecuteIfBound();
}

void UBossAnimInstance::AnimNotify_ShotProj_Fake()
{
	OnShotProj_Fake.ExecuteIfBound();
}

void UBossAnimInstance::AnimNotify_ShotProj_Delay()
{
	OnShotProj_Delay.ExecuteIfBound();
}

void UBossAnimInstance::AnimNotify_ShotProj_Homing()
{
	OnShotProj_Homing.ExecuteIfBound();
}

void UBossAnimInstance::AnimNotify_EndShotProj()
{
	bIsShotProjectile = false;
	bIsShotProjectile_Horizontal = false;
	bIsShotProjectile_Fake = false;
	bIsShotProjectile_Delay = false;
	bIsShotProjectile_Homing = false;
	OnEndShotProj.ExecuteIfBound();
}

void UBossAnimInstance::AnimNotify_RangeAttackCheck()
{
	bIsRangeAttack = false;
	OnRangeAttackCheck.ExecuteIfBound();
}
