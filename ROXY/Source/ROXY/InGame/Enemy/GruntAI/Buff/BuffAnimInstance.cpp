// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffAnimInstance.h"
#include "InGame/Enemy/GruntAI/Buff/BuffAI.h"

UBuffAnimInstance::UBuffAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_ATTACK(TEXT("/Game/Resources/Enemy/Grunt/Wizard/Mon_Wizard_Attack.Mon_Wizard_Attack"));
	if (MON_ATTACK.Succeeded())
	{
		AttackMontage = MON_ATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_HIT(TEXT("/Game/Resources/Enemy/Grunt/Wizard/Mon_Wizard_Hit.Mon_Wizard_Hit"));
	if (MON_HIT.Succeeded())
	{
		HitMontage = MON_HIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SWIFTHIT(TEXT("/Game/Resources/Enemy/Grunt/Wizard/Mon_Wizard_SwiftHit.Mon_Wizard_SwiftHit"));
	if (MON_SWIFTHIT.Succeeded())
	{
		SwiftHitMontage = MON_SWIFTHIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_KNOCKBACK(TEXT("/Game/Resources/Enemy/Grunt/Wizard/Mon_Wizard_Knockback.Mon_Wizard_Knockback"));
	if (MON_KNOCKBACK.Succeeded())
	{
		KnockBackMontage = MON_KNOCKBACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SPAWN(TEXT("/Game/Resources/Enemy/Grunt/Wizard/Mon_Wizard_Spawn.Mon_Wizard_Spawn"));
	if (MON_SPAWN.Succeeded())
	{
		SpawnMontage = MON_SPAWN.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_INVINCI(TEXT("/Game/Resources/Enemy/Grunt/Wizard/Mon_Wizard_Invincible.Mon_Wizard_Invincible"));
	if (MON_INVINCI.Succeeded())
	{
		InvincibleMontage = MON_INVINCI.Object;
	}
}

void UBuffAnimInstance::PlayInvinciMontage()
{
	Montage_Play(InvincibleMontage, 1.0f);
}

void UBuffAnimInstance::AnimNotify_InvincibleCheck()
{
	OnInvincibility.Broadcast();
}

void UBuffAnimInstance::AnimNotify_InvincibleEnd()
{
	OnInvincibilityEnd.Broadcast();
}

void UBuffAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	BuffAI = Cast<ABuffAI>(TryGetPawnOwner());
}

void UBuffAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
}
