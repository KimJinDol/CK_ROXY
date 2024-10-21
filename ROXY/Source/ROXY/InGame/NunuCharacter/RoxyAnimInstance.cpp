// Fill out your copyright notice in the Description page of Project Settings.


#include "RoxyAnimInstance.h"
#include "InGame/NunuCharacter/RoxyCharacter.h"
#include "InGame/Component/ComboComponent.h"

URoxyAnimInstance::URoxyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Resources/Roxy/Mon_Roxy_Attack.Mon_Roxy_Attack"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> KICK_MONTAGE(TEXT("/Game/Resources/Roxy/Mon_Roxy_Kick.Mon_Roxy_Kick"));
	if (KICK_MONTAGE.Succeeded())
	{
		KickMontage = KICK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PIERCE_MONTAGE(TEXT("/Game/Resources/Roxy/Mon_Roxy_Pierce.Mon_Roxy_Pierce"));
	if (PIERCE_MONTAGE.Succeeded())
	{
		PierceMontage = PIERCE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWIFTSLASH_MONTAGE(TEXT("/Game/Resources/Roxy/Mon_Roxy_SwiftSlash.Mon_Roxy_SwiftSlash"));
	if (SWIFTSLASH_MONTAGE.Succeeded())
	{
		SwiftSlashMontage = SWIFTSLASH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EVADE_MONTAGE(TEXT("/Game/Resources/Roxy/Mon_Roxy_Evade.Mon_Roxy_Evade"));
	if (EVADE_MONTAGE.Succeeded())
	{
		EvadeMontage = EVADE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HIT_MONTAGE(TEXT("/Game/Resources/Roxy/Mon_Roxy_Hit.Mon_Roxy_Hit"));
	if (HIT_MONTAGE.Succeeded())
	{
		HitMontage = HIT_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SHEATH_MONTAGE(TEXT("/Game/Resources/Roxy/Mon_Roxy_Sheath.Mon_Roxy_Sheath"));
	if (SHEATH_MONTAGE.Succeeded())
	{
		SheathMontage = SHEATH_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GHOSTSWORD_INSTALL_MONTAGE(TEXT("/Game/Resources/Roxy/Mon_Roxy_Vision_Install.Mon_Roxy_Vision_Install"));
	if (GHOSTSWORD_INSTALL_MONTAGE.Succeeded())
	{
		GhostSwordInstallMontage = GHOSTSWORD_INSTALL_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GHOSTSWORD_MONTAGE(TEXT("/Game/Resources/Roxy/Mon_Roxy_Vision_Attack.Mon_Roxy_Vision_Attack"));
	if (GHOSTSWORD_MONTAGE.Succeeded())
	{
		GhostSwordMontage = GHOSTSWORD_MONTAGE.Object;
	}
}

void URoxyAnimInstance::PlayAttackMontage()
{
	CHECK(AttackMontage != nullptr);
	Montage_Play(AttackMontage, 1.0f);
}

void URoxyAnimInstance::JumpToAttackMontageSection(float newSection)
{
	CHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(newSection), AttackMontage);
	//LOG_SCREEN("new Section : %d", newSection);
}

void URoxyAnimInstance::StopAttackMontage()
{
	CHECK(Montage_IsPlaying(AttackMontage));
	Montage_Stop(0, AttackMontage);
}

void URoxyAnimInstance::PlayKickMontage()
{
	Montage_Play(KickMontage);
}

void URoxyAnimInstance::StopKickMontage()
{
	CHECK(Montage_IsPlaying(KickMontage));
	Montage_Stop(0, KickMontage);
}

void URoxyAnimInstance::PlayPierceMontage()
{
	// 	if (Montage_IsActive(PierceMontage) || Montage_IsPlaying(PierceMontage))
	// 	{
	// 		LOG(Warning, "Pierce MOntage is Active");
	// 		Montage_Stop(0, PierceMontage);
	// 	}
	Montage_Play(PierceMontage, 1.0f);

}

void URoxyAnimInstance::PlaySwiftSlashMontage()
{
	Montage_Play(SwiftSlashMontage, 1.0f);
}

void URoxyAnimInstance::PlayEvadeMontage()
{
	Montage_Play(EvadeMontage, 1.0f);
}

void URoxyAnimInstance::PlayHitMontage()
{
	Montage_Play(HitMontage, 1.2f);
}

void URoxyAnimInstance::PlaySheathMontage()
{
	Montage_Play(SheathMontage, 0.7f);
}

void URoxyAnimInstance::PlayGhostSwordInstallMontage()
{
	Montage_Play(GhostSwordInstallMontage, 1.0f);
}

void URoxyAnimInstance::PlayGhostSwordMontage()
{
	Montage_Play(GhostSwordMontage, 1.0f);
}

/* 기본 공격 */
void URoxyAnimInstance::AnimNotify_CanInputNextCombo()
{
	OnCanInputNextCombo.Broadcast();
}

void URoxyAnimInstance::AnimNotify_ComboCheck()
{
	OnComboCheck.Broadcast();
}

void URoxyAnimInstance::AnimNotify_ClearOverlapActor()
{
	OnClearActor.Broadcast();
}

void URoxyAnimInstance::AnimNotify_OffAttackCollision()
{
	OnOffCollision.Broadcast();
}

/* 발차기 */
void URoxyAnimInstance::AnimNotify_KickCheck()
{
	OnKickCheck.Broadcast();
}

void URoxyAnimInstance::AnimNotify_EndKick()
{
	OnKickEnd.Broadcast();
}

/* 일섬 */
void URoxyAnimInstance::AnimNotify_PierceCheck()
{
	OnPierceCheck.Broadcast();
}

void URoxyAnimInstance::AnimNotify_StopPierce()
{
	OnStopPierce.Broadcast();
}

void URoxyAnimInstance::AnimNotify_EndPierce()
{
	OnPierceEnd.Broadcast();
}

/* 연참 */
void URoxyAnimInstance::AnimNotify_SwiftSlash()
{
	OnSwiftSlash.Broadcast();
}

void URoxyAnimInstance::AnimNotify_EndSwiftSlash()
{
	OnSwiftSlashEnd.Broadcast();
}

/* 대쉬 */
void URoxyAnimInstance::AnimNotify_EvadeCheck()
{
	OnEvadeCheck.Broadcast();
}

void URoxyAnimInstance::AnimNotify_GhostTrail()
{
	OnGhostTrail.Broadcast();
}

void URoxyAnimInstance::AnimNotify_StopLaunch()
{
	OnStopLaunch.Broadcast();
}

void URoxyAnimInstance::AnimNotify_EndEvade()
{
	OnEvadeEnd.Broadcast();
}

/* 환영검 */
void URoxyAnimInstance::AnimNotify_InstallSword()
{
	OnInstallSword.Broadcast();
}

void URoxyAnimInstance::AnimNotify_EndInstallSword()
{
	OnEndInstallSword.Broadcast();
}

void URoxyAnimInstance::AnimNotify_RushTowardSword()
{
	OnRushTowardSword.Broadcast();
}

/* 데이터 관련 */
void URoxyAnimInstance::AnimNotify_StepCheck()
{
	OnStepCheck.Broadcast();
}

FName URoxyAnimInstance::GetAttackMontageSectionName(int32 section)
{
	CHECK(FMath::IsWithinInclusive<int32>(section, 1, Roxy->GetComboAttackMaxNum()), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), section));
}


void URoxyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Roxy = Cast<ARoxyCharacter>(TryGetPawnOwner());
	_currentSpeedRate = 0.0f;
	_bOnMove = false;
	_currentForwardSpeedRate = 0.0f;
	_currentRightSpeedRate = 0.0f;
}

void URoxyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Roxy)
	{
		// CurrentSpeedRate 겟함수로
		_currentSpeedRate = Roxy->GetVelocity().Size();
		_bOnMove = Roxy->OnMove();
		_currentForwardSpeedRate = Roxy->GetForwardSpeedRate();
		_currentRightSpeedRate = Roxy->GetRightSpeedRate();
	}
	else
	{
		/*LOG(Warning, "Player isn't exist");*/
	}
}