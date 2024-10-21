// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargeAnimInstance.h"
#include "InGame/Enemy/GruntAI/Charge/ChargeAI.h"

UChargeAnimInstance::UChargeAnimInstance()
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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_QUAKE(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mon_Cactus_Quake.Mon_Cactus_Quake"));
	if (MON_QUAKE.Succeeded())
	{
		QuakeMontage = MON_QUAKE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_PROVOKE(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mon_Cactus_Provoke.Mon_Cactus_Provoke"));
	if (MON_PROVOKE.Succeeded())
	{
		ProvokeMontage = MON_PROVOKE.Object;
	}
}

void UChargeAnimInstance::PlayQuakeMontage()
{
	Montage_Play(QuakeMontage, 1.0f);
}

void UChargeAnimInstance::PlayProvokeMontage()
{
	Montage_Play(ProvokeMontage, 1.0f);
}

void UChargeAnimInstance::AnimNotify_QuakeCheck()
{
	OnQuake.Broadcast();
}

void UChargeAnimInstance::AnimNotify_QuakeEnd()
{
	OnQuakeEnd.Broadcast();
}

void UChargeAnimInstance::AnimNotify_ProvokeCheck()
{
	OnProvoke.Broadcast();
}

void UChargeAnimInstance::AnimNotify_ProvokeEnd()
{
	OnProvokeEnd.Broadcast();
}

void UChargeAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ChargeAI = Cast<AChargeAI>(TryGetPawnOwner());
}

void UChargeAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

}
