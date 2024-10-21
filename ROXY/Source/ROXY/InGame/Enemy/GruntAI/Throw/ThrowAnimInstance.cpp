// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowAnimInstance.h"
#include "InGame/Enemy/GruntAI/Throw/ThrowAI.h"

UThrowAnimInstance::UThrowAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_ATTACK(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mon_Skeleton_Attack.Mon_Skeleton_Attack"));
	if (MON_ATTACK.Succeeded())
	{
		AttackMontage = MON_ATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_HIT(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mon_Skeleton_Hit.Mon_Skeleton_Hit"));
	if (MON_HIT.Succeeded())
	{
		HitMontage = MON_HIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SWIFTHIT(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mon_Skeleton_SwiftHit.Mon_Skeleton_SwiftHit"));
	if (MON_SWIFTHIT.Succeeded())
	{
		SwiftHitMontage = MON_SWIFTHIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_KNOCKBACK(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mon_Skeleton_Knockback.Mon_Skeleton_Knockback"));
	if (MON_KNOCKBACK.Succeeded())
	{
		KnockBackMontage = MON_KNOCKBACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SPAWN(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mon_Skeleton_Spawn.Mon_Skeleton_Spawn"));
	if (MON_SPAWN.Succeeded())
	{
		SpawnMontage = MON_SPAWN.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SHOOT(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mon_Skeleton_Shoot.Mon_Skeleton_Shoot"));
	if (MON_SHOOT.Succeeded())
	{
		ShootMontage = MON_SHOOT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SKULLSHOOT(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mon_Skeleton_SkullShoot.Mon_Skeleton_SkullShoot"));
	if (MON_SKULLSHOOT.Succeeded())
	{
		SkullShootMontage = MON_SKULLSHOOT.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_METEOR(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mon_Skeleton_Meteor.Mon_Skeleton_Meteor"));
	if (MON_METEOR.Succeeded())
	{
		MeteorMontage = MON_METEOR.Object;
	}
}

void UThrowAnimInstance::PlayShootMontage(float value)
{
	Montage_Play(ShootMontage, value);
}

void UThrowAnimInstance::PlaySkullShootMontage(float value)
{
	Montage_Play(SkullShootMontage, value);
}

void UThrowAnimInstance::PlayMeteorMontage()
{
	Montage_Play(MeteorMontage, 1.0f);
}

void UThrowAnimInstance::AnimNotify_ShootCheck()
{
	OnShoot.Broadcast();
}

void UThrowAnimInstance::AnimNotify_EndShoot()
{
	OnShootEnd.Broadcast();
}

void UThrowAnimInstance::AnimNotify_SkullShootCheck()
{
	OnSkullShoot.Broadcast();
}

void UThrowAnimInstance::AnimNotify_EndSkullShoot()
{
	OnSkullShootEnd.Broadcast();
}

void UThrowAnimInstance::AnimNotify_MeteorCheck()
{
	OnMeteor.Broadcast();
}

void UThrowAnimInstance::AnimNotify_MeteorEnd()
{
	OnMeteorEnd.Broadcast();
}

void UThrowAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ThrowAI = Cast<AThrowAI>(TryGetPawnOwner());
}

void UThrowAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);


}
