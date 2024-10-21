// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntAnimInstance.h"
#include "InGame/Enemy/GruntAI/GruntAI.h"

UGruntAnimInstance::UGruntAnimInstance()
{
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_ATTACK(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mon_Cactus_Attack.Mon_Cactus_Attack"));
	//if (MON_ATTACK.Succeeded())
	//{
	//	AttackMontage = MON_ATTACK.Object;
	//}
	//
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_HIT(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mon_Cactus_Hit.Mon_Cactus_Hit"));
	//if (MON_HIT.Succeeded())
	//{
	//	HitMontage = MON_HIT.Object;
	//}
	//
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SWIFTHIT(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mon_Cactus_SwiftHit.Mon_Cactus_SwiftHit"));
	//if (MON_SWIFTHIT.Succeeded())
	//{
	//	SwiftHitMontage = MON_SWIFTHIT.Object;
	//}
	//
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_KNOCKBACK(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mon_Cactus_Knockback.Mon_Cactus_Knockback"));
	//if (MON_KNOCKBACK.Succeeded())
	//{
	//	KnockBackMontage = MON_KNOCKBACK.Object;
	//}
	//
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SPAWN(TEXT("/Game/Resources/Enemy/Grunt/Cactus/Mon_Cactus_Spawn.Mon_Cactus_Spawn"));
	//if (MON_SPAWN.Succeeded())
	//{
	//	SpawnMontage = MON_SPAWN.Object;
	//}
}

void UGruntAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Grunt = Cast<AGruntAI>(TryGetPawnOwner());
	_currentSpeedRate = 0.0f;
}

void UGruntAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Grunt)
	{
		_currentSpeedRate = Grunt->GetVelocity().Size();
	}
}

void UGruntAnimInstance::PlayHitMontage(/*EHitDirection direction*/)
{
	CHECK(HitMontage);
	if (!Montage_IsPlaying(HitMontage))
		Montage_Play(HitMontage, 0.8f);

	//switch (direction) {
	//case EHitDirection::HD_F:
	//	Montage_JumpToSection("Hit_F", HitMontage);
	//	break;
	//case EHitDirection::HD_B:
	//	Montage_JumpToSection("Hit_B", HitMontage);
	//	break;
	//case EHitDirection::HD_L:
	//	Montage_JumpToSection("Hit_L", HitMontage);
	//	break;
	//case EHitDirection::HD_R:
	//	Montage_JumpToSection("Hit_R", HitMontage);
	//	break;
	//}
}

void UGruntAnimInstance::PlaySideHitMontage(EHitDirection direction)
{
	CHECK(SideHitMontage);
	if (!Montage_IsPlaying(SideHitMontage))
		Montage_Play(SideHitMontage, 0.8f);

	switch (direction) {
	case EHitDirection::HD_L:
		Montage_JumpToSection("Hit_L", SideHitMontage);
		break;
	case EHitDirection::HD_R:
		Montage_JumpToSection("Hit_R", SideHitMontage);
		break;
	}
}

void UGruntAnimInstance::PlaySwiftHitMontage()
{
	CHECK(SwiftHitMontage);
	if (!Montage_IsPlaying(SwiftHitMontage))
		Montage_Play(SwiftHitMontage, 1.1f);
}

void UGruntAnimInstance::PlayKnockBackMontage()
{
	CHECK(KnockBackMontage);

	Montage_Play(KnockBackMontage);
}

void UGruntAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void UGruntAnimInstance::StopAttackMontage()
{
	Montage_Stop(0, AttackMontage);
	OnAttackEnd.Broadcast();
}

void UGruntAnimInstance::PlaySpawnMontage(float value)
{
	if (SpawnMontage)
	{
		Montage_Play(SpawnMontage, value);
	}
	else
	{
		return;
	}
}

void UGruntAnimInstance::AnimNotify_EndKnockback()
{
	CHECK(KnockBackMontage);
	if (Montage_IsPlaying(KnockBackMontage))
		Montage_Stop(0, KnockBackMontage);

	OnKnockbackEnd.Broadcast();
	
}

void UGruntAnimInstance::AnimNotify_AttackCheck()
{
	OnAttackCheck.Broadcast();
}

void UGruntAnimInstance::AnimNotify_AttackCheckEnd()
{
	OnAttackCheckEnd.Broadcast();
}

void UGruntAnimInstance::AnimNotify_OffAttackCollision()
{
	OnOffAttackCollision.Broadcast();
}

void UGruntAnimInstance::AnimNotify_AttackEnd()
{
	Montage_Stop(0, AttackMontage);
	OnAttackEnd.Broadcast();
}

void UGruntAnimInstance::AnimNotify_ChangeToOpacityMat ()
{
	OnChangeToOpacityMat.Broadcast();
}

void UGruntAnimInstance::AnimNotify_EndDieAnim ()
{
	OnDieAnimEnd.Broadcast();
}

void UGruntAnimInstance::AnimNotify_SpawnStart()
{
	OnSpawnAnimStart.Broadcast(true);
}

void UGruntAnimInstance::AnimNotify_SpawnEnd()
{
	OnSpawnAnimEnd.Broadcast(false);
}
