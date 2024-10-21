// Fill out your copyright notice in the Description page of Project Settings.


#include "RevivalAnimInstance.h"
#include "InGame/Enemy/GruntAI/Revival/RevivalAI.h"

URevivalAnimInstance::URevivalAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_ATTACK(TEXT("/Game/Resources/Enemy/Grunt/Wizard_Undead/Mon_WizardUndead_Attack.Mon_WizardUndead_Attack"));
	if (MON_ATTACK.Succeeded())
	{
		AttackMontage = MON_ATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_HIT(TEXT("/Game/Resources/Enemy/Grunt/Wizard_Undead/Mon_WizardUndead_Hit.Mon_WizardUndead_Hit"));
	if (MON_HIT.Succeeded())
	{
		HitMontage = MON_HIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SWIFTHIT(TEXT("/Game/Resources/Enemy/Grunt/Wizard_Undead/Mon_WizardUndead_SwiftHit.Mon_WizardUndead_SwiftHit"));
	if (MON_SWIFTHIT.Succeeded())
	{
		SwiftHitMontage = MON_SWIFTHIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SPAWN(TEXT("/Game/Resources/Enemy/Grunt/Wizard_Undead/Mon_WizardUndead_Spawn.Mon_WizardUndead_Spawn"));
	if (MON_SPAWN.Succeeded())
	{
		SpawnMontage = MON_SPAWN.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_REVIVAL(TEXT("/Game/Resources/Enemy/Grunt/Wizard_Undead/Mon_WizardUndead_Revival.Mon_WizardUndead_Revival"));
	if (MON_REVIVAL.Succeeded())
	{
		RevivalMontage = MON_REVIVAL.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SHOOT(TEXT("/Game/Resources/Enemy/Grunt/Wizard_Undead/Mon_WizardUndead_Projectile.Mon_WizardUndead_Projectile"));
	if(MON_SHOOT.Succeeded())
	{
		ShootMontage = MON_SHOOT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_TIRED(TEXT("/Game/Resources/Enemy/Grunt/Wizard_Undead/Mon_WizardUndead_Weary.Mon_WizardUndead_Weary"));
	if(MON_TIRED.Succeeded())
	{
		TiredMontage = MON_TIRED.Object;
	}
}

void URevivalAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	RevivalAI = Cast<ARevivalAI>(TryGetPawnOwner());
}

void URevivalAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
}

void URevivalAnimInstance::AnimNotify_OnRevive()
{
	OnRevive.Broadcast();
}

void URevivalAnimInstance::AnimNotify_EndRevive()
{
	OnEndRevive.Broadcast();
}


void URevivalAnimInstance::AnimNotify_OnShootProj()
{
	OnShootProj.Broadcast();
}

void URevivalAnimInstance::AnimNotify_EndShootProj()
{
	OnEndShootProj.Broadcast();
}

void URevivalAnimInstance::AnimNotify_OnDescent()
{
	RevivalAI->OnTired.Broadcast();
	OnDescent.Broadcast();
}

void URevivalAnimInstance::AnimNotify_OnRising()
{
	OnRising.Broadcast();
}

void URevivalAnimInstance::AnimNotify_OnTired()
{
	OnTired.Broadcast();
}

void URevivalAnimInstance::AnimNotify_EndTakeDamage()
{
	OnEndTakeDamage.Broadcast();
}

void URevivalAnimInstance::AnimNotify_EndTired()
{
	OnEndTired.Broadcast();
	_restTiredAnimMonTime = 0.f;
}

void URevivalAnimInstance::AnimNotify_EndSwiftHit()
{
	Montage_Play(TiredMontage, 1.f, EMontagePlayReturnType::MontageLength, _restTiredAnimMonTime, true);
}

void URevivalAnimInstance::AnimNotify_OnDie()
{
	OnDie.Broadcast();
}

void URevivalAnimInstance::AnimNotify_EndDie()
{
	OnEndDie.Broadcast();
}

void URevivalAnimInstance::PlayRevivalMontage()
{
	Montage_Play(RevivalMontage);
}

void URevivalAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage);
}

void URevivalAnimInstance::PlayShootProjMontage()
{
	Montage_Play(ShootMontage);
}

void URevivalAnimInstance::PlayTiredMontage()
{
	Montage_Play(TiredMontage);
}

void URevivalAnimInstance::PlaySwiftHitMontage()
{
	if(_restTiredAnimMonTime == 0.f)
	{
		_restTiredAnimMonTime = Montage_GetPosition(TiredMontage);
	}
	Montage_Play(SwiftHitMontage);
}
