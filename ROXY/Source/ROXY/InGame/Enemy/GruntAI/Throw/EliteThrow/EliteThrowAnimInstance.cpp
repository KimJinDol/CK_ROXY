// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteThrowAnimInstance.h"
#include "InGame/Enemy/GruntAI/Throw/EliteThrow/EliteThrowAI.h"

UEliteThrowAnimInstance::UEliteThrowAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_ATTACK(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mon_Elite_Skeleton_Attack.Mon_Elite_Skeleton_Attack"));
	if (MON_ATTACK.Succeeded())
	{
		AttackMontage = MON_ATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_HIT(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mon_Elite_Skeleton_Hit.Mon_Elite_Skeleton_Hit"));
	if (MON_HIT.Succeeded())
	{
		HitMontage = MON_HIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SWIFTHIT(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mon_Elite_Skeleton_SwiftHit.Mon_Elite_Skeleton_SwiftHit"));
	if (MON_SWIFTHIT.Succeeded())
	{
		SwiftHitMontage = MON_SWIFTHIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SPAWN(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mon_Elite_Skeleton_Spawn.Mon_Elite_Skeleton_Spawn"));
	if (MON_SPAWN.Succeeded())
	{
		SpawnMontage = MON_SPAWN.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SHOOT(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mon_Elite_Skeleton_Shoot.Mon_Elite_Skeleton_Shoot"));
	if (MON_SHOOT.Succeeded())
	{
		ShootMontage = MON_SHOOT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_METEOR(TEXT("/Game/Resources/Enemy/Grunt/Skeleton/Mon_Elite_Skeleton_Meteor.Mon_Elite_Skeleton_Meteor"));
	if (MON_METEOR.Succeeded())
	{
		MeteorMontage = MON_METEOR.Object;
	}
}

void UEliteThrowAnimInstance::PlayShootMontage(float value /*= 1.0f*/)
{
	Montage_Play(ShootMontage, value);
}

void UEliteThrowAnimInstance::PlayMeteorMontage()
{
	Montage_Play(MeteorMontage, 1.0f);
}

void UEliteThrowAnimInstance::AnimNotify_ShootCheck()
{
	OnShoot.Broadcast();
}

void UEliteThrowAnimInstance::AnimNotify_EndShoot()
{
	OnShootEnd.Broadcast();
}

void UEliteThrowAnimInstance::AnimNotify_MeteorCheck()
{
	OnMeteor.Broadcast();
}

void UEliteThrowAnimInstance::AnimNotify_MeteorEnd()
{
	OnMeteorEnd.Broadcast();
}

void UEliteThrowAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	EliteThrowAI = Cast<AEliteThrowAI>(TryGetPawnOwner());
}

void UEliteThrowAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

}
