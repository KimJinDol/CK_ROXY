// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAnimInstance.h"
#include "InGame/Enemy/GruntAI/Combat/CombatAI.h"

UCombatAnimInstance::UCombatAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_ATTACK(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mon_Cactus_Attack.Mon_Cactus_Attack"));
	if (MON_ATTACK.Succeeded())
	{
		AttackMontage = MON_ATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_HIT(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mon_Cactus_Hit.Mon_Cactus_Hit"));
	if (MON_HIT.Succeeded())
	{
		HitMontage = MON_HIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SIDEHIT(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mon_Cactus_SideHit.Mon_Cactus_SideHit"));
	if (MON_SIDEHIT.Succeeded())
	{
		SideHitMontage = MON_SIDEHIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SWIFTHIT(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mon_Cactus_SwiftHit.Mon_Cactus_SwiftHit"));
	if (MON_SWIFTHIT.Succeeded())
	{
		SwiftHitMontage = MON_SWIFTHIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_KNOCKBACK(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mon_Cactus_Knockback.Mon_Cactus_Knockback"));
	if (MON_KNOCKBACK.Succeeded())
	{
		KnockBackMontage = MON_KNOCKBACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SPAWN(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mon_Cactus_Spawn.Mon_Cactus_Spawn"));
	if (MON_SPAWN.Succeeded())
	{
		SpawnMontage = MON_SPAWN.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_PUSHOUT(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mon_Cactus_PushOut.Mon_Cactus_PushOut"));
	if (MON_PUSHOUT.Succeeded())
	{
		PushOutMontage = MON_PUSHOUT.Object;
	}
}

void UCombatAnimInstance::PlayPushOutMontage()
{
	Montage_Play(PushOutMontage, 1.0f);
}

void UCombatAnimInstance::AnimNotify_PlayAttackSound()
{
	OnAttackSound.Broadcast();
}

void UCombatAnimInstance::AnimNotify_PushOutCheck()
{
	OnPushOut.Broadcast();
}

void UCombatAnimInstance::AnimNotify_EndPushOut()
{
	OnPushOutEnd.Broadcast();
}

void UCombatAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	CombatAI = Cast<ACombatAI>(TryGetPawnOwner());
}

void UCombatAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

}
